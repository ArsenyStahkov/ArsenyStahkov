#pragma once

#include <iostream>
#include <vector>

using namespace std;

class String
{
    unsigned int _length;
    char* _str;
public:
    String();
    String(const char* str);
    String(const String& second);
    String(String&& second);
    ~String();
    unsigned int Length() const;
    void Print() const;
    char& operator [](const unsigned int index);
    bool operator ==(const String& second);
    bool operator !=(const String& second);
    String& operator =(const String& second);
    String& operator +=(const String& second);
    friend String operator +(const String& first, const String& second);
};
