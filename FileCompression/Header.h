#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

const std::string STD_INPUT = "long.txt";
const std::string STD_OUT = "out.txt";
const int MAX_CHAR_VAL = 256;

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
	std::string key;
	double data;
	Node *left, *right;
	Node() {
		key = "";
		data = 0;
		left = nullptr;
		right = nullptr;
	}
};

int convertStrToInt(std::string in);
std::string charToBitString(int c);
std::string charToBitStringSlow(int c);
int binarySearch(std::vector<Node*> nodeArr);
bool nodeDataGreaterThen(Node* left, Node* right);

#endif // !HEADER_H
