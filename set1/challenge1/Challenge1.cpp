#include <iostream>
#include <string>
#include <tchar.h>

char Base64Table[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',	//00-25
					'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',		//26-51
					'0','1','2','3','4','5','6','7','8','9',																		//52-61
					'+','/'};																										//62-63

std::string ASCIIToBase64(const char* input, size_t amount)
{
	unsigned int range = amount - amount % 3;
	unsigned int remainder = amount - range;
	unsigned char asciiBytes[3] = {};
	unsigned char mimeBytes[4] = {};
	std::string outputString;
	for(unsigned int i = 0; i < range; i+=3)
	{
		//clear mimeBytes array
		mimeBytes[0] = 0;
		mimeBytes[1] = 0;
		mimeBytes[2] = 0;
		mimeBytes[3] = 0;
		//
		asciiBytes[0] = input[i];
		asciiBytes[1] = input[i+1];
		asciiBytes[2] = input[i+2];
		// 11111111
		// >> 00111111 2
		mimeBytes[0] = asciiBytes[0] >> 2;
		//
		// 11111111
		// 00000011 &
		// 00000011
		mimeBytes[1] = asciiBytes[0] & 3;
		// 00110000
		mimeBytes[1] <<= 4;
		// 11111111
		// 11110000 &
		// 11110000
		// >> 00001111 4 
		// 00001111 
		// 00110000 | 
		// 00111111 OK
		mimeBytes[1] |= ((asciiBytes[1] & 0xF0) >> 4);
		// 11111111
		// 00001111 &
		// 00001111
		mimeBytes[2] = asciiBytes[1] & 0xF;
		// 00111100
		mimeBytes[2] <<= 2;
		// 11111111
		// 11000000 &
		// 11000000
		// >> 00000011 6
		// 00000011
		// 00111100 |
		// 00111111 OK
		mimeBytes[2] |= ((asciiBytes[2] & 0xC0) >> 6);
		// 11111111
		// 00111111 &
		// 00111111 OK
		mimeBytes[3] = asciiBytes[2] & 0x3F;
		//add new chars to output string
		outputString += Base64Table[mimeBytes[0]];
		outputString += Base64Table[mimeBytes[1]];
		outputString += Base64Table[mimeBytes[2]];
		outputString += Base64Table[mimeBytes[3]];
	}
	
	//clear mimeBytes array
	mimeBytes[0] = 0;
	mimeBytes[1] = 0;
	mimeBytes[2] = 0;
	mimeBytes[3] = 0;

	if(remainder != 0)
	{
		if(remainder == 1)
		{
			//has to be atleast 1 then, so start adding first 2 mimeBytes
			mimeBytes[0] = input[range] >> 2;
			mimeBytes[1] = input[range] & 3;
			mimeBytes[1] <<= 4;	

			outputString += Base64Table[mimeBytes[0]];
			outputString += Base64Table[mimeBytes[1]];
			outputString += '=';
			outputString += '=';

		}
		else if(remainder == 2)
		{
			//has to be atleast 1 then, so start adding first 2 mimeBytes
			mimeBytes[0] = input[range] >> 2;
			mimeBytes[1] = input[range] & 3;
			mimeBytes[1] = mimeBytes[1] << 4;
			mimeBytes[1] = mimeBytes[1] | ((input[range + 1] & 0xF0) >> 4);
			mimeBytes[2] = input[range+1] & 0xF;
			mimeBytes[2] = mimeBytes[2] << 2;	

			outputString += Base64Table[mimeBytes[0]];
			outputString += Base64Table[mimeBytes[1]];
			outputString += Base64Table[mimeBytes[2]];
			outputString += '=';
		}
	}
	return outputString;
}

unsigned char Base64TableToDecimal(char base64Char)
{
	if(base64Char >= 'A' && base64Char <= 'Z')
	{
		return base64Char - 'A';
	}
	else if(base64Char >= 'a' && base64Char <= 'z')
	{
		return base64Char - 'a' + 26;
	}
	else if(base64Char >= '0' && base64Char <= '9')
	{
		return base64Char - '0' + 52;
	}
	else if(base64Char == '+')
	{
		return 62;
	}
	else if(base64Char == '/')
	{
		return 63;
	}
	return 0;
}

