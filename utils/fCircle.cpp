#include <utility>
#include <cmath>
#include <SDL3/SDL_rect.h>
#include "./fCircle.h"
#include "./fRect.h"



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


bool FCircle::hasCircleIntersection(const FCircle& circle) const {
    float xDistance {m_centerX - circle.m_centerX};
    float yDistance {m_centerY - circle.m_centerY};
    float distance {sqrtf((xDistance * xDistance) + (yDistance * yDistance))};
    distance += 1.0f;
    return distance <= (m_radius + circle.m_radius);
};