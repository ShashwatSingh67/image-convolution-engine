#include "core/convolver.h"
#include <cassert>

float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode) {
    const int w = img.width();
    const int h = img.height();

    // Padding only affects x/y; channel must still be valid.
    assert(c >= 0 && c < img.channels());

    const bool in_bounds = (x >= 0 && x < w && y >= 0 && y < h);
    if (in_bounds) {
        return img.at(x, y, c);
    }

    if (mode == PaddingMode::Zero) {
        return 0.0f;
    }

    const int xr = reflect_index(x, w);
    const int yr = reflect_index(y, h);
    return img.at(xr, yr, c);
}

Image convolve2D_direct(const Image& in, const Kernel2D& kernel, PaddingMode mode) {
    // Output has same dimensions as input.
    Image out(in.width(), in.height(), in.channels(), 0.0f);

    const int kw = kernel.width();
    const int kh = kernel.height();
    const int rx = kernel.radius_x();
    const int ry = kernel.radius_y();

    // For every pixel in the output...
    for (int y = 0; y < in.height(); y++) {
        for (int x = 0; x < in.width(); x++) {
            for (int c = 0; c < in.channels(); c++) {

                float acc = 0.0f;

                // ...accumulate weighted sum of neighbors.
                //
                // kx,ky are kernel coordinates (0..kw-1, 0..kh-1)
                // We shift them by the radius to map to image offsets:
                //   dx = kx - rx, dy = ky - ry
                for (int ky = 0; ky < kh; ky++) {
                    const int dy = ky - ry;
                    for (int kx = 0; kx < kw; kx++) {
                        const int dx = kx - rx;

                        const float pix = sample_with_padding(in, x + dx, y + dy, c, mode);
                        const float w = kernel.at(kx, ky);
                        acc += pix * w;
                    }
                }

                out.at(x, y, c) = acc;
            }
        }
    }

    return out;
}
