#include "Image.h"

#include <fstream> 
#include <iostream> 
#include <algorithm> 
#include <exception> 

namespace cnn {

/**
 * @brief Overloads the + operator to translate a Rectangle by a Point.
 * @param delta The Point specifying the translation amounts for x and y.
 * @return A new Rectangle translated by the delta Point.
 */
Rectangle Rectangle::operator+(const Point& delta) const {
    return Rectangle(x + delta.x, y + delta.y, height, width);
}

/**
 * @brief Overloads the - operator to translate a Rectangle by a Point in the opposite direction.
 * @param delta The Point specifying the translation amounts for x and y.
 * @return A new Rectangle translated by the negative of the delta Point.
 */
Rectangle Rectangle::operator-(const Point& delta) const {
    return Rectangle(x - delta.x, y - delta.y, height, width);
}

/**
 * @brief Overloads the | operator to compute the intersection of two Rectangles.
 * @param other The other Rectangle to intersect with.
 * @return A new Rectangle representing the intersection of the two Rectangles.
 * If there is no intersection, a Rectangle with zero width or height is returned.
 */
Rectangle Rectangle::operator|(const Rectangle& other) const {
    unsigned int newX = std::max(x, other.x);
    unsigned int newY = std::max(y, other.y);
    unsigned int newWidth = std::min(x + width, other.x + other.width) - newX;
    unsigned int newHeight = std::min(y + height, other.y + other.height) - newY;

    return Rectangle(newX, newY, newHeight, newWidth);
}

/**
 * @brief Overloads the & operator to compute the union (bounding box) of two Rectangles.
 * @param other The other Rectangle to form a union with.
 * @return A new Rectangle representing the smallest bounding box containing both Rectangles.
 */
Rectangle Rectangle::operator&(const Rectangle& other) const {
    unsigned int newX = std::min(x, other.x);
    unsigned int newY = std::min(y, other.y);
    unsigned int newWidth = std::max(x + width, other.x + other.width) - newX;
    unsigned int newHeight = std::max(y + height, other.y + other.height) - newY;

    return Rectangle(newX, newY, newHeight, newWidth);
}

/**
 * @brief Default constructor for Image. Initializes an empty image.
 */
Image::Image() : m_data(nullptr), m_width(0), m_height(0) {
}

/**
 * @brief Constructs an Image with specified width and height.
 * Initializes pixel data to uninitialized values.
 * @param width The width of the image.
 * @param height The height of the image.
 */
Image::Image(unsigned int width, unsigned int height) : m_width(width), m_height(height) {
    m_data = new unsigned char* [m_height];

    for (unsigned int i = 0; i < m_height; i++) {
        m_data[i] = new unsigned char[m_width];
    }
}

/**
 * @brief Copy constructor for Image. Performs a deep copy of the other image.
 * @param other The Image object to copy.
 */
Image::Image(const Image& other) : m_height(other.height()), m_width(other.width()) {
    if (other.m_data && m_width > 0 && m_height > 0) {

        m_data = new unsigned char* [m_height];

        for (unsigned int i = 0; i < m_height; ++i) {
            m_data[i] = new unsigned char[m_width];
            for (unsigned int j = 0; j < m_width; ++j) {
                m_data[i][j] = other.m_data[i][j];
            }

        }
    }
    else {
        m_data = nullptr;
        m_width = m_height = 0;

    }
}

/**
 * @brief Destructor for Image. Releases allocated memory.
 */
Image::~Image() {
    release();
}

/**
 * @brief Assignment operator for Image. Performs a deep copy.
 * @param other The Image object to assign from.
 * @return A reference to this Image.
 */
Image& Image::operator= (const Image& other) {
    if (this != &other) {
        if (other.m_data && other.m_width > 0 && other.m_height > 0) {
            release();
            m_width = other.width();
            m_height = other.height();
            m_data = new unsigned char* [m_height];

            for (unsigned int i = 0; i < m_height; ++i) {

                m_data[i] = new unsigned char[m_width];
                for (unsigned int j = 0; j < m_width; ++j) {
                    m_data[i][j] = other.m_data[i][j];
                }
            }
        }
    }
    else
    {
        // Consider throwing an exception or specific handling for self-assignment
        // For now, it does nothing as per original code.
    }
    return *this;
}

/**
 * @brief Loads an image from a PGM file.
 * @param imagePath The path to the PGM image file.
 * @return True if loading was successful, false otherwise.
 */
bool Image::load(std::string imagePath) {
    release();

    std::ifstream file(imagePath);
    if (!file.is_open()) {
        std::cerr << "Error : " << imagePath << std::endl;
        return false;
    }

    std::string magic;
    file >> magic;
    if (magic != "P2") {
        std::cerr << "Invalid magic number: " << magic;
        return false;
    }

    file >> m_width >> m_height;
    unsigned int maxVal;
    file >> maxVal;

    if (maxVal > 255) {
        std::cerr << "Max value " << maxVal << " reached. Try again\n";
        return false;
    }

    m_data = new unsigned char* [m_height];
    for (unsigned int i = 0; i < m_height; ++i) {
        m_data[i] = new unsigned char[m_width];
    }

    for (unsigned int y = 0; y < m_height; ++y) {
        for (unsigned int x = 0; x < m_width; ++x) {
            int pixelValue;
            file >> pixelValue;
            m_data[y][x] = static_cast<unsigned char>(pixelValue);
        }
    }

    file.close();
    return true;
}

/**
 * @brief Saves the image to a PGM file.
 * @param imagePath The path to save the PGM image file.
 * @return True if saving was successful, false otherwise.
 */
bool Image::save(const std::string imagePath) {
    std::ofstream file(imagePath);

    if (!file) {
        std::cerr << "Error: " << imagePath << std::endl;
        return false;
    }

    file << "P2\n" <<
        m_width << " " << m_height
        << "\n255\n";

    for (unsigned int y = 0; y < m_height; y++) {
        for (unsigned int x = 0; x < m_width; x++) {
            file << static_cast<int>(m_data[y][x]) << " ";
        }
        file << "\n";
    }

    return true;
}

/**
 * @brief Overloads the + operator to add two images pixel-wise.
 * Pixel values are clamped to the range [0, 255].
 * @param other The other Image to add.
 * @return A new Image representing the sum. Returns an empty Image if dimensions mismatch.
 */
Image Image::operator+(const Image& other) {
    if (m_width != other.m_width || m_height != other.m_height) return Image();
    Image ans(m_width, m_height);

    for (unsigned int i = 0; i < m_height; ++i) {
        for (unsigned int j = 0; j < m_width; ++j) {
            int temp1, temp2;
            temp1 = static_cast<int>(m_data[i][j]);
            temp2 = static_cast<int>(other.m_data[i][j]);
            int sum = temp1 + temp2;
            if (sum > 255) sum = 255;
            ans.m_data[i][j] = static_cast<unsigned char>(sum);
        }
    }
    return ans;
}

/**
 * @brief Overloads the - operator to subtract two images pixel-wise.
 * Pixel values are clamped to the range [0, 255].
 * @param other The other Image to subtract.
 * @return A new Image representing the difference. Returns an empty Image if dimensions mismatch.
 */
Image Image::operator-(const Image& other) {
    if (m_width != other.m_width || m_height != other.m_height) return Image();

    Image ans(m_width, m_height);

    for (unsigned int i = 0; i < m_height; ++i) {
        for (unsigned int j = 0; j < m_width; ++j) {
            int temp1, temp2;
            temp1 = static_cast<int>(m_data[i][j]);
            temp2 = static_cast<int>(other.m_data[i][j]);
            int diff = temp1 - temp2;
            if (diff < 0) diff = 0;
            ans.m_data[i][j] = static_cast<unsigned char>(diff);
        }
    }
    return ans;
}

/**
 * @brief Gets the width of the image.
 * @return The width of the image.
 */
unsigned int Image::width() const { return m_width; }

/**
 * @brief Gets the height of the image.
 * @return The height of the image.
 */
unsigned int Image::height() const { return m_height; }

/**
 * @brief Gets the size (width and height) of the image.
 * @return A Size object representing the image dimensions.
 */
Size Image::size() const { return Size(m_width, m_height); }

/**
 * @brief Checks if the image is empty (no data or zero dimensions).
 * @return True if the image is empty, false otherwise.
 */
bool Image::isEmpty() const { return !m_data || m_width == 0 || m_height == 0; }

/**
 * @brief Accesses the pixel value at the specified coordinates (non-const version).
 * @param y The y-coordinate (row).
 * @param x The x-coordinate (column).
 * @return A reference to the pixel value.
 */
unsigned char& Image::at(unsigned int y, unsigned int x) { return m_data[y][x]; }

/**
 * @brief Accesses the pixel value at the specified Point (non-const version).
 * @param pt The Point specifying the coordinates.
 * @return A reference to the pixel value.
 */
unsigned char& Image::at(Point pt) { return m_data[pt.y][pt.x]; }

/**
 * @brief Accesses the pixel value at the specified coordinates (const version).
 * @param y The y-coordinate (row).
 * @param x The x-coordinate (column).
 * @return A const reference to the pixel value.
 */
const unsigned char& Image::at(unsigned int y, unsigned int x) const {
    return m_data[y][x];
}

/**
 * @brief Extracts a Region of Interest (ROI) from the image.
 * Performs a deep copy of the specified rectangular region into a new Image object.
 * @param roiImg The Image object to store the ROI.
 * @param x The x-coordinate of the top-left corner of the ROI.
 * @param y The y-coordinate of the top-left corner of the ROI.
 * @param w The width of the ROI.
 * @param h The height of the ROI.
 * @return True if ROI extraction was successful, false if the ROI is out of bounds.
 */
bool Image::getROI(Image& roiImg, unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
    if (x + w > m_width || y + h > m_height) return false;
    roiImg = Image(w, h);

    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            roiImg.m_data[i][j] = m_data[y + i][x + j];
        }
    }

    return true;
}

