#include <utility>
#include <iostream>
#include <exception>
#include "./fRect.h"



FRect::FRect() {
};

FRect::FRect(const SDL_FRect& rect) : m_sdlRect{rect.x, rect.y, rect.w, rect.h} {
};

FRect::FRect(float x, float y, float width, float height) : m_sdlRect{x, y, width, height} {
};

FRect::FRect(float x, float y) : m_sdlRect{x, y, 0, 0} {
};

FRect::operator SDL_FRect() const {
    return m_sdlRect;
};


const SDL_FRect& FRect::getSDLFRect() const {
    return m_sdlRect;
};

SDL_FRect& FRect::getSDLFRect() {
    return m_sdlRect;
};


bool FRect::containsRect(const FRect* rect) const {
    FRect unionResult{};
    if (!getRectUnion(rect, &unionResult)) {
        throw std::runtime_error(SDL_GetError());
    }

    auto [unionX, unionY] = unionResult.topleft();
    auto [unionWidth, unionHeight] = unionResult.size();
    return unionX == x() && unionY == y() && unionWidth == width() && unionHeight == height();
};

bool FRect::containsRect(const SDL_FRect* rect) const {
    SDL_FRect unionRect{};
    if (!getRectUnion(rect, &unionRect)) {
        throw std::runtime_error(SDL_GetError());
    }

    return unionRect.x == x() && unionRect.y == y() && unionRect.w == width() && unionRect.h == height();
};


// start wrappers for sdl rect functions

bool FRect::pointInRect(const SDL_FPoint* point) const {
    return SDL_PointInRectFloat(point, &m_sdlRect);
};

bool FRect::rectEmpty() const {
    return SDL_RectEmptyFloat(&m_sdlRect);
};

bool FRect::rectsEqualEpsilon(const SDL_FRect* rect, float epsilon) const {
    return SDL_RectsEqualEpsilon(&m_sdlRect, rect, epsilon);
};

bool FRect::rectsEqualEpsilon(const FRect* rect, float epsilon) const {
    if (!rect) {
        return false;
    }
    return rectsEqualEpsilon(&rect->getSDLFRect(), epsilon);
};

bool FRect::rectsEqual(const SDL_FRect* rect) const {
    return SDL_RectsEqualFloat(&m_sdlRect, rect);
};

bool FRect::rectsEqual(const FRect* rect) const {
    if (!rect) {
        return false;
    }
    return rectsEqual(&rect->getSDLFRect());
};

bool FRect::hasRectIntersection(const SDL_FRect* rect) const {
    return SDL_HasRectIntersectionFloat(&m_sdlRect, rect);
};

bool FRect::hasRectIntersection(const FRect* rect) const {
    if (!rect) {
        return false;
    }
    return hasRectIntersection(&rect->getSDLFRect());
};

bool FRect::getRectIntersection(const SDL_FRect* rect, SDL_FRect* result) const {
    return SDL_GetRectIntersectionFloat(&m_sdlRect, rect, result);
};

bool FRect::getRectIntersection(const FRect* rect, FRect* result) const {
    if (!rect || !result) {
        return false;
    }
    return getRectIntersection(&rect->getSDLFRect(), &result->getSDLFRect());
};

bool FRect::getRectUnion(const SDL_FRect* rect, SDL_FRect* result) const {
    return SDL_GetRectUnionFloat(&m_sdlRect, rect, result);
};

bool FRect::getRectUnion(const FRect* rect, FRect* result) const {
    if (!rect || !result) {
        return false;
    }
    return getRectUnion(&rect->getSDLFRect(), &result->getSDLFRect());
};

bool FRect::getRectAndLineIntersection(float* x1, float* y1, float* x2, float* y2) const {
    return SDL_GetRectAndLineIntersectionFloat(&m_sdlRect, x1, y1, x2, y2);
};

// end wrappers for sdl rect functions


// start getters and setters

std::pair<float, float> FRect::center() const {
    float centerX {m_sdlRect.x + (m_sdlRect.w / 2)};
    float cneterY {m_sdlRect.y + (m_sdlRect.h / 2)};
    return std::pair<float, float>{centerX, cneterY};
};

void FRect::setCenter(const std::pair<float, float>& center) {
    auto [centerX, centerY] = center;
    m_sdlRect.x = centerX - (m_sdlRect.w / 2);
    m_sdlRect.y = centerY - (m_sdlRect.h / 2);
};

void FRect::setCenter(float centerX, float centerY) {
    m_sdlRect.x = centerX - (m_sdlRect.w / 2);
    m_sdlRect.y = centerY - (m_sdlRect.h / 2);
};


float FRect::centerX() const {
    return m_sdlRect.x + (m_sdlRect.w / 2);
};

