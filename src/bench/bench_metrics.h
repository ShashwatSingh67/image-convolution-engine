#pragma once
#include "core/image.h"

struct ErrorMetrics {
    float max_abs = 0.0f;
    double mse = 0.0;
};

// Compare two images of same shape (float pipeline)
ErrorMetrics compare_images(const Image& a, const Image& b);
