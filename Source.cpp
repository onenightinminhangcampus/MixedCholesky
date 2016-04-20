#include "config.hpp"

int main()
{
	//TODO: Force to terminate the program after 1min
	using namespace spacePu;

	std::ofstream output("output.txt");

	output << "M" << "	" << "t_{mixed} (ms)" << "	" << "t_{CPU} (ms)"<< std::endl;

	//5 10 15 50 100 200 300 400 500 600 700 800 900 1000
	int* pts = new int[14];
	int o = 0;
	pts[o] = 5;
	o++;
	pts[o] = 10;
	o++;
	pts[o] = 15;
	o++;
	pts[o] = 50;
	o++;
	pts[o] = 100;
	o++;
	pts[o] = 200;
	o++;
	pts[o] = 300;
	o++;
	pts[o] = 400;
	o++;
	pts[o] = 500;
	o++;
	pts[o] = 600;
	o++;
	pts[o] = 700;
	o++;
	pts[o] = 800;
	o++;
	pts[o] = 900;
	o++;
	pts[o] = 1000;

	int M;
	float duration1;
	float duration2;
	float flag;

	for (int i = 0; i < 14; i++)
	{
		M = pts[i];

		duration1 = 0;
		duration2 = 0;
		flag = 1;
		//Test using module number M
		Test(M, duration1, duration2, flag);
		if (flag == 1)
		{
			output << M << "	" << duration1 << "	" << duration2 << std::endl;
		}
		else
		{
			output << M << "	" << duration1 << "	" << "Fail" << std::endl;
		}

		//std::cout << "Time Elapsed " << duration1 << " miliseconds\n";
		//std::string fl = flag == 1 ? "successful" : "fail";
		//std::cout << "Time Elapsed " << duration2 << " miliseconds " << fl << "\n";
	}

	output.close();
	return 0;
}