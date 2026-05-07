#include <utility>
#include <cmath>
#include "./math.h"



namespace Math {
    std::pair<float, float> toPolarCoordsRad(float x, float y) {
        float radius {sqrtf((x * x) + (y * y))};

        float angle {atanf(y / x)};
        if (x < 0.0f) {
            angle += static_cast<float>(Math::Pi);
        }

        return std::pair<float, float>{radius, angle};
    };


    std::pair<float, float> toPolarCoordsDeg(float x, float y) {
        float radius {sqrtf((x * x) + (y * y))};

        float angle {atanf(y / x)};
        if (x < 0.0f) {
            angle += static_cast<float>(Math::Pi);
        }

        angle *= Math::toDegrees;
        return std::pair<float, float>{radius, angle};
    };


    std::pair<float, float> toCartesianCoordsRad(float radius, float angle) {
        float x {radius * cosf(angle)};
        float y {radius * sinf(angle)};
        return std::pair<float, float>{x, y};
    };


    std::pair<float, float> toCartesianCoordsDeg(float radius, float angle) {
        angle *= Math::toRadians;
        float x {radius * cosf(angle)};
        float y {radius * sinf(angle)};
        return std::pair<float, float>{x, y};
    };


    float remap(float srcMin, float srcMax, float dstMin, float dstMax, float value) {
        float percentOfRange {(value - srcMin) / (srcMax - srcMin)};
        return percentOfRange * (dstMax - dstMin) + dstMin;
    };


    double remap(double srcMin, double srcMax, double dstMin, double dstMax, double value) {
        double percentOfRange {(value - srcMin) / (srcMax - srcMin)};
        return percentOfRange * (dstMax - dstMin) + dstMin;
    };


    double remap(int srcMin, int srcMax, int dstMin, int dstMax, int value) {
        double percentOfRange {(value - srcMin) / static_cast<double>((srcMax - srcMin))};
        return percentOfRange * (dstMax - dstMin) + dstMin;
    };
};