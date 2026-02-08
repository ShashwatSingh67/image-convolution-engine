#include "core/convolver.h"

float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode) {
    const int w = img.width();
    const int h = img.height();

    // Padding only affects x/y; channel must still be valid.
    assert(c >= 0 && c < img.channels());

    const bool in_bounds = (x >= 0 && x < w && y >= 0 && y < h);
    if (in_bounds) {
        return img.at(x, y, c);
        //if its inbounds returning the image else return zero
    }

    if (mode == PaddingMode::Zero) {
        return 0.0f;
    }

    //reflecting into range
    const int xr = reflect_index(x, w);
    const int yr = reflect_index(y, h);
    return img.at(xr, yr, c);
}
