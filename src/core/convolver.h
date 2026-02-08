#pragma once
#include "core/image.h"
#include "core/padding.h"
#include "core/kernel.h"

// Sample a pixel with padding rules applied.
float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode);

/*
Direct 2D convolution (baseline).

For each output pixel (x,y,c):
  out(x,y,c) = sum_{ky,kx}  in(x + (kx-rx), y + (ky-ry), c) * K(kx,ky)

- K is a 2D kernel (odd width/height).
- PaddingMode decides what happens when sampling goes outside the image.
*/
Image convolve2D_direct(const Image& in, const Kernel2D& kernel, PaddingMode mode);
