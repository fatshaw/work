#pragma once

#include <stdarg.h>

#define FIXED_STRING_DEFAULT_BUFFER_SIZE 128

template<size_t buffer_size = FIXED_STRING_DEFAULT_BUFFER_SIZE>
class FixedString
{
public:
    static const size_t max_length = buffer_size - 1;
    static const size_t npos = -1;

public:
    inline FixedString(void)
    {
        buffer_[0] = '\0';
        length_ = 0;
    };

    inline ~FixedString(void)
    {
        
    };

    inline FixedString(const FixedString& other)
    {
        memcpy(buffer_, other.data(), other.size());
        length_ = other.length();
    };

public:
    inline size_t capacity() const
    {
        return buffer_size;
    };

    inline size_t max_size() const 
    {
        return capacity();
    };

    inline size_t length() const
    {
        return length_;
    };

    inline size_t size() const
    {
        return length_ + 1;
    };

    inline bool empty() const
    {
        return (length_ == 0);
    };

    inline void clear() 
    {
        buffer_[0] = '\0';
        length_ = 0;
    };

    inline const char* c_str() const
    {
        return buffer_;
    };

    inline const char* data() const
    {
        return buffer_;
    };

    inline bool assign(const char* other)
    {
        size_t other_len = strlen(other);
        if (other_len > max_length)
            return false;

        strcpy(buffer_, other);
        length_ = other_len;
        return true;
    };

    inline bool assign(const char* other, size_t other_len)
    {
        if (other_len > max_length)
            return false;

        strncpy(buffer_, other, other_len);
        length_ = strlen(buffer_);
        return true;
    };

    inline int compare(const char* other) const
    {
        return strcmp(buffer_, other);
    };

    inline int compare(const char* other, size_t other_len) const
    {
        return strncmp(buffer_, other, other_len);
    };

    inline bool append(const char* other)
    {
        size_t other_len = strlen(other);
        size_t new_len = length_ + other_len;
        if (new_len > max_length)
            return false;

        strcat(buffer_, other);
        length_ = new_len;
        return true;
    };

    inline bool append(const char* other, size_t other_len)
    {
        size_t new_len = length_ + other_len;
        if (new_len > max_length)
            return false;

        strncat(buffer_, other, other_len);
        length_ = strlen(buffer_);
        return true;
    };

    inline bool push_back(char ch)
    {
        if (ch == '\0')
            return false;

        size_t new_len = length_ + 1;
        if (new_len > max_length)
            return false;

        buffer_[length_++] = ch;
        buffer_[length_] = '\0';
        return true;
    };

    inline bool pop()
    {
        if (length_ <= 0)
            return false;

        buffer_[--length_] = '\0';
        return true;
    };

    inline size_t find(char ch) const
    {
        for (size_t i=0; i<length_; i++) {
            if (buffer_[i] == ch)
                return i;
        }

        return npos;
    };

    inline bool format(const char* format_string, ...)
    {
        int new_len = 0;
        va_list arglist;
        va_start(arglist, format_string);
        new_len = vsnprintf(NULL, 0, format_string, arglist);
        va_end(arglist);
        if (new_len > max_length)
            return false;

        va_start(arglist, format_string);
        new_len = vsnprintf(buffer_, buffer_size, format_string, arglist);
        va_end(arglist);

        length_ = new_len;
        return true;
    };

    inline operator const char* () const
    {
        return c_str();
    };

    inline bool operator = (const char* other) 
    {
        return assign(other);
    };

    template<size_t other_buffer_size>
    inline bool operator = (const FixedString<other_buffer_size>& other)
    {
        if (this == &other)
            return true;

        return assign(other.c_str(), other.length());
    };

    inline bool operator == (const char* other) const
    {
        return (compare(other) == 0);
    };

    template<size_t other_buffer_size>
    inline bool operator == (const FixedString<other_buffer_size>& other) const 
    {
        return (compare(other.c_str(), other.length()) == 0);
    };

    inline bool operator < (const char* other) const
    {
        return (compare(other) < 0);
    };

    template<size_t other_buffer_size>
    inline bool operator < (const FixedString<other_buffer_size>& other) const 
    {
        return (compare(other.c_str(), other.length()) < 0);
    };

private:
    char buffer_[buffer_size];
    size_t length_;
};

template<>
class FixedString<0>
{
private:
    inline FixedString() {};
    inline ~FixedString() {};
    inline FixedString(const FixedString& other) {};
};
