#include <iostream>
#include <thread>

int main()
{
   std::thread t([] { std::cout<<"\n This is a thread";});
   return 0;
}
