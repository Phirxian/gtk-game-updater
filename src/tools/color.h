#pragma once

namespace tools
{
    enum COLOR_REF
    {
        BLACK,       BLACK_BLUE,
        GREEN,       BLUE_GREY,
        BROWN,       PURPEL,
        BLACK_GREEN, LIGHT_GREY,
        GREY,        LIGHT_BLUE,
        LIGHT_GREEN, CYAN,
        RED,         PINK,
        YELLOW,      WHITE,
        COUNT_DO_NOT_USE
    };

    static const char* e_color_ref_name[] =
    {
        "black",       "black_blue",
        "green",       "blue_grey",
        "brown",       "purpel",
        "black_green", "light_grey",
        "grey",        "light_blue",
        "light_green", "cyan",
        "red",         "pink",
        "yellow",      "white",
        0
    };

    // change console line color (background and text)
    void colorConsole(const unsigned char,const unsigned char) noexcept;
    // change console line color (background and text)
    void colorConsole(const unsigned char[2]) noexcept;
    // clear buffer of the console
    void clearConsole() noexcept;

    void init(struct Color_Data*) noexcept;
    void white(struct Color_Data*) noexcept;
    void black(struct Color_Data*) noexcept;
    void grey(struct Color_Data*) noexcept;

    struct Color_Data
    {
        Color_Data() noexcept : DEF_FONT(0) { init(this); }
        unsigned char DEF_FONT;
        unsigned char COLOR_LICENCE[2];
        unsigned char COLOR_COMMENT[2];
        unsigned char COLOR_WARNING[2];
        unsigned char COLOR_ERROR  [2];
        unsigned char COLOR_RESET  [2];
    };

    Color_Data *color_data() noexcept;
    void FreeColorData() noexcept;
}
