#include <iostream>
#include "Serializer.hpp"

int main(int argc, char *argv[])
{
   int num = 42;
   std::string serializedNum = DataSerializer<int>::Serialize(num);
   int deserializedNum = DataSerializer<int>::Deserialize(serializedNum);
   std::cout << "Serialized num: " << serializedNum << std::endl;
   std::cout << "Deserialized num: " << deserializedNum << std::endl;

   std::vector<int> vec = {10, 20, 30, 40, 50};
   std::string serializedVec = DataSerializer<std::vector<int>>::Serialize(vec);
   std::vector<int> deserializedVec = DataSerializer<std::vector<int>>::Deserialize(serializedVec);
   std::cout << "Serialized vector: " << serializedVec << std::endl;
   std::cout << "Deserialized vector: ";
   for (const auto &item : deserializedVec)
   {
      std::cout << item << " ";
   }
   std::cout << std::endl;

   std::string text = "Hello, EncrypT!";
   std::string serializedText = DataSerializer<std::string>::Serialize(text);
   std::string deserializedText = DataSerializer<std::string>::Deserialize(serializedText);
   std::cout << "Serialized text: " << serializedText << std::endl;
   std::cout << "Deserialized text: " << deserializedText << std::endl;
}