#pragma once
#include <cassert>

// padding mode with zero padding and relfect padding options
enum class PaddingMode {
    Zero,
    Reflect
};

// Reflect index into [0, len-1] range so that it can be blured
inline int reflect_index(int i, int len) {
    assert(len > 0);
    if (len == 1) return 0;

    while (i < 0 || i >= len) {
        if (i < 0) i = -i;
        if (i >= len) i = 2 * len - 2 - i;
    }
    return i;
}
