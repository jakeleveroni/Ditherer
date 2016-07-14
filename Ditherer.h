#pragma once
#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>

namespace PostProcessing
{
    class Ditherer
    {
    public:
        // Constructor and Destructor
        Ditherer();
        ~Ditherer();

        // Seirra Dithering techniques
        cv::Mat sierra2Dither(std::string imageFile);
        cv::Mat sierraLiteDither(std::string imageFile);
		void displayDitheredImage(cv::Mat image);

        // updates the active dithering selection
        std::string getDitherType();
        void updateDitherType(std::string type);

    private:
        std::string activeDithering;
    };
}



