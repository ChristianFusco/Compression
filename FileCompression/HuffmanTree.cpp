#include "HuffmanTree.h"
#include <queue>

/*
Given some binary string, search for a node form the tree.  This node will
contain a string with a single character.  If the character was not found,
currentChar will be null and the character will be printed as normal.

PARA:
out			- some output stream, usually the soon to be uncompressed file
fullChar	- A string of 1's and 0's of at least length 9.  If fullChar is
			ever less than 9 characters, it's probably at eof for input.
*/
void HuffmanTree::printChar(std::ofstream &out, std::string &fullChar) {
	Node* currentChar = getNodeFromTree(fullChar);
	if (currentChar != NULL) {
		//character was found, just print it's key
		out << currentChar->key;
	}
	else {
		//The character wasn't found, so print it as plaintext
		//It's the normal char value prepended by 0
		//A == 0+01100001
		out << (char)convertStrToInt(fullChar.substr(1, MAX_CHAR_BITS));
		fullChar.erase(0, MAX_CHAR_BITS + 1);
	}
}

/*
Decompresses a file.  Uses STD_OUT as the file to decompress.  
Characters are read one at a time, changed to binary format,
and then replaced using the tree.  Replacements require at least
9 bits, since that's the largest representation of an uncompressed
char.  Replacements don't require 9 bits at the end of the file,
any zeros can just be thrown out.

PARA:
file - The filename of the soon to be uncompressed file.
*/
void HuffmanTree::decode(std::string file)
{
	//it has to be binary or WINDOWS DECIDES IT KNOWS BETTER THAN NI DO
	//AND IT INSERTS RANDOM CHARACTERS
	std::ifstream in(file, std::ios::in | std::ios::binary);
	std::ofstream out("long2.txt", std::ios::out | std::ios::binary);

	if (in.is_open()) {
		char input = NULL;
		std::string fullChar = "";	//this will save leftover bits from each char
		while (in.get(input)) {
			fullChar = fullChar + charToBitString(input);

			//ensures theres enough characters for a swap
			while (fullChar.length() > MAX_CHAR_BITS) {
				printChar(out, fullChar);
			}
		}
		//sometimes garbage will be left in the file so kick that out
		while (fullChar.length() > 0) {
			printChar(out, fullChar);
		}
	}
	in.close();
	out.close();
}

/*
Moves each of the character values of the HuffmanTree into a table for faster access.

Searching the huffman tree over and over again wastes time.
Since input is often just a character, replacements can be found in O(1).

If a value does not exist in the tree, it's replaced with 0+it's original value.
*/
void HuffmanTree::fillHash()
{
	for (int i = 0; i < MAX_CHAR_VAL; i++) {
		//lul casting magic
		std::string searchWord = search((char)i);
		if (searchWord != "" && searchWord.length() < 9) {
			hashTable[i] = searchWord;
		}
		else {
			//Character does not exist in tree
			hashTable[i] = "0" + charToBitString(i);
		}
	}
}

/*
This function fills the Huffman Tree.
The left child of the root will always be an escape bit.
The right child will be the root of a standard HuffmanTree
PARA:
A list of descending nodes with no children.  The list is shortened
in order to keep characters from being represented as more than 9 bits each.
*/
void HuffmanTree::setUpFill(Node** nodeArr) {
	root = new Node;
	Node *escape = new Node;
	Node* toReturn = NULL;
	root->right = fillTree(nodeArr, 256);
	root->left = escape;
}

/*
A recursive function that builds a Huffman tree based on the contents of the
parameter.

Nodes with a key of "" are bridges between other nodes.  These nodes will never be leaves.
Nodes that have non-empty keys will never have children.

The absolute maximum number of times that this function can be on the stack is near
the 255 range at this moment.  This is in a file where all characters apppear at roughly
the same frequency.

PARA:
The current array of nodes.
These nodes will never have parents.
They will be a mix of bridge nodes, with children, and data nodes, that are leaves.
RETURN:
The root of the HuffmanTree.
*/
Node* HuffmanTree::fillTree(Node** arrPtr, int index)
{
	//continue until there is only one item in the list
	while (index != 1) {
		index--;
		if (!arrPtr[index]->data) {
			delete arrPtr[index];
			arrPtr[index] = NULL;
			continue;
		}
		//allocate a new node
		//This is a bridge node between the two worst nodes in the list
		Node* bridge = new Node;
		bridge->right = arrPtr[index];
		bridge->left = arrPtr[index - 1];
		bridge->data = arrPtr[index]->data + arrPtr[index - 1]->data;
		arrPtr[index] = NULL;
		arrPtr[index - 1] = bridge;
		insertionSort_inplace(arrPtr, index);
	}
	return arrPtr[0];
}

