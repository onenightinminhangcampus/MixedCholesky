#pragma once

namespace spacePu
{
	//reference code
	int cholesky_gold(int M, float* A)
	{
		int multi = 0;

		auto t1 = std::chrono::high_resolution_clock::now();
		auto t2 = std::chrono::high_resolution_clock::now();

		int return_flag = 1;

		for (int x = 0; x < 18 * M; x++)
		{

			for (int j = 0; j < 18 * M; j++)
			{
				if (j == x)
				{
					A[x * 18 * M + j] = sqrt(A[x * 18 * M + j] - multi);
					break;
				}
				A[x * 18 * M + j] = (A[x  * 18 * M + j] - multi) / A[j * 18 * M + j];

				multi = 0;
				for (int i = 0; i <= j; i++)
				{
					multi += A[(j + 1) * 18 * M + i] * A[x * 18 * M + i];
				}
			}

			t2 = std::chrono::high_resolution_clock::now();
			float duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
			if (duration>50000)
			{
				return_flag = 0;
				break;
			}

		}


		return return_flag;

	}

}