#include<iostream>
#include<algorithm>
#include<string>
#include<unordered_map>

inline int count_anagram(const std::string& s0, const std::string& s1)
{
	std::unordered_map<char, int> m0;
	std::unordered_map<char, int> m1;

	for (auto c : s0)
	{
		auto iter = m0.find(c);
		if (iter == m0.end()) m0[c] = 1;
		else ++iter->second;
	}
	for (auto c : s1)
	{
		auto iter = m1.find(c);
		if (iter == m1.end()) m1[c] = 1;
		else ++iter->second;
	}

	int common_count = 0;
	for (auto kv : m0)
	{
		auto iter = m1.find(kv.first);
		if (iter != m1.end()) common_count += std::min(kv.second, iter->second);
	}
	return common_count;
}

void test_anagram()
{
	std::cout << "\nAnagram = " << count_anagram("aaabbbbbbcccaaa", "abbbbbbaaaacccded");	
}