/*
Returns an array of all of the characters in the file, and what percentage
they make up of the total file size.
RETURN:
An array of tree node pointers that have no children.
They contain the character as a key, and percentage as a double.
*/
Node **HuffmanTree::getTextAnalysis()
{
	Node **words = new Node*[MAX_CHAR_VAL];
	//init dynamic array
	for (int i = 0; i < MAX_CHAR_VAL; i++) {
		words[i] = new Node;
	}
	//BIG BUFFERS MEAN READ GOES ALL FAST LIKE
	//This is actually the fastest buffer size, nochangerino
	unsigned int fileSize = 500000;
	unsigned int charsRead = 0;
	char* _BIG_BUFFERS_ = new char[fileSize];
	char* STD_IN = "./long.txt";
	int IN_FD;
	_asm {
		MOV ecx, 255
		init:
		MOV edi, words
			MOV edi, [edi + ecx * 4]
			MOV [edi], ecx
			loop init

		PUSH NULL
		PUSH FILE_ATTRIBUTE_NORMAL
		PUSH OPEN_EXISTING
		PUSH NULL
		PUSH FILE_SHARE_READ
		PUSH GENERIC_READ
		PUSH STD_IN
		CALL CreateFileA
		MOV IN_FD, eax

		read :
			PUSH NULL
			LEA edx, charsRead
			PUSH edx
			PUSH fileSize
			PUSH _BIG_BUFFERS_
			PUSH IN_FD
			CALL ReadFile
			MOV ecx, charsRead
			MOV esi, _BIG_BUFFERS_
			MOV edi, words
			CMP charsRead, 0
			JZ quit
		count :
			XOR eax, eax
			MOV al, [esi + ecx - 1]
			MOV eax, [edi + eax * 4]
			MOV ebx, [eax + 4]
			INC ebx
			MOV [eax + 4], ebx
			loop count
		JMP read
		quit :
		PUSH IN_FD
		CALL CloseHandle
	}
	delete[] _BIG_BUFFERS_;	
	return words;
}

HuffmanTree::HuffmanTree() {
	Node **words = getTextAnalysis();
	insertionSort_inplace(words, 256);
	setUpFill(words);
	trimTree();
	fillHash(); 
	compress("out.txt");
	decode("out.txt");
}

/*
Replaces characters in the file with their character equivalents in hashTable

PARA:
Output file name
*/
void HuffmanTree::compress(std::string file)
{

	std::ifstream myIn("long.txt", std::ios::in | std::ios::binary);
	std::ofstream myOut(file, std::ios::out | std::ios::binary);
	if (myIn.is_open())
	{
		char input = NULL;
		std::string binaryString = "";
		//Characters read in one at a time
		while (myIn.get(input))
		{
			//TODO: stop casting 3 seperate times
			unsigned char unsignedInput = input;
			//convert character
			std::string newChar = hashTable[(int)unsignedInput];
			binaryString = binaryString + newChar;
			//sometimes you can end up with mutltiple characters in the binary string
			while (binaryString.length() > MAX_CHAR_BITS - 1)
			{
				//convert binary string to a letter and output it
				char output = convertStrToInt(binaryString.substr(0, MAX_CHAR_BITS));
				myOut << output;
				binaryString.erase(0, MAX_CHAR_BITS);
			}
		}
		//Append 0's if there aren't enough bits to make a full character
		//This is eof
		while (binaryString.length() != MAX_CHAR_BITS)
			binaryString = binaryString + "0";
		char output = (int)convertStrToInt(binaryString);
		myOut << output;
	}
	myIn.close();
}

