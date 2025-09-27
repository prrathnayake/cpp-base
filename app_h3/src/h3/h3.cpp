#include "h3.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <stdexcept>

namespace
{
    constexpr double kLatMin = -90.0;
    constexpr double kLatMax = 90.0;
    constexpr double kLonMin = -180.0;
    constexpr double kLonMax = 180.0;
    constexpr double kCoordinateScale = 1'000'000.0; // six decimal places
    constexpr double kPi = 3.14159265358979323846;

    constexpr std::uint64_t kResolutionBits = 6;               // up to 64 discrete resolutions
    constexpr std::uint64_t kCoordinateBits = 29;              // fits scaled coordinates
    constexpr std::uint64_t kResolutionShift = kCoordinateBits * 2;
    constexpr std::uint64_t kLatShift = kCoordinateBits;
    constexpr std::uint64_t kCoordinateMask = (1ULL << kCoordinateBits) - 1ULL;
    constexpr std::uint64_t kResolutionMask = (1ULL << kResolutionBits) - 1ULL;

    double clampLatitude(double latitude)
    {
        return std::clamp(latitude, kLatMin, kLatMax);
    }

    double clampLongitude(double longitude)
    {
        if (longitude < kLonMin)
        {
            return kLonMin;
        }
        if (longitude > kLonMax)
        {
            return kLonMax;
        }
        return longitude;
    }

    std::uint64_t encodeCoordinate(double value, double minBound)
    {
        const double maxBound = (minBound == kLatMin) ? kLatMax : kLonMax;
        const double clamped = std::clamp(value, minBound, maxBound);
        const double shifted = (clamped - minBound) * kCoordinateScale;
        const double boundedShift = std::clamp(shifted, 0.0, static_cast<double>(kCoordinateMask));
        return static_cast<std::uint64_t>(std::llround(boundedShift));
    }

    double decodeCoordinate(std::uint64_t encoded, double minBound)
    {
        return (static_cast<double>(encoded) / kCoordinateScale) + minBound;
    }

    std::uint64_t encodeIndex(double latitude, double longitude, int resolution)
    {
        const int clampedResolution = std::clamp(resolution, 0, static_cast<int>(kResolutionMask));
        const std::uint64_t encodedLat = encodeCoordinate(latitude, kLatMin);
        const std::uint64_t encodedLon = encodeCoordinate(longitude, kLonMin);

        return (static_cast<std::uint64_t>(clampedResolution) << kResolutionShift) |
               (encodedLat << kLatShift) |
               encodedLon;
    }

    int decodeResolution(h3::H3Index index)
    {
        return static_cast<int>((index >> kResolutionShift) & kResolutionMask);
    }

    double decodeLatitude(h3::H3Index index)
    {
        const std::uint64_t encodedLat = (index >> kLatShift) & kCoordinateMask;
        return decodeCoordinate(encodedLat, kLatMin);
    }

    double decodeLongitude(h3::H3Index index)
    {
        const std::uint64_t encodedLon = index & kCoordinateMask;
        return decodeCoordinate(encodedLon, kLonMin);
    }

    double hexagonRadiusForResolution(int resolution)
    {
        // Smaller radius for higher resolution to emulate finer granularity
        const int clampedResolution = std::max(resolution, 0);
        return 1.0 / std::pow(2.0, static_cast<double>(clampedResolution) + 4.0);
    }

    h3::CellBoundary buildBoundary(double latitude, double longitude, int resolution)
    {
        h3::CellBoundary boundary;
        const double radius = hexagonRadiusForResolution(resolution);
        boundary.verts.reserve(6);

        for (int vertex = 0; vertex < 6; ++vertex)
        {
            const double angle = (kPi / 3.0) * vertex;
            const double lat = clampLatitude(latitude + radius * std::sin(angle));
            const double lon = clampLongitude(longitude + radius * std::cos(angle));
            boundary.verts.push_back({lat, lon});
        }

        return boundary;
    }
}

namespace h3
{
    H3Index H3::getH3Index(double latitude, double longitude, int resolution)
    {
        return encodeIndex(latitude, longitude, resolution);
    }

    CellBoundary H3::getBoundaries(H3Index indexed)
    {
        return buildBoundary(getCenter(indexed).lat, getCenter(indexed).lng, getResolution(indexed));
    }

    LatLng H3::getCenter(H3Index indexed)
    {
        return {decodeLatitude(indexed), decodeLongitude(indexed)};
    }

    std::vector<H3Index> H3::getNeighbors(H3Index indexed, int k)
    {
        if (k <= 0)
        {
            return {};
        }

        const LatLng center = getCenter(indexed);
        const int resolution = getResolution(indexed);
        const double step = hexagonRadiusForResolution(resolution) * 1.5;

        std::vector<H3Index> neighbors;
        neighbors.reserve(static_cast<std::size_t>((2 * k + 1) * (2 * k + 1) - 1));

        for (int latOffset = -k; latOffset <= k; ++latOffset)
        {
            for (int lonOffset = -k; lonOffset <= k; ++lonOffset)
            {
                if (latOffset == 0 && lonOffset == 0)
                {
                    continue;
                }

                const double neighborLat = clampLatitude(center.lat + step * static_cast<double>(latOffset));
                const double neighborLon = clampLongitude(center.lng + step * static_cast<double>(lonOffset));
                neighbors.push_back(encodeIndex(neighborLat, neighborLon, resolution));
            }
        }

        return neighbors;
    }

    bool H3::isValid(H3Index index)
    {
        const int resolution = getResolution(index);
        if (resolution < 0 || resolution > static_cast<int>(kResolutionMask))
        {
            return false;
        }

        const double latitude = decodeLatitude(index);
        const double longitude = decodeLongitude(index);

        return latitude >= kLatMin && latitude <= kLatMax &&
               longitude >= kLonMin && longitude <= kLonMax;
    }

    int H3::getResolution(H3Index index)
    {
        return decodeResolution(index);
    }

    std::string H3::toString(H3Index index)
    {
        std::ostringstream stream;
        stream << std::hex << index;
        return stream.str();
    }

    H3Index H3::fromString(const std::string &indexStr)
    {
        std::istringstream stream(indexStr);
        H3Index value = 0;
        stream >> std::hex >> value;
        if (!stream)
        {
            throw std::invalid_argument("Invalid H3Index representation: " + indexStr);
        }
        return value;
    }
}
