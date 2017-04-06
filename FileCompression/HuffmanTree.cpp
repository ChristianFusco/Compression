#include "HuffmanTree.h"

int nodeArrDiv = 1;

/*
Uncompresses the file.
PARA:
The output file name.
*/
void HuffmanTree::decode(std::string file)
{
	std::ifstream myFile(STD_OUT);
	std::ofstream myFileOut(file);

	if (myFile.is_open()) {
		char input = NULL;
		std::string fullChar = "";
		while (myFile.get(input)) {

			//TODO: what the hell am i doing here
			int tempInput = input;
			//converts the input to an unsigned version
			if (tempInput < 0)
				tempInput += MAX_CHAR_VAL;

			//TODO: This should be a function
			//converts the input to a binary value
			//binary value will be backwards
			while (tempInput > 0) {
				if (tempInput % 2 == 0) {
					fullChar += "0";
				}
				else {
					fullChar += "1";
				}
				tempInput /= 2;
			}
			
		}
	}
	myFile.close();
	myFileOut.close();
}
/**/

/*
Moves each of the character values of the HuffmanTree into a table for faster access.

Searching the huffman tree over and over again wastes time.
Since input is often just a character, replacements can be found in O(1).

If a value does not exist in the tree, it's replaced with 0+it's original value.
*/
void HuffmanTree::fillHash()
{
	for (int i = 0; i < MAX_CHAR_VAL; i++) {
		std::string searchWord = "";
		searchWord += (char)i;
		searchWord = search(searchWord);
		if (searchWord != "") {
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
void HuffmanTree::setUpFill(std::vector<Node*> nodeArr) {
	//A really big list can make the tree less efficent, cut the size down.
	//TODO: Do the math on this again when I'm less sleep deprived
	nodeArr.resize(nodeArr.size() / nodeArrDiv);
	root = new Node;
	Node *escape = new Node;
	//Big trees can actually make files bigger
	//This node will work as an escape to have characters be printed as their
	//normal encoding with a prepended 0
	root->left = escape;
	//sets the right child to be the root of a normal huffmantree
	//fillTree is called recursively
	root->right = fillTree(nodeArr);
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
Node* HuffmanTree::fillTree(std::vector<Node*> nodeArr)
{
	//nodeArr is sorted in order of each nodes effectiveness.
	//Nodes that don't contain characters contain the sum of their children's effectiveness

	if (nodeArr.size() == 1) {
		return nodeArr.back();
	}

	//get the two worst character replacements
	Node *smallPtr1, *smallPtr2;
	smallPtr1 = nodeArr.back();
	nodeArr.pop_back();
	if (smallPtr1->data == 0) {
		return fillTree(nodeArr);
	}
	smallPtr2 = nodeArr.back();
	nodeArr.pop_back();
	if (smallPtr2->data == 0) {
		return fillTree(nodeArr);
	}

	//creates a new node that points to two worst nodes
	//This node is a bridge between leaves, or character replacements
	Node *head = new Node;
	head->data = smallPtr1->data + smallPtr2->data;
	head->left = smallPtr1;
	head->right = smallPtr2;

	//The temp node that was created will now need to be placed somewhere inside of the list again
	int size = nodeArr.size();
	if (nodeArr.size() <= 1 || head->data < nodeArr.back()->data) {
		nodeArr.push_back(head);
		return fillTree(nodeArr);
	}
	if (size == 0) {
		nodeArr.push_back(head);
		return fillTree(nodeArr);	//tree size shouldn't be too big so recursion seems fine
	}

	for (int i = 0; i < size; i++){
		if (head->data >= nodeArr[i]->data){
			nodeArr.emplace(nodeArr.begin() + i, head);
			break;
		}
	}
	return fillTree(nodeArr);
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
	std::ifstream in(STD_INPUT);
	if (in.is_open())
	{
		//Get the count of each character
		char input = NULL;
		while (in.get(input))
		{
			words[(int)input + MAX_CHAR_VAL / 2]->data += 1;
		}
	}
	//close, open with binary, and seek to get file size
	in.close();
	in.open(STD_INPUT, std::ifstream::binary);
	in.seekg(0, in.end);
	//set key and get percent chance of each char
	for (int i = 0; i < MAX_CHAR_VAL; i++) {
		words[i]->key = i + MAX_CHAR_VAL / 2;
		words[i]->data /= in.tellg();
	}	
	in.close();
	return words;
}

HuffmanTree::HuffmanTree() {
	while (nodeArrDiv < 5) {
		Node **words = getTextAnalysis();
		std::vector<Node*> nodeArr(words, words + MAX_CHAR_VAL);
		std::sort(nodeArr.begin(), nodeArr.end(), nodeDataGreaterThen);
		setUpFill(nodeArr);
		fillHash(); 
		outputTree();
		for (int i = 0; i < MAX_CHAR_VAL; i++) {
			std::cout << (char)i << hashTable[i] << "\n";
		}
		compress("output" + std::to_string(nodeArrDiv) + ".txt");
		std::cout << "output" + std::to_string(nodeArrDiv) + ".txt\n";
		nodeArrDiv++;
		root = NULL;
	}
}

/*
Replaces characters in the file with their character equivalents in hashTable

PARA:
Output file name
*/
void HuffmanTree::compress(std::string file)
{
	std::ifstream myIn(STD_INPUT);
	std::ofstream myOut(file);
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
			binaryString += newChar;
			//sometimes you can end up with mutltiple characters in the binary string
			while (binaryString.length() > 7)
			{
				//convert binary string to a letter and output it
				char output = convertStrToInt(binaryString);
				myOut << output;
				binaryString = binaryString.erase(0, 8);
			}
		}
		//Append 0's if there aren't enough bits to make a full character
		//This is eof
		while (binaryString.length() != 8)
			binaryString += "0";
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
std::string HuffmanTree::search(Node *ptr, std::string output, std::string key)
{
	//key was found in tree
	if (ptr->key == key)
		return output;
	//Returns no string if ptr reaches the escape node
	//Also returns no string if it has reached a leaf
	//TODO: Figure out of ptr->left != NULL is a better choice
	if (output == "0" || ptr->key != "")
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

*
Node* HuffmanTree::getNodeFromTree(std::string binaryStr)
{
	if (binaryStr[0] == '0') {
		binaryStr.erase(0, 1);
		std::cout << hashTable[convertStrToInt(binaryStr)] << "\n";
		return hashTable[convertStrToInt(binaryStr)];
	}
	Node *ptr = root;
	std::string out = 0;
	for (int i = 0; i < binaryStr.length(); i++)
	{
		if (binaryStr[i] == '0')
			ptr = ptr->left;
		else if (binaryStr[i] == '1')
			ptr = ptr->right;
	}
	out = ptr->key;
	return out;
}*/
