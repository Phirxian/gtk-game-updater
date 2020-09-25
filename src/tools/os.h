#pragma once

#include <string>
#include <future>

struct tm;

namespace tools
{
    /************************************************************/
        std::string getOS() noexcept;                                // "windows"/"windows64"/"linux"/"OSX"
    /************************************************************/
        void openURL(const char*) noexcept;                          // portable ShellExecute/execlp
    /************************************************************/
        struct tm* getLocalTimer() noexcept;
        std::string getSystemDate(const bool space = true) noexcept; // return "day / mon / years" ex : "17 / 01 / 2009" ( day/month/years )
        std::string getSystemTime(const bool space = true) noexcept; // return if true "23 : 20 : 10" else "23:20:10" ( h:min:sec )
        std::string getUserHome() noexcept;                          // return home directorie of curent user, max buffer is 64 ex : c:/Documents and Setting/Ovan/ or /home/ovan/
        std::string getExecutablePath() noexcept;                    // return executable directorie ex: c:/programe file/FEDA-Corp/IGZ
        std::string getUserName() noexcept;                          // return current login user name
        void Sleeping(const unsigned long ms) noexcept;              // is multiplatform of Sleep() on windows and usleep in unix
    /************************************************************/
        void setWorkingDirectory(const char*) noexcept;              // set the current filesystem directory
        std::string getWorkingDirectory() noexcept;                  // return current filesystem directory
    /************************************************************/
        bool deleteFile(const std::string&) noexcept;                // delete file in the filesystem
        bool readable(const std::string&) noexcept;                  // query filesystem to check if the file exists
        unsigned long checkFileSize(const std::string&) noexcept;    // open the file, seek to the end, and return the number of bit
    /************************************************************/
        unsigned long strsize(const char *str) noexcept;             // while(str[i]) ++i;
        unsigned long strsize(const wchar_t *str) noexcept;          // while(str[i]) ++i;
        wchar_t*char2wchar(const char*msg) noexcept;                 // convert char* to wchar_t*
        char*wchar2char(const wchar_t*msg) noexcept;                 // convert wchar_t* to char*
    /************************************************************/
        template<typename T>
        void waitForSingleObject(std::function<void()> f, T *a) noexcept
        {
            std::async(std::launch::async, [&]{
                while(!a) Sleeping(1);
                f();
            });
        }
    /************************************************************/
        template<typename T>
        T neareastPower2(T in) noexcept
        {
            if(in <= 1) return 1;
            T result = 2;

            while(in > 3)
            {
                in = in >> 1;
                result = result << 1;
            }

            if(in == 3) return result << 1;
            else return result;
        }
    /************************************************************/
        size_t getPeakRSS() noexcept;
        size_t getCurrentRSS() noexcept;
    /************************************************************/
        FILE * popen2(std::string command, std::string type, int & pid);
}
