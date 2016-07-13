#include <opencv2\opencv.hpp>
#include <iostream>
#include "Ditherer.h"


int main(int argc, char** argv)
{
    std::string imageFile = "assets/cat.jpg";
    
    PostProcessing::Ditherer* ditherer = new PostProcessing::Ditherer();
    
    ditherer->sierraLiteDither(imageFile);

    char x = ' ';

    while (x != 'q')
    {
        std::cin >> x;
    }


    return 0;
}