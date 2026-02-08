#include <cassert>
#include <iostream>

#include "core/image.h"
#include "core/padding.h"
#include "core/convolver.h"

int main() {
    // 1x3 grayscale image: [10, 20, 30]
    Image img(3, 1, 1, 0.0f);
    img.at(0, 0, 0) = 10.0f;
    img.at(1, 0, 0) = 20.0f;
    img.at(2, 0, 0) = 30.0f;

    // Zero padding assumes past border is black makes edges darker on blur
    assert(sample_with_padding(img, -1, 0, 0, PaddingMode::Zero) == 0.0f);
    assert(sample_with_padding(img,  3, 0, 0, PaddingMode::Zero) == 0.0f);

    // Reflect padding reflects the border makes contect less blurry
    assert(sample_with_padding(img, -1, 0, 0, PaddingMode::Reflect) == 20.0f);
    assert(sample_with_padding(img,  3, 0, 0, PaddingMode::Reflect) == 20.0f);

    std::cout << "test_padding passed\n";
    return 0;
}
