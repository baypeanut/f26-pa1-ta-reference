#ifndef _my_utils_h_
#define _my_utils_h_

#include "include/GColor.h"
#include "include/GMath.h"
#include "include/GPixel.h"
#include "include/GRect.h"

#include <algorithm>

static unsigned div255(unsigned x) {
    return (x + 128) * 257 >> 16;
}

static GPixel color_to_pixel(const GColor& c) {
    float a = c.a;
    int ai = GRoundToInt(a * 255);
    int ri = GRoundToInt(c.r * a * 255);
    int gi = GRoundToInt(c.g * a * 255);
    int bi = GRoundToInt(c.b * a * 255);
    return GPixel_PackARGB(ai, ri, gi, bi);
}

static GPixel src_over(GPixel src, GPixel dst) {
    unsigned sa = GPixel_GetA(src);
    if (sa == 255) {
        return src;
    }
    if (sa == 0) {
        return dst;
    }

    unsigned inv = 255 - sa;
    unsigned a = sa + div255(GPixel_GetA(dst) * inv);
    unsigned r = GPixel_GetR(src) + div255(GPixel_GetR(dst) * inv);
    unsigned g = GPixel_GetG(src) + div255(GPixel_GetG(dst) * inv);
    unsigned b = GPixel_GetB(src) + div255(GPixel_GetB(dst) * inv);
    return GPixel_PackARGB(a, r, g, b);
}

#endif
