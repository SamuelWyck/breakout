#ifndef PLAYER_H
#define PLAYER_H


#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"


class Player {
    FRect m_rect{};
    int m_speed {};

    bool m_moveLeft {false};
    bool m_moveRight {false};

    bool m_movedLeft {false};
    bool m_movedRight {false};


public:
    Player(float centerX, float centerY, int speed) : m_speed{speed} {
        m_rect.setCenter(centerX, centerY);
        m_rect.setWidth(200);
        m_rect.setHeight(30);
    };

    void update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect) {
        handleMovement(screenRect, deltaTime);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    };

    void moveLeft() {
        m_moveLeft = true;
    };

    void moveRight() {
        m_moveRight = true;
    };


private:
    void handleMovement(const FRect& screenRect, double deltaTime) {
        if (m_moveLeft) {
            m_rect.setX(m_rect.x() - (m_speed * deltaTime));
            m_moveLeft = false;
            m_movedLeft = true;
        }
        if (m_moveRight) {
            m_rect.setX(m_rect.x() + (m_speed * deltaTime));
            m_moveRight = false;
            m_movedRight = true;
        }

        if (!screenRect.containsRect(&m_rect)) {
            if (m_rect.x() < 0.0f) {
                m_rect.setX(0.0f);
                m_movedLeft = false;
            } else if ((m_rect.x() + m_rect.width()) > screenRect.right()) {
                m_rect.setRight(screenRect.right());
                m_movedRight = false;
            }
        }
    };
};



#endif