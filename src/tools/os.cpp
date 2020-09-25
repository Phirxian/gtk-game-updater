#include <unistd.h>
#include <cerrno>
#include <thread>
#include <future>

#include <time.h>
#include <fstream>
#include <iostream>

#include "../config.h"
#include "string.h"
#include "os.h"

#if defined WINDOWS
#include <windows.h>
#include <psapi.h>
#elif defined LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#elif OSX
//#include "MacOSX/OSXClipboard.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#endif

using namespace std;

namespace tools
{
    std::string getOS() noexcept
    {
        #if defined WIN64
            return "windows 64bits";
        #elif defined WIN32
            return "windows 32bits";
        #elif defined LINUX
            return "linux";
        #else
            return "unknow";
        #endif
    }

    void openURL(const char *url) noexcept
    {
        #if defined WINDOWS
            ShellExecute(NULL, NULL,url, NULL, NULL, SW_SHOWNORMAL);
        #elif defined LINUX
            execlp(url,url,NULL);
        #else
            std::cout << "url " << url
                      << "is not open because os not supported"
                      << std::endl;
        #endif
    }

    wchar_t *char2wchar(const char *msg) noexcept
    {
        int size = strsize(msg);
        wchar_t *str = new wchar_t[size];
        mbstowcs(str, msg, size+1);
        return str;
    }

    char *wchar2char(const wchar_t *msg) noexcept
    {
        int size = strsize(msg);
        char *str = new char[size];
        wcstombs(str, msg, size+1);
        return str;
    }

    struct tm *getLocalTimer() noexcept
    {
        struct tm *newTime;
        time_t timer;
        time(&timer);
        newTime = localtime(&timer);
        return newTime;
    }

    std::string getSystemDate(const bool space) noexcept
    {
        std::string tmp;
        tmp.resize(80);

        static const char *format[] =
        {
            "%d/%m/%Y",     // false
            "%d / %m / %Y"  // true
        };

        strftime(
            const_cast<char *>(tmp.c_str()),
            80, format[space], getLocalTimer()
        );

        return tools::split(tmp);
    }

    std::string getSystemTime(const bool space) noexcept
    {
        char *str = new char[80];
        if(space) strftime(str,80,"%H : %M : %S",getLocalTimer());
        else      strftime(str,80,"%H:%M:%S",    getLocalTimer());
        return str;
    }

    std::string getUserHome() noexcept
    {
        return getenv("HOME");
    }

    std::string getUserName() noexcept
    {
        return getenv("USER");
    }

    void setWorkingDirectory(const char *path) noexcept
    {
        #ifndef __WIN32
            setenv("PWD", path, 1);
        #else
            chdir(path);
        #endif
    }

    std::string getWorkingDirectory() noexcept
    {
        return getenv("PWD");
    }

    std::string getExecutablePath() noexcept
    {
        std::string buffer;
        buffer.resize(BUFSIZ);

        int bytes;
        #if defined WINDOWS
            bytes = GetModuleFileName(NULL, buffer.str, BUFSIZ);
        #elif defined LINUX
            if(getpid())
            {
                sprintf(const_cast<char *>(buffer.c_str()), "/proc/%d/exe", getpid());
                bytes = readlink(
                    const_cast<char *>(buffer.c_str()),
                    const_cast<char *>(buffer.c_str()), BUFSIZ
                );
            }
            else
            {
                bytes = readlink(
                    "/proc/self/exe",
                    const_cast<char *>(buffer.c_str()), BUFSIZ
                );
            }
        #else
        #warning undefined os
        #endif
        buffer[bytes] = '\0';

        return tools::split(buffer);
    }

    void Sleeping(const unsigned long ms) noexcept
    {
        if(!ms) return;
        #if defined WINDOWS
            Sleep(ms);
        #elif defined LINUX
            usleep(ms*1000);
//                    struct timeval tv;
//                    tv.tv_sec = ms/1000;
//                    tv.tv_usec = (ms%1000) * 1000;
//                    select(0, NULL, NULL, NULL, &tv);
        #else
        #warning undefined os
        #endif
    }

    unsigned long strsize(const char *str) noexcept
    {
        unsigned long i = 0;
        for(; *str; ++str, ++i) ;
        return i;
    }

