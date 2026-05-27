#include <cmath>
#include <string_view>
#include <stdexcept>
#include <utility>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>
#include "./font.h"



void Font::init() {
    if (instanceCount > 0) {
        return;
    }

    bool initSuccess {TTF_Init()};
    if (!initSuccess) {
        TTF_Quit();
        throw std::runtime_error(SDL_GetError());
    }
};



Font::Font(std::string_view filePath, float size) {
    Font::init();

    m_font = TTF_OpenFont(filePath.data(), size);
    if (!m_font) {
        if (Font::instanceCount == 0) {
            TTF_Quit();
        }
        throw std::runtime_error(SDL_GetError());
    }

    Font::instanceCount += 1;
};

Font::Font(const Font& font) {
    m_font = TTF_CopyFont(font.m_font);
    Font::instanceCount += 1;
};

Font::Font(Font&& font) {
    m_font = font.m_font;
    font.m_font = nullptr;
    Font::instanceCount += 1;
};

Font::~Font() {
    TTF_CloseFont(m_font);

    Font::instanceCount -= 1;
    if (Font::instanceCount == 0) {
        TTF_Quit();
    }
};

Font& Font::operator=(const Font& font) {
    if (this == &font) {
        return *this;
    }

    TTF_CloseFont(m_font);
    m_font = TTF_CopyFont(font.m_font);

    return *this;
};

Font& Font::operator=(Font&& font) {
    if (this == &font) {
        return *this;
    }

    TTF_CloseFont(m_font);
    m_font = font.m_font;
    font.m_font = nullptr;

    return *this;
};


// Render text to a surface. Caller is responsible for managing the returned surface.
// For wrapLength: -1 = no wrap, 0 = wrap on newline, > 0 = wrap after that many pixels. 
SDL_Surface* Font::renderSurface(std::string_view text, const SDL_Color& color, int wrapLength) const {
    constexpr int textLength {0}; // zero for null terminated strings
    SDL_Surface* renderedText {nullptr};

    if (wrapLength == -1) {
        renderedText = TTF_RenderText_Blended(m_font, text.data(), textLength, color);
    } else {
        renderedText = TTF_RenderText_Blended_Wrapped(m_font, text.data(), textLength, color, wrapLength);
    }

    return renderedText;
};

// Render text to a texture. Caller is responsible for managing the returned texture.
// For wrapLength: -1 = no wrap, 0 = wrap on newline, > 0 = wrap after that many pixels. 
SDL_Texture* Font::renderTexture(SDL_Renderer* renderer, std::string_view text, const SDL_Color& color, int wrapLength) const {

    SDL_Surface* surface {renderSurface(text, color, wrapLength)};
    SDL_Texture* texture {SDL_CreateTextureFromSurface(renderer, surface)};
    SDL_DestroySurface(surface);
    return texture;
};


// Set the font's renderering direction. Returns false on error.
bool Font::setDirection(Font::Direction direction) {
    return TTF_SetFontDirection(m_font, static_cast<TTF_Direction>(direction));
};

// Get font's current rendering direction.
Font::Direction Font::direction() const {
    return static_cast<Font::Direction>(TTF_GetFontDirection(m_font));
};

// Get the current horizontal alignment option for the font. This governs how wrapped text lines are aligned. 
Font::Align Font::alignment() const {
    return static_cast<Font::Align>(TTF_GetFontWrapAlignment(m_font));
};

// Set the current horizontal alignment option for the font. This governs how wrapped text lines are aligned. 
// Returns object ref for chaining.
Font& Font::setAlignment(Font::Align alignment) {
    TTF_SetFontWrapAlignment(m_font, static_cast<TTF_HorizontalAlignment>(alignment));
    return *this;
};

// Get font point size.
float Font::size() const {
    return TTF_GetFontSize(m_font);
};

// Set font point size. Returns false on error.
bool Font::setSize(float size) {
    return TTF_SetFontSize(m_font, size);
};

// Get the amount of space between lines when text wraps.
// The default is equal to the font point size rounded up.
int Font::lineSpacing() const {
    return TTF_GetFontLineSkip(m_font);
};

// Set the space between lines of text when text wraps.
// Negative values will be clamped to 0. Returns object ref for chaining.
Font& Font::setLineSpacing(int spacing) {
    if (spacing < 0) {
        spacing = 0;
    }
    TTF_SetFontLineSkip(m_font, spacing);

    return *this;
};

// Get the current DPI (dots-per-inch) the of the font.
std::pair<int, int> Font::DPI() const {
    int horizontalDPI {};
    int verticalDPI {};
    TTF_GetFontDPI(m_font, &horizontalDPI, &verticalDPI);
    return std::pair<int, int>{horizontalDPI, verticalDPI};
};

// Set the DPI of the font. Font size will scale with the new DPI. Returns false on error. 
bool Font::setDPI(int hDPI, int vDPI) {
    return TTF_SetFontSizeDPI(m_font, size(), hDPI, vDPI);
};

