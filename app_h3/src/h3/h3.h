#pragma once
#include <iostream>
#include <h3/h3api.h>

namespace h3
{
    class H3
    {
    public:
        H3Index getH3Index(double latitude, double longitude, int resolution);
        CellBoundary getBoundaries(H3Index indexed);
        LatLng getCenter(H3Index indexed);
    };
}