#include "Ditherer.h"
#include "Coordinate.h"
#include "Types.h"

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

int Ditherer::sierra2Dither(std::string imageFile)
{
    return 0;
}


/*
// Uses a matrix of the form  |       X(0)  2(1) |
//                            | 1(2)  1(3)       |
// the number in parenthesis represents the index at which 
// the pixel will be mapped to in our vector
// with a divisor of 1/4
*/
int Ditherer::sierraLiteDither(std::string imageFile)
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
        return RETURN_CODES::INVALID_FILE;
    }

    // get the rows and columns
    int channels = image.channels();
    nRows = image.rows;
    nCols = image.cols * channels;

    // create empty image header that will be written to with the 
    // final dithered version of the image
    cv::Mat* ditheredImage(&image);

    // these will be used to store the pixels that will be dithered
    uchar pixelCurrent, pixel1, pixel2, pixel3;
    uchar *pixelRow;

    // These will be used to tell what row were on in a 
    // continuous Mat
    int colCounter = 0;
    int currentRow = 0;

    // these will be used for the dithering colors
    int newColor = 0;
    int remainder;
    int blendingFactor;


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
                pixelCurrent = pixelRow[nCols * rowIndex + colIndex];

                // get the pixel to the right of the current pixel
                if ((colIndex + 1) < nCols)
                {
                    pixel1 = pixelRow[nCols * rowIndex + colIndex + 1];
                }
                // get the pixel down one row and one to the left of the current pixel
                else if ((rowIndex + 1) < nRows)
                {
                    if (((colIndex - 1) >= 0))
                    {
                        pixel2 = pixelRow[nCols * (rowIndex + 1) + colIndex - 1];
                    }

                    // Get the pixel directly below the current pixel
                    pixel3 = pixelRow[nCols * (rowIndex + 1) + colIndex];
                }

                // we have the pixel neiighbor hood now we can dither them
                // first find out what the current pixel is close to, black or white
                newColor = pixelCurrent > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;
                remainder = (int)pixelCurrent;
                blendingFactor = floor(remainder / divisor);

                // write that new color to the image
                // TODO

                // now we figure out the adjusted values of the neighboring pixels
                for (auto &value : ditherMatrix)
                {
                    // multiply the value in the dither mat with the blending factor
                    newColor = (value.blendVal() * blendingFactor) > COLORS::THRESHOLD ? COLORS::WHITE : COLORS::BLACK;

                    // write the new color to the approprate place
                    // TODO (use coordinate.x/y to write them to the correct positions)
                }
            }
        }
    }

    // log end of image scan 
    printf("[SIERRA-LITE DITHERER] : Scan Complete!\n");
    return RETURN_CODES::RESULT_OK;
}

void Ditherer::updateDitherType(std::string type)
{
    activeDithering = type;
}

std::string Ditherer::getDitherType()
{
    return activeDithering;
}
