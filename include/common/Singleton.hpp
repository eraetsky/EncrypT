#pragma once

template <typename T>
class Singleton
{
protected:
    Singleton() {}

public:
    static Singleton<T> &getInstance()
    {
        static Singleton<T> instance;
        return instance;
    }

public:
    Singleton(Singleton const &) = delete;
    Singleton &operator=(Singleton const &) = delete;
    void operator=(Singleton &&) = delete;
    Singleton(Singleton &&) = delete;
};
