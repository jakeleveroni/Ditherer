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

    // TODO there is a difference on how iteration will work if 
    // the Mat is continuous or not

    // iterate through the rows and columns from 
    // top-to-bottom and left-to-right
    for (int i = 0; i < nRows; ++i)
    {
        for (int j = 0; j < nCols; ++j) // NOTE : Verify if you need to increment column by more than 1 
        {
            // get the values of the necessary pixels at the 
            // current position at function does not bound check 
            // unless in debug mode so manual bounds checking is necessary
            if (i < nRows && j < nCols)
            {
                printf("[HERE! 1]");
                pixelCurrent = image.at<uchar>(i, j);
                printf("\t[CURRENT PIXEL] : %u\n", (uint)pixelCurrent);
            }
            else if (i < nRows && (j + 1) < nCols)
            {
                printf("[HERE! 2]");
                pixel1 = image.at<uchar>(i, j + 1);
                printf("\t[PIXEL 1] : %u\n", (uint)pixel1);
            }
            else if ((i + 1) < nRows && (j - 1) > 0)
            {
                printf("[HERE! 3]");
                pixel2 = image.at<uchar>(i + 1, j - 1);
                printf("\t[PIXEL 2] : %u\n", (uint)pixel2);
            }
            else if ((i + 1) < nRows && (j + 1) < nCols)
            {
                printf("[HERE! 4]");
                pixel3 = image.at<uchar>(i + 1, j + 1);
                printf("\t[PIXEL 3] : %u\n", (uint)pixel3);
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
