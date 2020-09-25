#pragma once

#define LOGO_PATH "asset/gtk/logo.png"
#define CSS_PATH  "asset/gtk.css"
#define CFG_PATH  "config/launcher"

#define WEB_GAME_HOST  "immortalgalaxy.free.fr"
#define WEB_GAME_INDEX "http://" WEB_GAME_HOST "/web/index.php"
#define WEB_GAME_FORUM "http://" WEB_GAME_HOST "/forum/index.php"
#define WEB_GAME_NEWS  "http://" WEB_GAME_HOST "/web/news.php"
#define WEB_GAME_UPDATE(ip) ip = "sleek-think.ovh";

static const char Licence[] = "by Magun on www.immortal-galaxy.com\n"
                              "please specified my web site on your modifed source and credit\n"
                              "this is a creative commons licence with by-nc-sa attribute\n";
static const char WebSite[] = "http://creativecommons.org/licenses/by-nc-sa/2.0/\n";

#if defined linux || defined __linux
#   define LINUX
#elif ( defined WIN64 || defined _WIN64 )// && undefined WINDOWS
#   define WINDOWS 64_BIT_PROCESOR
#elif ( defined _WIN32 || defined __WIN32__ || defined _WIN32_WCE )// && defined !WINDOWS
#   define WINDOWS 32_BIT_PROCESOR
#elif defined _XBOX// && defined !XBOX
#   define XBOX
#elif ( defined __APPLE__ && __MACH__ )// && defined !OSX
#   define OSX
#else
#   error OS NOT SUPPORTED
#endif