#pragma once
#include "config.hpp"

namespace spacePu
{
	//elimination chain
	void mixedcholesky_part1_chain(float *arrp, int start, int M)
	{
		int len = 6 + 15 * (M - 1);

		float* arr = new float[6 + 15 * (M - 1)];
		int start0 = start;
		for (int i = 0; i < len; i++)
		{
			arr[i] = arrp[start];
			start++;
		}
		start = start0;

		float* arr2 = new float[6 + 15 * (M - 1)];
		for (int i = 0; i < len; i++)
		{
			arr2[i] = 0;
		}


		start = 0;
		int stemp;
		for (int t = 1; t < (M - 1); t++)
		{
			//starting index
			start = (t - 1) * 15;

			//three consecutive columns always has the same length
			for (int le = 1; le < 3; le++)
			{
				//hop from the top of the current column until the bottom
				int hop = 7 - le;
				stemp = start + hop;

				//traverse through each column
				for (int j = 1; j < hop - 1; j++)
					{
					//co=0,hop-1,hop-2,hop-3,hop-4
					int co = hop - j + 1;
					co = j >= (5 - le) ? co + 3 : co;

					co = j == 1 ? 0 : co;

					int ho = hop - j - 1;
					for (int k = stemp; k <= stemp + ho; k++)
					{
						arr2[k] = -arr2[start + j + k - stemp] * arr2[start + j] / arr2[start];
					}

					//saxpy(int n, float a, float *x, incx, float *y, incy)
					//y[i] = a*x[i] + y[i];
					cblas_saxpy(len, 1, arr2, 0, arr, 0);

					stemp = stemp + co;
				}

				start = start + hop;

			}
		}

		//post processing the last three columns
		start = start + 4;
		stemp = start + 3;
		arr[stemp] = arr[stemp] - arr[start + 1] * arr[stemp - 2] / arr[start];
		arr[stemp + 1] = arr[stemp + 1] - arr[start + 1] * arr[stemp - 1] / arr[start];
		arr[stemp + 2] = arr[stemp + 2] - arr[stemp + 1] * arr[stemp + 1] / arr[stemp];

		for (int i = 0; i < len; i++)
		{
			arrp[start] = arr[i];
			start++;
		}
	}

	//split an array into three chains or combine three chains to an array
	void mixedcholesky_part1_splitcombine(int M, float* arr, float* arr1, float* arr2, float* arr3, int scflag)
	{
		int len = 3 * (6 + 15 * (M - 1));
		int lensub = len / 3;

		int start = 0, start1 = 0, start2 = 0, start3 = 0, hop = 0;

		for (int t = 1; t <= M - 1; t++)
		{
			start = 15 * (t - 1);
			start1 = (t - 1) * 45;
			start2 = start1 + 6;
			start3 = start2 + 6;

			for (int le = 1; le < 3; le++)
			{
				hop = 6 - le;

				for (int i = 0; i <= hop; i++)
				{
					//split
					if (scflag == 0)
					{
						arr1[start + i] = arr[start1 + i];
						arr2[start + i] = arr[start2 + i];
						arr3[start + i] = arr[start3 + i];
					}
					//combine
					else if (scflag == 1)
					{
						arr[start1 + i] = arr1[start + i];
						arr[start2 + i] = arr2[start + i];
						arr[start3 + i] = arr3[start + i];
					}

				}

				start = start + hop + 1;
				start1 = start1 + (hop + 1) * 3;
				start2 = start2 + (hop + 1) * 2 + hop;
				start3 = start3 + (hop + 1) + 2 * hop;
			}
		}


		for (int last = 1; last <= 3; last++)
		{
			hop--;

			for (int i = 0; i <= hop; i++)
			{
				//split
				if (scflag == 0)
				{
					arr1[start + i] = arr[start1 + i];
					arr2[start + i] = arr[start2 + i];
					arr3[start + i] = arr[start3 + i];
				}
				//combine
				else if (scflag == 1)
				{
					arr[start1 + i] = arr1[start + i];
					arr[start2 + i] = arr2[start + i];
					arr[start3 + i] = arr3[start + i];
				}

			}

			start = start + 3 - last + 1;
			start1 = start1 + (hop + 1) * 3;
			start2 = start2 + (hop + 1) * 2 + hop;
			start3 = start3 + (hop + 1) + 2 * hop;
		}


	}

	//parallelization
	void mixedcholesky_part1(float* arr, int M)
	{
		int len = 3 * (6 + 15 * (M - 1));
		int lensub = len / 3;

		float* arr1 = new float[lensub];
		float* arr2 = new float[lensub];
		float* arr3 = new float[lensub];

		//split
		mixedcholesky_part1_splitcombine(M, arr, arr1, arr2, arr3, 0);

		//combo
		float* arrc = new float[len];
		for (int i = 0; i < lensub; i++)
		{
			arrc[i] = arr1[i];
			arrc[i + lensub] = arr2[i];
			arrc[i + 2 * lensub] = arr3[i];
		}

		//mixedcholesky_part1_chain(float *arrp, float* arr, int start, int M)
#pragma omp parallel num_threads(3)
		{
			int threadidx = omp_get_thread_num();
			mixedcholesky_part1_chain(arrc, threadidx * lensub, M);
		}

		//decombo
		for (int i = 0; i < lensub; i++)
		{
			arr1[i] = arrc[i];
			arr2[i] = arrc[i + lensub];
			arr3[i] = arrc[i + 2 * lensub];
		}

		mixedcholesky_part1_splitcombine(M, arr, arr1, arr2, arr3, 1);
	}

}