#include "core/gaussian.h"
#include <cmath>
#include <cassert>

/*
Gaussian 1D:
g(x) = exp(-(x^2)/(2*sigma^2))

We sample x in [-r, r] where r = ksize/2.

Normalization:
We compute all weights, sum them, then divide each weight by the sum
so that total sum becomes 1.0.

That normalization is important because blur should preserve overall brightness.
*/
Kernel1D gaussian1d(float sigma, int ksize) {
    assert(sigma > 0.0f);
    assert(ksize > 0 && (ksize % 2 == 1));

    Kernel1D k(ksize);
    const int r = k.radius();

    const float denom = 2.0f * sigma * sigma;

    float sum = 0.0f;
    for (int i = 0; i < ksize; i++) {
        const int x = i - r;                 // map index -> coordinate in [-r, r]
        const float w = std::exp(-(x * x) / denom);
        k.at(i) = w;
        sum += w;
    }

    // Normalize so sum becomes 1
    for (int i = 0; i < ksize; i++) {
        k.at(i) /= sum;
    }

    return k;
}
