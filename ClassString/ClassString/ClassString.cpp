#include "ClassString.h"

int main()
{
    String string_1("World!");
    String string_2;

    string_2 = "Hello ";

    string_2 += string_1;

    string_2.Print();
    cout << "\n\n";

    String string_3 = string_2;

    string_3.Print();
    cout << "\n\n";

    string_1 = "Hi ";
    string_2 = "there";
    string_3 = string_1 + string_2;
    string_3.Print();
    cout << "\n";

    return 0;
}

String::String()
{
    _length = 0;
    _str = nullptr;
}

String::String(const char* str)
{
    _length = strlen(str);
    _str = new char[_length + 1];

    for (size_t i = 0; i < _length; i++)
        _str[i] = str[i];
    _str[_length] = '\0';
}

String::String(const String& second)
{
    _length = second.Length();
    _str = new char[_length + 1];

    for (size_t i = 0; i < _length; i++)
        _str[i] = second._str[i];
    _str[_length] = '\0';
}

String::String(String&& second)
{
    _length = second._length;
    _str = second._str;
    second._str = nullptr;
}

String::~String()
{
    delete[] _str;
}

size_t String::Length() const
{
    return _length;
}

void String::Print() const
{
    printf(_str);
}

char& String::operator [](const size_t index)
{
    return _str[index];
}

bool String::operator ==(const String& second)
{
    if (_length != second._length)
        return false;

    for (size_t i = 0; i < _length; i++)
        if (_str[i] != second._str[i])
            return false;

    return true;
}

bool String::operator !=(const String& second)
{
    return !(operator ==(second));
}

String& String::operator =(const String& second)
{
     delete[] _str;

    _length = second.Length();
    _str = new char[_length + 1];

    for (size_t i = 0; i < _length; i++)
        _str[i] = second._str[i];
    _str[_length] = '\0';

    return *this;
}

String& String::operator +=(const String& second)
{
    char* resultStr = new char[_length + second.Length() + 1];

    size_t i = 0;
    for (; i < _length; i++)
        resultStr[i] = _str[i];

    size_t secondLength = second.Length();

    for (size_t j = 0; j < secondLength; j++, i++)
        resultStr[i] = second._str[j];

    _length += secondLength;
    resultStr[_length] = '\0';

    _str = resultStr;

    return *this;
}

String operator +(const String& first, const String& second)
{
    String newStr;

    size_t firstLength = first.Length();
    size_t secondLength = second.Length();
    char* resultStr = new char[firstLength + secondLength + 1];

    size_t i = 0;
    for (; i < firstLength; i++)
        resultStr[i] = first._str[i];

    for (size_t j = 0; j < secondLength; j++, i++)
        resultStr[i] = second._str[j];

    newStr._length = firstLength + secondLength;
    resultStr[newStr._length] = '\0';

    newStr._str = resultStr;

    return newStr;
}
