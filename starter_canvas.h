#ifndef _g_starter_canvas_h_
#define _g_starter_canvas_h_

#include "include/GBitmap.h"
#include "include/GCanvas.h"
#include "include/GColor.h"
#include "include/GPoint.h"
#include "include/GRect.h"

class MyCanvas : public GCanvas {
public:
    MyCanvas(const GBitmap& device) : fDevice(device) {}

    void clear(const GColor&) override;
    void hairLine(GPoint, GPoint, const GColor&) override;
    void fillRect(const GRect&, const GColor&) override;

private:
    const GBitmap fDevice;
};

#endif
