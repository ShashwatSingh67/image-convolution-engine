#include <cassert>
#include <cmath>
#include <iostream>
#include "core/image.h"

static bool approx(float a, float b, float eps = 1e-6f) {
    return std::fabs(a - b) <= eps;
}

int main() {
    Image img(3, 2, 4, 0.5f);

    assert(img.width() == 3);
    assert(img.height() == 2);
    assert(img.channels() == 4);
    assert(img.size() == 3u * 2u * 4u);

    img.at(0, 0, 0) = 1.0f;
    img.at(2, 1, 3) = -2.0f;

    assert(approx(img.at(0, 0, 0), 1.0f));
    assert(approx(img.at(2, 1, 3), -2.0f));

    img.fill(0.25f);
    assert(approx(img.at(0, 0, 0), 0.25f));
    assert(approx(img.at(2, 1, 3), 0.25f));

    img.at(1, 0, 2) = 5.0f;
    img.at(1, 1, 2) = -5.0f;
    img.clamp_inplace(0.0f, 1.0f);

    assert(approx(img.at(1, 0, 2), 1.0f));
    assert(approx(img.at(1, 1, 2), 0.0f));

    std::cout << "test_image passed\n";
    return 0;
}
