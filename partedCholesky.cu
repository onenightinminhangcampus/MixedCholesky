#include <cuda_runtime.h>
#include "device_functions.h"
#include "device_launch_parameters.h"
#include "math_functions.hpp"

__global__ void partedCholesky_GPU(int M, float* A)
{
	int x = threadIdx.x+blockIdx.x*1024;
	float multi = 0;

	//Left part
	int j = 0;
	for (; j < 9 * M; j++)
	{
		A[(x + 9 * M) * 18 * M + j] = (A[(x + 9 * M) * 18 * M + j]-multi)/A[j*18*M+j];

		multi = 0;
		for (int i = 0; i <= j; i++)
		{
			multi += A[(j+1) * 18 * M + i] * A[(x + 9 * M) * 18 * M + i];
		}
	}

	__syncthreads();

	multi = 0;
	for (int i = 0; i <= j; i++)
	{
		multi += A[j * 18 * M + i] * A[(x + 9 * M) * 18 * M + i];
	}


	//Right part
	for (j = 9 * M; j < 18 * M; j++)
	{
		if (j - 9 * M == x)
		{
			A[(x + 9 * M) * 18 * M + j] = sqrt(A[(x + 9 * M) * 18 * M + j] - multi);
			break;
		}
		A[(x + 9 * M) * 18 * M + j] = (A[(x + 9 * M) * 18 * M + j] - multi) / A[j * 18 * M + j];
		
		multi = 0;
		for (int i = 0; i <= j; i++)
		{
			multi += A[(j+1) * 18 * M + i] * A[(x + 9 * M) * 18 * M + i];
		}

		//synchronize every column
		__syncthreads();
	}

	__syncthreads();


}


void partedCholesky(int M, float* A)
{
	int size = 18 * M;
	const dim3 blockSize(1024);
	const dim3 gridSize(18*M/1024+1);

	partedCholesky_GPU << <gridSize, blockSize >> >(M, A);

}