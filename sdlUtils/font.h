#ifndef MY_SDL_FONT_H
#define MY_SDL_FONT_H


#include <utility>
#include <string_view>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>


class Font {
public:
    enum class Direction {
        LEFT_TO_RIGHT = TTF_DIRECTION_LTR,
        RIGHT_TO_LEFT = TTF_DIRECTION_RTL,
        TOP_TO_BOTTOM = TTF_DIRECTION_TTB,
        BOTTOM_TO_TOP = TTF_DIRECTION_BTT
    };

    enum class Style : Uint32 {
        NORMAL = TTF_STYLE_NORMAL,
        BOLD = TTF_STYLE_BOLD,
        ITALIC = TTF_STYLE_ITALIC,
        UNDERLINE = TTF_STYLE_UNDERLINE,
        STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH
    };

    enum class Align {
        LEFT = TTF_HORIZONTAL_ALIGN_LEFT,
        RIGHT = TTF_HORIZONTAL_ALIGN_RIGHT,
        CENTER = TTF_HORIZONTAL_ALIGN_CENTER
    };


private:
    inline static int instanceCount {0};

    static void init();



    TTF_Font* m_font {nullptr};


public:
    Font(std::string_view filePath, float size);
    Font(const Font& font);
    Font(Font&& font);
    ~Font();
    Font& operator=(const Font& font);
    Font& operator=(Font&& font);


    // Render text to a surface. Caller is responsible for managing the returned surface.
    // For wrapLength: -1 = no wrap, 0 = wrap on newline, > 0 = wrap after that many pixels. 
    SDL_Surface* renderSurface(std::string_view text, const SDL_Color& color, int wrapLength=-1) const;

    // Render text to a texture. Caller is responsible for managing the returned texture.
    // For wrapLength: -1 = no wrap, 0 = wrap on newline, > 0 = wrap after that many pixels. 
    SDL_Texture* renderTexture(SDL_Renderer* renderer, std::string_view text, const SDL_Color& color, int wrapLength=-1) const;


    // Set the font's renderering direction. Returns false on error.
    bool setDirection(Font::Direction direction);

    // Get font's current rendering direction.
    Font::Direction direction() const;

    // Get the current horizontal alignment option for the font. This governs how wrapped text lines are aligned. 
    Font::Align alignment() const;

    // Set the current horizontal alignment option for the font. This governs how wrapped text lines are aligned. 
    // Returns object ref for chaining.
    Font& setAlignment(Font::Align alignment);


    // Get font point size.
    float size() const;

    // Set font point size. Returns false on error.
    bool setSize(float size);

    // Get the amount of space between lines when text wraps.
    // The default is equal to the font point size rounded up.
    int lineSpacing() const;

    // Set the space between lines of text when text wraps.
    // Negative values will be clamped to 0. Returns object ref for chaining.
    Font& setLineSpacing(int spacing);

    // Get the current DPI (dots-per-inch) the of the font.
    std::pair<int, int> DPI() const;

    // Set the DPI of the font. Font size will scale with the new DPI. Returns false on error. 
    bool setDPI(int hDPI, int vDPI);


    // Check if font will be rendered with fake bold.
    bool isBold() const;

    // Set if font should render with fake bold. Returns object ref for chaining.
    Font& setBold(bool emboldFont);

    // Check if font will rendered italic.
    bool isItalic() const;

    // Set if font should render italic. Returns object ref for chaining.
    Font& setItalic(bool italicizeFont);

    // Check if the font will render underlined.
    bool isUnderlined() const;

    // Set if font should render underlined. Returns object ref for chaining.
    Font& setUnderline(bool underlineFont);

    // Check if the font will render struckthrough.
    bool isStruckThrough() const;

    // Set if font should render struckthrough. Returns object ref for chaining.
    Font& setStrikeThrough(bool strikethroughFont);

    // Returns the current active style flags OR'd together.
    Font::Style styles() const;

    // Set the style for the font using Font::Style flags OR'd together. Overwrites any previously set styles.
    // Returns object ref for chaining.
    Font& setStyles(Font::Style flags);


    // Get width of the current outline on the font. 0 is the default.
    int outline() const;

    // Set width of the current outline on the font. 0 is the default.
    bool setOutline(int outline);


    // Returns true if kerning is enabled for this font. False otherwise.
    bool kerning() const;

    // Set if kerning should be enabled. (true to enable | false to disable).
    // Returns object ref for chaining.
    Font& setKerning(bool enabled);


    // Check if Signed Distance Field rendering is enabled.
    bool SDF() const;

    // Set if Signed Distance Field rendering should be enabled. Returns false on error.
    bool setSDF(bool enabled);


    // Get the current height of the font characters not including line spacing.
    // Assuming no line wrap, this will be the height of the rendered text.
    int height() const;

    // Returns the offset from the baseline to the bottom of a font.
    // This is a negative value, relative to the baseline.
    int descent() const;

    // Returns the offset from the baseline to the top of a font.
    // This is a positive value, relative to the baseline.
    int ascent() const;

    // Returns true if the font has a character with the given code.
    bool hasGlyph(Uint32 characterCode);
};


Font::Style operator|(Font::Style a, Font::Style b);
Font::Style operator&(Font::Style a, Font::Style b);
Font::Style operator^(Font::Style a, Font::Style b);


#endif