#pragma once
#include "core/kernel.h"

// Build a normalized 2D Gaussian kernel (direct convolution use).
Kernel2D gaussian2d(float sigma, int ksize);
