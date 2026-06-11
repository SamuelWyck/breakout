#ifndef MY_MENU_TYPES_H
#define MY_MENU_TYPES_H


#include <utility>
#include <optional>
#include <functional>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>


using MenuReturn = std::optional<std::pair<int, int>>;
using MenuCb = std::function<MenuReturn(SDL_Renderer*, SDL_Surface*)>;


#endif