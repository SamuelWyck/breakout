#include <exception>
#include <typeinfo>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>
#include "./iBlock.h"
#include "./ball.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/collision/fRect.h"
#include "./ghostBlock.h"
#include "../framework/colors.h"
#include "../framework/framework.h"



GhostBlock::GhostBlock(
    float x, float y, SDL_Surface* ghostImgMask, SDL_Texture* img, const Color& color, int health, int scoreVal
) 
    : m_ghostImgMask{ghostImgMask}, m_img{img}, m_color{color}, m_health{health}, m_scoreValue{scoreVal} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_img->w, m_img->h);

    createGhostImage();
};


GhostBlock::GhostBlock(const IBlock* block) {
    if (typeid(*block) != typeid(GhostBlock)) {
        throw std::runtime_error("Wrong underlying block type.\n");
    }

    const GhostBlock* realBlockPtr {dynamic_cast<const GhostBlock*>(block)};
    m_rect.setTopleft(realBlockPtr->m_rect.topleft());
    m_rect.setSize(realBlockPtr->m_rect.size());

    m_ghostImgMask = realBlockPtr->m_ghostImgMask;
    m_img = realBlockPtr->m_img;
    m_color = realBlockPtr->m_color;
    
    m_scoreValue = realBlockPtr->m_scoreValue;
    m_health = realBlockPtr->m_health;

    createGhostImage();
};


GhostBlock::~GhostBlock() {
    SDL_DestroyTexture(m_ghostImg);
};



void GhostBlock::createGhostImage() {
    if (m_ghostImg) {
        SDL_DestroyTexture(m_ghostImg);
        m_ghostImg = nullptr;
    }

    SDL_Surface* baseSurface {SDL_CreateSurface(m_img->w, m_img->h, SDL_PIXELFORMAT_RGBA8888)};
    const SDL_PixelFormatDetails* pixelFormat {SDL_GetPixelFormatDetails(baseSurface->format)};
    Uint32 fillColor {SDL_MapRGB(pixelFormat, nullptr, m_color.red(), m_color.green(), m_color.blue())};
    SDL_FillSurfaceRect(baseSurface, nullptr, fillColor);
    SDL_BlitSurface(m_ghostImgMask, nullptr, baseSurface, nullptr);

    SDL_Surface* finalSurface {SDL_CreateSurface(m_img->w, m_img->h, SDL_PIXELFORMAT_RGBA8888)};
    Uint32 colorKey {SDL_MapRGB(pixelFormat, nullptr, Colors::KEY.red(), Colors::KEY.green(), Colors::KEY.blue())};
    SDL_SetSurfaceColorKey(finalSurface, true, colorKey);

    SDL_BlitSurface(baseSurface, nullptr, finalSurface, nullptr);
    m_ghostImg = SDL_CreateTextureFromSurface(Framework::display.renderer(), finalSurface);

    SDL_DestroySurface(baseSurface);
    SDL_DestroySurface(finalSurface);
};


bool GhostBlock::isDead() const {
    return m_health <= 0;
};


void GhostBlock::update(SDL_Renderer* renderer) {
    if (!m_stillHit && m_deactiveGhost) {
        m_isGhost = false;
        m_deactiveGhost = false;
    }
    m_stillHit = false;

    if (m_isGhost) {
        SDL_RenderTexture(renderer, m_ghostImg, nullptr, &m_rect.getSDLFRect());

    } else {
        SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
        SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
        SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
    }
};


bool GhostBlock::hasBallCollision(const Ball& ball) {
    if (!ball.m_hitbox.hasRectIntersection(&m_rect)) {
        return false;
    }
    if (m_isGhost && !m_deactiveGhost) {
        m_deactiveGhost = true;
        m_stillHit = true;
        return false;
    }
    if (m_isGhost && m_deactiveGhost) {
        m_stillHit = true;
        return false;
    }
    
    m_health -= 1;
    return true;
};


void GhostBlock::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void GhostBlock::setColor(const Color& color) {
    m_color = color;
    createGhostImage();
};


const FRect& GhostBlock::getRect() const {
    return m_rect;
};


bool GhostBlock::isInvincible() const {
    return false;
};


int GhostBlock::getScore() const {
    return m_scoreValue;
};