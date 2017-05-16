#include "HuffmanTree.h"
#include <iostream>
#include <string>
#include "Clock.h"

int main()
{
    int bufferSize1 = 5;
    int bufferSize2 = 5;
    for(int j = 0; j < 6; j++){
        for(int i = 0; i < 6; i++){
            Clock *timer = new Clock("Buff1 " + std::to_string(bufferSize1) + " Buff2 " + std::to_string (bufferSize2));
            for(int i = 0; i < 30; i++){
                HuffmanTree *tree = new HuffmanTree(bufferSize1, bufferSize2);
                delete tree;
            }
            timer->stopClock();
            delete timer;
            bufferSize2*=10;
        }
        bufferSize2 = 5;
        bufferSize1*=10;
    }
	return 0;
}
