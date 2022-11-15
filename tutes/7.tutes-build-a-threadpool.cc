/*
 * @Date: 2022-11-15 09:57:53
 * @LastEditors: ThetisEliza wxf199601@gmail.com
 * @LastEditTime: 2022-11-15 16:38:02
 * @FilePath: /gobang-epoll-server-tute.hub/tutes/7.tutes-build-a-threadpool.cc
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <syslog.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include <map>

const size_t NUM = 5;


class Task
{
public:
    Task() {};

    Task(int x, int y, char op):_x(x), _y(y), _op(op) {
        
    };

    void Run() {
        int ret = 0;
        printf("waiting for calculation\n");
        sleep(6);
        switch(_op) {
            case '+': ret = _x + _y; break;
            case '-': ret = _x - _y; break;
            case '*': ret = _x * _y; break;
            case '/': ret = _x / _y; break;
            default: printf("operation err\n"); break;
        }
        printf("Thread [%lu]: [ %3d %2c %3d ] = %3d\n", pthread_self(), _x, _op, _y, ret);
    }

private:
    int _x;
    int _y;
    char _op;
};


/**
 * @brief We initize TheadPool with a lock and cond,
* and create nums of threads, with task `Routine`, in which
* a loop waiting for unfinished task is created. In the loop,
* if there exists some task unfinished in the task queue, 
* it will be poped out to be execute. The task queue will be
* set to be locked before and after any task poping happens
* in each thread.
 * 
 * @tparam T 
 */
template<class T>
class ThreadPool
{
public:  
    ThreadPool(const size_t num=NUM)
        :_threadNum(num)
        ,_tids(num){

        pthread_mutex_init(&_lock, nullptr);
        pthread_cond_init(&_empty, nullptr);
        
        pthread_t id;
        for(size_t i=0; i<_threadNum; ++i) {
            pthread_create(&id, nullptr, Routine, this);
            _tids[i] = id;
            _tsts[id] = 0;
        }
    }

    ~ThreadPool() {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_empty);

        for(size_t i=0; i<_threadNum; ++i) {
            pthread_cancel(_tids[i]);
        }
    }

    bool Empty() {
        return _taskQueue.empty();
    }

    void Lock() {
        pthread_mutex_lock(&_lock);
    }

    void Unlock() {
        pthread_mutex_unlock(&_lock);
    }

    void Wait() {
        pthread_cond_wait(&_empty, &_lock);
    }

    void Wakeup() {
        pthread_cond_signal(&_empty);
    }

    void Push(const T& task) {
        Lock();
        _taskQueue.push(task);
        Unlock();
        Wakeup();
    }

    void Pop(T& task) {
        task = _taskQueue.front();
        _taskQueue.pop();
    }

    int TaskSize() {
        return _taskQueue.size();
    }

    int RunningThreadSize() {
        int i = 0;
        for(const auto& e: _tsts) {
            if (e.second == 1) {
                i++;
            }
        }
        return i;
    }

private:
    static void* Routine(void* arg) {
        pthread_detach(pthread_self());
        
        ThreadPool *argThis = (ThreadPool*)arg;
        while (1 > 0) {
            argThis->Lock();
            argThis->_tsts[pthread_self()] = 0;
            while(argThis->Empty()) {
                argThis->Wait();
            }
            T task;
            argThis->Pop(task);
            argThis->Unlock();
            argThis->_tsts[pthread_self()] = 1;
            task.Run();
            argThis->_tsts[pthread_self()] = 0;
        }
    }



private:
    std::queue<T> _taskQueue;
    size_t _threadNum;
    std::vector<pthread_t> _tids;
    std::map<pthread_t, int> _tsts;
    pthread_mutex_t _lock;
    pthread_cond_t  _empty;
};
    


/**
 * @description: 
 * @param {int} argc
 * @param {char*} argv
 * @return {*}
 */
int main(int argc, char* argv[])
{
    srand((size_t)time(nullptr));
    char opAll[5] = {'+', '-', '*', '/'};
    ThreadPool<Task> tp;
    while (1) {
        int x = rand() % 100 + 1;
        int y = rand() % 100 + 1;
        char op = opAll[rand() % 4];
        tp.Push(Task(x, y, op));
        sleep(1);
        printf("Current thread pool status: running thread:%d, remining tasks:%d\n", tp.RunningThreadSize(), tp.TaskSize());
    }
    return 0;
}