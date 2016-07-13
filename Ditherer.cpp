#include "Ditherer.h"
#include "Coordinate.h"
#include "Types.h"

using namespace PostProcessing;

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
    std::vector<Core::Coordinate> ditherMatrix;
    uint divisor = 4;

    // push valid matrix locations onto the ditherMatrix 
    // relative to current pixel position
    ditherMatrix.push_back(Core::Coordinate(0, 0, 0));
    ditherMatrix.push_back(Core::Coordinate(1, 0, 2));
    ditherMatrix.push_back(Core::Coordinate(-1, 1, 1));
    ditherMatrix.push_back(Core::Coordinate(0, 1, 1));

    // now open up the image in single channel grayscale 
    cv::Mat image = cv::imread(imageFile, cv::IMREAD_GRAYSCALE);


    // verify image was read correctly if not log error and exit
    if (!image.data)
    {
        printf("\t[ERROR]: Image was not opened succesfully.\n");
        return Core::ReturnCodes::INVALID_FILE;
    }

    // get the rows and columns
    int channels = image.channels();
    nRows = image.rows;
    nCols = image.cols * channels;

    // check if the image is stored continuously and if so 
    // set the rows and columns accordingly
    if (image.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    // create empty image header that will be written to with the 
    // final dithered version of the image
    cv::Mat* ditheredImage(&image);

    // these will be used to store the pixels that will be dithered
    uchar pixelCurrent, pixel1, pixel2, pixel3;
    uchar *pixelRow, *nextPixelRow;


    // Case: The Mat is continuous (1D array)
    if (image.isContinuous)
    {
        for (int i = 0; i < nRows; ++i)
        {
            // get the current and next row of pixels
            pixelRow = image.ptr<uchar>(i);

            for (int j = 0; j < nCols; ++j) // NOTE : Verify if you need to increment column by more than 1 
            {
                // get current pixel
                pixelCurrent = pixelRow[j];
                printf("\t[CURRENT PIXEL] : %u\n", (uint)pixelCurrent);

                // get pixel at matrix position 1
                if ((j + 1) < nCols)
                {
                    pixel1 = pixelRow[j + 1];
                    printf("\t[PIXEL 1] : %u\n", (uint)pixel1);
                }
                // get pixel at matrix position 2
                else if ((j - 1) > 0)
                {
                    pixel2 = pixelRow[nCols *  - 1];
                    printf("\t[PIXEL 2] : %u\n", (uint)pixel2);
                }
                // get pixel at matrix position 3
                else if (nextPixelRow != NULL)
                {
                    pixel3 = pixelRow[j];
                    printf("\t[PIXEL 3] : %u\n", (uint)pixel3);
                }
            }
        }
    }
   
    return Core::ReturnCodes::RESULT_OK;
}

void Ditherer::updateDitherType(std::string type)
{
    activeDithering = type;
}

std::string Ditherer::getDitherType()
{
    return activeDithering;
}
