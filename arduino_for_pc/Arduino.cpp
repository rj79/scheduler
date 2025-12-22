#include "Arduino.h"

#include <string.h>

extern void setup(void);
extern void loop(void);

uint32_t __baseMillis__ = 0;

PcSerial Serial;

uint32_t __millis__()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t result = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return result;
}

void __setup__()
{
    __baseMillis__ = __millis__();
}

void delay(uint32_t millis)
{
    usleep(millis * 1000);
}

uint32_t millis()
{
    return __millis__() - __baseMillis__;
}

int main()
{
    __setup__();
    setup();
    while (true) {
        loop();
    }
    return 0;
}

String::~String()
{
    if (Text) {
        delete[] Text;
    }
}

String::String() :
    Text(nullptr),
    Length(0)
{
    // Empty
}

String::String(const String& other) :
    String()
{
    if (other.Text) {
        Text = new char[other.Length + 1];
        strncpy(Text, other.Text, other.Length);
        Length = other.Length;
    }
}

String::String(const char* text)
{
    Length = strlen(text);
    Text = new char[Length + 1];
    strncpy(Text, text, Length);
}

String::String(int32_t i)
{
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", i);
    Length = strlen(buffer);
    Text = new char[Length + 1];
    strncpy(Text, buffer, Length);
}

String::String(uint32_t i)
{
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%u", i);
    Length = strlen(buffer);
    Text = new char[Length + 1];
    strncpy(Text, buffer, Length);
}

String::String(float f)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%f", f);
    Length = strlen(buffer);
    Text = new char[Length + 1];
    strncpy(Text, buffer, Length);
}

String& String::operator=(const String& other)
{
    if (this == &other) {
        return *this;
    }

    if (Length != other.Length) {
        auto temp = new char[Length + 1];
        delete[] Text;
        Text = temp;
        Length = other.Length;
    }
    strncpy(Text, other.Text, Length);
    return *this;
}

uint32_t String::length() const
{
    return Length;
}

const char* String::c_str() const
{
    return Text;
}
