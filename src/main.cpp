#include <iostream>
#include "Serializer.hpp"
#include "ClientApp.hpp"

void virtualMethodsTest()
{
   asio::io_context io_context;
   tcp::endpoint endpoint;
   Client *c1_ptr = new Client(io_context, "user1", "qwerty");
   Client *c2_ptr = new ClientApp("sfbgnd-62673-sfg", endpoint);

   auto ep1 = c1_ptr->getEndpoint();
   auto ep2 = c2_ptr->getEndpoint();

   std::string c1_userId = c1_ptr->getUserId();
   std::string c2_userId = c2_ptr->getUserId();
   std::cout << c1_userId << " " << c2_userId << std::endl;
}

void serializerTest()
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

int main(int argc, char *argv[])
{
   virtualMethodsTest();
   serializerTest();
   return 0;
}