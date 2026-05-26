#ifndef MY_MATH_H
#define MY_MATH_H


#include <utility>


namespace Math {
    constexpr double Pi {3.14159265358979323846264};
    constexpr double toRadians {Pi / 180.0};
    constexpr double toDegrees {180.0 / Pi};

    constexpr float Pif {3.14159265358979323846264f};
    constexpr float toRadiansf {Pif / 180.0f};
    constexpr float toDegreesf {180.0f / Pif};

    // returns a pair containing the radius and angle in radians
    std::pair<float, float> toPolarCoordsRad(float x, float y);

    // returns a pair containing the radius and angle in degrees
    std::pair<float, float> toPolarCoordsDeg(float x, float y);

    // returns a pair contianing the x and y coords, angle is expected in radians
    std::pair<float, float> toCartesianCoordsRad(float radius, float angle);

    // returns a pair containing the x and y coords, angle is expected in degrees
    std::pair<float, float> toCartesianCoordsDeg(float radius, float angle);

    // takes value in range srcMin-srcMax and remaps it to range dstMin-dstMax
    float remap(float srcMin, float srcMax, float dstMin, float dstMax, float value);
    double remap(double srcMin, double srcMax, double dstMin, double dstMax, double value);
    double remap(int srcMin, int srcMax, int dstMin, int dstMax, int value);
};



#endif