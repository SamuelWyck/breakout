#include <utility>
#include <cmath>
#include <SDL3/SDL_rect.h>
#include "./fCircle.h"
#include "./fRect.h"
#include "../math.h"



FCircle::FCircle() : m_centerX{0.0f}, m_centerY{0.0f}, m_radius{0.0f} {
};

FCircle::FCircle(float centerX, float centerY, float radius) 
    : m_centerX{centerX}, m_centerY{centerY}, m_radius{radius} {
};

FCircle::FCircle(float centerX, float centerY) 
    : m_centerX{centerX}, m_centerY{centerY}, m_radius{0.0f} {
};


std::pair<float, float> FCircle::center() const {
    return std::pair<float, float>{m_centerX, m_centerY};
};

void FCircle::setCenter(const std::pair<float, float>& center) {
    auto [centerX, centerY] {center};
    m_centerX = centerX;
    m_centerY = centerY;
};

void FCircle::setCenter(float centerX, float centerY) {
    m_centerX = centerX;
    m_centerY = centerY;
};

float FCircle::centerX() const {
    return m_centerX;
};

void FCircle::setCenterX(float centerX) {
    m_centerX = centerX;
};

float FCircle::centerY() const {
    return m_centerY;
};

void FCircle::setCenterY(float centerY) {
    m_centerY = centerY;
};

float FCircle::radius() const {
    return m_radius;
};

void FCircle::setRadius(float radius) {
    m_radius = radius;
};


SDL_FRect FCircle::getSDLFRect() const {
    float x {m_centerX - m_radius};
    float y {m_centerY - m_radius};
    float sideSize {m_radius * 2};
    return SDL_FRect{x, y, sideSize, sideSize};
};

FRect FCircle::getFRect() const {
    float x {m_centerX - m_radius};
    float y {m_centerY - m_radius};
    float sideSize {m_radius * 2};
    return FRect{x, y, sideSize, sideSize};
};

FCircle::operator SDL_FRect() const {
    return getSDLFRect();
};

FCircle::operator FRect() const {
    return getFRect();
};


bool FCircle::hasCircleIntersection(const FCircle* circle) const {
    if (!circle) {
        return false;
    }

    float xDistance {m_centerX - circle->m_centerX};
    float yDistance {m_centerY - circle->m_centerY};
    float distance {sqrtf((xDistance * xDistance) + (yDistance * yDistance))};
    distance += 1.0f;
    return distance <= (m_radius + circle->m_radius);
};


bool FCircle::hasRectIntersection(const FRect* rect) const {
    if (!rect) {
        return false;
    }

    auto [rectCenterX, rectCenterY] {rect->center()};
    SDL_FPoint closestToCenter{edgePointClosestTo(rectCenterX, rectCenterY)};
    if (SDL_PointInRectFloat(&closestToCenter, &rect->getSDLFRect())) {
        return true;
    }

    float oppositeDis {rectCenterY - m_centerY};
    float adjacentDis {rectCenterX - m_centerX};
    float distanceBetweenCenters {sqrtf((oppositeDis * oppositeDis) + (adjacentDis * adjacentDis))};
    if (distanceBetweenCenters <= m_radius) {
        return true;
    }

    auto [rectEdgeX, rectEdgeY] {closestRectEdgePoint(rect)};
    SDL_FPoint closestToRectEdge {edgePointClosestTo(rectEdgeX, rectEdgeY)};
    if (SDL_PointInRectFloat(&closestToRectEdge, &rect->getSDLFRect())) {
        return true;
    }

    return false;
};


SDL_FPoint FCircle::edgePointClosestTo(float x, float y) const {
    float oppositeDis {y - m_centerY};
    float adjacentDis {x - m_centerX};

    float angleToRect {atanf(oppositeDis / adjacentDis)};
    // correct radian angle
    if (adjacentDis < 0.0f) {
        angleToRect += Math::Pif;
    }

    float xDisToCircleEdge {m_radius * cosf(angleToRect)};
    float yDisToCircleEdge {m_radius * sinf(angleToRect)};
    float edgePointX {xDisToCircleEdge + m_centerX};
    float edgePointY {yDisToCircleEdge + m_centerY};
    return SDL_FPoint{edgePointX, edgePointY};
};


SDL_FPoint FCircle::closestRectEdgePoint(const FRect* rect) const {
    float xCoord {m_centerX};
    if (xCoord < rect->x()) {
        xCoord = rect->x();
    } else if (xCoord > rect->right()) {
        xCoord = rect->right();
    }

    float yCoord {m_centerY};
    if (yCoord < rect->y()) {
        yCoord = rect->y();
    } else if (yCoord > rect->bottom()) {
        yCoord = rect->bottom();
    }

    return SDL_FPoint{xCoord, yCoord};
};


bool FCircle::hasRectIntersection(const SDL_FRect* rect) const {
    if (!rect) {
        return false;
    }

    FRect wrappedRect{*rect};
    return hasRectIntersection(&wrappedRect);
};


// pivot circle (angle) degrees clockwise about the given x y coord
void FCircle::pivot(float x, float y, double angle) {
    float correctedCenterX {m_centerX - x};
    float correctedCenterY {m_centerY - y};

    auto [radius, oldAngle] {Math::toPolarCoordsDeg(correctedCenterX, correctedCenterY)};
    float newAngle {oldAngle + static_cast<float>(angle)};

    auto [newCorrectedCenterX, newCorrectCenterY] {Math::toCartesianCoordsDeg(radius, newAngle)};
    setCenterX(newCorrectedCenterX + x);
    setCenterY(newCorrectCenterY + y);
};