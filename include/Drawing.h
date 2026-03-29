#pragma once
#include "Processing.h"
#include <algorithm>
#include <cmath>

namespace cnn {

class Draw {
public:
    void drawCircle(Image& img, Point center, int radius, unsigned char color);

    void drawLine(Image& img, Point p1, Point p2, unsigned char color);

    void drawRectangle(Image& img, Rectangle rect, unsigned char color);

    void drawRectangle(Image& img, Point topL, Point botR, unsigned char color);
};

} // namespace cnn