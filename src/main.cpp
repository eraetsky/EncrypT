#include <iostream>
#include "Serializer.hpp"
#include "ClientApp.hpp"
#include "ChatMessage.hpp"

void operatorsTest()
{
   std::cout << "Performing operators overloading tests...\n";
   ChatMessage m1(100), m2, m3;
   char array[100] = "Hello, let's run some tests";
   m1.set_data(array, 100);
   m2 = m1;
   std::cout << m2 << "\n\n---------------\n";

   if (m1 == m2)
   {
      std::cout << "Objects are equal\n\n---------------\n";
   }
   else
   {
      std::cout << "Objects are not equal\n\n---------------\n";
   }

   m3 = m1 + m2;
   std::cout << m3 << "\n\n---------------\n";

   std::cin >> m3;
   std::cout << m3.body_length() << std::endl;
   ++m3;
   std::cout << m3.body_length() << std::endl;
   m2 = ChatMessage(5);

   std::cout << m2.data() << std::endl;

   std::cout << (char *)m3 << std::endl;

   auto m = m2(array, 100, 150);
   std::cout << m.data() << std::endl;

   std::cout << "Operators overloading: passed succesfully!\n";
}

void virtualMethodsTest()
{
   std::cout << "Performing dynamic polymorphism test...\n";
   asio::io_context io_context;
   tcp::endpoint endpoint;
   Client *c1_ptr = new Client(io_context, "user1", "qwerty");
   Client *c2_ptr = new ClientApp("sfbgnd-62673-sfg", endpoint);

   auto ep1 = c1_ptr->getEndpoint();
   auto ep2 = c2_ptr->getEndpoint();

   std::string c1_userId = c1_ptr->getUserId();
   std::string c2_userId = c2_ptr->getUserId();
   std::cout << c1_userId << " " << c2_userId << std::endl;
   std::cout << "Dynamic polymorphism: passed succesfully!\n";
}

void serializerTest()
{
   std::cout << "Performing template polymorphism test...\n";
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
   std::cout << "Template polymorphism: passed succesfully!\n";
}

int main(int argc, char *argv[])
{
   operatorsTest();
   virtualMethodsTest();
   serializerTest();
   return 0;
}