#include <iostream>
#include <string>
#include <tchar.h>
#include "Challenge3.h"

void Initialize()
{
	m_BitSequence = new BitSequence();
}

void Terminate()
{
	delete m_BitSequence;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Initialize();
	std::cout << "**********************************" << std::endl;
	std::cout << "Single-byte XOR cipher" << std::endl;
	std::cout << "**********************************" << std::endl << std::endl;
	bool retry = false;
	do
	{
	}
	while(retry);

	Terminate();
	return 0;
}
