#pragma once

#include <iostream>
#include <vector>

using namespace std;

class String
{
    size_t _length;
    char* _str;
public:
    String();
    String(const char* str);
    String(const String& second);
    String(String&& second);
    ~String();
    size_t Length() const;
    void Print() const;
    char& operator [](const size_t index);
    bool operator ==(const String& second);
    bool operator !=(const String& second);
    String& operator =(const String& second);
    String& operator +=(const String& second);
    friend String operator +(const String& first, const String& second);
};