    unsigned long strsize(const wchar_t *str) noexcept
    {
        unsigned long i = 0;
        for(; *str; ++str, ++i) ;
        return i;
    }

    bool deleteFile(const std::string &file) noexcept
    {
        const char *filename = file.c_str();
        //return remove(filename) == 0;
        return unlink(filename) == 0;
    }

    bool readable(const std::string &file) noexcept
    {
        return access(file.c_str(), F_OK) == 0;
    }

    unsigned long checkFileSize(const std::string &file) noexcept
    {
        unsigned long begin,end;
        ifstream myfile(file.c_str(),ios::in);
        begin = myfile.tellg();
        myfile.seekg(0,ios::end);
        end = myfile.tellg();
        myfile.close();
        return end-begin;
    }
    /************************************************************/
    size_t getPeakRSS() noexcept
    {
        #if defined(_WIN32)
            /* Windows -------------------------------------------------- */
            PROCESS_MEMORY_COUNTERS info;
            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
            return (size_t)info.PeakWorkingSetSize;
        #elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
            /* AIX and Solaris ------------------------------------------ */
            struct psinfo psinfo;
            int fd = -1;
            if((fd = open("/proc/self/psinfo", O_RDONLY)) == -1)
                return (size_t)0L;      /* Can't open? */
            if(read(fd, &psinfo, sizeof(psinfo)) != sizeof(psinfo))
            {
                close(fd);
                return (size_t)0L;      /* Can't read? */
            }
            close(fd);
            return (size_t)(psinfo.pr_rssize * 1024L);

        #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
            /* BSD, Linux, and OSX -------------------------------------- */
            struct rusage rusage;
            getrusage(RUSAGE_SELF, &rusage);
            #if defined(__APPLE__) && defined(__MACH__)
                return (size_t)rusage.ru_maxrss;
            #else
                return (size_t)(rusage.ru_maxrss * 1024L);
            #endif
        #else
            /* Unknown OS ----------------------------------------------- */
            return (size_t)0L;          /* Unsupported. */
        #endif
    }

    size_t getCurrentRSS() noexcept
    {
        #if defined(_WIN32)
            /* Windows -------------------------------------------------- */
            PROCESS_MEMORY_COUNTERS info;
            GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
            return (size_t)info.WorkingSetSize;

        #elif defined(__APPLE__) && defined(__MACH__)
            /* OSX ------------------------------------------------------ */
            struct mach_task_basic_info info;
            mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
            if(task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                         (task_info_t)&info, &infoCount) != KERN_SUCCESS)
                return (size_t)0L;      /* Can't access? */
            return (size_t)info.resident_size;

        #elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
            /* Linux ---------------------------------------------------- */
            long rss = 0L;
            FILE *fp = NULL;
            if((fp = fopen("/proc/self/statm", "r")) == NULL)
                return (size_t)0L;      /* Can't open? */
            if(fscanf(fp, "%*s%ld", &rss) != 1)
            {
                fclose(fp);
                return (size_t)0L;      /* Can't read? */
            }
            fclose(fp);
            return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);

        #else
            /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
            return (size_t)0L;          /* Unsupported. */
        #endif
    }
    /************************************************************/

    #define READ   0
    #define WRITE  1
    FILE * popen2(std::string command, std::string type, int & pid)
    {
        pid_t child_pid;
        int fd[2];
        pipe(fd);

        if((child_pid = fork()) == -1)
        {
            perror("fork");
            exit(1);
        }

        /* child process */
        if (child_pid == 0)
        {
            if (type == "r")
            {
                close(fd[READ]);    //Close the READ end of the pipe since the child's fd is write-only
                dup2(fd[WRITE], 1); //Redirect stdout to pipe
            }
            else
            {
                close(fd[WRITE]);    //Close the WRITE end of the pipe since the child's fd is read-only
                dup2(fd[READ], 0);   //Redirect stdin to pipe
            }

            execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
            exit(0);
        }
        else
        {
            if (type == "r")
            {
                close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
            }
            else
            {
                close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
            }
        }

        pid = child_pid;

        if (type == "r")
        {
            return fdopen(fd[READ], "r");
        }

        return fdopen(fd[WRITE], "w");
    }
}
