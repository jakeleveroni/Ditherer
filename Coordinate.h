#pragma once

namespace Core
{
    struct Coordinate
    {
    private:
        int xPos, yPos;
        int blendingCoef;

    public:
        Coordinate(int x, int y, int blend)
        {
            xPos = x;
            yPos = y;
        }

        int x()
        {
            return xPos;
        }

        int y()
        {
            return yPos;
        }

        int blendVal()
        {
            return blendingCoef;
        }
    };
}
