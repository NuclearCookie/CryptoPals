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

private:
	unsigned char Base64ValueToDecimal(char pBase64Value) const;
	char AsciiToHex(char pAsciiValue) const;
	char HexToAscii(char pHexValue) const;

	unsigned int m_Range;
	static const char* BASE64TABLE;
};

