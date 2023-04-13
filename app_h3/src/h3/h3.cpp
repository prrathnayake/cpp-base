#include <iostream>
#include <h3/h3api.h>

#include "h3.h"

H3Index h3::H3::getH3Index(double latitude, double longitude, int resolution)
{
    LatLng location;
    location.lat = degsToRads(latitude);
    location.lng = degsToRads(longitude);
    H3Index indexed;
    if (latLngToCell(&location, resolution, &indexed) != E_SUCCESS)
    {
        std::cout << "Failed\n";
    }
    return indexed;
}

CellBoundary h3::H3::getBoundaries(H3Index indexed)
{
    CellBoundary boundary;
    if (cellToBoundary(indexed, &boundary) != E_SUCCESS)
    {
        std::cout << "Failed\n";
    }
    return boundary;
}

LatLng h3::H3::getCenter(H3Index indexed)
{
    LatLng center;
    if (cellToLatLng(indexed, &center) != E_SUCCESS)
    {
        std::cout << "Failed\n";
    }

    return center;
}