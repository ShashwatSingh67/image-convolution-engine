#pragma once

#include <vector>
#include <cstddef>   // size_t
#include <cassert>   // assert
#include <algorithm> // needed for std::clamp

// A lightweight float image container.
//
// Why float?
// - keps data precice withought int roundings when performing convolutions
//
// Memory layout used interleaving channels meaning each pixel has its RGB value stored so 3 x 1 image would be stored as RGB RGB RGB
// index = (y * width + x) * channels + c
    //the order and RGG value is stored in a float (x,y,c) -> i
    //each full row contains width pixels
    //row y has y * width pixels before it
    //then x pixels before column x
        //p = y * width + x
    //channels is how many nums are stored per pixel, as an array is a long line of floats need to see which val we looking at (R, G or B)
    //chanels is used to mulitply the vals so that we have the right factor of adressing the data:
        //1 for greyscale
        //3 for RGB
        //4 for RGBA (A is alpha which is the transperency)
        //index = (y * width + x) * channels + c;
            //+ c is for which float we are looking at

    //source to find this "interleaved" format: https://learn.arm.com/learning-paths/mobile-graphics-and-gaming/android_halide/intro/?utm_source=chatgpt.com


    //will be using pngs- widespread and supports RBG
class Image {
public:
    Image() = default;

    // Construct an image with given dimensions.
    // All pixels are initialized to `init_value`.
    Image(int w, int h, int ch, float init_value = 0.0f) //width, height, channel size, float val
        : width_(w), height_(h), channels_(ch),
          data_(static_cast<size_t>(w) * static_cast<size_t>(h) * static_cast<size_t>(ch), init_value)
    {
        assert(w > 0 && h > 0 && ch > 0); //assert function checks if (these statements) are true and continues if it is
                                          //this stops invalid data sizes from passing
    }

    //getters
    int width() const { return width_; }
    int height() const { return height_; }
    int channels() const { return channels_; }

    // w*h*c = size
    size_t size() const { return data_.size(); }

    // returns raw pointer will be used in testing
    float* raw() { return data_.data(); }
    const float* raw() const { return data_.data(); }

    // Bounds-checked access.
    // NOTE: uses assert; in Release builds asserts may be disabled.
    //converts the float to a 1D index and returns
    float& at(int x, int y, int c) {    
        assert(in_bounds(x, y, c));
        return data_[index(x, y, c)];
    }
    
    const float& at(int x, int y, int c) const {
        assert(in_bounds(x, y, c));
        return data_[index(x, y, c)];
    }

    // fill entire image with a single float.
    void fill(float value) {
        std::fill(data_.begin(), data_.end(), value);
    }

    // keeps float vals 0 <= float val <= 1
    void clamp_inplace(float lo, float hi) {
        for (float& v : data_) {
            v = std::clamp(v, lo, hi);
        }
    }

private:
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
    std::vector<float> data_;

    bool in_bounds(int x, int y, int c) const {
        return (x >= 0 && x < width_) &&
               (y >= 0 && y < height_) &&
               (c >= 0 && c < channels_);
    }

    //converts the interleaved coords to 1D offset of storage
        //index = (y * width + x) * channels + c;
    size_t index(int x, int y, int c) const {
        // Cast to size_t to avoid signed overflow issues.
        return (static_cast<size_t>(y) * static_cast<size_t>(width_) + static_cast<size_t>(x))
                * static_cast<size_t>(channels_) + static_cast<size_t>(c);
    }
};
