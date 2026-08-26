#include "starter_canvas.h"
#include "my_utils.h"

#include <cmath>

void MyCanvas::clear(const GColor& color) {
    GPixel src = color_to_pixel(color);
    int w = fDevice.width();
    int h = fDevice.height();

    for (int y = 0; y < h; y++) {
        GPixel* row = fDevice.getAddr(0, y);
        for (int x = 0; x < w; x++) {
            row[x] = src;
        }
    }
}

void MyCanvas::fillRect(const GRect& rect, const GColor& color) {
    int L = GFloorToInt(rect.left + 0.5f);
    int T = GFloorToInt(rect.top + 0.5f);
    int R = GFloorToInt(rect.right - 0.5f) + 1;
    int B = GFloorToInt(rect.bottom - 0.5f) + 1;

    if (L < 0) {
        L = 0;
    }
    if (T < 0) {
        T = 0;
    }
    if (R > fDevice.width()) {
        R = fDevice.width();
    }
    if (B > fDevice.height()) {
        B = fDevice.height();
    }

    if (L >= R || T >= B) {
        return;
    }

    GPixel src = color_to_pixel(color);
    unsigned sa = GPixel_GetA(src);

    if (sa == 0) {
        return;
    }

    if (sa == 255) {
        for (int y = T; y < B; y++) {
            GPixel* row = fDevice.getAddr(0, y);
            for (int x = L; x < R; x++) {
                row[x] = src;
            }
        }
        return;
    }

    for (int y = T; y < B; y++) {
        GPixel* row = fDevice.getAddr(0, y);
        for (int x = L; x < R; x++) {
            row[x] = src_over(src, row[x]);
        }
    }
}

void MyCanvas::hairLine(GPoint a, GPoint b, const GColor& color) {
    int W = fDevice.width();
    int H = fDevice.height();
    if (W <= 0 || H <= 0) {
        return;
    }

    GPixel src = color_to_pixel(color);
    if (GPixel_GetA(src) == 0) {
        return;
    }

    float x0 = a.x;
    float y0 = a.y;
    float x1 = b.x;
    float y1 = b.y;
    float dx = x1 - x0;
    float dy = y1 - y0;

    if (std::fabs(dx) >= std::fabs(dy)) {
        if (x0 > x1) {
            float tmp = x0;
            x0 = x1;
            x1 = tmp;
            tmp = y0;
            y0 = y1;
            y1 = tmp;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        if (dx == 0) {
            return;
        }

        int i0 = GFloorToInt(x0 + 0.5f);
        int i1 = GFloorToInt(x1 - 0.5f);
        if (i0 < 0) {
            i0 = 0;
        }
        if (i1 > W - 1) {
            i1 = W - 1;
        }

        for (int i = i0; i <= i1; i++) {
            float cy = y0 + ((i + 0.5f) - x0) * dy / dx;
            int j = GFloorToInt(cy);
            if (j >= 0 && j < H) {
                GPixel* p = fDevice.getAddr(i, j);
                *p = src_over(src, *p);
            }
        }
    } else {
        if (y0 > y1) {
            float tmp = x0;
            x0 = x1;
            x1 = tmp;
            tmp = y0;
            y0 = y1;
            y1 = tmp;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        if (dy == 0) {
            return;
        }

        int j0 = GFloorToInt(y0 + 0.5f);
        int j1 = GFloorToInt(y1 - 0.5f);
        if (j0 < 0) {
            j0 = 0;
        }
        if (j1 > H - 1) {
            j1 = H - 1;
        }

        for (int j = j0; j <= j1; j++) {
            float cx = x0 + ((j + 0.5f) - y0) * dx / dy;
            int i = GFloorToInt(cx);
            if (i >= 0 && i < W) {
                GPixel* p = fDevice.getAddr(i, j);
                *p = src_over(src, *p);
            }
        }
    }
}

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) {
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

std::string GDrawSomething(GCanvas* canvas, GISize dim) {
    canvas->clear({0.06f, 0.07f, 0.10f, 1});

    float cx = dim.width * 0.5f;
    float cy = dim.height * 0.5f;

    for (int i = 0; i < 8; i++) {
        float t = i / 7.0f;
        float s = dim.width * (0.85f - 0.08f * i);
        GRect r = GRect::XYWH(cx - s * 0.5f, cy - s * 0.5f, s, s);
        canvas->fillRect(r, {0.15f + 0.7f * t, 0.35f, 0.95f - 0.6f * t, 0.35f});
    }

    int N = 72;
    for (int i = 0; i < N; i++) {
        float theta = i * gFloatPI * 2 / N;
        float radius = dim.width;
        if (dim.height < dim.width) {
            radius = dim.height;
        }
        radius = radius * 0.42f;

        GPoint p0;
        p0.x = cx;
        p0.y = cy;

        GPoint p1;
        p1.x = cx + std::cos(theta) * radius;
        p1.y = cy + std::sin(theta) * radius;

        float u = i / (float)N;
        canvas->hairLine(p0, p1, {u, 1 - u, 0.4f, 0.85f});
    }

    canvas->fillRect(GRect::XYWH(cx - 18, cy - 18, 36, 36), {1, 0.92f, 0.55f, 0.9f});

    return "Nested Squares & Spokes";
}
