#include<iostream>
#include<filesystem>

void test_filesystem()
{
	// (1) basic information
	std::filesystem::path s("D:/DEV/cplusplus/TraderRun source");
	std::cout << "\nexists()        = " << std::filesystem::exists(s)
			  << "\nroot_name()     = " << s.root_name()
			  << "\nroot_path()     = " << s.root_path()
			  << "\nrelative_path() = " << s.relative_path()
			  << "\nparent_path()   = " << s.parent_path() 
			  << "\nfilename()      = " << s.filename() 
			  << "\nstem()          = " << s.stem() 
			  << "\nextension()     = " << s.extension() << "\n\n";

	// (2) check if it is a file
	std::filesystem::path s0("D:/DEV/cplusplus/TraderRun source/Parser");
	std::filesystem::path s1("D:/DEV/cplusplus/TraderRun source/Parser/primitive_BCD.h");
	std::filesystem::path s2("D:/DEV/cplusplus/Yubo/main.cpp");
	std::cout << "\n" << s0 << (std::filesystem::is_regular_file(s0)? " is" : " is not") << " a file.";
	std::cout << "\n" << s1 << (std::filesystem::is_regular_file(s1)? " is" : " is not") << " a file.";
	std::cout << "\n" << s2 << (std::filesystem::is_regular_file(s2)? " is" : " is not") << " a file.";
	std::cout << "\n\n";

	// (3) traverse a directory
	std::cout << "\ndir " << s;
	if (std::filesystem::exists(s) && 
		std::filesystem::is_directory(s))
	{				
	//	for(const auto& x : std::filesystem::directory_iterator(s)) // non-recursive
		for(const auto& x : std::filesystem::recursive_directory_iterator(s))
		{
			auto str = x.path().filename();
			if      (std::filesystem::is_directory(x.status()))         std::cout << "\nfolder  --- " << str;
			else if (std::filesystem::is_regular_file(x.status()))		std::cout << "\nfile    --- " << str;
			else														std::cout << "\nunknown --- " << str;
		}
	}
	std::cout << "\n\n";
}
