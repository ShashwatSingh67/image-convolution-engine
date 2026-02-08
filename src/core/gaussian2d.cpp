#include "core/gaussian2d.h"
#include <cmath>
#include <cassert>

/*
2D Gaussian:
G(x,y) = exp(-(x^2 + y^2) / (2*sigma^2))

We sample x,y in [-r, r], normalize so total sum = 1.
*/
Kernel2D gaussian2d(float sigma, int ksize) {
    assert(sigma > 0.0f);
    assert(ksize > 0 && (ksize % 2 == 1));

    Kernel2D k(ksize, ksize);
    const int r = k.radius_x();
    const float denom = 2.0f * sigma * sigma;

    float sum = 0.0f;
    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            const int dx = x - r;
            const int dy = y - r;
            const float w = std::exp(-(dx*dx + dy*dy) / denom);
            k.at(x, y) = w;
            sum += w;
        }
    }

    for (int y = 0; y < ksize; y++) {
        for (int x = 0; x < ksize; x++) {
            k.at(x, y) /= sum;
        }
    }

    return k;
}
