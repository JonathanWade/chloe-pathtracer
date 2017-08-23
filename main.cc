// Main entry point for our simple path tracer

#include <algorithm>
#include <cstdlib>
#include <iostream>

// Globals //
int imageWidth = 800;
int imageHeight = 600;


////////////////////////////////////////////////////////////////////////
// Some command line parsing utilities lifted from Stack Overflow
// https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
// by "iain"
// Call: getCmdOption(argv, argv + argc, "-o") where "-o" is the option you want to find
////////////////////////////////////////////////////////////////////////

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void processCommandLine(int argc, char* argv[])
{
	// Output image width
	char* clWidth = getCmdOption(argv, argv + argc, "-w");
	if(clWidth != nullptr) {
		int tmpWidth = std::atoi(clWidth);
		if (tmpWidth > 0) {
			imageWidth = tmpWidth;
		} else {
			std::cout << "Bad output image width " << tmpWidth << ". Should be > 0" << std::endl;
		}
	}

	char* clHeight = getCmdOption(argv, argv + argc, "-h");
	if(clHeight != nullptr) {
		int tmpHeight = std::atoi(clHeight);
		if(tmpHeight > 0) {
			imageHeight = tmpHeight;
		} else {
			std::cout << "Bad output image height " << tmpHeight << ". Should be > 0" << std::endl;
		}
	}
}

// Main //
int main(int argc, char* argv[])
{
    processCommandLine(argc, argv);
    std::cout << "Output image size: " << imageWidth << "x" << imageHeight << std::endl;

    return 0;
}
