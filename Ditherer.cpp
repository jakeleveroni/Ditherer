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
//		| NA   X   2  |
//      | 1    1   NA |
// with a divisor of 1/4
*/
cv::Mat Ditherer::sierraLiteDither(std::string imageFile)
{
    // used to traverse the image
    int nRows, nCols;

    // matrix for dithering and value of divisor
    std::vector<Coordinate> ditherMatrix;
    uint divisor = 4;

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

	// create output imafge 
	cv::Mat outputImage = image.clone();

    // these will be used to store the pixels that will be dithered
    int currentColor;
    uchar *pixelRow, *outputPixelRow;

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
		// create vector for storing values
		std::vector<std::vector<int> > lookupTable;
		lookupTable.resize(nRows, std::vector<int>(nCols));

		std::vector<std::vector<int> > ditheredTable(lookupTable);

		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			// get the pixels from the continuous image
			pixelRow = image.ptr<uchar>(0);

			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// load pixel values into vector
				lookupTable[rNdx][cNdx] = pixelRow[nCols * rNdx + cNdx];
			}
		}

		// now apply dithering to the vectors values TODO change to looping with iterators
		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// handle negative pixel values
				currentColor = lookupTable[rNdx][cNdx];

				// get quantum 
				quantumError = (lookupTable[rNdx][cNdx] > COLORS::THRESHOLD) ? (currentColor - COLORS::WHITE) : currentColor;

				if (quantumError < 0)
				{
					quantumError *= -1;
				}

				// save current pixel
				ditheredTable[rNdx][cNdx] = currentColor > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;

				// bounds checking
				if (cNdx + 1 < nCols)
				{
					ditheredTable[rNdx][cNdx + 1] = (lookupTable[rNdx][cNdx + 1]) + (quantumError * (SIERRALITE::SL_ONE / divisor))
						> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
				}

				// bounds checking 
				if (rNdx + 1 < nRows)
				{
					if (cNdx > 0)
					{
						ditheredTable[rNdx + 1][cNdx - 1] = (lookupTable[rNdx + 1][cNdx - 1]) + (quantumError * (SIERRALITE::SL_TWO / divisor))
							> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
					}

					ditheredTable[rNdx + 1][cNdx] = (lookupTable[rNdx + 1][cNdx]) + (quantumError * (SIERRALITE::SL_THREE / divisor))
					> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
				}
			}
		}

		// write the lookupTable back to the new image
		outputPixelRow = outputImage.ptr<uchar>(0);

		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// load pixel values into vector
				outputPixelRow[nCols * rNdx + cNdx] = ditheredTable[rNdx][cNdx];
			}
		}
    }

    // log end of image scan 
    printf("[SIERRA-LITE DITHERER] : Scan Complete!\n");
    return outputImage;
}


// Dithers using the Floyd Steinberg matrix
cv::Mat Ditherer::floydDither(std::string imageFile)
{
	// used to traverse the image
	int nRows, nCols;

	// matrix for dithering and value of divisor
	std::vector<Coordinate> ditherMatrix;
	uint divisor = 16;

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

	// create output imafge 
	cv::Mat outputImage = image.clone();

	// these will be used to store the pixels that will be dithered
	int currentColor;
	uchar *pixelRow, *outputPixelRow;

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
		// create vector for storing values
		std::vector<std::vector<int> > lookupTable;
		lookupTable.resize(nRows, std::vector<int>(nCols));

		std::vector<std::vector<int> > ditheredTable(lookupTable);

		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			// get the pixels from the continuous image
			pixelRow = image.ptr<uchar>(0);

			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// load pixel values into vector
				lookupTable[rNdx][cNdx] = pixelRow[nCols * rNdx + cNdx];
			}
		}

		// now apply dithering to the vectors values TODO change to looping with iterators
		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// handle negative pixel values
				currentColor = lookupTable[rNdx][cNdx];

				// get quantum 
				quantumError = (lookupTable[rNdx][cNdx] > COLORS::THRESHOLD) ? (currentColor - COLORS::WHITE) : currentColor;

				if (quantumError < 0)
				{
					quantumError *= -1;
				}

				// save current pixel
				ditheredTable[rNdx][cNdx] = currentColor > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;

				// bounds checking
				if (cNdx + 1 < nCols)
				{
					if (rNdx + 1 < nRows)
					{
						ditheredTable[rNdx + 1][cNdx + 1] = (lookupTable[rNdx + 1][cNdx + 1]) + (quantumError * (FLOYD::F_FOUR / divisor))
					> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
					}

					ditheredTable[rNdx][cNdx + 1] = (lookupTable[rNdx][cNdx + 1]) + (quantumError * (FLOYD::F_ONE / divisor))
						> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
				}

				// bounds checking 
				if (rNdx + 1 < nRows)
				{
					if (cNdx > 0)
					{
						ditheredTable[rNdx + 1][cNdx - 1] = (lookupTable[rNdx + 1][cNdx - 1]) + (quantumError * (FLOYD::F_TWO / divisor))
							> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
					}

					ditheredTable[rNdx + 1][cNdx] = (lookupTable[rNdx + 1][cNdx]) + (quantumError * (FLOYD::F_THREE / divisor))
						> COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
				}
			}
		}

		// write the lookupTable back to the new image
		outputPixelRow = outputImage.ptr<uchar>(0);

		for (int rNdx = 0; rNdx < nRows; ++rNdx)
		{
			for (int cNdx = 0; cNdx < nCols; ++cNdx)
			{
				// load pixel values into vector
				outputPixelRow[nCols * rNdx + cNdx] = ditheredTable[rNdx][cNdx];
			}
		}
	}

	// log end of image scan 
	printf("[SIERRA-LITE DITHERER] : Scan Complete!\n");
	return outputImage;
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
