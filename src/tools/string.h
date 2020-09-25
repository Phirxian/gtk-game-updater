#pragma once

#include "os.h"

#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#define stringify(s) #s

namespace tools
{
    inline std::wstring char2wchar(const std::string &msg) noexcept
    {
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        return std::move(converter.from_bytes(std::move(msg)));
    }

    inline std::string wchar2char(const std::wstring &msg) noexcept
    {
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        return std::move(converter.to_bytes(std::move(msg)));
    }

    inline std::string split(const std::string &s)
    {
        int realsize = s.size()-1;
        for(; realsize>=0 && s[realsize] != 0; --realsize);
        return s.substr(0, realsize);
    }

    inline size_t number(const std::string &s, const char &fd) noexcept
    {
        size_t c = 0;
        for(size_t i = 0; i<s.size(); ++i)
            c += s[i] == fd;
        return c;
    }

    inline void remove(std::string &s, const size_t spk, const size_t size = 0) noexcept
    {
        s = s.substr(0,spk)+s.substr(spk+size, s.size());
    }

    inline size_t number(const std::string &s, const std::string &fd) noexcept
    {
        size_t count = 0, tmp = s.find(fd,0);
        while(tmp <= s.size())
        {
            tmp = s.find(fd, tmp+fd.size());
            count++;
        }
        return count;
    }

    inline void crypt(std::string &source, const std::string &code) noexcept
    {
        size_t j = 0;
        for(size_t i = 0; i<source.size(); ++i)
        {
            source[i] ^= code[j];
            j = (j+1) % code.size();
        }
    }

    inline void replace(std::string &source, const std::string &last, const std::string &by) noexcept
    {
        std::string tnew;
        long pos = 0, get = source.find(last,0);
        while(get <= source.size())
        {
            get = source.find(last,pos);
            if(get < 0) break;
            else
            {
                tnew += source.substr(pos, get-pos);
                tnew += by;
                pos = get+last.size();
            }
        }
        source = tnew;
    }

    inline void insert(std::string &source, const std::string &car, const size_t pos) noexcept
    {
        std::string end = source.substr(pos, source.size()-pos);
        source  = source.substr(0, pos);
        source += car;
        source += end;
    }

    inline void saveToFile(const std::string &file, const std::string &data, const bool isBinary = false) noexcept
    {
        std::ofstream stream;
        if(isBinary) stream.open(file.c_str(),std::ios::out | std::ios::binary);
        else stream.open(file.c_str(),std::ios::out);
        stream.write(data.c_str(),data.size());
        stream.close();
    }

    inline std::string loadFromFile(const std::string &file, const bool isBinary = false) noexcept
    {
        std::string o;
        std::ifstream stream;

            stream.open(file.c_str(), isBinary ?
                std::ios::in | std::ios::binary : std::ios::in);

            if(!stream.is_open())
            {
                std::cout << "can't open " << file.c_str() << std::endl;
                return o;
            }

            stream.seekg(0, std::ios::end);
            o.resize(stream.tellg());
            stream.seekg(0, std::ios::beg);
            stream.read(&o[0], o.size());

        stream.close();
        return o;
    }
}
