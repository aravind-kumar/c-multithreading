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

## Problem 2:

```c++

int main()
{
   std::thread t([] 
                   { std::cout<<"\n This is a thread";
                     throw std::logic_error("Simple execption");
                   });
   t.join();
   //t.detach();
   return 0;
}

```
In this case the thread throws an exeption or say the main throws an execption before the join.
What would be the bhevaiour of the program ?
We would have an dangalling thread whose bhevaiour is unknown.
In order to fix this we use somthing known as RAII.

RAII:

```c++

class RAII_thread 
{
   thread t;
   
   RAII_thread(thread givenThread) : t(std::move(givenThread))
   {
       if(!givenThread.joinable())
       {
         throw std::logic_error("Non joinable threads");
       }
   }
   ~RAII_thread()
   {
      t.join();
   }
 
   RAII_thread(RAII_thread&) = delete;

   RAII_thread& operator=(RAII_thread const &) = delete; 

};


int main()
{
    RAII_thread(std::thread([] { cout<<"\n This is a scoped thread";}););

}

```

This binds a thread to a local stack variable,Hence even if an execption is thrown its made sure that the threads are joined in the end.
RAII isnt somthing specific to threads.Often its also used with memory management. 
 
```c++

template <typedef T> 
class dummy_RAII
{
   T* heap_mem;
   dummy_RAII(T&* givenInput) : heap_mem(givenInput)
   {
 
   } 
   
   ~dummy_RAII()
   {
      delete heap_mem;
   }
};
  
int main()
{
   dummy_RAII temp(allocate_mem());
   temp->performOperation();
   return 0;
}

```
In this case as well the heap is bounded to a stack variable hence there's no need for explicit memory dellocation or handling of cases whrere an execption is thrown.
The scope of the stack variable will make sure that the variable is deallocated whenever it goes out of scope.


## Problem 3:

``` c++

struct Worker
{
  string workerName;

  public:

  Worker(string name) : workerName(name);

  operator()
  {
     cout<<"\n the name of the worker is "<<workerName;
  }

};


int main()
{
    cout<<"\n Start";
    thread t1 = thread(Worker("Aravind"));
    thread t2 = thread(Worker("Kumar"));
    thread t3 = thread(Worker("Is"));
    thread t4 = thread(Worker("Awesome"));
    thread t5 = thread(Worker("Hero"));
   
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join(); 
    cout<<"\n End";
}

```
Problem ? cout is not thread safe and hence it would have undefined bhevaiour.

In order to fix we need to add a mutex/lock to protect the critical section.

```c++

mutex coutMutex;

struct Worker
{
  string workerName;

  public:

  Worker(string name) : workerName(name);

  operator()
  {
     coutMutex.lock();
     cout<<"\n the name of the worker is "<<workerName;
     coutMutex.unlock();
  }

};

```

Problem2 ? What if the Critical section between a lock and unlock throws an execption ?

If the CS section between a lock and unlock throws an execption then the lock remains held and doesnt get unlocked.
There are 2 solutions to get out of this problem

1.use lock_guard instead of lock to protect the CS.
  
  the lock guard will ensure that the lock is unlocked after the lock.

2.Use RAII of lock which essentially is the implmentation of lock guard.



Problem 3: Singleton Pattern (Double checked locking pattern)

A singleton class is a class wherein there can be only object throught the complete system usually global.

``` c++

class singleton
{
   singleton* instance;
   public:
    static singleton* getInstance() 
    {
        if(!instance)
             instance = new singleton();
         return instance;
    }

};

```
The above code would work for cases wherein the above code is being invoked in single threaded instances Acutally the copy and assignments operators should be deleted for it work.

However it wouldnt work on multithreaded environments.
Why ? because if multiple threads invoke getInstance at the same time then each thread would get its own instance.


Solution 1:


``` c++
class singleton
{
   singleton* instance;
   static mutex m;
   public:
    static singleton* getInstance() 
    {
       {
         lock_guard<mutex>(m);
        if(!instance)
             instance = new singleton();
         return instance;
       }
    }

``` 
This would be the problem however we are taking a lock for every call of getInstance however we only need the lock for the first instance this would be a bottleneck in multithreaded scenario



C++ clean solution:

``` c++
class singleton {

      public singleton getInstance() {
          static singleton s;
          return &s; 
      }
};

```
