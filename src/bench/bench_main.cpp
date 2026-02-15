#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <cstdlib>

#include "cli/io_stb.h"
#include "core/convolver.h"
#include "core/gaussian.h"
#include "core/gaussian2d.h"
#include "core/padding.h"
#include "bench/bench_metrics.h"

using SteadyClock = std::chrono::steady_clock;

static PaddingMode parse_padding(const std::string& s) {
    if (s == "reflect") return PaddingMode::Reflect;
    if (s == "zero") return PaddingMode::Zero;

    std::cerr << "Unknown padding: " << s << "\n";
    std::exit(1);
}

static double mean_ms(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / (double)v.size();
}

static double std_ms(const std::vector<double>& v, double mean) {
    double acc = 0.0;
    for (double x : v) {
        acc += (x - mean) * (x - mean);
    }
    return std::sqrt(acc / (double)v.size());
}

static void usage() {
    std::cout <<
    "Usage:\n"
    "  ./imgbench --in data/wolf.jpg --sigma 1.6 --ksize 11 "
    "--padding reflect|zero --runs 30 --warmup 3 --mode direct|separable|both\n\n"
    "Outputs CSV to stdout:\n"
    "version,width,height,channels,ksize,sigma,padding,runs,warmup,"
    "mean_ms,std_ms,max_err,mse,speedup_vs_direct\n";
}

int main(int argc, char** argv) {
    std::string in_path;
    float sigma = 1.6f;
    int ksize = 11;
    std::string padding_s = "reflect";
    int runs = 30;
    int warmup = 3;
    std::string mode = "both";

    for (int i = 1; i < argc; i++) {
        std::string a = argv[i];

        if (a == "--in" && i + 1 < argc) in_path = argv[++i];
        else if (a == "--sigma" && i + 1 < argc) sigma = std::stof(argv[++i]);
        else if (a == "--ksize" && i + 1 < argc) ksize = std::stoi(argv[++i]);
        else if (a == "--padding" && i + 1 < argc) padding_s = argv[++i];
        else if (a == "--runs" && i + 1 < argc) runs = std::stoi(argv[++i]);
        else if (a == "--warmup" && i + 1 < argc) warmup = std::stoi(argv[++i]);
        else if (a == "--mode" && i + 1 < argc) mode = argv[++i];
        else {
            usage();
            return 1;
        }
    }

    if (in_path.empty()) {
        usage();
        return 1;
    }

    PaddingMode padding = parse_padding(padding_s);

    Image img = load_image_stb(in_path);
    if (img.width() <= 0) {
        std::cerr << "Failed to load image: " << in_path << "\n";
        return 1;
    }

    // Precompute kernels (do NOT time kernel generation)
    Kernel2D g2 = gaussian2d(sigma, ksize);
    Kernel1D g1 = gaussian1d(sigma, ksize);

    // Baseline for error comparison
    Image baseline = convolve2D_direct(img, g2, padding);

    auto benchmark = [&](auto fn, Image& last_output) {
        // Warmup
        for (int i = 0; i < warmup; i++) {
            Image tmp = fn();
        }

        std::vector<double> times;
        times.reserve(runs);

        for (int i = 0; i < runs; i++) {
            auto t0 = SteadyClock::now();
            last_output = fn();
            auto t1 = SteadyClock::now();

            double ms =
                std::chrono::duration<double, std::milli>(t1 - t0).count();
            times.push_back(ms);
        }

        double m = mean_ms(times);
        double s = std_ms(times, m);

        return std::make_pair(m, s);
    };

    std::cout <<
    "version,width,height,channels,ksize,sigma,padding,runs,warmup,"
    "mean_ms,std_ms,max_err,mse,speedup_vs_direct\n";

    double direct_mean = 0.0;

    // ---- Direct 2D ----
    if (mode == "both" || mode == "direct") {
        Image out_direct;
        auto result = benchmark(
            [&]() { return convolve2D_direct(img, g2, padding); },
            out_direct
        );

        direct_mean = result.first;
        ErrorMetrics err = compare_images(out_direct, baseline);

        std::cout
            << "direct2d,"
            << img.width() << ","
            << img.height() << ","
            << img.channels() << ","
            << ksize << ","
            << sigma << ","
            << padding_s << ","
            << runs << ","
            << warmup << ","
            << result.first << ","
            << result.second << ","
            << err.max_abs << ","
            << err.mse << ","
            << 1.0
            << "\n";
    }

    // ---- Separable 1D ----
    if (mode == "both" || mode == "separable") {
        Image out_sep;
        auto result = benchmark(
            [&]() {
                Image tmp = convolve1D_horizontal(img, g1, padding);
                return convolve1D_vertical(tmp, g1, padding);
            },
            out_sep
        );

        ErrorMetrics err = compare_images(out_sep, baseline);

        double speedup = (direct_mean > 0.0)
            ? direct_mean / result.first
            : 0.0;

        std::cout
            << "separable1d,"
            << img.width() << ","
            << img.height() << ","
            << img.channels() << ","
            << ksize << ","
            << sigma << ","
            << padding_s << ","
            << runs << ","
            << warmup << ","
            << result.first << ","
            << result.second << ","
            << err.max_abs << ","
            << err.mse << ","
            << speedup
            << "\n";
    }

    return 0;
}
