#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <atomic>

template<typename T>
struct WaitingQueue
{
    WaitingQueue() :
        m_headPtr{new Node},
        m_tailPtr{m_headPtr.get()},
        m_stopped{false}
    {
        std::cout << "Queue ctor" << std::endl;
    }

    WaitingQueue(const WaitingQueue&) = delete;
    WaitingQueue(WaitingQueue &&) = delete;
    WaitingQueue& operator=(const WaitingQueue&) = delete;
    WaitingQueue& operator=(const WaitingQueue&&) = delete;

    bool pop(T &entry)
    {
        return popHead(entry);
    }

    bool tryPop(T &entry)
    {
        std::unique_ptr<Node> prevHead = tryPopHead(entry);
        return prevHead.get() != nullptr;
    }

    template<typename TT>
    void push(TT &&value)
    {
        std::unique_ptr<Node> node{new Node};
        {
            std::lock_guard<std::mutex> lock{m_tailMutex};
            if(m_stopped) return;
            m_tailPtr->value = std::forward<TT>(value);
            m_tailPtr->next = std::move(node);
            m_tailPtr = m_tailPtr->next.get();
        }
        m_conditional.notify_one();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock{m_headMutex};
        return m_headPtr.get() == tail();
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock1{m_headMutex};
        std::lock_guard<std::mutex> lock2{m_tailMutex};
        m_stopped = true;
        m_conditional.notify_one();
    }

private:
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
    };

    Node* tail()
    {
        std::lock_guard<std::mutex> lock{m_tailMutex};
        return m_tailPtr;
    }

    std::unique_ptr<Node> takeHeadUnsafe()
    {
        std::unique_ptr<Node> prevHead = std::move(m_headPtr);
        m_headPtr = std::move(prevHead->next);
        return prevHead;
    }

    std::unique_ptr<Node> tryPopHead(T &entry)
    {
        std::lock_guard<std::mutex> lock{m_headMutex};
        if(m_headPtr.get() == tail()) return{};
        entry = std::move(m_headPtr->value);
        return takeHeadUnsafe();
    }

    bool popHead(T &entry)
    {
        std::unique_lock<std::mutex> lock{m_headMutex};
        m_conditional.wait(lock, [&](){
            if(m_stopped) return true;
            return (m_headPtr.get() != tail());
        });
        
        if(m_stopped && m_headPtr.get() == tail())
        {
            return false;
        }

        entry = std::move(m_headPtr->value);
        takeHeadUnsafe();
        return true;
    }

    std::mutex m_headMutex;
    std::mutex m_tailMutex;
    std::condition_variable m_conditional;
    std::unique_ptr<Node> m_headPtr;
    Node *m_tailPtr;
    bool m_stopped;
};

// void work_producer(WaitingQueue<int> &queue, std::atomic<bool> &stopFlag)
// {
//     std::cout << "Production started" << std::endl;
//     for(int i = 0; i < 100; ++i)
//     {
//         std::cout << "produced: " << i << std::endl;
//         if(!stopFlag) queue.push(i);
//         else break;
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     std::cout << "Production finished" << std::endl;
// }

// void work_consumer(WaitingQueue<int> &queue)
// {
//     int value = 0;
//     std::cout << "Consume started" << std::endl;
//     while(queue.pop(value))
//     {
//         std::cout << "consumed: " << value << std::endl;
//     }
//     std::cout << "Consume finished" << std::endl;
// }

// int main()
// {
//     std::atomic<bool> stopFlag{false};
//     WaitingQueue<int> workQueue;

//     std::thread producerThread{work_producer, std::ref(workQueue), std::ref(stopFlag)};
//     std::thread consumerThread1{work_consumer, std::ref(workQueue)};
//     std::thread consumerThread2{work_consumer, std::ref(workQueue)};

//     std::this_thread::sleep_for(std::chrono::seconds(5));

//     stopFlag = true;
//     workQueue.stop();

//     producerThread.join();
//     consumerThread1.join();
//     consumerThread2.join();


//     return 0;
// }