#include "Threadx.hpp"

namespace socketx{
    /********** Semaphore ************/
    semaphore::semaphore(ssize_t c){
        count = c;
    }

    void semaphore::P(){
        std::unique_lock<std::mutex> lk(mut);
        cv.wait(lk,[this]{return count>0;});
        --count;
    }

    void semaphore::V(){
        std::unique_lock<std::mutex> lk(mut);
        ++count;
        cv.notify_one();
    }
    
    /********* Thread pool ************/
    void ThreadPool::worker(){
        while(!done){
            std::function<void()> task;
            if(tasks.try_pop(task))
                task();
            else
                std::this_thread::yield();
        }
    }
    
    ThreadPool::ThreadPool(size_t num):thread_num(num),done(false){
        for(size_t i=0;i<num;++i){
            workers.push_back(std::thread(&ThreadPool::worker,this));
        }
    }

    void ThreadPool::addThread(){
       workers.push_back(std::thread(&ThreadPool::worker,this));
    }

    ThreadPool::~ThreadPool(){
        done = true;
        for(std::thread &x:workers)
            x.join();
    }

}

