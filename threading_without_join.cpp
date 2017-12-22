#include <iostream>
#include <thread>

int main()
{
   std::thread t([] { std::cout<<"\n This is a thread";});
   return 0;
}


// This would cause the program to abort because before the child thread terminates the parent is already dead
// There are two options 
// We should either join or detach a child thread.
// A Detach is used to run the child thread as a seperate deamon cases where the child and parent are independent.
// A join on the other hand is used to indicate that the parent must wait for the child process to exit.
// Failing to do so would cause in undefined bheaviour most probably a crash
