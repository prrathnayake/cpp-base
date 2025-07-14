#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <h3/h3api.h>

namespace h3
{
    class H3
    {
    public:
        H3Index getH3Index(double latitude, double longitude, int resolution);
        CellBoundary getBoundaries(H3Index indexed);
        LatLng getCenter(H3Index indexed);
        std::vector<H3Index> getNeighbors(H3Index indexed, int kRing);
        bool isValid(H3Index index);
        int getResolution(H3Index index);
        std::string toString(H3Index index);
        H3Index fromString(const std::string &indexStr);
    };
}
