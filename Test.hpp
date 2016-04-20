//This file is used to test the proposed method against the existing one using a generated identity matrix
#pragma once
#include "config.hpp"
#include "partedCholesky.cuh"

void Test(int M, float &duration1, float &duration2, float &flag)
{
	int len = 3 * (6 + 15 * (M - 1));
	int lensub = len / 3;
	float* arr = new float[len];
	for (int i = 0; i < len; i++)
	{
		arr[i] = 0;
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	spacePu::mixedcholesky_part1(arr, M);

	int sizeA = 18 * 18 * M*M;
	int rowsA = 18 * M;
	int colsA = 18 * M;
	//float *A=0;
	//cudaMallocManaged(&A, sizeA*sizeof(float));

	float* A=new float[sizeA];
	for (int i = 0; i < rowsA; i++)
	{
		for (int j = 0; j < colsA; j++)
		{
			A[colsA*i + j] = 0;
			if (i == j)
			{
				A[colsA*i + j] = 1;
			}
		}
	}


	float* Ad;
	cudaMalloc(&Ad, sizeA*sizeof(float));
	cudaMemcpy(Ad, A, sizeA*sizeof(float), cudaMemcpyHostToDevice);

	//GpuTimer timer;
	//timer.Start();
	partedCholesky(M, Ad);
	//timer.Stop();
	//printf("Your code ran in: %f msecs.\n", timer.Elapsed());
	cudaMemcpy(A, Ad, sizeA*sizeof(float), cudaMemcpyDeviceToHost);

	auto t2 = std::chrono::high_resolution_clock::now();
	duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	float *B = new float[sizeA];
	for (int i = 0; i < rowsA; i++)
	{
		for (int j = 0; j < colsA; j++)
		{
			B[colsA*i + j] = 0;
			if (i == j)
			{
				B[colsA*i + j] = 1;
			}
		}
	}

	t1 = std::chrono::high_resolution_clock::now();
	flag = spacePu::cholesky_gold(M, B);
	t2 = std::chrono::high_resolution_clock::now();
	duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	//cudaFree(A);

	delete A;
	cudaFree(Ad);
	delete arr;
	delete B;
}