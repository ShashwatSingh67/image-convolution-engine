#include <cassert>
#include <cmath>
#include <iostream>

#include "core/image.h"
#include "core/kernel.h"
#include "core/convolver.h"
#include "core/padding.h"

static bool approx(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) <= eps;
}

static Kernel2D identity_kernel_3x3() {
    // 3x3 identity: only the center is 1, everything else 0.
    Kernel2D k(3, 3);
    k.at(1, 1) = 1.0f;
    return k;
}

static Kernel2D ones_kernel_3x3() {
    // Not normalized. Sum = 9.
    Kernel2D k(3, 3);
    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++) k.at(x, y) = 1.0f;
    return k;
}

int main() {
    // -------- Test 1: Identity kernel should return exactly the input --------
    {
        Image img(4, 3, 1, 0.0f);

        // Fill with distinct values so we catch indexing issues.
        float v = 1.0f;
        for (int y = 0; y < img.height(); y++) {
            for (int x = 0; x < img.width(); x++) {
                img.at(x, y, 0) = v;
                v += 1.0f;
            }
        }

        Kernel2D id = identity_kernel_3x3();
        Image out = convolve2D_direct(img, id, PaddingMode::Zero);

        for (int y = 0; y < img.height(); y++) {
            for (int x = 0; x < img.width(); x++) {
                assert(approx(out.at(x, y, 0), img.at(x, y, 0)));
            }
        }
    }

    // -------- Test 2: Impulse response (kernel shape appears) --------
    {
        // Make a 7x7 image with a single "impulse" = 1 at the center.
        // Convolution of an impulse with a kernel should produce the kernel,
        // centered at the impulse location (for interior pixels).
        Image img(7, 7, 1, 0.0f);
        img.at(3, 3, 0) = 1.0f;

        Kernel2D k = ones_kernel_3x3(); // 3x3 of ones
        Image out = convolve2D_direct(img, k, PaddingMode::Zero);

        // For a 3x3 ones kernel, the 3x3 region around the center should be 1,
        // and everything else should be 0 (since padding is Zero and impulse is centered).
        for (int y = 0; y < 7; y++) {
            for (int x = 0; x < 7; x++) {
                const bool in_center_3x3 = (x >= 2 && x <= 4 && y >= 2 && y <= 4);
                const float expected = in_center_3x3 ? 1.0f : 0.0f;
                assert(approx(out.at(x, y, 0), expected));
            }
        }
    }

    std::cout << "test_convolve2d passed\n";
    return 0;
}
