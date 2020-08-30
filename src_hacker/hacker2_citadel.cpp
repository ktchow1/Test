#include<iostream>
#include<string>
#include<vector>

//   0 ->   0
//   1 ->   1
//   2 ->   1
//   9 ->   1
//  10 ->  10
//  11 ->  10
//  99 ->  10
// 100 -> 100
// 101 -> 100
int order(int n)
{
	int m = 1;
	while (n>=m) m *= 10;
	return m/10;
}

std::string int2str(int n)
{
	std::string str;		
	
	int m = order(n);	
	if (m==0) return "0";
	while(m > 0)
	{		
		str.push_back('0' + (n/m));
		n = n % m;
		m = m / 10;
	}
	return str;
}

void test_int2str()
{	
	for(int n=0; n!=100; ++n)
	{
		int m;
		int k = rand() % 5;
		if      (k == 0) m = rand() % 10;
		else if (k == 1) m = rand() % 100;
		else if (k == 2) m = rand() % 1000;
		else if (k == 3) m = rand() % 10000;
		else             m = rand() % 100000;

		if      (n == 95) m = 0;
		else if (n == 96) m = 10;
		else if (n == 97) m = 100;
		else if (n == 98) m = 1000;
		else if (n == 99) m = 10000;

		auto s = int2str(m);
		std::cout << "\n" << m << " " << s << " size = " << s.size();
	}
	std::cout << "\n\n";
}

template<typename T, int SZY, int SZX> struct matrix
{
	enum direction { L2R, DOWN, R2L, UP };

	matrix(const T& initial_value) : ptr(new T[SZY * SZX])
	{
		T temp = initial_value;
		for(int y=0; y!=SZY; ++y)
		{
			for(int x=0; x!=SZX; ++x)
			{
				ptr[y*SZX+x] = temp;
				++temp;
			}
		}		
	}

	void spiral_traverse(std::vector<T>& ans)
	{
		int offset_y = 0;
		int offset_x = 0;
		int opsize_y = SZY;
		int opsize_x = SZX;

		while(opsize_y > 1 && opsize_x > 1)
		{
			line_traverse(offset_y,            offset_x,            opsize_x-1, L2R,  ans);
			line_traverse(offset_y,            offset_x+opsize_x-1, opsize_y-1, DOWN, ans);
			line_traverse(offset_y+opsize_y-1, offset_x+opsize_x-1, opsize_x-1, R2L,  ans);
			line_traverse(offset_y+opsize_y-1, offset_x,            opsize_y-1, UP,   ans);
			opsize_y -= 2;
			opsize_x -= 2;
			++offset_y;
			++offset_x;
		}		
		if (opsize_y == 1)
		{
			line_traverse(offset_y, offset_x, opsize_x, L2R, ans);
		}
		else if (opsize_x == 1)
		{
			line_traverse(offset_y, offset_x, opsize_y, DOWN, ans);
		}
	}

	void line_traverse(int y, int x, int len, direction dir, std::vector<T>& ans)
	{		
		for(int n=0; n!=len; ++n)
		{
			ans.push_back(ptr[y*SZX+x]);
			if      (dir == L2R)  ++x;
			else if (dir == DOWN) ++y;
			else if (dir == R2L)  --x;
			else                  --y;
		}
	}

	void debug() const
	{
		for(int y=0; y!=SZY; ++y)
		{			
			for(int x=0; x!=SZX; ++x) std::cout << ptr[y*SZX+x] << " ";
			std::cout << "\n";
		}
	}
	
	T* ptr;
};


void test_matrix_spiral()
{
	// Test all combinations : 1x1, 2x2, 3x3, 1x100 ...
	matrix<int, 8, 7> m(0);
	m.debug();

	std::vector<int> vec;
	m.spiral_traverse(vec);

	std::cout << "\n\n";
	for(auto& x : vec) std::cout << x << " ";
	std::cout << "\n\n";
}

