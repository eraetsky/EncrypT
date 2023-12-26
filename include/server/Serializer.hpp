#pragma once

#include <sstream>
#include <vector>

template <typename T>
class DataSerializer
{
public:
    static std::string Serialize(const T &data)
    {
        std::stringstream ss;
        ss << data;
        return ss.str();
    }

    static T Deserialize(const std::string &serializedData)
    {
        T data;
        std::stringstream ss(serializedData);
        ss >> data;
        return data;
    }
};

// Partial specialization for std::vector
template <typename T>
class DataSerializer<std::vector<T>>
{
public:
    static std::string Serialize(const std::vector<T> &data)
    {
        std::stringstream ss;
        for (const auto &item : data)
        {
            ss << DataSerializer<T>::Serialize(item) << " ";
        }
        return ss.str();
    }

    static std::vector<T> Deserialize(const std::string &serializedData)
    {
        std::vector<T> data;
        std::stringstream ss(serializedData);
        T item;
        while (ss >> item)
        {
            data.push_back(item);
        }
        return data;
    }
};

// Full specialization for std::string
template <>
class DataSerializer<std::string>
{
public:
    static std::string Serialize(const std::string &data)
    {
        return data;
    }

    static std::string Deserialize(const std::string &serializedData)
    {
        return serializedData;
    }
};