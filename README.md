# c-plus-plus-multithreading
multithreading in c++

## Problem 1:

```c++

int main()
{
   std::thread t([] { std::cout<<"\n This is a thread";});
   return 0;
}

```
This would cause the program to abort because before the child thread terminates the parent is already dead

There are two options 

We should either join or detach a child thread.

A Detach is used to run the child thread as a seperate deamon cases where the child and parent are independent.

A join on the other hand is used to indicate that the parent must wait for the child process to exit.

Failing to do so would cause in undefined bheaviour most probably a crash


## Solution :

```c++

int main()
{
   std::thread t([] { std::cout<<"\n This is a thread";});
   t.join();
   //t.detach();
   return 0;
}


```

In the above program the child wouldnt crash because the parent thread would wait for the child to exit

Before exiting i.e. the t.join() hence the program wouldnt crash as seen on the case before


