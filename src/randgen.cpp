#include<iostream>
#include<random>

void test_rand()
{
	std::default_random_engine engine;
	std::normal_distribution<double> normal(0,1);	
	std::poisson_distribution<int> poisson(12.34);

	double x = 0, xx = 0;
	double y = 0, yy = 0;
	int N = 10000;
	for(int n=0; n!=N; ++n)
	{
		auto sample_x = normal(engine);
		auto sample_y = poisson(engine);
		x  += sample_x;
		xx += sample_x * sample_x;
		y  += sample_y;
		yy += sample_y * sample_y;
	}
	std::cout << "\nmean = " << x /N;
	std::cout << "\nvar  = " << xx/N - (x/N)*(x/N);
	std::cout << "\nmean = " << y /N;
	std::cout << "\nvar  = " << yy/N - (y/N)*(y/N);

}

