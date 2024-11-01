#include <iostream>
#include "SHA256.h"

int main(int argc, const char** argv)
{
	if (argc == 2)
	{	
		SHA256 algo;
		return algo.Hash(argv[1]);
	}
	
	std::cout << "You did not pass a path to a file.\n";
	return 1;
};