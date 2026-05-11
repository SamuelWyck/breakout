#ifndef IMG_STAT_TRACKER_H
#define IMG_STAT_TRACKER_H


#include <functional>
#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"


class ImageStatTracker {
    float m_x {};
    float m_y {};
    float m_gap {};

    SDL_Texture* m_img {nullptr};

    FRect m_rect{};

    std::function<int()> m_getterCb{};

public:
    ImageStatTracker(float x, float y, float gap, SDL_Texture* img, const std::function<int()>& getterCb);

    ImageStatTracker();

    ImageStatTracker(const ImageStatTracker&) = default;
    ImageStatTracker(ImageStatTracker&&) = default;
    ImageStatTracker& operator=(const ImageStatTracker&) = default;
    ImageStatTracker& operator=(ImageStatTracker&&) = default;
    ~ImageStatTracker() = default;



    void update(SDL_Renderer* renderer);

    void setGetterCb(const std::function<int()>& getterCb);
};



#endif