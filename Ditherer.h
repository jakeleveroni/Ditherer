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
        int sierra2Dither(std::string imageFile);
        int sierraLiteDither(std::string imageFile);

        // updates the active dithering selection
        std::string getDitherType();
        void updateDitherType(std::string type);

    private:
        std::string activeDithering;
    };
}



