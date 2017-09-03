// Main entry point for our simple path tracer
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "ray.h"

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

float HitSphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.Origin() - center;
    float a = dot(r.Direction(), r.Direction());
    float b = 2.0f * dot(oc, r.Direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }
}

// Ray 
vec3 color(const ray& r) {
    float t = HitSphere(vec3(0,0,-1), 0.5, r);
    if(t > 0.0) {
        vec3 norm = UnitVector(r.PointAtParameter(t) - vec3(0, 0, -1));
        return 0.5f*vec3(norm.x()+1, norm.y()+1, norm.z()+1);
    }

    vec3 unit = UnitVector(r.Direction());
    t = 0.5f*(unit.y() + 1.0f);
    return (t)*vec3(1.0f, 1.0f, 1.0f) + (1.0f-t)*vec3(0.5f, 0.7f, 1.0f);
}

// Main ////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    processCommandLine(argc, argv);
    std::cout << "Output image size: " << imageWidth << "x" << imageHeight << std::endl;

    uint8_t* outputData = new uint8_t[imageWidth * imageHeight * 3];

    vec3 lowerLeft(-2.0, -1.5, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 3.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = 0; j < imageHeight; j++) {
        for (int i = 0; i < imageWidth; i++) {
            float u = float(i) / float(imageWidth);
            float v = float(j) / float(imageHeight);

            ray r(origin, lowerLeft + u * horizontal + v * vertical);
            vec3 col = color(r);

            int index = (j * imageWidth * 3) + (i * 3);
            outputData[index] = uint8_t(col.r()*255.99);
            outputData[index + 1] = uint8_t(col.g()*255.99);
            outputData[index + 2] = uint8_t(col.b()*255.99);
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
