#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Header.h"

/*
Use these nodes to create a huffman tree.
In this case, the key is the value the tree searches
based on.  This is always the unsigned character value
of some letter or symbol (0-255).  Data holds the number
of times that key is used in your document.

Nodes with a key of -1 DONT contain letters.  These bridge
the gaps between letter nodes.  Their data variable contains
the number of times that ALL of their children are used in your document.

So, if a -1 node has two children that have data values of 7 and 14,
the -1 node's data value is 21.
*/
struct Node{
	int key, data;
	Node *left, *right;
	Node(){
		key = 0;
		data = 0;
		left = nullptr;
		right = nullptr;
	}
};

class HuffmanTree{
private:
	Node *root;
	std::string hashTable[256];
	long long unsigned int numChar;

	void fillHash();
	void fillTree(std::vector<Node*> nodeArr);
	int *getTextAnalysis();
	void output(Node *ptr, int count);
	std::string search(Node *ptr, std::string output, int key);
	int search(std::string binaryStr);

public:
	HuffmanTree();

	void decode(std::string file);
	void inline output(){ output(root, 0); };
	void output(std::string file);
	std::string inline search(int key) { return search(root, "", key); };
};

/*
Uncompresses the file.
PARA:
The output file name.
*/
void HuffmanTree::decode(std::string file)
{
	std::ifstream myFile(STD_OUT);
	std::ofstream myFileOut(file);

	//keeps track of the number of characters, to find eof.
	long long unsigned int charCounter = 0;
	if (myFile.is_open())
	{
		char input = NULL;
		std::string fullChar = "";
		while (myFile.get(input))
		{
			std::string partChar = fullChar;
			fullChar = "";

			int tempInput = input;
			//converts the input to an unsigned version
			if (tempInput < 0)
				tempInput += MAX_CHAR_VAL;
			//converts the input to a binary value
			//binary value will be backwards
			while (tempInput > 0)
			{
				if (tempInput % 2 == 0)
					fullChar += "0";
				else
				{
					fullChar += "1";
				}
				tempInput /= 2;
			}
			//Adds leading zeros if neccessary, redudant with parity bit.
			//while (fullChar.length() != 8)
			//	fullChar += "0";
			std::string temp = "";
			//reverses the binary string
			for (int i = 0; i < fullChar.length(); i++)
			{
				temp += fullChar[fullChar.length() - i - 1];
			}
			//delete parity bit
			temp.erase(0, 1);
			//combines leftover input with current input
			fullChar = partChar + temp;
			partChar = "";
			while (fullChar.length() != 0)
			{
				bool cont = false;
				//if search != -1, a character was found
				while (-1 == search(partChar) && !cont)
				{
					//continues if our binary string doesn't contain a full character
					if (fullChar.length() == 0)
						cont = true;
					else
					{
						//splits the full binary string into bits.
						partChar += fullChar[0];
						fullChar.erase(0, 1);
					}
				}
				if (cont)
				{
					//continues if our binary string doesn't contain a full character
					//saves leftover input
					fullChar = partChar;
					break;
				}
				else
				{
					//outputs if not at eof.
					//lel worst code 2k15
					if (charCounter < numChar)
						myFileOut << (char)(search(partChar) - (MAX_CHAR_VAL / 2));
					partChar = "";
				}
			}
		}
	}
	myFile.close();
	myFileOut.close();
}

void HuffmanTree::fillHash()
{
	for (int i = 0; i < MAX_CHAR_VAL; i++)
	{
		hashTable[i] = search(i);
	}
}

