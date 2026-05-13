#include <SDL3/SDL_pixels.h>
#include "./color.h"



Color::Color(int red, int green, int blue, int alpha) 
: m_red{clampValue(red)}, 
m_green{clampValue(green)}, 
m_blue{clampValue(blue)}, 
m_alpha{clampValue(alpha)} {
};

Color::Color(int red, int green, int blue)
    : m_red{clampValue(red)}, 
    m_green{clampValue(green)}, 
    m_blue{clampValue(blue)}, 
    m_alpha{255} {
};

Color::Color(const SDL_Color& color)
    : m_red{clampValue(color.r)},
    m_green{clampValue(color.g)},
    m_blue{clampValue(color.b)},
    m_alpha{clampValue(color.a)} {
};

Color::Color() {
};


Color& Color::blendColor(const Color& color, Color::BlendFlag blendFlag) {
    switch (blendFlag) {
        case Color::BLEND_RGB_ADD:
            setRed(m_red + color.m_red);
            setGreen(m_green + color.m_green);
            setBlue(m_blue + color.m_blue);
            break;

        case Color::BLEND_RGB_SUB:
            setRed(m_red - color.m_red);
            setGreen(m_green - color.m_green);
            setBlue(m_blue - color.m_blue);
            break;
        
        case Color::BLEND_RGB_MULT:
            setRed(static_cast<int>((m_red * color.m_red) / 256));
            setGreen(static_cast<int>((m_green * color.m_green) / 256));
            setBlue(static_cast<int>((m_blue * color.m_blue) / 256));
            break;

        case Color::BLEND_RGB_MIN:
            setRed((m_red < color.m_red) ? m_red : color.m_red);
            setGreen((m_green < color.m_green) ? m_green : color.m_green);
            setBlue((m_blue < color.m_blue) ? m_blue : color.m_blue);
            break;

        case Color::BLEND_RGB_MAX:
            setRed((m_red > color.m_red) ? m_red : color.m_red);
            setGreen((m_green > color.m_green) ? m_green : color.m_green);
            setBlue((m_blue > color.m_blue) ? m_blue : color.m_blue);
            break;

        case Color::BLEND_RGBA_ADD:
            setRed(m_red + color.m_red);
            setGreen(m_green + color.m_green);
            setBlue(m_blue + color.m_blue);
            setAlpha(m_alpha + color.m_alpha);
            break;

        case Color::BLEND_RGBA_SUB:
            setRed(m_red - color.m_red);
            setGreen(m_green - color.m_green);
            setBlue(m_blue - color.m_blue);
            setAlpha(m_alpha - color.m_alpha);
            break;
        
        case Color::BLEND_RGBA_MULT:
            setRed(static_cast<int>((m_red * color.m_red) / 256));
            setGreen(static_cast<int>((m_green * color.m_green) / 256));
            setBlue(static_cast<int>((m_blue * color.m_blue) / 256));
            setAlpha(static_cast<int>((m_alpha * color.m_alpha) / 256));
            break;

        case Color::BLEND_RGBA_MIN:
            setRed((m_red < color.m_red) ? m_red : color.m_red);
            setGreen((m_green < color.m_green) ? m_green : color.m_green);
            setBlue((m_blue < color.m_blue) ? m_blue : color.m_blue);
            setAlpha((m_alpha < color.m_alpha) ? m_alpha : color.m_alpha);
            break;

        case Color::BLEND_RGBA_MAX:
            setRed((m_red > color.m_red) ? m_red : color.m_red);
            setGreen((m_green > color.m_green) ? m_green : color.m_green);
            setBlue((m_blue > color.m_blue) ? m_blue : color.m_blue);
            setAlpha((m_alpha > color.m_alpha) ? m_alpha : color.m_alpha);
            break;
    };

    return *this;
};


SDL_Color Color::getSDLColor() const {
    return SDL_Color{
        static_cast<Uint8>(m_red), 
        static_cast<Uint8>(m_green), 
        static_cast<Uint8>(m_blue), 
        static_cast<Uint8>(m_alpha)
    };
};


int Color::red() const {
    return m_red;
};

void Color::setRed(int channelVal) {
    m_red = clampValue(channelVal);
};

int Color::green() const {
    return m_green;
};

void Color::setGreen(int channelVal) {
    m_green = clampValue(channelVal);
};

int Color::blue() const {
    return m_blue;
};

void Color::setBlue(int channelVal) {
    m_blue = clampValue(channelVal);
};

int Color::alpha() const {
    return m_alpha;
};

void Color::setAlpha(int channelVal) {
    m_alpha = clampValue(channelVal);
};


bool Color::operator==(const Color& color) const {
    return m_red == color.m_red && m_green == color.m_green && m_blue == color.m_blue && m_alpha == color.m_alpha;
};


bool Color::operator==(const SDL_Color& color) const {
    return m_red == color.r && m_green == color.g && m_blue == color.b && m_alpha == color.a;
};


Color::operator SDL_Color() const {
    return SDL_Color{
        static_cast<Uint8>(m_red), 
        static_cast<Uint8>(m_green), 
        static_cast<Uint8>(m_blue), 
        static_cast<Uint8>(m_alpha)
    };
};


Color& Color::operator=(const SDL_Color& color) {
    m_red = clampValue(color.r);
    m_green = clampValue(color.g);
    m_blue = clampValue(color.b);
    m_alpha = clampValue(color.a);

    return *this;
};


int Color::clampValue(int value) {
    if (value > Color::maxColorVal) {
        value = Color::maxColorVal;
    } else if (value < Color::minColorVal) {
        value = Color::minColorVal;
    }
    return value;
};