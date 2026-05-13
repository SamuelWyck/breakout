#include <vector>
#include <exception>
#include <typeinfo>
#include <SDL3/SDL_render.h>
#include "../sdlUtils/fRect.h"
#include "../sdlUtils/color.h"
#include "./iBlock.h"
#include "./ball.h"
#include "./ballBlock.h"



BallBlock::BallBlock(
    float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal, std::vector<Ball*>* balls
) : m_img{img}, m_color{color}, m_health{health}, m_scoreValue{scoreVal}, m_ballsPtr{balls} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_img->w, m_img->h);
};


BallBlock::BallBlock(const IBlock* block) {
    if (typeid(*block) != typeid(BallBlock)) {
        throw std::runtime_error("Wrong underlying block type.\n");
    }

    const BallBlock* realBlockPtr {dynamic_cast<const BallBlock*>(block)};
    m_rect.setTopleft(realBlockPtr->m_rect.topleft());
    m_rect.setSize(realBlockPtr->m_rect.size());

    m_img = realBlockPtr->m_img;
    m_color = realBlockPtr->m_color;

    m_health = realBlockPtr->m_health;
    m_scoreValue = realBlockPtr->m_scoreValue;

    m_ballsPtr = realBlockPtr->m_ballsPtr;
};



bool BallBlock::isDead() const {
    return m_health <= 0;
};


const FRect& BallBlock::getRect() const {
    return m_rect;
};


bool BallBlock::hasBallCollision(const Ball& ball) {
    if (!ball.m_hitbox.hasRectIntersection(&m_rect)) {
        return false;
    }

    Ball* newBall {new Ball{ball}};
    newBall->setCenter(m_rect.center());
    
    float ballAbsXSpeed {(ball.xSpeed() > 0.0f) ? ball.xSpeed() : ball.xSpeed() * -1};
    float ballAbsYSpeed {(ball.ySpeed() > 0.0f) ? ball.ySpeed() : ball.ySpeed() * -1};
    newBall->setYSpeed((ballAbsYSpeed > ballAbsXSpeed) ? ballAbsYSpeed : ballAbsXSpeed);

    m_ballsPtr->push_back(newBall);

    m_health -= 1;
    return true;
};


void BallBlock::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void BallBlock::setColor(const Color& color) {
    m_color = color;
};


void BallBlock::update(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


bool BallBlock::isInvincible() const {
    return false;
};