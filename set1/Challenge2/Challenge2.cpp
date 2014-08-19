#include <iostream>
#include <string>
#include <tchar.h>
#include "Challenge2.h"

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
	std::cout << "Fixed XOR" << std::endl;
	std::cout << "**********************************" << std::endl << std::endl;
	bool retry = false;
	do
	{
		retry = false;
		std::cout << "Input HexString" << std::endl;
		std::string inputString;
		std::cin >> inputString;
		std::cout << std::endl << "XOR value HexString" << std::endl;
		std::string xorString;
		std::cin >> xorString;
		if(inputString.size() == xorString.size())
		{
			char* inputStream = new char[inputString.size()];
			char* xorStream = new char[xorString.size()];
			m_BitSequence->AsciiToHexArray(inputString.c_str(), inputStream, inputString.size());
			m_BitSequence->AsciiToHexArray(xorString.c_str(), xorStream, xorString.size());
			m_BitSequence->XOREqualSizeBuffers(inputStream, xorStream, inputString.size());
			m_BitSequence->HexToAsciiArray(inputStream, inputStream, inputString.size());
			std::string outputString(inputStream, inputString.size());
			std::cout << std::endl << "Result after XOR: " << std::endl;
			std::cout << outputString << std::endl;
			std::cout << std::endl << "Would you like to do another conversion? (Y/N)" << std::endl;
			bool correctAnswer = false;
			while(!correctAnswer)
			{
				char answer;
				std::cin >> answer;
				if(answer == 'N' || answer == 'n')
				{
					correctAnswer = true;
				}
				else if(answer == 'Y' || answer == 'y')
				{
					correctAnswer = true;
					retry = true;
				}
				else if(!std::cin || correctAnswer == false)
				{
					std::cin.clear(); // unset failbit
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
					std::cout << "Unrecognised answer.. try again!" << std::endl;
				}
			}
			delete[] inputStream;
			delete[] xorStream;
		}
		else
		{
			std::cout << "input size does not match output size! Please make sure this is the case." << std::endl << std::endl;
			retry = true;
		}
	}
	while(retry);

	Terminate();
	return 0;
}
