#include "HuffmanTree.h"
#include <iostream>
#include <string>
#include "Clock.h"

int main()
{
	Clock *timer = new Clock("Fast");
	HuffmanTree tree;
	timer->stopClock();
	delete timer;
	return 0;
}