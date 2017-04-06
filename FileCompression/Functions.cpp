#include "Header.h"
#include <cstdio>
#include <ctime>

/*
Converts a string from base 2 to base 10
PARA:
a string of 1's and 0's
RETURN:
the base 10 equivalant of the binary string.
*/
int convertStrToInt(std::string in){
	int num = 0;
	for (int i = 0; i < in.length(); i++){
		char singleDigit = in[i];
		if (singleDigit == '1'){
			num += pow(2, (in.length() - i - 1));
		}
	}
	return num;
}

/*
Converts a string that contains a single character to a string
that contains its ASCII value in 8 bits.
PARA:
An integer that represents a character value
RETURN:
The character's bit value as a string.
*/
std::string charToBitString(int c) {
	std::string output = "        ";
	_asm {
		lea edi, output
		mov eax, c
		mov ebx, 128
		mov ecx, 8
		mov edx, 4
		shiftAndAppend:
		cmp al, 128
			jb lessThan
			mov bl, '1'
			mov[edi + (edx)], bl
			jmp end
			lessThan :
		mov bl, '0'
			mov[edi + (edx)], bl
			end :
		inc edx
			shl al, 1
			loop shiftAndAppend
	}
	return output;
}

/*
Converts a string that contains a single character to a string
that contains its ASCII value in 8 bits.
PARA:
An integer that represents a character value
RETURN:
The character's bit value as a string.
*/
std::string charToBitStringSlow(int c) {
	std::string toReturn = "";
	while (c != 0) {
		if (c % 2 == 0) {
			toReturn = "0" + toReturn;
		}
		else {
			toReturn = "1" + toReturn;
		}
		c /= 2;
	}
	//Prepend 0's if length is too small.
	while (toReturn.length() < 8) {
		toReturn = "0" + toReturn;
	}
	return toReturn;
}

/*
Does a binary search on the vector to find target location
PARA:
A vector of nodes sorted by node.data
A target double value, usually the data of the node to be inserted
RETURN:
The index of the node that was found
*/
int binarySearch(std::vector<Node*> nodeArr, double target) {
	//find a midpoint
	int min = 0;
	int max = nodeArr.size();
	while (min < max) {
		int i = (min + max) / 2;
		//double values will rarely be equal, so find a point where it just fits
		if (nodeArr[i - 1]->data > target) {
			if (nodeArr[i]->data <= target) {
				return i;
			}
			else {
				min = i + 1;
			}
		}
		else {
			max = i - 1;
		}
	}
	return -1;
}

//Used for sorting with std::sort
bool nodeDataGreaterThen(Node* left, Node* right) {
	return left->data > right->data;
}
