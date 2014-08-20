#include "BitSequence.h"

const char* BitSequence::BASE64TABLE = 
{"ABCDEFGHIJKLMNOPQRSTUVWXYZ" //00-25
"abcdefghijklmnopqrstuvwxyz"  //26-51
"0123456789"				  //52-61
"+/"};						  //62-63

BitSequence::BitSequence(void)
	: m_Range(0)
{
}


BitSequence::~BitSequence(void)
{
}

unsigned char BitSequence::Base64ValueToDecimal(char pBase64Char) const
{
	if(pBase64Char >= 'A' && pBase64Char <= 'Z')
	{
		return pBase64Char - 'A';
	}
	else if(pBase64Char >= 'a' && pBase64Char <= 'z')
	{
		return pBase64Char - 'a' + 26;
	}
	else if(pBase64Char >= '0' && pBase64Char <= '9')
	{
		return pBase64Char - '0' + 52;
	}
	else if(pBase64Char == '+')
	{
		return 62;
	}
	else if(pBase64Char == '/')
	{
		return 63;
	}
	return 0;
}

void BitSequence::Base64ToAscii(const char* pInput, size_t pCount, std::string & pOutputString)
{
	// 4 entries to 3 entries...
	m_Range = pCount - pCount % 4;
	unsigned char base64Values[4] = {};
	char asciiValues[3] = {};
	for(unsigned int i = 0; i < m_Range; i+=4)
	{
		for(unsigned int j = 0; j < 4; ++j)
		{
			base64Values[j] = Base64ValueToDecimal(pInput[i+j]);
		}
		
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

		for(unsigned int j = 0; j < 3; ++j)
		{
			pOutputString += asciiValues[j];
			asciiValues[j] = 0;
		}
	}

	for(unsigned int i = 0; i < pCount - m_Range; ++i)
	{
		if(i == 0)
		{
			base64Values[0] = Base64ValueToDecimal(pInput[m_Range+i]);
			asciiValues[0] = base64Values[0];
			asciiValues[0] <<= 2;
		}
		else if(i == 1)
		{
			base64Values[1] = Base64ValueToDecimal(pInput[m_Range+i]);
			asciiValues[0] |= ((base64Values[1] & 0x30) >> 4);
			//00001111
			//11110000 << 4
			asciiValues[1] = base64Values[1] & 0xF;
			asciiValues[1] <<= 4;
		}
		else if(i == 2)
		{
			base64Values[2] = Base64ValueToDecimal(pInput[m_Range+i]);
			asciiValues[1] |= ((base64Values[2] & 0x3C) >> 2);
			//00000011
			//11000000 << 6
			asciiValues[2] = base64Values[2] & 0x3;
			asciiValues[2] <<= 6;	
		}
	}

	if(pCount - m_Range != 0)
	{
		for(unsigned int i = 0; i < 3; ++i)
		{
			pOutputString += asciiValues[i];
		}

	}
}


void BitSequence::AsciiToBase64(const char* pInput, size_t pCount, std::string & pOutputString)
{
	m_Range = pCount - pCount % 3;
	unsigned char asciiBytes[3] = {};
	unsigned char mimeBytes[4] = {};
	for(unsigned int i = 0; i < m_Range; i+=3)
	{
		for(unsigned int j = 0; j < 3; ++j)
		{
			asciiBytes[j] = pInput[i+j];
		}

		// 11111111
		// 00111111 >> 2
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
		// 00001111 >> 4 
		// 00001111 
		// 00110000 | 
		// 00111111 
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
		// 00000011 >> 6
		// 00000011
		// 00111100 |
		// 00111111 OK
		mimeBytes[2] |= ((asciiBytes[2] & 0xC0) >> 6);
		// 11111111
		// 00111111 &
		// 00111111 OK
		mimeBytes[3] = asciiBytes[2] & 0x3F;
		//add new chars to output string
		for(unsigned int j = 0; j < 4; ++j)
		{
			pOutputString += BASE64TABLE[mimeBytes[j]];
			mimeBytes[j] = 0;
		}
	}

	for(unsigned int i = 0; i < pCount - m_Range; ++i)
	{
		if(i == 0)
		{
			//has to be atleast 1 then, so start adding first 2 mimeBytes
			mimeBytes[0] = pInput[m_Range] >> 2;
			mimeBytes[1] = pInput[m_Range] & 3;
			mimeBytes[1] <<= 4;	
		}
		else if(i == 1)
		{
			mimeBytes[1] |= ((pInput[m_Range + i] & 0xF0) >> 4);
			mimeBytes[2] = pInput[m_Range+i] & 0xF;
			mimeBytes[2] = mimeBytes[2] << 2;	
		}
	}

	if(pCount - m_Range != 0)
	{
		pOutputString += BASE64TABLE[mimeBytes[0]];
		pOutputString += BASE64TABLE[mimeBytes[1]];
		pOutputString += pCount - m_Range == 2 ? BASE64TABLE[mimeBytes[2]] : '=';
		pOutputString += '=';
	}
}

