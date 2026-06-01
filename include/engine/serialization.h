#ifndef GEC_ENGINE_SERIALIZE_H
#define GEC_ENGINE_SERIALIZE_H 1

#include <iostream>
#include <string.h>

#pragma once
namespace GEC {
namespace Serial {
    template <typename T>
    void Write(std::ostream& out, const T& value)
    {
        out.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    inline void WriteString(std::ostream& out, const std::string& str)
    {
        size_t len = str.size();
        Write(out, len);
        out.write(str.data(), len);
    }

    template <typename T>
    void Read(std::istream& in, T& value)
    {
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
    }

    inline void ReadString(std::istream& in, std::string& str)
    {
        size_t len;
        Read(in, len);
        str.resize(len);
        in.read(&str[0], len);
    }
}
}

#endif