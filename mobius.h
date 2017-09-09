#pragma once
#include "hitablelist.h"
#include "vec3.h"
#include <cmath>

const double pi = 3.1415926535897;

// Code mutated from http://www.devenezia.com/downloads/mob/

class MobiusPoints {
public:
    MobiusPoints(int H, int W, int L, int N, int K, int R);


    vec3 P[100][5]; // second dimension are 5 points of a slice
                             // future would replace slice with cross-sectional function

    void ToWorld(hitableList& world, const vec3 offset);

    /*
        a slice
        4-----1
        |\ | /|
        |--0--|
        |/ | \|
        3-----2

        segment[i] is bounded by slice[i] and slice[i+1]
        when ends are joined, slice [last] = slice[0]
    */

private:
    int NS; // number of segments requested
    int k;  // number of half twists
};

MobiusPoints::MobiusPoints(
      int height
    , int width
    , int length
    , int nSegments
    , int nHalfTwists
    , int nCurlIterations)
{
    double h, w, l, x, y, z;
    int n, m, i, R;

    double thetaN, cosN, sinN;

    // initialize the slices of the strip
    h = (double) height / 2.0 ;
    w = (double) width  / 2.0 ;
    l = (double) length ;

    NS = nSegments;
    k = nHalfTwists;        // class variable
    R = nCurlIterations;

    // NS is the number of segments
    // the last segment slice will wrap around to mate the first segment slice

    for (n=0; n<=NS; n++)
    {
        P [n][0].e[0] =   (double) n / (double) (NS) * l;
        P [n][0].e[1] =   0.0;
        P [n][0].e[2] =   0.0;

        P [n][1].e[0] =   P [n][0].e[0];
        P [n][1].e[1] =   w;
        P [n][1].e[2] =   h;

        P [n][2].e[0] =   P [n][0].e[0];
        P [n][2].e[1] =   w;
        P [n][2].e[2] = - h;

        P [n][3].e[0] =   P [n][0].e[0];
        P [n][3].e[1] = - w;
        P [n][3].e[2] = - h;

        P [n][4].e[0] =   P [n][0].e[0];
        P [n][4].e[1] = - w;
        P [n][4].e[2] =   h;
    }

    // twist the strip about the x-axis k(pi) times by computing
    // slice rotational transformations in the y-z plane

    for (n=0;n<=NS;n++)
    {
        thetaN = (double) n / (double) (NS) * (double) k * pi;
        cosN = cos (thetaN);
        sinN = sin (thetaN);

        // rotate slice n thetaN radians
        for (m=1;m<5;m++)
        {
            y = P [n][m].e[1];
            z = P [n][m].e[2];
            // rotate transform
            P [n][m].e[1] = y * cosN - z * sinN;
            P [n][m].e[2] = y * sinN + z * cosN;
        }
    }

    // curl the strip back on itself
    // perform a series of repetitive translate-rotate-translate transformations
    // on each slice. rotations are in the x-z plane

    thetaN = 2.0 * pi / double (NS);
    cosN = cos (thetaN);
    sinN = sin (thetaN);

    // start at 1 since first slice (index 0) will not be rotated
    // N iterations of this loop total N*thetaN radians, or 2pi.
    // Note the inner loops transform fewer and fewer slices as i approaches N
    for (i=1;i<=R;i++)
    {
        // rotation will be about center of previous slice
        double x0 = P [i-1][0].e[0];
        double z0 = P [i-1][0].e[2];

        for (n=i;n<=NS;n++)
        {   // rotate slice n thetaN radians about center of slice n-1
            for (m=0;m<5;m++)
            {   // translate
                x = P [n][m].e[0] - x0;
                z = P [n][m].e[2] - z0;
                // rotate and translate back
                P [n][m].e[0] = (x * cosN - z * sinN) + x0;
                P [n][m].e[2] = (x * sinN + z * cosN) + z0;
            }
        }
    }

    if (R == NS)
    {
        if (k & 0x1)
        { // odd, swap opposite vertices
            P [NS][0].e[0] = P [0][0].e[0]; P [NS][0].e[1] = P [0][0].e[1]; P [NS][0].e[2] = P [0][0].e[2];
            P [NS][1].e[0] = P [0][3].e[0]; P [NS][1].e[1] = P [0][3].e[1]; P [NS][1].e[2] = P [0][3].e[2];
            P [NS][2].e[0] = P [0][4].e[0]; P [NS][2].e[1] = P [0][4].e[1]; P [NS][2].e[2] = P [0][4].e[2];
            P [NS][3].e[0] = P [0][1].e[0]; P [NS][3].e[1] = P [0][1].e[1]; P [NS][3].e[2] = P [0][1].e[2];
            P [NS][4].e[0] = P [0][2].e[0]; P [NS][4].e[1] = P [0][2].e[1]; P [NS][4].e[2] = P [0][2].e[2];
        }
        else
        { // even
            P [NS][0].e[0] = P [0][0].e[0]; P [NS][0].e[1] = P [0][0].e[1]; P [NS][0].e[2] = P [0][0].e[2];
            P [NS][1].e[0] = P [0][1].e[0]; P [NS][1].e[1] = P [0][1].e[1]; P [NS][1].e[2] = P [0][1].e[2];
            P [NS][2].e[0] = P [0][2].e[0]; P [NS][2].e[1] = P [0][2].e[1]; P [NS][2].e[2] = P [0][2].e[2];
            P [NS][3].e[0] = P [0][3].e[0]; P [NS][3].e[1] = P [0][3].e[1]; P [NS][3].e[2] = P [0][3].e[2];
            P [NS][4].e[0] = P [0][4].e[0]; P [NS][4].e[1] = P [0][4].e[1]; P [NS][4].e[2] = P [0][4].e[2];
        }
    }
}

void MobiusPoints::ToWorld(hitableList& world, const vec3 offset) {
    for(int i = 0; i < NS; i++) {
        float u = float(i) / float(NS);
        float fuzz = abs(sin(u * pi));
        
        for(int j = 0; j < 5; j++)
        {
			vec3 color(u, 1.0f - u, j / 4.0);
            world.list.push_back(std::unique_ptr<hitable>(new sphere(P[i][j] + offset, 0.2f, new metal(color, fuzz))));
        }
    }
}

