#pragma once
#include "Image.h"
#include <vector>


namespace cnn {

class ImageProcessing {
public:
	// pure virtual to force all subclasses to implement this method
	virtual void process(const Image& src, Image& dst) = 0;
};

class BrightnessAndContrast : public ImageProcessing {
private:
	float alpha;
	int beta;

public:
	BrightnessAndContrast() : alpha(1.0), beta(0) {}

	BrightnessAndContrast(float alpha, int beta);

	void process(const Image& src, Image& dst) override;
};

class GammaCorrection : public ImageProcessing {
private:
	double gamma;

public:
	GammaCorrection() : gamma(1.0) {}
	GammaCorrection(double gamma) : gamma(gamma) {}

	void process(const Image& src, Image& dst) override;
};

class ImageConvolution : public ImageProcessing {
private:
	std::vector<std::vector<float>> kernel;

public:
	ImageConvolution() = default;
	ImageConvolution(const std::vector<std::vector<float>>& kernel) : kernel(kernel) {}

	void process(const Image& src, Image& dst) override;
};

} // namespace cnn