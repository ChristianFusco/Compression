#include "Header.h"

/*
Sometimes eof gets printed when it shouldn't be.  This prevents an output of 00000000,
aka eof.  This should also prevent the printing of extra chars at the eof.

PARA:
a string of 1's and 0's that has a length of 7.
RETURN:
on success: the string with a 1 before it.
on failure: returns zeros, which translates to a null char
*/
std::string addParityBit(std::string input)
{
	if (input.length() == 7)
		return "1" + input;
	std::cout << "parity err";
	return "00000000";
}

/*
Converts a string from base 2 to base 10
PARA:
a string of 1's and 0's
RETURN:
the base 10 equivalant of the binary string.
*/
int convertStrToInt(std::string in)
{
	int num = 0;
	for (int i = 0; i < in.length(); i++)
	{
		char singleDigit = in[i];
		if (singleDigit == '1')
		{
			num += pow(2, (in.length() - i - 1));
		}
	}
	return num;
}
