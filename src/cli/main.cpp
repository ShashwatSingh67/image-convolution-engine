#include <iostream>
#include <string>

#include "cli/io_stb.h"
#include "core/gaussian2d.h"
#include "core/convolver.h"
#include "core/padding.h"

static void usage() {
    std::cout << "Usage:\n"
              << "  ./imgconv --in input.jpg --out output.png --sigma 1.6 --ksize 11 --padding reflect|zero\n";
}

int main(int argc, char** argv) {
    std::string in_path, out_path;
    float sigma = 1.6f;
    int ksize = 11;
    PaddingMode padding = PaddingMode::Reflect;

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--in" && i + 1 < argc) in_path = argv[++i];
        else if (a == "--out" && i + 1 < argc) out_path = argv[++i];
        else if (a == "--sigma" && i + 1 < argc) sigma = std::stof(argv[++i]);
        else if (a == "--ksize" && i + 1 < argc) ksize = std::stoi(argv[++i]);
        else if (a == "--padding" && i + 1 < argc) {
            std::string p = argv[++i];
            if (p == "reflect") padding = PaddingMode::Reflect;
            else if (p == "zero") padding = PaddingMode::Zero;
            else {
                std::cerr << "Unknown padding: " << p << "\n";
                return 1;
            }
        } else {
            usage();
            return 1;
        }
    }

    if (in_path.empty() || out_path.empty()) {
        usage();
        return 1;
    }

    Image img = load_image_stb(in_path);
    if (img.width() <= 0) {
        std::cerr << "Failed to load image: " << in_path << "\n";
        return 1;
    }

    std::cout << "Loaded " << in_path << " (" << img.width() << "x" << img.height() << ", ch=" << img.channels() << ")\n";

    Kernel2D g = gaussian2d(sigma, ksize);
    Image out = convolve2D_direct(img, g, padding);

    if (!save_image_stb(out, out_path)) {
        std::cerr << "Failed to save image: " << out_path << "\n";
        return 1;
    }

    std::cout << "Saved " << out_path << "\n";
    return 0;
}
