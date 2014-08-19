#pragma once
#include <string>

class BitSequence
{
public:
	BitSequence(void);
	~BitSequence(void);

	void Base64ToAscii(const char* pInput, size_t pCount, std::string & pOutputString);
	void AsciiToBase64(const char* pInput, size_t pCount, std::string & pOutputString);

	void Base64ToHex(const char* pInput, size_t pCount, std::string & pOutputString);
	void HexToBase64(const char* pInput, size_t pCount, std::string & pOutputString);

	template<typename T>
	void XOREqualSizeBuffers(T pInputOutput[], const T pXORValue[], size_t pCount)
	{
		for(size_t i = 0; i < pCount; ++i)
		{
			pInputOutput[i] ^= pXORValue[i];
		}
	}

	void HexToAsciiArray(const char* pInput, char* pOutput, size_t pCount);
	void AsciiToHexArray(const char* pInput, char* pOutput, size_t pCount);

private:
	unsigned char Base64ValueToDecimal(char pBase64Value) const;
	char AsciiToHex(char pAsciiValue) const;
	char HexToAscii(char pHexValue) const;
	

	unsigned int m_Range;
	static const char* BASE64TABLE;
};

