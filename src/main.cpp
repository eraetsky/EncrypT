#include <iostream>
#include "ChatMessage.hpp"

void operatorsExample()
{
   ChatMessage m1(100), m2, m3;
   char array[100] = "Hello, let's run some tests";
   m1.set_data(array, 100);
   m2 = m1;
   std::cout << m2 << "\n\n---------------\n";

   if(m1 == m2) 
   {
      std::cout<< "Objects are equal\n\n---------------\n";
   }
   else
   {
      std::cout<< "Objects are not equal\n\n---------------\n";
   }

   m3 = m1 + m2;
   std::cout << m3 << "\n\n---------------\n";

   std::cin >> m3;
   std:: cout << m3.body_length() << "\n";
   ++m3;
   std::cout << m3.body_length();
}

int main(int argc, char *argv[]) 
{
   std::cout << "Hello, EncrypT!\n";
   std::cout << "EncrypT project started!\n";
   std::cout << "Lets go!\n";
}