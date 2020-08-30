#include<iostream>
#include<initializer_list>

struct distance
{
	distance(double x) : m(x){}
	distance operator+(const distance& rhs) { return distance{ m+rhs.m }; }
	distance operator-(const distance& rhs) { return distance{ m-rhs.m }; }
	distance operator*(double n)            { return distance{ m*n }; }
	distance operator/(double n)            { return distance{ m/n }; }
	double m;
};

distance average(const std::initializer_list<distance>& list)
{
	distance sum{0};
	for(const auto& x:list)
	{
		sum = sum + x;
	}
	return sum / list.size();
}

distance operator""_cm(long double x) { return distance(x) / 100;  }
distance operator""_m (long double x) { return distance(x);        }
distance operator""_km(long double x) { return distance(x) * 1000; }

std::ostream& operator<<(std::ostream& os, const distance& x)
{
	os << "distance " << x.m << " meters";
	return os;
}

void test_literal_operator()
{
	// in practice, please :
	// 1. add namespace and 
	// 2. make distance constructor explicit
	std::cout << "\n" << 1.23;
	std::cout << "\n" << 1.23_cm;
	std::cout << "\n" << 1.23_m;
	std::cout << "\n" << 1.23_km;
	std::cout << "\naverage = " << average({ 1.23_cm, 1.23_m, 1.23_km });
	std::cout << "\naverage = " << average({ 1.23, 1.23, 1.23 });
}