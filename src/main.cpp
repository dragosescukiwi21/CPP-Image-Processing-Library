#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "Image.h"
#include "Processing.h"
#include "Drawing.h"

using namespace cnn;

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            return value;
        }
    }
}

float getFloatInput(const std::string& prompt) {
    float value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";

            std::cin.clear();
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            return value;
        }
    }
}

// Global Image object to hold the current image being processed
Image currentImage;


void handleLoadImage() {
    std::string path;
    std::cout << "Enter path to PGM image file to load: ";
    std::getline(std::cin, path);

    if (currentImage.load(path)) {
        std::cout << "Image loaded successfully from " << path << ".\n";
    }
    else {
        std::cout << "Failed to load image from " << path << "\n";
    }
}

void handleSaveImage() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded to save. Please load an image first.\n";
        return;
    }

    std::string path;
    std::cout << "Enter path to save PGM image file (e.g., output.pgm): ";
    std::getline(std::cin, path);

    if (currentImage.save(path)) {
        std::cout << "Image saved successfully to " << path << ".\n";
    }
    else {
        std::cout << "Failed to save image to " << path << ".\n";
    }
}

void handleBrightnessAndContrast() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    float alpha = getFloatInput("Enter alpha value (e.g., 1.0 for no change, <1 for darker, >1 for brighter contrast): ");
    int beta = getIntInput("Enter beta value (e.g., 0 for no change, negative for darker, positive for brighter): ");

    Image dstImage;
    BrightnessAndContrast bc(alpha, beta);
    bc.process(currentImage, dstImage);

    currentImage = dstImage;
    std::cout << "Brightness and contrast applied successfully.\n";
}

void handleGammaCorrection() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    float gamma = getFloatInput("Enter gamma value (e.g., 1.0 for no change, <1 for darker, >1 for brighter): ");

    Image dstImage;
    GammaCorrection gc(gamma);
    gc.process(currentImage, dstImage);

    currentImage = dstImage;
    std::cout << "Gamma correction applied successfully.\n";
}

void handleImageConvolution() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    std::cout << "\nSelect a Convolution Kernel:\n";
    std::cout << "1. Identity Kernel (No-op)\n";
    std::cout << "2. Mean Blur (3x3)\n";
    std::cout << "3. Gaussian Blur (3x3)\n";
    std::cout << "4. Horizontal Sobel Edge Detection\n";
    std::cout << "5. Vertical Sobel Edge Detection\n";
    int choice = getIntInput("Enter your choice: ");

    std::vector<std::vector<float>> kernel;
    std::string kernelName = "";

    switch (choice) {
    case 1: // Identity
        kernel = { {0, 0, 0}, {0, 1, 0}, {0, 0, 0} };
        kernelName = "Identity";
        break;
    case 2: // Mean Blur
        kernel = { {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
                  {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
                  {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f} };
        kernelName = "Mean Blur";
        break;
    case 3: // Gaussian Blur
        kernel = { {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f},
                  {2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f},
                  {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f} };
        kernelName = "Gaussian Blur";
        break;
    case 4: // Horizontal Sobel
        kernel = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
        kernelName = "Horizontal Sobel";
        break;
    case 5: // Vertical Sobel
        kernel = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
        kernelName = "Vertical Sobel";
        break;
    default:
        std::cout << "Invalid kernel choice.\n";
        return;
    }

    Image dstImage;
    ImageConvolution conv(kernel);
    conv.process(currentImage, dstImage);

    currentImage = dstImage;
    std::cout << kernelName << " convolution applied successfully.\n";
}

void handleDrawCircle() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    int centerX = getIntInput("Enter center X coordinate: ");
    int centerY = getIntInput("Enter center Y coordinate: ");
    int radius = getIntInput("Enter radius: ");
    unsigned char color = static_cast<unsigned char>(getIntInput("Enter color (0-255): "));

    Draw drawer;
    drawer.drawCircle(currentImage, Point(centerX, centerY), radius, color);
    std::cout << "Circle drawn successfully.\n";
}

void handleDrawLine() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    int p1x = getIntInput("Enter start point X coordinate (P1.x): ");
    int p1y = getIntInput("Enter start point Y coordinate (P1.y): ");
    int p2x = getIntInput("Enter end point X coordinate (P2.x): ");
    int p2y = getIntInput("Enter end point Y coordinate (P2.y): ");
    unsigned char color = static_cast<unsigned char>(getIntInput("Enter color (0-255): "));

    Draw d;
    d.drawLine(currentImage, Point(p1x, p1y), Point(p2x, p2y), color);
    std::cout << "Line drawn successfully.\n";
}

void handleDrawRectangle() {
    if (currentImage.isEmpty()) {
        std::cout << "No image loaded. Please load an image first.\n";
        return;
    }

    std::cout << "Choose rectangle input method:\n";
    std::cout << "1. Top-left (x,y), width, height\n";
    std::cout << "2. Top-left (x,y), Bottom-right (x,y)\n";
    int choice = getIntInput("Enter your choice: ");

    Draw d;
    unsigned char color = static_cast<unsigned char>(getIntInput("Enter color (0-255): "));

    if (choice == 1) {
        int rectX = getIntInput("Enter top-left X coordinate: ");
        int rectY = getIntInput("Enter top-left Y coordinate: ");
        int rectWidth = getIntInput("Enter width: ");
        int rectHeight = getIntInput("Enter height: ");
        d.drawRectangle(currentImage, Rectangle(rectX, rectY, rectHeight, rectWidth), color);
        std::cout << "Rectangle drawn successfully.\n";
    }
    else if (choice == 2) {
        int tlX = getIntInput("Enter top-left X coordinate: ");
        int tlY = getIntInput("Enter top-left Y coordinate: ");
        int brX = getIntInput("Enter bottom-right X coordinate: ");
        int brY = getIntInput("Enter bottom-right Y coordinate: ");
        d.drawRectangle(currentImage, Point(tlX, tlY), Point(brX, brY), color);
        std::cout << "Rectangle drawn successfully.\n";
    }
    else {
        std::cout << "Invalid choice.\n";
    }
}


// Menu
int main() {
    int choice;
    do {

        std::cout << "1. Load \n";
        std::cout << "2. Save \n";
        std::cout << "3. Apply Brightness and Contrast\n";
        std::cout << "4. Apply Gamma Correction\n";
        std::cout << "5. Apply Image Convolution (Blur, Edge Detection)\n";
        std::cout << "6. Draw Circle\n";
        std::cout << "7. Draw Line\n";
        std::cout << "8. Draw Rectangle\n";
        std::cout << "9. Exit\n";
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
        case 1: handleLoadImage(); break;
        case 2: handleSaveImage(); break;
        case 3: handleBrightnessAndContrast(); break;
        case 4: handleGammaCorrection(); break;
        case 5: handleImageConvolution(); break;
        case 6: handleDrawCircle(); break;
        case 7: handleDrawLine(); break;
        case 8: handleDrawRectangle(); break;
		case 9: std::cout << "Exiting..\n"; break;
        case 0: std::cout << "Error\n"; break;
        default: std::cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);

}