// Check if font will be rendered with fake bold.
bool Font::isBold() const {
    TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
    return (flags & TTF_STYLE_BOLD);
};

// Set if font should render with fake bold. Returns object ref for chaining.
Font& Font::setBold(bool emboldFont) {
    bool currentlyBold {isBold()};
    
    if ((emboldFont && !currentlyBold) || (!emboldFont && currentlyBold)) {
        TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
        TTF_SetFontStyle(m_font, flags ^ TTF_STYLE_BOLD);
    } 

    return *this;
};

// Check if font will rendered italic.
bool Font::isItalic() const {
    TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
    return (flags & TTF_STYLE_ITALIC);
};

// Set if font should render italic. Returns object ref for chaining.
Font& Font::setItalic(bool italicizeFont) {
    bool currentlyItalic {isItalic()};
    
    if ((italicizeFont && !currentlyItalic) || (!italicizeFont && currentlyItalic)) {
        TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
        TTF_SetFontStyle(m_font, flags ^ TTF_STYLE_ITALIC);
    }

    return *this;
};

// Check if the font will render underlined.
bool Font::isUnderlined() const {
    TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
    return (flags & TTF_STYLE_UNDERLINE);
};

// Set if font should render underlined. Returns object ref for chaining.
Font& Font::setUnderline(bool underlineFont) {
    bool currentlyUnderlined {isUnderlined()};
    
    if ((currentlyUnderlined && !underlineFont) || (!currentlyUnderlined && underlineFont)) {
        TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
        TTF_SetFontStyle(m_font, flags ^ TTF_STYLE_UNDERLINE);
    }

    return *this;
};

// Check if the font will render struckthrough.
bool Font::isStruckThrough() const {
    TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
    return (flags & TTF_STYLE_STRIKETHROUGH);
};

// Set if font should render struckthrough. Returns object ref for chaining.
Font& Font::setStrikeThrough(bool strikethroughFont) {
    bool currentlyStruckthrough {isStruckThrough()};

    if ((currentlyStruckthrough && !strikethroughFont) || (!currentlyStruckthrough && strikethroughFont)) {
        TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
        TTF_SetFontStyle(m_font, flags ^ TTF_STYLE_STRIKETHROUGH);
    }

    return *this;
};

// Returns the current active style flags OR'd together.
Font::Style Font::styles() const {
    TTF_FontStyleFlags flags {TTF_GetFontStyle(m_font)};
    return static_cast<Font::Style>(flags);
};

// Set the style for the font using Font::Style flags OR'd together. Overwrites any previously set styles.
// Returns object ref for chaining.
Font& Font::setStyles(Font::Style flags) {
    TTF_SetFontStyle(m_font, static_cast<TTF_FontStyleFlags>(flags));
    return *this;
};

// Get width of the current outline on the font. 0 is the default.
int Font::outline() const {
    return TTF_GetFontOutline(m_font);
};

// Set width of the current outline on the font. 0 is the default.
bool Font::setOutline(int outline) {
    if (outline < 0) {
        outline = 0;
    }
    return TTF_SetFontOutline(m_font, outline);
};

// Returns true if kerning is enabled for this font. False otherwise.
bool Font::kerning() const {
    return TTF_GetFontKerning(m_font);
};

// Set if kerning should be enabled. (true to enable | false to disable).
// Returns object ref for chaining.
Font& Font::setKerning(bool enabled) {
    TTF_SetFontKerning(m_font, enabled);
    return *this;
};

// Check if Signed Distance Field rendering is enabled.
bool Font::SDF() const {
    return TTF_GetFontSDF(m_font);
};

// Set if Signed Distance Field rendering should be enabled. Returns false on error.
bool Font::setSDF(bool enabled) {
    return TTF_SetFontSDF(m_font, enabled);
};

// Get the current height of the font characters not including line spacing.
// Assuming no line wrap, this will be the height of the rendered text.
int Font::height() const {
    return TTF_GetFontHeight(m_font);
};

// Returns the offset from the baseline to the bottom of a font.
// This is a negative value, relative to the baseline.
int Font::descent() const {
    return TTF_GetFontDescent(m_font);
};

// Returns the offset from the baseline to the top of a font.
// This is a positive value, relative to the baseline.
int Font::ascent() const {
    return TTF_GetFontAscent(m_font);
};


bool Font::hasGlyph(Uint32 characterCode) {
    return TTF_FontHasGlyph(m_font, characterCode);
};


Font::Style operator|(Font::Style a, Font::Style b) {
    return static_cast<Font::Style>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
};

Font::Style operator&(Font::Style a, Font::Style b) {
    return static_cast<Font::Style>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
};

Font::Style operator^(Font::Style a, Font::Style b) {
    return static_cast<Font::Style>(static_cast<Uint32>(a) ^ static_cast<Uint32>(b));
};