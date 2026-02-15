#pragma once
#include "core/image.h"
#include "core/padding.h"
#include "core/kernel.h"

// Sample a pixel with padding rules applied.
float sample_with_padding(const Image& img, int x, int y, int c, PaddingMode mode);

// Baseline: direct 2D convolution
Image convolve2D_direct(const Image& in, const Kernel2D& kernel, PaddingMode mode);

// 1D convolution passes (for separable filters)
// Horizontal: apply Kernel1D along x direction
Image convolve1D_horizontal(const Image& in, const Kernel1D& kernel, PaddingMode mode);

// Vertical: apply Kernel1D along y direction
Image convolve1D_vertical(const Image& in, const Kernel1D& kernel, PaddingMode mode);
