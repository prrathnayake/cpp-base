#include "h3.h"

#include <algorithm>


H3Index h3::H3::getH3Index(double latitude, double longitude, int resolution)
{
    LatLng location;
    location.lat = degsToRads(latitude);
    location.lng = degsToRads(longitude);
    H3Index indexed;
    if (latLngToCell(&location, resolution, &indexed) != E_SUCCESS)
    {
        std::cerr << "Failed to convert lat/lng to H3 index\n";
    }
    return indexed;
}

CellBoundary h3::H3::getBoundaries(H3Index indexed)
{
    CellBoundary boundary;
    if (cellToBoundary(indexed, &boundary) != E_SUCCESS)
    {
        std::cerr << "Failed to get cell boundary\n";
    }
    return boundary;
}

LatLng h3::H3::getCenter(H3Index indexed)
{
    LatLng center;
    if (cellToLatLng(indexed, &center) != E_SUCCESS)
    {
        std::cerr << "Failed to get cell center\n";
    }
    return center;
}

std::vector<H3Index> h3::H3::getNeighbors(H3Index indexed, int k) {
    int maxSize = (3 * k * (k + 1)) + 1;
    std::vector<H3Index> neighbors(maxSize, 0);

    if (gridDisk(indexed, k, neighbors.data()) != E_SUCCESS) {
        std::cerr << "Failed to get neighbors\n";
        return {};
    }

    neighbors.erase(
        std::remove(neighbors.begin(), neighbors.end(), static_cast<H3Index>(0)),
        neighbors.end()
    );

    return neighbors;
}

bool h3::H3::isValid(H3Index index)
{
    return isValidCell(index);
}

int h3::H3::getResolution(H3Index index)
{
    return getResolution(index);
}

std::string h3::H3::toString(H3Index index)
{
    char buffer[17]; // H3 strings are 16 characters + null terminator
    h3ToString(index, buffer, sizeof(buffer));
    return std::string(buffer);
}

H3Index h3::H3::fromString(const std::string &indexStr)
{
    H3Index index = 0;
    if (stringToH3(indexStr.c_str(), &index) != E_SUCCESS)
    {
        std::cerr << "Failed to convert string to H3Index\n";
    }
    return index;
}
