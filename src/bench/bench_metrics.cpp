#include "bench/bench_metrics.h"
#include <cassert>
#include <cmath>
#include <algorithm>

ErrorMetrics compare_images(const Image& a, const Image& b) {
    assert(a.width() == b.width());
    assert(a.height() == b.height());
    assert(a.channels() == b.channels());

    const int total = a.width() * a.height() * a.channels();
    ErrorMetrics m;

    double sum_sq = 0.0;
    for (int i = 0; i < total; i++) {
        const float da = a.raw()[i];
        const float db = b.raw()[i];
        const float diff = da - db;
        m.max_abs = std::max(m.max_abs, std::fabs(diff));
        sum_sq += static_cast<double>(diff) * static_cast<double>(diff);
    }

    m.mse = sum_sq / static_cast<double>(total);
    return m;
}
