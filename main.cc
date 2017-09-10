// Main entry point for our simple path tracer
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// This turns of double -> float lack of precision
#pragma warning( disable : 4244)

#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "material.h"
#include "utility.h"
#include "mobius.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>

// Globals //
int imageWidth = 800;
int imageHeight = 600;
int samples = 5;
int bounces = 10;

std::random_device r;
std::mt19937 gen(r());
std::uniform_real_distribution<double> rd;

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

    // Samples per pixel
	char* clSamples = getCmdOption(argv, argv + argc, "-s");
	if(clSamples != nullptr) {
		int tmpSamples = std::atoi(clSamples);
		if(tmpSamples >= 0) {
			samples = tmpSamples;
		} else {
			std::cout << "Bad sample count" << tmpSamples << ". Should be >= 0" << std::endl;
		}
	}

    // Samples per pixel
	char* clBounces = getCmdOption(argv, argv + argc, "-b");
	if(clBounces != nullptr) {
		int tmpBounces = std::atoi(clBounces);
		if(tmpBounces >= 0) {
			bounces = tmpBounces;
		} else {
			std::cout << "Bad bounce count" << tmpBounces << ". Should be >= 0" << std::endl;
		}
	}
}

// Ray
vec3 color(const ray& r, hitable* world, int depth) {
    hitRecord rec;
    if(world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < bounces && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        } else{
            return vec3(0,0,0);
        }
    } else {
        // Sky
        vec3 unit = UnitVector(r.Direction());
        float t = 0.5f*(unit.y() + 1.0f);
        return (1.0f-t)*vec3(1.0f, 1.0f, 1.0f) + (t)*vec3(0.5f, 0.7f, 1.0f);
    }
}

// Main ////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    processCommandLine(argc, argv);
    std::cout << "Output image size: " << imageWidth << "x" << imageHeight << std::endl;
    std::cout << "Samples per pixel: " << samples << std::endl;

    uint8_t* outputData = new uint8_t[imageWidth * imageHeight * 3];


    hitableList world;
    // Ground!
    world.list.push_back(std::unique_ptr<hitable>(new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.05f, 0.05f, 0.05f)))));
    // Three spheres
    world.list.push_back(std::unique_ptr<hitable>(new sphere(vec3(0,0,-1), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)))));
    world.list.push_back(std::unique_ptr<hitable>(new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.1f))));
    world.list.push_back(std::unique_ptr<hitable>(new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5f) )));
    world.list.push_back(std::unique_ptr<hitable>(new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5f) )));

    // Mobius World
    //MobiusPoints* mobius = new MobiusPoints(1, 1, 5, 20, 3, 0);
    //mobius->ToWorld(world, vec3(-2.5, 0.5, -1.7));

    camera cam;

    for (int j = 0; j < imageHeight; j++) {
        std::cout << "[" << j << "]";
        for (int i = 0; i < imageWidth; i++) {

            vec3 col(0, 0, 0);
            for(int n = 0; n < samples; n++) {
                float u = float(i + rd(gen)) / float(imageWidth);
                float v = float(j + rd(gen)) / float(imageHeight);

                ray r = cam.GetRay(u, v);
                //vec3 p = r.PointAtParameter(2.0);
                col += color(r, &world, 0);
            }
            col /= float(samples); // average samples
            // Adjust to 2.0 gamma
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int index = (j * imageWidth) + i;
            index *= 3; // 3-bytes per pixel
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