void FRect::setCenterX(float centerX) {
    m_sdlRect.x = centerX - (m_sdlRect.w / 2);
};


float FRect::centerY() const {
    return m_sdlRect.y + (m_sdlRect.h / 2);
};

void FRect::setCenterY(float centerY) {
    m_sdlRect.y = centerY - (m_sdlRect.h / 2);
};


std::pair<float, float> FRect::topleft() const {
    return std::pair<float, float>{m_sdlRect.x, m_sdlRect.y};
};

void FRect::setTopleft(const std::pair<float, float>& topleft) {
    auto [x, y] = topleft;
    m_sdlRect.x = x;
    m_sdlRect.y = y;
};

void FRect::setTopleft(float x, float y) {
    m_sdlRect.x = x;
    m_sdlRect.y = y;
};


float FRect::x() const {
    return m_sdlRect.x;
};

void FRect::setX(float x) {
    m_sdlRect.x = x;
};


float FRect::y() const {
    return m_sdlRect.y;
};

void FRect::setY(float y) {
    m_sdlRect.y = y;
};


std::pair<float, float> FRect::midTop() const {
    float topX {m_sdlRect.x + (m_sdlRect.w / 2)};
    return std::pair<float, float>{topX, m_sdlRect.y};
};

void FRect::setMidTop(std::pair<float, float>& midTop) {
    auto [topX, topY] = midTop;
    m_sdlRect.x = topX - (m_sdlRect.w / 2);
    m_sdlRect.y = topY;
};

void FRect::setMidTop(float topX, float topY) {
    m_sdlRect.x = topX - (m_sdlRect.w / 2);
    m_sdlRect.y = topY;
};


std::pair<float, float> FRect::midBottom() const {
    float bottomX {m_sdlRect.x + (m_sdlRect.w / 2)};
    float bottomY {m_sdlRect.y + m_sdlRect.h};
    return std::pair<float, float>{bottomX, bottomY};
};

void FRect::setMidBottom(std::pair<float, float>& midBottom) {
    auto [bottomX, bottomY] = midBottom;
    m_sdlRect.x = bottomX - (m_sdlRect.w / 2);
    m_sdlRect.y = bottomY - m_sdlRect.h;
};

void FRect::setMidBottom(float bottomX, float bottomY) {
    m_sdlRect.x = bottomX - (m_sdlRect.w / 2);
    m_sdlRect.y = bottomY - m_sdlRect.h;
};


std::pair<float, float> FRect::midLeft() const {
    float leftY {m_sdlRect.y + (m_sdlRect.h / 2)};
    return std::pair<float, float>{m_sdlRect.x, leftY};
};

void FRect::setMidLeft(std::pair<float, float>& midLeft) {
    auto [leftX, leftY] = midLeft;
    m_sdlRect.x = leftX;
    m_sdlRect.y = leftY - (m_sdlRect.h / 2);
};

void FRect::setMidLeft(float leftX, float leftY) {
    m_sdlRect.x = leftX;
    m_sdlRect.y = leftY - (m_sdlRect.h / 2);
};


std::pair<float, float> FRect::midRight() const {
    float rightX {m_sdlRect.x + m_sdlRect.w};
    float rightY {m_sdlRect.y + (m_sdlRect.h / 2)};
    return std::pair<float, float>{rightX, rightY};
};

void FRect::setMidRight(std::pair<float, float>& midRight) {
    auto [rightX, rightY] = midRight;
    m_sdlRect.x = rightX - m_sdlRect.w;
    m_sdlRect.y = rightY - (m_sdlRect.h / 2);
};

void FRect::setMidRight(float rightX, float rightY) {
    m_sdlRect.x = rightX - m_sdlRect.w;
    m_sdlRect.y = rightY - (m_sdlRect.h / 2);
};


float FRect::width() const {
    return m_sdlRect.w;
};

void FRect::setWidth(float width) {
    m_sdlRect.w = width;
};


float FRect::height() const {
    return m_sdlRect.h;
};

void FRect::setHeight(float height) {
    m_sdlRect.h = height;
};


std::pair<float, float> FRect::size() const {
    return std::pair<float, float>{m_sdlRect.w, m_sdlRect.h};
};

void FRect::setSize(std::pair<float, float>& size) {
    auto [width, height] = size;
    m_sdlRect.w = width;
    m_sdlRect.h = height;
};

void FRect::setSize(float width, float height) {
    m_sdlRect.w = width;
    m_sdlRect.h = height;
};

// end getters and setters


std::ostream& operator<<(std::ostream& out, const FRect& rect) {
    out << "FRect((" << rect.x() << ", " << rect.y() << "), ";
    out << "(" << rect.width() << ", " << rect.height() << "))";
    return out;
};