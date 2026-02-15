#include "core/convolver.h"
#include <cassert>

float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode) {
    const int w = img.width();
    const int h = img.height();
    assert(c >= 0 && c < img.channels());

    if (x >= 0 && x < w && y >= 0 && y < h) {
        return img.at(x, y, c);
    }
    if (mode == PaddingMode::Zero) return 0.0f;

    const int xr = reflect_index(x, w);
    const int yr = reflect_index(y, h);
    return img.at(xr, yr, c);
}

Image convolve2D_direct(const Image& in, const Kernel2D& kernel, PaddingMode mode) {
    Image out(in.width(), in.height(), in.channels(), 0.0f);

    const int kw = kernel.width();
    const int kh = kernel.height();
    const int rx = kernel.radius_x();
    const int ry = kernel.radius_y();

    for (int y = 0; y < in.height(); y++) {
        for (int x = 0; x < in.width(); x++) {
            for (int c = 0; c < in.channels(); c++) {
                float acc = 0.0f;
                for (int ky = 0; ky < kh; ky++) {
                    const int dy = ky - ry;
                    for (int kx = 0; kx < kw; kx++) {
                        const int dx = kx - rx;
                        const float pix = sample_with_padding(in, x + dx, y + dy, c, mode);
                        acc += pix * kernel.at(kx, ky);
                    }
                }
                out.at(x, y, c) = acc;
            }
        }
    }
    return out;
}

Image convolve1D_horizontal(const Image& in, const Kernel1D& kernel, PaddingMode mode) {
    Image out(in.width(), in.height(), in.channels(), 0.0f);
    const int r = kernel.radius();

    for (int y = 0; y < in.height(); y++) {
        for (int x = 0; x < in.width(); x++) {
            for (int c = 0; c < in.channels(); c++) {
                float acc = 0.0f;
                for (int k = 0; k < kernel.size(); k++) {
                    const int dx = k - r;
                    const float pix = sample_with_padding(in, x + dx, y, c, mode);
                    acc += pix * kernel.at(k);
                }
                out.at(x, y, c) = acc;
            }
        }
    }
    return out;
}

Image convolve1D_vertical(const Image& in, const Kernel1D& kernel, PaddingMode mode) {
    Image out(in.width(), in.height(), in.channels(), 0.0f);
    const int r = kernel.radius();

    for (int y = 0; y < in.height(); y++) {
        for (int x = 0; x < in.width(); x++) {
            for (int c = 0; c < in.channels(); c++) {
                float acc = 0.0f;
                for (int k = 0; k < kernel.size(); k++) {
                    const int dy = k - r;
                    const float pix = sample_with_padding(in, x, y + dy, c, mode);
                    acc += pix * kernel.at(k);
                }
                out.at(x, y, c) = acc;
            }
        }
    }
    return out;
}
