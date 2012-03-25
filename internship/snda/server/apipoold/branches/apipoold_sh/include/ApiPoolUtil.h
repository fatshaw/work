#pragma once

#include <new>

namespace ApiPool
{

template<typename T>
class DoBeforeReturn
{
public:
    DoBeforeReturn(const T& do_object) :
      do_object_(do_object),
      dispose_(false)
    {};

    ~DoBeforeReturn()
    {
        if (!dispose_)
            do_object_.Do();
    };

    void dispose()
    {
        dispose_ = true;
    };

private:
    DoBeforeReturn(const DoBeforeReturn&);
    void operator = (const DoBeforeReturn&);
    void* operator new(size_t);
    void operator delete(void*);

private:
    T do_object_;
    bool dispose_;
};

template<typename T0, typename T1>
class Assign
{
public:
    Assign(T0* dst, T1* src) :
      dst_(dst),
      src_(src)
    {};

    void Do()
    {
        if (dst_ != 0) {
            *dst_ = *src_;
        }
    };

private:
    T0* dst_;
    T1* src_;
};

template<typename T0, typename T1>
class AssignBeforeReturn : 
    public DoBeforeReturn<Assign<T0, T1> >
{
public:
    AssignBeforeReturn(T0* dst, T1* src) :
      DoBeforeReturn<Assign<T0, T1> >(Assign<T0, T1>(dst, src))
    {
    };
};

unsigned int GetCurrentTimeMillis();

class TimeOut
{
public:
    TimeOut(unsigned int time_out);

    ~TimeOut();

    bool isTimeOut();

    void setTimeOut(unsigned int time_out);

    unsigned int getTimeLeft();

private:
    unsigned int time_out_;
    unsigned int start_;
};

class Profiler
{
public:
    Profiler();

    ~Profiler();

public:
    void reset();

    unsigned int elapsed();

private:
    unsigned int start_;
};

}; //namespace ApiPool

