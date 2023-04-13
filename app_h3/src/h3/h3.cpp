#include <iostream>
#include <h3/h3api.h>

#include "h3.h"

void h3::H3::getH3Index(double latitude, double longitude, int resolution)
{
    LatLng location;
    location.lat = degsToRads(latitude);
    location.lng = degsToRads(longitude);
    H3Index indexed;
    if (latLngToCell(&location, resolution, &indexed) != E_SUCCESS)
    {
        std::cout << "Failed\n";
    }
    std::cout << indexed << "\n";
}