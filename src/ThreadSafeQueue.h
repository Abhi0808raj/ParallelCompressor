#pragma once
#include <QByteArray>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue {
    std::queue<QByteArray> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;

public:
    void push(QByteArray&& item);
    QByteArray pop();
    bool empty() const;
};
