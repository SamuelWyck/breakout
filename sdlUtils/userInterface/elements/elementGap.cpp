#include <utility>
#include <SDL3/SDL_render.h>
#include "./elementGap.h"




ElementGap::ElementGap() {
};

ElementGap::ElementGap(int gap) : m_gap{gap} {      
};


int ElementGap::gap() const {
    return m_gap;
};


void ElementGap::update(SDL_Renderer*) {
};

float ElementGap::width() const {
    return 0.0f;
};

float ElementGap::height() const {
    return 0.0f;
};

std::pair<float, float> ElementGap::topleft() const {
    return {};
};

void ElementGap::setTopleft(float, float) {
};

std::pair<float, float> ElementGap::center() const {
    return {};
};

void ElementGap::setCenter(float, float) {
};

void ElementGap::setX(float) {
};

void ElementGap::setY(float) {
};