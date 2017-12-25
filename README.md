# c-plus-plus-multithreading
multithreading in c++

There are 3 types of concurrecy models
1.Multithreading
2.Multi Processing
3.Async programming


1.Mutithreading
  Are within the same process but concurrently when the cpu schedules every in a very fast manner 
  Such that it gives an illusion that they are simultaneously running
  
  Communication between two or more threads is often faster because the communicate using shared memory
  Threads share the same heap but have their own stack.
 
  Its lightweight and the cpu does not have to provide alot of protection when interleaving threads.
  
  Its also hard to implement it correctly.

  In c++ new threads are created using thread object


2.MultiProcessing
  Are when two or more process are simultaneously running and communicating with each other

  usually using pipe,memory buffers and files.

  Communication is not as fast as threads 

  The cpu has to keep alot of bookeeping in order to make sure that two or more threads do not
  overlap or access each other memory storage
 
  The cpu has PCB (Process control block) where it stores all the proces's stack,heaps and register
  
  Everytime a process interleaves its added to the PCB and everytime a process is schedules again its picked from the PCB.

3. Async programming is similar to what we do when wash clothes. 

   We put them into them into the washing machine 

   we continue onto doing other work until we get an event from the washing machine when clother are done.
   
   Asyn programing is also similar based on event-loops and call-backs wherein events loops contains all the events which are to executed and once the events are exectured the event-callbacks are called.
  
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

We could have also used t.detach here but we shouldnt use detach here because there is no gaurantee that the thread's cout would live long// enough before the parent exits, hence it causes undefined behaviour thats why detach shouldnt be used in this case.


