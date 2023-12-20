#pragma once

#include <sstream>
#include <vector>

template <typename T>
class DataSerializer
{
public:
    static std::string Serialize(const T &data);
    static T Deserialize(const std::string &serializedData);
};

// Partial specialization for std::vector
template <typename T>
class DataSerializer<std::vector<T>>
{
public:
    static std::string Serialize(const std::vector<T> &data);
    static std::vector<T> Deserialize(const std::string &serializedData);
};

// Full specialization for std::string
template <>
class DataSerializer<std::string>
{
public:
    static std::string Serialize(const std::string &data);
    static std::string Deserialize(const std::string &serializedData);
};
