#ifndef MY_TEXT_ENGINE_H
#define MY_TEXT_ENGINE_H


#include <stdexcept>
#include <string>
#include <SDL3/SDL_error.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>



class TextEngine {
    inline static bool created {false};

    TTF_TextEngine* m_textEngine {nullptr};


public:
    TextEngine(SDL_Renderer* renderer) {
        if (created) {
            throw std::runtime_error("TextEngine already created.\n");
        }

        bool initSuccess {TTF_Init()};
        if (!initSuccess) {
            throw std::runtime_error(SDL_GetError());
        }

        m_textEngine = TTF_CreateRendererTextEngine(renderer);
        if (!m_textEngine) {
            throw std::runtime_error(SDL_GetError());
        }

        TextEngine::created = true;
    };

    TextEngine(const TextEngine&) = delete;
    TextEngine(TextEngine&&) = delete;
    TextEngine& operator=(const TextEngine&) = delete;
    TextEngine& operator=(TextEngine&&) = delete;

    ~TextEngine() {
        TTF_DestroyRendererTextEngine(m_textEngine);
        TTF_Quit();
    };


    SDL_Texture* getTextTexture(
        SDL_Renderer* renderer, const std::string& text, const SDL_Color& color, TTF_Font* font, int wrapWidth=-1
    ) {
        SDL_Surface* textSurface {getTextSurface(text, color, font, wrapWidth)};
        SDL_Texture* texture {SDL_CreateTextureFromSurface(renderer, textSurface)};
        SDL_DestroySurface(textSurface);
        return texture;
    };


    SDL_Surface* getTextSurface(const std::string& text, const SDL_Color& color, TTF_Font* font, int wrapWidth=-1) {
        SDL_Surface* textSurface {nullptr};
        constexpr int textLength {0};
        if (wrapWidth == -1) {
            textSurface = TTF_RenderText_Blended(font, text.data(), textLength, color);
        } else {
            textSurface = TTF_RenderText_Blended_Wrapped(font, text.data(), textLength, color, wrapWidth);
        }

        return textSurface;
    };
};



#endif