/*

*/
void HuffmanTree::fillTree(std::vector<Node*> nodeArr)
{
	if (nodeArr.size() == 1)
	{
		root = nodeArr.front();
	}
	else
	{
		Node *smallPtr1, *smallPtr2;
		smallPtr1 = nodeArr.back();
		nodeArr.pop_back();
		smallPtr2 = nodeArr.back();
		nodeArr.pop_back();

		Node *head = new Node;
		head->data = smallPtr1->data + smallPtr2->data;
		head->left = smallPtr1;
		head->right = smallPtr2;
		head->key = -1;

		int size = nodeArr.size();
		for (int i = 0; i < size; i++)
		{
			if (i == size - 1 && head->data <= nodeArr[i]->data)
				nodeArr.push_back(head);
			else if (head->data > nodeArr[i]->data)
			{
				nodeArr.emplace(nodeArr.begin() + i, head);
				break;
			}
		}
		if (size == 0)
		{
			nodeArr.push_back(head);
		}
		fillTree(nodeArr);
	}
}

int *HuffmanTree::getTextAnalysis()
{
	int *characters = new int[MAX_CHAR_VAL];
	std::ifstream myFile(STD_INPUT);
	for (int i = 0; i < MAX_CHAR_VAL; i++)
	{
		characters[i] = NULL;
	}
	if (myFile.is_open())
	{
		char input = NULL;
		while (myFile.get(input))
		{
			characters[(int)input + MAX_CHAR_VAL / 2] += 1;
		}
	}
	myFile.close();
	return characters;
}

//key holds letter, data holds freq
HuffmanTree::HuffmanTree()
{
	numChar = 0;
	int *characters = getTextAnalysis();
	std::vector<Node*> nodeArr;
	for (int i = 0; i < MAX_CHAR_VAL; i++)
	{
		if (characters[i] != 0)
		{
			Node *tmp = new Node;
			tmp->key = i;
			tmp->data = characters[i];
			nodeArr.push_back(tmp);
			for (int j = 0; j < nodeArr.size(); j++)
			{
				int k = j;
				while (k > 0 && nodeArr[k - 1]->data < nodeArr[k]->data)
				{
					tmp = nodeArr[k];
					nodeArr[k] = nodeArr[k - 1];
					nodeArr[k - 1] = tmp;
					k--;
				}
			}
		}
	}
	fillTree(nodeArr);
	fillHash();
}

void HuffmanTree::output(std::string file)
{
	std::ifstream myIn(STD_INPUT);
	std::ofstream myOut(file);
	if (myIn.is_open())
	{
		char input = NULL;
		std::string binaryString = "";
		while (myIn.get(input))
		{
			numChar++;
			unsigned char input2 = input + MAX_CHAR_VAL / 2;
			std::string temp = hashTable[(int)input2];
			binaryString += temp;
			while (binaryString.length() > 6)  //CHANGE
			{
				std::string word = addParityBit(binaryString.substr(0, 7));
				char output = (int)convertStrToInt(word);
				myOut << output;
				binaryString = binaryString.substr(7, binaryString.length() - 7);
			}
		}
		while (binaryString.length() != 7)
			binaryString += "0";
		binaryString = addParityBit(binaryString);
		char output = (int)convertStrToInt(binaryString);
		myOut << output;
	}
	myIn.close();
}

void HuffmanTree::output(Node *ptr, int count)
{
	for (int i = 0; i < count; i++)
	{
		std::cout << " ";
	}
	std::cout << ptr->key << "/" << ptr->data << "\n";
	count++;
	if (ptr->left != nullptr)
		output(ptr->left, count);
	if (ptr->right != nullptr)
		output(ptr->right, count);
}

std::string HuffmanTree::search(Node *ptr, std::string output, int key)
{
	if (ptr->key == key)
		return output;
	if (ptr->key != -1)
		return "";
	std::string tempOutput;
	tempOutput = search(ptr->left, output + "0", key);
	if (tempOutput != "")
		return tempOutput;
	tempOutput = search(ptr->right, output + "1", key);
	if (tempOutput != "")
		return tempOutput;
	return "";
}

int HuffmanTree::search(std::string binaryStr)
{
	Node *ptr = root;
	int out = 0;
	for (int i = 0; i < binaryStr.length(); i++)
	{
		if (binaryStr[i] == '0')
			ptr = ptr->left;
		else if (binaryStr[i] == '1')
			ptr = ptr->right;
	}
	out = ptr->key;
	return out;
}