#pragma once
#include "core/kernel.h"

// Build a normalized 1D Gaussian kernel.
//
// Parameters:
// - sigma: blur strength (standard deviation). Must be > 0.
// - ksize: odd kernel size (e.g., 3,5,11...). Must be odd and > 0.
//
// Returns:
// - Kernel1D where sum of all weights ~= 1.0
Kernel1D gaussian1d(float sigma, int ksize);
