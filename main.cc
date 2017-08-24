// Main entry point for our simple path tracer


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

// This apps specifc command line processing
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

    // Output image height
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

// Main ////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    processCommandLine(argc, argv);
    std::cout << "Output image size: " << imageWidth << "x" << imageHeight << std::endl;

    uint8_t* outputData = new uint8_t[imageWidth * imageHeight * 3];

    for (int j = 0; j < imageHeight; j++) {
        for (int i = 0; i < imageWidth; i++) {
            float r = float(i) / float(imageWidth);
            float g = float(j) / float(imageHeight);
            float b = 0.2;
            int index = (j * imageWidth * 3) + (i * 3);
            outputData[index] = uint8_t(r*255.99);
            outputData[index + 1] = uint8_t(g*255.99);
            outputData[index + 2] = uint8_t(b*255.99);
        }
    }

    std::cout << "Writing image to disk" << std::endl;
    int saveImageResult = stbi_write_png(
                "outputImage.png",
                imageWidth,
                imageHeight,
                3,                  // RGB = 3 bytes
                outputData,
                imageWidth * 3 );   // Image stride in bytes


    if(!saveImageResult) {
        std::cout << "Error writing image file to disk" << std::endl;
    }

    return 0;
}
