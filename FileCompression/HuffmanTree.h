#include "Header.h"

class HuffmanTree{
private:
	Node *root;
	std::string hashTable[256];

	void fillHash();
	Node* fillTree(Node** nodeArr, int size);
	void setUpFill(Node** nodeArr);
	Node **getTextAnalysis();
	void outputTree(Node *ptr, int count);
	std::string search(Node *ptr, std::string output, char key);
	Node* getNodeFromTree(std::string &binaryStr);
	void printChar(std::ofstream &out, std::string &fullChar);

	void trimTree(Node* node, int depth);
	void trimTree();
	Node* findNextNode(Node* node, int depth);

public:
	HuffmanTree();

	void decode(std::string file);
	void inline outputTree(){ outputTree(root, 0); };
	void compress(std::string file);
	std::string inline search(char key) { return search(root, "", key); };
};