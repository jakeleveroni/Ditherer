#include <opencv2\opencv.hpp>
#include <iostream>
#include "Ditherer.h"


int main(int argc, char** argv)
{
	PostProcessing::Ditherer* ditherer = new PostProcessing::Ditherer();
	std::string imageFile = "assets/cat.jpg";

	// dispaly original image
	//cv::Mat image = imread(imageFile, cv::IMREAD_GRAYSCALE);
	//cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Original Image", image);
	//cv::waitKey(0);
    
	// Display the dithered image
    cv::Mat ditherImage = ditherer->floydDither(imageFile);
	ditherer->displayDitheredImage(ditherImage);

    return 0;
}