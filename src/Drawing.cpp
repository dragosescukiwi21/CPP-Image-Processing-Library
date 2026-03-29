#include "Drawing.h"

namespace cnn {

/**
 * @brief Draws a circle on the given image.
 * @param img The image to draw on.
 * @param center The center point of the circle.
 * @param rad The radius of the circle.
 * @param col The color of the circle.
 */
void Draw::drawCircle(Image& img, Point center, int rad, unsigned char col) {
    int x = rad;
    int y = 0;
    int error_val = 0;

    while (x >= y) {
        img.at(center.x + x, center.y + y) = col;
        img.at(center.x + y, center.y + x) = col;

        img.at(center.x - y, center.y + x) = col;
        img.at(center.x - x, center.y + y) = col;

        img.at(center.x - x, center.y - y) = col;
        img.at(center.x - y, center.y - x) = col;

        img.at(center.x + y, center.y - x) = col;
        img.at(center.x + x, center.y - y) = col;

        if (error_val <= 0) {
            y += 1;
            error_val += 2 * y + 1;
        }
        if (error_val > 0) {
            x -= 1;
            error_val -= 2 * x + 1;
        }
    }
}

/**
 * @brief Draws a line on the given image using Bresenham's line algorithm.
 * @param img The image to draw on.
 * @param p1 The starting point of the line.
 * @param p2 The ending point of the line.
 * @param col The color of the line.
 */
void Draw::drawLine(Image& img, Point p1, Point p2, unsigned char col) {
    int dx = std::abs(static_cast<int>(p2.x) - static_cast<int>(p1.x));
    int dy = -std::abs(static_cast<int>(p2.y) - static_cast<int>(p1.y));

    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;

    int err = dx + dy;

    while (true) {
        if (p1.x >= 0 && p1.x < img.width() && p1.y >= 0 && p1.y < img.height())
            img.at(p1.x, p1.y) = col;

        if (p1.x == p2.x && p1.y == p2.y) break;
        int e2 = 2 * err;

        if (e2 >= dy) { err += dy; p1.x += sx; }
        if (e2 <= dx) { err += dx; p1.y += sy; }
    }
}

/**
 * @brief Draws a rectangle on the given image.
 * @param img The image to draw on.
 * @param rect The rectangle to draw.
 * @param col The color of the rectangle.
 */
void Draw::drawRectangle(Image& img, Rectangle rect, unsigned char col) {
    Point topL(rect.x, rect.y);
    Point botR(rect.x + rect.width, rect.y + rect.height);

    drawRectangle(img, topL, botR, col);
}

/**
 * @brief Draws a rectangle on the given image using two points.
 * @param img The image to draw on.
 * @param topL The top-left point of the rectangle.
 * @param botR The bottom-right point of the rectangle.
 * @param col The color of the rectangle.
 */
void Draw::drawRectangle(Image& img, Point topL, Point botR, unsigned char col) {
    Point topR(botR.x, topL.y);
    Point botL(topL.x, botR.y);

    drawLine(img, topL, topR, col);
    drawLine(img, topR, botR, col);
    drawLine(img, botR, botL, col);
    drawLine(img, botL, topL, col);
}

} // namespace cnn