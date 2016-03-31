#include "HuffmanTree.h"
#include <iostream>
#include <string>
#include "Clock.h"

int main()
{
		Clock *timer = new Clock("Building Tree");
		HuffmanTree tree;
		timer->stopClock();
		delete timer;
		timer = new Clock("Compressing");
		tree.output("out.txt");
		timer->stopClock();
		delete timer;
		timer = new Clock("Decompressing");
		tree.decode("out2.txt");
		timer->stopClock();
		delete timer;
	return 0; 
}