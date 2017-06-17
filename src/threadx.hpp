#ifndef THREADX_HPP
#define THREADX_HPP

#include "utilx.hpp"

namespace socketx{

    /*Thread safe queue*/
    template<typename T>
    class squeue{
        private:
            std::queue<std::shared_ptr<T>> data_queue;
            std::condition_variable cond;
        public:
            squeue()=default;
            void wait_pop(T& value);
            bool try_pop(T& value);
            std::shared_ptr<T> wait_pop();
            std::shared_ptr<T> try_pop();
            void push<T value>;
            bool empty() const;
    };

    /*********Thread pool************/
    class thread_pool{
        private:
            std::vector<std::thread> workers;
            socketx::squeue<std::function<void()>> tasks;
            size_t thread_num;
            std::atomic_bool done;

            /*Get a task from tasks queue*/
            void worker();
        public:
            thread_pool(size_t num);
            virtual ~thread_pool();

            /*Submit a task f to tasks queue, then wait for the return value.*/
            template<typename F>
            auto submit<F f) -> std::future<typename std::result_of<F()>::type>;
    };

    void thread_pool::worker(){
        while(!done){
            std::function<void()> task;
            if(tasks.try_pop(task))
                task();
            else
                std::this_thread::yield();
        }
    }

    thread_pool::thread_pool(size_t num=std::thread::hardware_concurrency()):done(false){
        for(size_t i=0;i<num;++i){
            workers.push_back(std::thread(thread_pool::worker(),this));
        }
    }

    thread_pool::~thread_pool(){
        done = true;
        for(std::thread &x:workers)
            x.join();
    }

    template<typename F>
    auto thread_pool::submit(F f) -> std::future<typename std::result_of<F()>::type>{
        using return_type = typename std::result_of<F()>::type;

        /*Since packaged_task is non-copyable, we cannot pass it to std::function<>.
        * Instead, making a shared_ptr point to it and invoking it by a lambda function.
        */
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::move(f));
        std::function<void()> func = [task]{(*task)()};
        tasks.push(func);

        std::future<return_type> res = task->get_future();
        return res;
    }
}

#endif