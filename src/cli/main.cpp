#include <iostream>
#include "core/image.h"

int main(int argc, char** argv) {
    // For now, just prove we can link and run.
    // Phase 3 will add real CLI args + image I/O + Gaussian blur.
    std::cout << "imgconv running. Args = " << argc << "\n";

    // Tiny smoke test: create a 2x2 RGB image.
    Image img(2, 2, 3, 0.0f);
    img.at(0, 0, 0) = 1.0f;

    std::cout << "Created image: " << img.width() << "x" << img.height() << " ch=" << img.channels() << "\n";
    return 0;
}
