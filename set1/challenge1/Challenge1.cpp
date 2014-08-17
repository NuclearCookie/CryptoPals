#include <iostream>
#include <string>
#include <tchar.h>
#include <limits>
#include "Challenge1.h"

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
	std::cout << "Base64 Encoder - Decoder" << std::endl;
	std::cout << "**********************************" << std::endl << std::endl;
	bool done = false;
	while(!done)
	{
		std::cout << "Please enter number of the desired conversion:" << std::endl;
		std::cout << "1. ASCII -> Base64" << std::endl;
		std::cout << "2. Base64 -> ASCII" << std::endl; 
		std::cout << "3. hex -> Base64" << std::endl;
		std::cout << "4. Base64 -> hex" << std::endl;
		int choice = -1;
		while (choice == -1)
		{
			std::cin >> choice;
			if(!std::cin || choice < 0 || choice > 4)
			{
	            std::cin.clear(); // unset failbit
	            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
				choice = -1;
				std::cout << "Incorrect number. Try again" << std::endl;
			}
		}
		std::cout << "Please provide data to convert..." << std::endl << std::endl;
		std::string inputString;
		std::cin >> inputString;
		std::cout << std::endl << "OUTPUT:" << std::endl << std::endl;
		std::string outputString;
		switch(choice)
		{
		case 1:
			m_BitSequence->AsciiToBase64(inputString.c_str(), inputString.size(), outputString);
			break;
		case 2:
			m_BitSequence->Base64ToAscii(inputString.c_str(), inputString.size(), outputString);
			break;
		case 3:
			m_BitSequence->HexToBase64(inputString.c_str(), inputString.size(), outputString);
			break;
		case 4:
			m_BitSequence->Base64ToHex(inputString.c_str(), inputString.size(), outputString);
			break;
		default:
			std::cout << "Unknown number!" << std::endl;
			break;
		}
		std::cout << outputString << std::endl << std::endl;
		std::cout << "new conversion? (Y/N)" << std::endl;
		bool correctAnswer = false;
		while(!correctAnswer)
		{
			char answer;
			std::cin >> answer;
			if(answer == 'N' || answer == 'n')
			{
				correctAnswer = true;
				done = true;
			}
			else if(answer == 'Y' || answer == 'y')
			{
				correctAnswer = true;
			}
			else if(!std::cin || correctAnswer == false)
			{
				std::cin.clear(); // unset failbit
	            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
				std::cout << "Unrecognised answer.. try again!" << std::endl;
			}
		}
	}
	Terminate();
	return 0;
}
