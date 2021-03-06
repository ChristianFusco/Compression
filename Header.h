#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

const int MAX_CHAR_VAL = 256;
const int MAX_CHAR_BITS = 8;

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
struct Node {
	char key;
	int data;
	Node *left, *right;
	Node() {
		key = 0;
		data = 0;
		left = nullptr;
		right = nullptr;
	}
};

int convertStrToInt(std::string in);
std::string charToBitString(int c);
//std::string charToBitStringSlow(int c);
void printTree(Node* head);
void printTree(Node* head, std::string spaces);
void insertionSort_inplace(Node** nodes, int size);
#endif // !HEADER_H
