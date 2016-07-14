#include "Types.h"
#include "Ditherer.h"
#include "Coordinate.h"


using namespace PostProcessing;
using namespace Core;

Ditherer::Ditherer()
{
    // default to sierralite
    activeDithering = "sierralite";

}


Ditherer::~Ditherer()
{
}

cv::Mat Ditherer::sierra2Dither(std::string imageFile)
{
	cv::Mat image;
	return image;
}


/*
// Uses a matrix of the form where X is the current pixel
//		| 0   X   2 |
//      | 1   1   0 |
// with a divisor of 1/4
*/
cv::Mat Ditherer::sierraLiteDither(std::string imageFile)
{
    // used to traverse the image
    int nRows, nCols;

    // matrix for dithering and value of divisor
    std::vector<Coordinate> ditherMatrix;
    uint divisor = 4;

    // push valid matrix locations onto the ditherMatrix 
    // relative to current pixel position
    ditherMatrix.push_back(Coordinate(0, 0, 0));
    ditherMatrix.push_back(Coordinate(1, 0, 2));
    ditherMatrix.push_back(Coordinate(-1, 1, 1));
    ditherMatrix.push_back(Coordinate(0, 1, 1));

    // now open up the image in single channel grayscale 
    cv::Mat image = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);


    // verify image was read correctly if not log error and exit
    if (!image.data)
    {
        printf("[ERROR]: Image was not opened succesfully.\n");
        return image;
    }

    // get the rows and columns
    int channels = image.channels();
    nRows = image.rows;
    nCols = image.cols * channels;

    // these will be used to store the pixels that will be dithered
    uchar currentColor;
    uchar *pixelRow;

    // these will be used for the dithering colors
    int blendingFactor;
	int newColor;
	int quantumError = 0;

    // log beginning of scan 
    printf("[SIERRA-LITE DITHERER] : Beginning Scan...\n");

    // Case: The Mat is continuous (1D array)
    // we will map 2D array coordinates to 1D array
    if (image.isContinuous())
    {
		// loop through rows
        for (int rowIndex = 0; rowIndex < nRows; ++rowIndex)
        {
			// get the only row of pixels
			pixelRow = image.ptr<uchar>(0);

            for (int colIndex = 0; colIndex < nCols; ++colIndex) // NOTE : Verify if you need to increment column by more than 1 
            {
                // Get the current pixel 
				currentColor = pixelRow[nCols * rowIndex + colIndex];
	
				// dither current pixel and get the error quantum 
				newColor = currentColor > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;

				// make sure not to apply dithering to already dithered pixels
				if(pixelRow[nCols * rowIndex + colIndex] != 0 && pixelRow[nCols * rowIndex + colIndex] == 1)
				{
					pixelRow[nCols * rowIndex + colIndex] = newColor;
					quantumError = currentColor - newColor;
				}

                // bounds checking to the right of the pixel
                if ((colIndex + 1) < nCols)
                {
					// dont apply to dithered pixels
					if (pixelRow[nCols * rowIndex + colIndex + 1] != 0 && pixelRow[nCols * rowIndex + colIndex + 1] != 1)
					{
						// dither the pixel to the right of the current pixel
						pixelRow[nCols * rowIndex + colIndex + 1] = (ditherMatrix[1].blendVal() * quantumError / divisor) > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
					}
                }
                // bounds checking for below the pixel
				else if ((rowIndex + 1) < nRows)
				{
					// bounds checking for below and to the left of the pixel
					if (((colIndex - 1) >= 0))
					{
						// dont apply to dithered pixels
						if (pixelRow[nCols * (rowIndex + 1) + colIndex - 1] != 0 && pixelRow[nCols * rowIndex + colIndex + 1] != 1)
						{
							// dither the pixel to the bottom and left of the current pixel
							pixelRow[nCols * (rowIndex + 1) + colIndex - 1] = (ditherMatrix[1].blendVal() * quantumError / divisor) > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
						}
					}

					// dont apply to dithered pixels
					if (pixelRow[nCols * (rowIndex + 1) + colIndex] != 0 && pixelRow[nCols * rowIndex + colIndex + 1] != 1)
					{
						// dither the pixel to the bottom of the current pixel
						pixelRow[nCols * (rowIndex + 1) + colIndex] = (ditherMatrix[1].blendVal() * quantumError / divisor) > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
					}
				}
            }
        }
    }

    // log end of image scan 
    printf("[SIERRA-LITE DITHERER] : Scan Complete!\n");
    return image;
}


// displays an image that is passed into it
void Ditherer::displayDitheredImage(cv::Mat image)
{
	cv::namedWindow("Dithered Image", cv::WINDOW_AUTOSIZE);
	imshow("Dithered Image", image);

	cv::waitKey(0);
	return;
}


// setter and getter for dither type
void Ditherer::updateDitherType(std::string type) { activeDithering = type; }
std::string Ditherer::getDitherType() { return activeDithering; }