std::string Base64ToASCII(const char* input, size_t amount)
{
	// 4 entries to 3 entries...
	int range = amount - amount % 4;
	unsigned char base64Values[4] = {};
	char asciiValues[3] = {};
	std::string outputString;
	for(int i = 0; i < range; i+=4)
	{
		base64Values[0] = Base64TableToDecimal(input[i]);
		base64Values[1] = Base64TableToDecimal(input[i+1]);
		base64Values[2] = Base64TableToDecimal(input[i+2]);
		base64Values[3] = Base64TableToDecimal(input[i+3]);

		//00111111
		//11111100 << 2
		asciiValues[0] = base64Values[0];
		asciiValues[0] <<= 2;
		//11111100
		//00110000
		//00000011 >> 4
		//11111111 |
		asciiValues[0] |= ((base64Values[1] & 0x30) >> 4);

		//00001111
		//11110000 << 4
		asciiValues[1] = base64Values[1] & 0xF;
		asciiValues[1] <<= 4;
		//11110000
		//00111100
		//00001111 >> 2
		//11111111 |
		asciiValues[1] |= ((base64Values[2] & 0x3C) >> 2);

		//00000011
		//11000000 << 6
		asciiValues[2] = base64Values[2] & 0x3;
		asciiValues[2] <<= 6;
		//11000000
		//00111111 |
		asciiValues[2] |= base64Values[3] & 0x3F;
		outputString += asciiValues[0];
		outputString += asciiValues[1];
		outputString += asciiValues[2];
	}

	return outputString;
}

char CharToHexChar(char input)
{
	if(input >= '0' && input <= '9')
	{
		return input - '0';
	}
	else if(input >= 'A' && input <= 'F')
	{
		return input - 'A' + 10;
	}
	else if(input >= 'a' && input <= 'f')
	{
		return input - 'a' + 10;
	}
	return 0;
}

std::string HexToBase64(const char* input, size_t amount) 
{
	//single hex values can only take up to 4 bits (F == 1111) so convert 6 hex values to 4 base64 values
	unsigned int range = amount - (amount % 3);
	unsigned int remainder = amount - range;
	unsigned char base64Values[2] = {};
	unsigned char hexValues[3] = {};
	std::string outputString;
	for(unsigned int i = 0; i < range; i+=3)
	{
		base64Values[0] = 0;
		base64Values[1] = 0;
		hexValues[0] = CharToHexChar(input[i]);
		hexValues[1] = CharToHexChar(input[i+1]);
		hexValues[2] = CharToHexChar(input[i+2]);
		base64Values[0] = hexValues[0];
		base64Values[0] <<= 2;
		base64Values[0] |= ((hexValues[1] & 0xC) >> 2);

		base64Values[1] = hexValues[1] & 0x3;
		base64Values[1] <<= 4;
		base64Values[1] |= hexValues[2];

		outputString += Base64Table[base64Values[0]];
		outputString += Base64Table[base64Values[1]];
	}
	base64Values[0] = 0;
	base64Values[1] = 0;
	if(remainder == 1)
	{
		hexValues[0] = CharToHexChar(input[range]);

		base64Values[0] = hexValues[0];
		base64Values[0] <<= 2;

		outputString += Base64Table[base64Values[0]];
		outputString += '=';
	}
	else if(remainder == 2)
	{
		hexValues[0] = CharToHexChar(input[range]);
		hexValues[1] = CharToHexChar(input[range+1]);

		base64Values[0] = hexValues[0];
		base64Values[0] <<= 2;
		base64Values[0] |= ((hexValues[1] & 0xC) >> 2);

		base64Values[1] = hexValues[1] & 0x3;
		base64Values[1] <<= 4;

		outputString += Base64Table[base64Values[0]];
		outputString += Base64Table[base64Values[1]];
	}
	return outputString;
}

int _tmain(int argc, _TCHAR* argv[])
{
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
		switch(choice)
		{
		case 1:
			std::cout << ASCIIToBase64(inputString.c_str(), inputString.size());
			break;
		case 2:
			std::cout << Base64ToASCII(inputString.c_str(), inputString.size());
			break;
		case 3:
			std::cout << HexToBase64(inputString.c_str(), inputString.size());
			break;
		case 4:
			std::cout << "Currently unimplemented :(" << std::endl;
			break;
		default:
			std::cout << "Unknown number!" << std::endl;
			break;
		}
		std::cout << std::endl << std::endl << "new conversion? (Y/N)" << std::endl;
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
	return 0;
}
