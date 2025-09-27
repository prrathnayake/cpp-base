#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace h3
{
    using H3Index = std::uint64_t;

    struct LatLng
    {
        double lat{0.0};
        double lng{0.0};
    };

    struct CellBoundary
    {
        std::vector<LatLng> verts;
    };

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
