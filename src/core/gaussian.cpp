#include "core/gaussian.h"
#include <cmath>
#include <cassert>

/*
Gaussian 1D:
g(x) = exp(-(x^2)/(2*sigma^2))
    -sigma controlles blur strenght
        -small sigma sharp blur
        -big sigma wide blur
this is the gaussian equation
https://www.w3.org/Talks/2012/0125-HTML-Tehran/Gaussian.xhtml

We sample x in [-r, r] where r = ksize/2.

Normalization:
We compute all weights, sum them, then divide each weight by the sum
so that total sum becomes 1.0.

That normalization is important because blur should preserve overall brightness.
*/
//This function builds a normalized 1D Gaussian kernel
Kernel1D gaussian1d(float sigma, int ksize) {
    assert(sigma > 0.0f);
    assert(ksize > 0 && (ksize % 2 == 1)); //input validation

    Kernel1D k(ksize);
    const int r = k.radius(); //edge to edge

    const float denom = 2.0f * sigma * sigma;
    //computed the denomination part of the guassian blur equation before hand

    float sum = 0.0f;
    for (int i = 0; i < ksize; i++) {
        const int x = i - r;                 // map index to coordinate in [-r, r]
        const float w = std::exp(-(x * x) / denom); //computing gaussian weight
        k.at(i) = w;
        sum += w;
    }

    // Normalize weight sum otherwise the brightness would be getting effected (kernel > 1 is  brighter than before instead of just blurred, kernel < 1 is darker)
    for (int i = 0; i < ksize; i++) {
        k.at(i) /= sum;
    }

    return k;
}
