//
// Created by root on 17-5-19.
//

#ifndef INC_4OVER6_MODERN_MSG_QUEUE_H
#define INC_4OVER6_MODERN_MSG_QUEUE_H
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class MsgQueue {

public:
    MsgQueue() = default;
    MsgQueue(const MsgQueue&) = delete;
    MsgQueue &operator= (const MsgQueue&) = delete;

    void pop(T& elem);
    bool empty() const;

    void push(const T& elem);

    void push(T& elem);
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;

};

#endif //INC_4OVER6_MODERN_MSG_QUEUE_H
