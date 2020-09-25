#include "color.h"
#include <iostream>
#include <unistd.h>
#include "../config.h"

#if defined WINDOWS
    #include <windows.h>
#elif defined LINUX
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <stdlib.h>
#elif defined OSX
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif

namespace tools
{
    static Color_Data *static_color = new Color_Data();
    Color_Data *color_data() noexcept
    {
        return static_color;
    }
    void FreeColorData() noexcept
    {
        delete static_color;
    }
    void init(Color_Data*cd) noexcept
    {
        white(cd);
    }
    void white(Color_Data*cd) noexcept
    {
        #ifndef RELEASE
            #if defined WINDOWS
                system("color f0");
            #elif defined LINUX
            #endif
        #endif
        cd->DEF_FONT = WHITE;
        cd->COLOR_LICENCE[0] = GREEN;       cd->COLOR_LICENCE[1] = cd->DEF_FONT;
        cd->COLOR_COMMENT[0] = BLUE_GREY;   cd->COLOR_COMMENT[1] = cd->DEF_FONT;
        cd->COLOR_WARNING[0] = PURPEL;      cd->COLOR_WARNING[1] = cd->DEF_FONT;
        cd->COLOR_ERROR  [0] = RED;         cd->COLOR_ERROR  [1] = cd->DEF_FONT;
        cd->COLOR_RESET  [0] = BLACK;       cd->COLOR_RESET  [1] = cd->DEF_FONT;
        colorConsole(cd->COLOR_RESET);
    }
    void black(Color_Data*cd) noexcept
    {
        #ifndef RELEASE
            #if defined WINDOWS
                system("color 0f");
            #elif defined LINUX
            #endif
        #endif
        cd->DEF_FONT = BLACK;
        cd->COLOR_LICENCE[0] = RED;         cd->COLOR_LICENCE[1] = cd->DEF_FONT;
        cd->COLOR_COMMENT[0] = LIGHT_GREY;  cd->COLOR_COMMENT[1] = cd->DEF_FONT;
        cd->COLOR_WARNING[0] = WHITE;       cd->COLOR_WARNING[1] = cd->DEF_FONT;
        cd->COLOR_ERROR  [0] = CYAN;        cd->COLOR_ERROR  [1] = cd->DEF_FONT;
        cd->COLOR_RESET  [0] = GREY;        cd->COLOR_RESET  [1] = cd->DEF_FONT;
        colorConsole(cd->COLOR_RESET);
    }
    void grey(Color_Data*cd) noexcept
    {
        #ifndef RELEASE
            #if defined WINDOWS
                system("color 8f");
            #elif defined LINUX
            #endif
        #endif
        cd->DEF_FONT = GREY;
        cd->COLOR_LICENCE[0] = RED;         cd->COLOR_LICENCE[1] = cd->DEF_FONT;
        cd->COLOR_COMMENT[0] = LIGHT_GREY;  cd->COLOR_COMMENT[1] = cd->DEF_FONT;
        cd->COLOR_WARNING[0] = WHITE;       cd->COLOR_WARNING[1] = cd->DEF_FONT;
        cd->COLOR_ERROR  [0] = GREY;        cd->COLOR_ERROR  [1] = cd->DEF_FONT;
        cd->COLOR_RESET  [0] = GREY;        cd->COLOR_RESET  [1] = cd->DEF_FONT;
        colorConsole(cd->COLOR_RESET);
    }
    void colorConsole(const unsigned char csl[2]) noexcept
    {
        colorConsole(csl[0], csl[1]);
    }
    void colorConsole(const unsigned char couleurDuTexte, const unsigned char couleurDeFond) noexcept
    {   //! do not use in release because the release have not a console
        if(!couleurDuTexte || !couleurDeFond) return;
        #ifndef RELEASE
            #if defined WINDOWS
                SetConsoleTextAttribute(
                    GetStdHandle(STD_OUTPUT_HANDLE),
                    couleurDeFond*16+couleurDuTexte
                );
            #elif defined LINUX
                switch(couleurDuTexte)
                {
                    case 1:
                    break;
                };
                printf("\033[4%d;3%dm",couleurDeFond,couleurDuTexte);
            #else
                #warning no effect with this os
            #endif
        #endif
    }
    void clearConsole() noexcept
    {
        #if defined WINDOWS
            system("cls");
        #elif defined LINUX
            system("clear");
        #else
            // dos ? ascii 11 = CLR = clear
            printf("\0x11");
            #warning not tested
        #endif
    }
}
