#include "cli/io_stb.h"
#include <algorithm>
#include <cctype>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third_party/stb_image_write.h"

static std::string lower_ext(const std::string& p) {
    auto pos = p.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = p.substr(pos + 1);
    for (char& ch : ext) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return ext;
}

Image load_image_stb(const std::string& path) {
    int w = 0, h = 0, ch = 0;

    // Force 3 channels (RGB) for the project to keep convolution simple.
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 3);
    if (!data) {
        // Return empty image on failure (you can later replace with exceptions).
        return Image();
    }

    Image img(w, h, 3, 0.0f);

    // Convert uint8 [0..255] -> float [0..1]
    const int total = w * h * 3;
    for (int i = 0; i < total; i++) {
        img.raw()[i] = static_cast<float>(data[i]) / 255.0f;
    }

    stbi_image_free(data);
    return img;
}

bool save_image_stb(const Image& img, const std::string& path) {
    if (img.width() <= 0 || img.height() <= 0 || img.channels() <= 0) return false;

    const int w = img.width();
    const int h = img.height();
    const int c = img.channels();

    // We’ll save 3-channel RGB only for now.
    if (c != 3) return false;

    std::vector<unsigned char> out(static_cast<size_t>(w * h * c));

    // float [0..1] -> uint8 [0..255]
    for (int i = 0; i < w * h * c; i++) {
        float v = img.raw()[i];
        v = std::clamp(v, 0.0f, 1.0f);
        out[static_cast<size_t>(i)] = static_cast<unsigned char>(v * 255.0f + 0.5f);
    }

    const std::string ext = lower_ext(path);

    if (ext == "png") {
        // stride = bytes per row
        const int stride = w * c;
        return stbi_write_png(path.c_str(), w, h, c, out.data(), stride) != 0;
    } else if (ext == "jpg" || ext == "jpeg") {
        // quality: 90 is reasonable
        return stbi_write_jpg(path.c_str(), w, h, c, out.data(), 90) != 0;
    } else {
        // Default to png if extension unknown
        const std::string fallback = path + ".png";
        const int stride = w * c;
        return stbi_write_png(fallback.c_str(), w, h, c, out.data(), stride) != 0;
    }
}
