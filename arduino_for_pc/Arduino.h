#ifndef ARDUINO_H
#define ARDUINO_H

/**
 * Implementation of selected Arduino functions and classes for PC.
 */

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>

void delay(uint32_t millis);
uint32_t millis();

class String
{
public:
    String();
    String(const String& other);
    String(const char* str);
    String(int32_t i);
    String(uint32_t i);
    String(float f);

    ~String();

    String& operator=(const String& other);

    String& operator+=(const String& rhs) 
    {
        auto tmp = new char[Length + rhs.Length + 1];
        strncpy(tmp, Text, Length);
        strncpy(tmp + Length, rhs.Text, rhs.Length);
        return *this;
    }

    friend String operator+(String lhs, const String& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    uint32_t length() const;
    const char* c_str() const;
private:
    char* Text;
    uint32_t Length;

};

struct PcSerial 
{
    void begin(uint32_t) {};
    template <typename T> void print(T x) { std::cout << x; };
    template <typename T> void println(T x) { std::cout << x << std::endl; };
};

extern PcSerial Serial;

#endif