/**
 * @brief Extracts a Region of Interest (ROI) from the image using a Rectangle object.
 * @param roiImg The Image object to store the ROI.
 * @param roiRect The Rectangle defining the ROI.
 * @return True if ROI extraction was successful, false otherwise.
 */
bool Image::getROI(Image& roiImg, Rectangle roiRect) {
    return getROI(roiImg, roiRect.x, roiRect.y, roiRect.width, roiRect.height);
}

/**
 * @brief Releases the memory allocated for the image data and resets dimensions.
 */
void Image::release() {
    if (m_data) {
        for (unsigned int i = 0; i < m_height; ++i) {
            delete[] m_data[i];
        }
        delete[] m_data;
        m_data = nullptr;
    }

    m_width = 0;
    m_height = 0;
}

/**
 * @brief Gets a pointer to a specific row of the image data.
 * @param y The row index.
 * @return A pointer to the first element of the specified row, or nullptr if y is out of bounds.
 */
unsigned char* Image::row(int y) {
    if (y < 0 || static_cast<unsigned int>(y) >= m_height) return nullptr;
    return m_data[y];
}

/**
 * @brief Creates a new image of specified dimensions, initialized with zeros.
 * @param w The width of the new image.
 * @param h The height of the new image.
 * @return A new Image object filled with zeros.
 */
Image Image::zeros(unsigned int w, unsigned int h) {
    Image img(w, h);
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            img.m_data[i][j] = 0;
        }
    }
    return img;
}

/**
 * @brief Creates a new image of specified dimensions, initialized with ones.
 * @param w The width of the new image.
 * @param h The height of the new image.
 * @return A new Image object filled with ones.
 */
Image Image::ones(unsigned int w, unsigned int h) {
    Image img(w, h);

    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            img.m_data[i][j] = 1;
        }
    }

    return img;
}

/**
 * @brief Overloads the << operator to print image information and pixel data to an ostream.
 * @param os The ostream object.
 * @param img The Image to print.
 * @return A reference to the ostream object.
 */
std::ostream& operator<<(std::ostream& os, const Image& img) {
    os << "Image: " << img.m_width << "x" << img.m_height << "\n";

    for (unsigned int i = 0; i < img.m_height; ++i) {
        for (unsigned int j = 0; j < img.m_width; ++j) {
            os << static_cast<int>(img.m_data[i][j]) << " ";
        }
        os << "\n";
    }

    return os;
}

} // namespace cnn