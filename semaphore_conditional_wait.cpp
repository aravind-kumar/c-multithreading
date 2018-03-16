#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

bool read = false;
int current = 0;
int maxNum = 10;

std::mutex  mt;
std::condition_variable cv;

using namespace std;

void printThread(int currentThread,int maxThread) {

    {
	unique_lock<std::mutex> lg(mt);
        while(!read || currentThread != current) cv.wait(lg);
        
        std::cout<<"\n Now executing thread " <<currentThread;
        ++current;
        cv.notify_all(); 
    }
}


void run() {

  {
       unique_lock<std::mutex> lg(mt);
       read = true;
       cv.notify_all();
  }

}


int main() {

    std::thread th[10];
    for(int i=0;i<10;++i) 
	th[i] = std::thread(printThread,i,10);
     run();
     for(int i=0;i<10;++i)
        th[i].join();

    return 0;
}

