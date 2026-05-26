#include <functional>
#include <SDL3/SDL_render.h>
#include "../sdlUtils/collision/fRect.h"
#include "./imageStatTracker.h"



ImageStatTracker::ImageStatTracker(float x, float y, float gap, SDL_Texture* img, const std::function<int()>& getterCb)
    : m_x{x}, m_y{y}, m_gap{gap}, m_img{img}, m_getterCb{getterCb} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_img->w, m_img->h);
};

ImageStatTracker::ImageStatTracker() {
};



void ImageStatTracker::update(SDL_Renderer* renderer) {
    int rangeEnd {m_getterCb()};
    for (int i {0}; i < rangeEnd; i += 1) {
        float xPos {m_x + (i * m_rect.width()) + (i * m_gap)};
        m_rect.setX(xPos);

        SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
    }
};


void ImageStatTracker::setGetterCb(const std::function<int()>& getterCb) {
    m_getterCb = getterCb;
};