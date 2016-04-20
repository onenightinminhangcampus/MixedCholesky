#pragma once
////VS
#include <chrono>
#include <stdio.h>
#include <ostream>
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

////OpenMP
#include <omp.h>
#include <cblas.h>

////UDF
#include "cholesky_gold.hpp"
#include "mixedCholesky_part1.hpp"
#include "timer.h"
#include "Test.hpp"

////UDF_device
#include "partedCholesky.cuh"
