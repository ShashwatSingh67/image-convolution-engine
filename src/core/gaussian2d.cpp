#include "core/gaussian2d.h"
#include <cmath>
#include <cassert>

/*
2D Gaussian function
G(x,y) = exp(-(x^2 + y^2) / (2*sigma^2))

*/
Kernel2D gaussian2d(float sigma, int ksize) { //kernel object
    assert(sigma > 0.0f);                       //validating inputs for sigma (blur amount)
    assert(ksize > 0 && (ksize % 2 == 1));      //ksize(blur width)

    Kernel2D k(ksize, ksize);   //ksize, by ksize grid of floats
    const int r = k.radius_x(); //radius of kernel  
    const float denom = 2.0f * sigma * sigma; //gausian denominator

    float sum = 0.0f;
    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            const int dx = x - r;
            const int dy = y - r;
            const float w = std::exp(-(dx*dx + dy*dy) / denom); //guassian math for each float
            k.at(x, y) = w;
            sum += w;
        }
    }


    //normallising it so that the images are not brighet or darker than expected
    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            k.at(x, y) /= sum;
        }
    }

    //this is later factorised down and improved in the 1D version

    return k;
}
