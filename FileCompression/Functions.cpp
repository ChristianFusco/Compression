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

void insertionSort_inplace(Node** list, int size) {
	for (int i = size - 1; i >= 0; i--) {
		Node* tmp = list[i];
		int j = i + 1;
		while (j < size && list[j]->data > tmp->data) {
			list[j - 1] = list[j];
			j++;
		}
		list[j - 1] = tmp;
	}
}


void printTree(Node* head) {
	printTree(head, "");
}

void printTree(Node* head, std::string spaces) {
	if (spaces.length() == 9) {
		return;
	}
	if (head->key == 0) {
		std::cout << spaces.length() << " /0" << "\n";
	}
	else
		std::cout << spaces.length() << " " << head->key << "\n";
	if(head->left != NULL)
		printTree(head->left, spaces + " ");
	if(head->right != NULL)
		printTree(head->right, spaces + " ");
}