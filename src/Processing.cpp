#include "Processing.h"

namespace cnn {

/**
 * @brief Constructs a BrightnessAndContrast processor.
 * @param alpha The contrast control value.
 * @param beta The brightness control value.
 */
BrightnessAndContrast::BrightnessAndContrast(float alpha, int beta)
    : alpha(alpha), beta(beta) {
}

/**
 * @brief Processes the source image to adjust brightness and contrast.
 * @param sourceImage The input image.
 * @param img The output image with adjusted brightness and contrast.
 * The output image will be resized to match the source image.
 * If the source image is empty, the output image will be released.
 */
void BrightnessAndContrast::process(const Image& sourceImage, Image& img) {
    if (sourceImage.isEmpty()) {
        img.release();
        return;
    }

    img = Image(sourceImage.width(), sourceImage.height());

    for (unsigned int rowId = 0; rowId < sourceImage.height(); ++rowId) {
        for (unsigned int colId = 0; colId < sourceImage.width(); ++colId) {
            float adjustedValue = static_cast<float>(sourceImage.at(rowId, colId)) * alpha + beta;

            if (adjustedValue < 0) adjustedValue = 0;
            if (adjustedValue > 255) adjustedValue = 255;

            img.at(rowId, colId) = static_cast<unsigned char>(adjustedValue);
        }
    }
}


/**
 * @brief Processes the source image to apply gamma correction.
 * @param sourceImage The input image.
 * @param img The output image with gamma correction applied.
 * The output image will be resized to match the source image.
 * If the source image is empty, the output image will be released.
 */
void GammaCorrection::process(const Image& sourceImage, Image& img) {
    if (sourceImage.isEmpty()) {
        img.release();
        return;
    }

    img = Image(sourceImage.width(), sourceImage.height());
    double invGamma = 1.0 / gamma;

    for (unsigned int rowIdx = 0; rowIdx < sourceImage.height(); ++rowIdx) {
        for (unsigned int colIdx = 0; colIdx < sourceImage.width(); ++colIdx) {

            double normalizedPixel = static_cast<double>(sourceImage.at(rowIdx, colIdx)) / 255.0;
            double correctedPixel = std::pow(normalizedPixel, invGamma) * 255.0;

            if (correctedPixel > 255.0) correctedPixel = 255.0;
            if (correctedPixel < 0.0) correctedPixel = 0.0;

            img.at(rowIdx, colIdx) = static_cast<unsigned char>(correctedPixel);
        }
    }
}


/**
 * @brief Processes the source image by applying convolution with the stored kernel.
 * @param sourceImage The input image.
 * @param img The output convolved image.
 * The output image will be resized to match the source image.
 * If the source image or kernel is empty, the output image will be released.
 */
void ImageConvolution::process(const Image& sourceImage, Image& img) {
    if (sourceImage.isEmpty() || kernel.empty() || kernel[0].empty()) {
        img.release();
        return;
    }

    const int kernelHeight = kernel.size();
    const int kernelWidth = kernel[0].size();
    const int kernelCenterY = kernelHeight / 2;
    const int kernelCenterX = kernelWidth / 2;

    img = Image(sourceImage.width(), sourceImage.height());

    for (unsigned int imgY = 0; imgY < sourceImage.height(); imgY++) {
        for (unsigned int imgX = 0; imgX < sourceImage.width(); imgX++) {
            float pixelSum = 0.0;

            for (int kY = 0; kY < kernelHeight; kY++) {
                for (int kX = 0; kX < kernelWidth; kX++) {
                    int sourceY = static_cast<int>(imgY) + kY - kernelCenterY;
                    int sourceX = static_cast<int>(imgX) + kX - kernelCenterX;

                    if (sourceY >= 0 && static_cast<unsigned int>(sourceY) < sourceImage.height() &&
                        sourceX >= 0 && static_cast<unsigned int>(sourceX) < sourceImage.width()) {
                        pixelSum += kernel[kY][kX] * static_cast<float>(sourceImage.at(sourceY, sourceX));
                    }
                }
            }

            if (pixelSum > 255.0f) pixelSum = 255.0f;
            if (pixelSum < 0.0f) pixelSum = 0.0f;

            img.at(imgY, imgX) = static_cast<unsigned char>(pixelSum);
        }
    }
}

} // namespace cnn