/*
I really should have named this Timer, shouldn't I have?
When you create a timer object, it keeps track of a description,
some string of your choosing, and the start time, which is handled in
the constructor.  Stopping the clock outputs the total time that the 
Clock was running.

ChRiStIaN fUsCo \o/\o/\o/
*/

#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>

class Clock{
private:
	std::clock_t start;
	std::string des;
public:
	Clock(std::string descriptor);
	double stopClock();
};

Clock::Clock(std::string descriptor)
{
	std::cout << "Clock started with descriptor: " << descriptor << "\n";
	des = descriptor;
	start = std::clock();
}

double Clock::stopClock()
{
	double dur = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Clock with descriptor: " << des << " took " << dur << " seconds.\n";
	return dur;
}