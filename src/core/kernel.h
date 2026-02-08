#pragma once
#include <vector>
#include <cassert>

// Stores a 1D convolution kernel of odd length (e.g., 3,5,11,...)
//will be used later

//the kernel represents weights not pixels and it is what you are mulitplying the image by in order to get certain transoformations:
//we have the class for a 1D Kernal and a 2D where the 1D is the optimisaed variant of the 2D kernel    

//https://www.geeksforgeeks.org/deep-learning/types-of-convolution-kernels/
//in 1D kernel at each position the kernel's weights are multiplied by the corresponding input values and the results are summed up to produce a single value.
class Kernel1D {
public:
    Kernel1D() = default;

    // size must be odd and > 0
    explicit Kernel1D(int size)
        : size_(size), data_(static_cast<size_t>(size), 0.0f)
    {
        assert(size > 0 && (size % 2 == 1));
    }

    int size() const { return size_; }
    int radius() const { return size_ / 2; }

    float& at(int i) {
        assert(i >= 0 && i < size_);
        return data_[static_cast<size_t>(i)];
    }

    const float& at(int i) const {
        assert(i >= 0 && i < size_);
        return data_[static_cast<size_t>(i)];
    }

    const std::vector<float>& data() const { return data_; }

private:
    int size_ = 0;
    std::vector<float> data_;
};

// Stores a 2D convolution kernel of odd width/height.
//used for current calculations
//basically the brute force way of sliding over the input image and performing calculations 
class Kernel2D {
public:
    Kernel2D() = default;

    Kernel2D(int w, int h)
        : w_(w), h_(h), data_(static_cast<size_t>(w) * static_cast<size_t>(h), 0.0f)
    {
        assert(w > 0 && h > 0);
        assert(w % 2 == 1 && h % 2 == 1); //checking bounds and if its odd or evern
    }

    int width() const { return w_; }
    int height() const { return h_; }
    int radius_x() const { return w_ / 2; }
    int radius_y() const { return h_ / 2; }

    float& at(int x, int y) {
        assert(x >= 0 && x < w_);
        assert(y >= 0 && y < h_);
        return data_[static_cast<size_t>(y * w_ + x)];
    }

    const float& at(int x, int y) const {
        assert(x >= 0 && x < w_);
        assert(y >= 0 && y < h_);
        return data_[static_cast<size_t>(y * w_ + x)];
    }

    const std::vector<float>& data() const { return data_; } //stored in 1d array

private:
    int w_ = 0;
    int h_ = 0;
    std::vector<float> data_;
};
