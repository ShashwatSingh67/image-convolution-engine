#include <cassert>
#include <cmath>
#include <iostream>

#include "core/kernel.h"
#include "core/gaussian.h"

// float compare helper
static bool approx(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) <= eps;
}

int main() {
    // ---- Kernel1D shape ----
    Kernel1D k1(5);
    assert(k1.size() == 5);
    assert(k1.radius() == 2);

    // ---- Kernel2D shape ----
    Kernel2D k2(3, 3);
    assert(k2.width() == 3);
    assert(k2.height() == 3);
    assert(k2.radius_x() == 1);
    assert(k2.radius_y() == 1);

    // ---- Gaussian1D correctness ----
    Kernel1D g = gaussian1d(1.0f, 11);

    // (1) Sum ~= 1
    float sum = 0.0f;
    for (int i = 0; i < g.size(); i++) sum += g.at(i);
    assert(approx(sum, 1.0f, 1e-4f));

    // (2) Symmetry: g[i] == g[n-1-i]
    for (int i = 0; i < g.size(); i++) {
        assert(approx(g.at(i), g.at(g.size() - 1 - i), 1e-6f));
    }

    // (3) Peak in the center
    const int r = g.radius();
    for (int i = 0; i < g.size(); i++) {
        assert(g.at(r) >= g.at(i));
    }

    std::cout << "test_kernel passed\n";
    return 0;
}
