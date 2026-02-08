#pragma once
#include "core/image.h"
#include "core/padding.h"

// Sample a pixel with padding rules applied.
// - If (x,y) is inside image bounds -> returns img.at(x,y,c)
// - If outside:
//    - Zero padding -> 0
//    - Reflect padding -> reflect coordinates back into valid range
float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode);
