#ifndef COLOR_H
#define COLOR_H


#include <SDL3/SDL_pixels.h>


class Color {
public:
    static const int maxColorVal {255};
    static const int minColorVal {0};

    enum BlendFlag {
        BLEND_RGB_ADD,
        BLEND_RGB_SUB,
        BLEND_RGB_MULT,
        BLEND_RGB_MIN,
        BLEND_RGB_MAX,
        BLEND_RGBA_ADD,
        BLEND_RGBA_SUB,
        BLEND_RGBA_MULT,
        BLEND_RGBA_MIN,
        BLEND_RGBA_MAX
    };


private:
    int m_red {0};
    int m_green {0};
    int m_blue {0};
    int m_alpha {255};


public:


    Color(int red, int green, int blue, int alpha);
    Color(int red, int green, int blue);
    Color(const SDL_Color& color);
    Color();


    Color& blendColor(const Color& color, Color::BlendFlag blendFlag=BLEND_RGB_ADD);

    SDL_Color getSDLColor() const;


    int red() const;
    void setRed(int channelVal);
    int green() const;
    void setGreen(int channelVal);
    int blue() const;
    void setBlue(int channelVal);
    int alpha() const;
    void setAlpha(int channelVal);


    bool operator==(const Color& color) const;
    bool operator==(const SDL_Color& color) const;

    Color& operator=(const SDL_Color& color);

    explicit operator SDL_Color() const;


private:
    int clampValue(int value);
};



#endif