char BitSequence::AsciiToHex(char pAsciiValue) const
{
	if(pAsciiValue >= '0' && pAsciiValue <= '9')
	{
		return pAsciiValue - '0';
	}
	else if(pAsciiValue >= 'A' && pAsciiValue <= 'F')
	{
		return pAsciiValue - 'A' + 10;
	}
	else if(pAsciiValue >= 'a' && pAsciiValue <= 'f')
	{
		return pAsciiValue - 'a' + 10;
	}
	return 0;
}

void BitSequence::HexToBase64(const char* pInput, size_t pCount, std::string & pOutputString) 
{
	//single hex values can only take up to 4 bits (F == 1111) so convert 6 hex values to 4 base64 values
	m_Range = pCount - (pCount % 3);
	unsigned char base64Values[2] = {};
	unsigned char hexValues[3] = {};
	for(unsigned int i = 0; i < m_Range; i+=3)
	{
		for(unsigned int j = 0; j < 3; ++j)
		{
			hexValues[j] = AsciiToHex(pInput[i+j]);
		}
		
		//00001111
		//00111100 << 2
		//00001111 
		//00001100 &
		//00000011 >> 2
		//00111111 |
		base64Values[0] = hexValues[0];
		base64Values[0] <<= 2;
		base64Values[0] |= ((hexValues[1] & 0xC) >> 2);

		//00000011
		//00110000 << 4
		//00001111 |
		//00111111
		base64Values[1] = hexValues[1] & 0x3;
		base64Values[1] <<= 4;
		base64Values[1] |= hexValues[2];

		for(unsigned int j = 0; j < 2; ++j)
		{
			pOutputString += BASE64TABLE[base64Values[j]];
			base64Values[j] = 0;
		}
	}

	for(unsigned int i = 0; i < pCount - m_Range; ++i)
	{
		if(i == 0)
		{	
			hexValues[0] = AsciiToHex(pInput[m_Range]);

			base64Values[0] = hexValues[0];
			base64Values[0] <<= 2;
		}
		else if(i == 1)
		{
			base64Values[0] |= ((hexValues[1] & 0xC) >> 2);

			base64Values[1] = hexValues[1] & 0x3;
			base64Values[1] <<= 4;
		}
	}

	if(pCount - m_Range != 0)
	{
		pOutputString += BASE64TABLE[base64Values[0]];
		pOutputString += pCount - m_Range == 2 ? BASE64TABLE[base64Values[1]] : '=';
	}
}

char BitSequence::HexToAscii(char pHexValue) const
{
	if(pHexValue >= 0 && pHexValue <= 9)
	{
		return pHexValue + '0';
	}
	else if(pHexValue >= 10 &&  pHexValue <= 16)
	{
		return pHexValue - 10 + 'A';
	}
	return 0;
}

void BitSequence::Base64ToHex(const char* pInput, size_t pCount, std::string & pOutputString)
{
	//6 bit to 4 bit and decoded to text
	m_Range = pCount - pCount % 2;
	unsigned char base64Values[2] = {};
	unsigned char hexValues[3] = {};
	for(unsigned int i = 0; i < m_Range; i+=2)
	{
		for(unsigned int j = 0; j < 2; ++j)
		{
			base64Values[j] = Base64ValueToDecimal(pInput[i+j]);
		}

		//00111111
		//00001111 >> 2
		hexValues[0] = base64Values[0] >> 2;

		//00000011
		//00001100 << 2
		//00111111
		//00110000 &
		//00000011 >> 4
		//00001111 | 
		hexValues[1] = base64Values[0] & 0x3;
		hexValues[1] <<= 2;
		hexValues[1] |= ((base64Values[1] & 0x30) >> 4);

		//00001111
		hexValues[2] = base64Values[1] & 0xF;

		for(unsigned int j = 0; j < 3; ++j)
		{
			pOutputString += HexToAscii(hexValues[j]);
			hexValues[j] = 0;
		}
	}
	if(pCount - m_Range != 0)
	{
		base64Values[0] = Base64ValueToDecimal(pInput[m_Range]);
		hexValues[0] = base64Values[0] >> 2;
		hexValues[1] = base64Values[0] & 0x3;
		hexValues[1] <<= 2;

		for(unsigned int j = 0; j < 3; ++j)
		{
			pOutputString += HexToAscii(hexValues[j]);
			hexValues[j] = 0;
		}
	}
}

void BitSequence::HexToAsciiArray(const char* pInput, char* pOutput, size_t pCount)
{
	for(unsigned int i = 0; i < pCount; ++i)
	{
		pOutput[i] = HexToAscii(pInput[i]);
	}
}

void BitSequence::AsciiToHexArray(const char* pInput, char* pOutput, size_t pCount)
{
	for(unsigned int i = 0; i < pCount; ++i)
	{
		pOutput[i] = AsciiToHex(pInput[i]);
	}
}