/*
DEBUG-FUN

Prints the Huffman Tree preorder.
Spacing is saved so that the depth is somewhat visable.
*/
void HuffmanTree::outputTree(Node *ptr, int count)
{
	for (int i = 0; i < count; i++)
	{
		std::cout << " ";
	}
	std::cout << ptr->key << "/" << ptr->data << "\n";
	count++;
	if (ptr->left != nullptr)
		outputTree(ptr->left, count);
	if (ptr->right != nullptr)
		outputTree(ptr->right, count);
}

/*
Searches for a node that has the given key in the HuffmanTree.

PARA:
A pointer to a node.  The root on the first call.
A running output in order to keep track of the path down the tree.
The value to search for.
RETURN:
The path down the tree if the node has been found or if the search has not 
reached the end.  Otherwise, empty string.
*/
std::string HuffmanTree::search(Node *ptr, std::string output, char key)
{
	if (!ptr) {
		return "";
	}
	//key was found in tree
	if (ptr->key == key)
		return output;
	//Returns no string if ptr reaches the escape node
	//Also returns no string if it has reached a leaf
	//TODO: Figure out of ptr->left != NULL is a better choice
	if (output == "0" || ptr->key != 0)
		return "";
	std::string tempOutput;
	tempOutput = search(ptr->left, output + "0", key);
	//Return output if some key was found
	if (tempOutput != "")
		return tempOutput;
	tempOutput = search(ptr->right, output + "1", key);
	//Return output if some key was found
	if (tempOutput != "")
		return tempOutput;
	//Return empty if no key was found
	return "";
}

/*
Gets a node from a tree based on it's binary path.
1's mean right, 0's mean left.

PARAMS: binaryStr-	a string of 1's and 0's that might represent a path
					to a node.  If it starts with 0, it won't lead to 
					anything.  If the node is found, binary string will
					no longer contain the path to the node.	If it's too 
					short, it's saved back to it's original value.  If
					it finds a node, it may still contain characters.
RETURN	The node of the found character, or NULL if the node was not found.
		if node was not found, binaryStr stays the same.  Otherwise,
		it no longer contains the path to the found node.
*/
Node* HuffmanTree::getNodeFromTree(std::string &binaryStr) {
	//charcter won't exist
	if (binaryStr[0] == '0') {
		return NULL;
	}
	std::string tmp = binaryStr; //in case we mess up we still have the old string
	Node* runner = root;
	while (runner->key == 0) {
		if (binaryStr.length() == 0) {
			binaryStr = tmp;
			return NULL;
		}
		if (binaryStr[0] == '1') {
			runner = runner->right;
		}
		else {
			runner = runner->left;
		}
		binaryStr.erase(0, 1);
	}
	return runner;
}

/*
Shuffles the tree around to lower average character lengh.

Basically just a helper function
stupid function pls ignore
*/
void HuffmanTree::trimTree() {
	trimTree(root, 0);
}

/*
Recursively calls findNextNode on each of the nodes with a depth
greater than 8.  Swaps over just the key for later searches
*/
void HuffmanTree::trimTree(Node* node, int depth) {
	if (!node) {
		return;
	}
	if (depth < 8) {
		trimTree(node->left, depth + 1);
		trimTree(node->right, depth + 1);
	}
	else if(!node->key) {
		Node * tmp = findNextNode(root, 8);
		node->key = tmp->key;
		tmp->key = 0;
	}
}


/*
Does a breadth first search on the tree.
Continue until we find the first node that's below the target depth,
and has a non-null character.  Return that one.

TODO: holy fucking refactor.  It should use root.
Slow af
I'm not even sure if this works
*/
Node* HuffmanTree::findNextNode(Node *node, int target) {
	std::queue<Node*> nodes;
	std::queue<int> depths;
	nodes.push(node);
	depths.push(0);
	while (!nodes.empty()) {
		Node* tmp = nodes.front();
		int curDepth = depths.front();
		nodes.pop();
		depths.pop();
		if (curDepth < target + 1 || !tmp->key) {
			if (tmp->left) {
				nodes.push(tmp->left);
				depths.push(curDepth + 1);
			}
			if (tmp->right) {
				nodes.push(tmp->right);
				depths.push(curDepth + 1);
			}
		}
		else if (tmp->key) {
			return tmp;
		}
	}
}