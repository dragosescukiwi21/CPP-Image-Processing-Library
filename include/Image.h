#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace cnn {

class Size {
public:
	unsigned int width;
	unsigned int height;

	Size(unsigned int w, unsigned int h) : width(w), height(h) {}
};

class Point {
public:
	unsigned int x;
	unsigned int y;
	Point() : x(0), y(0) {}
	Point(unsigned int x, unsigned int y) : x(x), y(y) {}

	friend std::ostream& operator<<(std::ostream& os, const Point& pt) {
		os << pt.x << " " << pt.y;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Point& pt) {
		is >> pt.x >> pt.y;
		return is;
	}
};

class Rectangle {
public:
	unsigned int x, y, height, width;
	Rectangle() : x(0), y(0), height(0), width(0) {}
	Rectangle(unsigned int x, unsigned int y, unsigned int height, unsigned int width)
		: x(x), y(y), height(height), width(width) {
	}

	Rectangle(const Point& topLeft, const Point& bottomRight) {
		x = topLeft.x;
		y = topLeft.y;
		width = bottomRight.x - topLeft.x;
		height = bottomRight.y - topLeft.y;
	}

	friend std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
		os << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;
		return os;
	}
	friend std::istream& operator>>(std::istream& is, Rectangle& rect) {
		is >> rect.x >> rect.y >> rect.width >> rect.height;
		return is;
	}

	Rectangle operator+(const Point& delta) const;
	Rectangle operator-(const Point& delta) const;
	Rectangle operator&(const Rectangle& other) const;
	Rectangle operator|(const Rectangle& other) const;
};

class Image {
private:
	unsigned char** m_data;
	unsigned int m_width;
	unsigned int m_height;

public:
	Image();
	Image(unsigned int width, unsigned int height);
	Image(const Image& other);

	~Image();

	bool load(std::string ImagePath);
	bool save(std::string ImagePath);

	Image& operator=(const Image& other);

	Image operator+(const Image& other);
	Image operator-(const Image& other);

	bool getROI(Image& roiImg, Rectangle roiRect);

	bool getROI(Image& roiImg, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

	bool isEmpty() const;

	Size size() const;

	unsigned int width() const;
	unsigned int height() const;

	unsigned char& at(unsigned int x, unsigned int y);
	unsigned char& at(Point pt);
	const unsigned char& at(unsigned int x, unsigned int y) const;

	unsigned char* row(int y);

	void release();

	friend std::ostream& operator<<(std::ostream& os, const Image& dt);

	static Image zeros(unsigned int width, unsigned int height);
	static Image ones(unsigned int width, unsigned int height);

};

} // namespace cnn