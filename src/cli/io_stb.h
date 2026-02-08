#pragma once
#include <string>
#include "core/image.h"

// Load image from disk into float Image.
// - Returns Image with channels = 1 or 3 (we'll force 3 for simplicity).
Image load_image_stb(const std::string& path);

// Save float Image to disk (png/jpg based on extension).
// - Clamps to [0,1], converts to uint8 [0,255].
bool save_image_stb(const Image& img, const std::string& path);
