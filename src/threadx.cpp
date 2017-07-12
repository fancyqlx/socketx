#include "threadx.hpp"

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
    void thread_pool::worker(){
        while(!done){
            std::function<void()> task;
            if(tasks.try_pop(task))
                task();
            else
                std::this_thread::yield();
        }
    }
    
    thread_pool::thread_pool(size_t num):thread_num(num),done(false){
        for(size_t i=0;i<num;++i){
            workers.push_back(std::thread(&thread_pool::worker,this));
        }
    }

    thread_pool::~thread_pool(){
        done = true;
        for(std::thread &x:workers)
            x.join();
    }

}

