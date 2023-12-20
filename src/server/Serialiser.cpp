#include "Serializer.hpp"

template <typename T>
std::string DataSerializer<T>::Serialize(const T &data)
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}

template <typename T>
T DataSerializer<T>::Deserialize(const std::string &serializedData)
{
    T data;
    std::stringstream ss(serializedData);
    ss >> data;
    return data;
}

template <typename T>
std::string DataSerializer<std::vector<T>>::Serialize(const std::vector<T> &data)
{
    std::stringstream ss;
    for (const auto &item : data)
    {
        ss << DataSerializer<T>::Serialize(item) << " ";
    }
    return ss.str();
}

template <typename T>
std::vector<T> DataSerializer<std::vector<T>>::Deserialize(const std::string &serializedData)
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

std::string DataSerializer<std::string>::Serialize(const std::string &data)
{
    return data;
}

std::string DataSerializer<std::string>::Deserialize(const std::string &serializedData)
{
    return serializedData;
}
