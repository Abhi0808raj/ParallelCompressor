#include "ThreadSafeQueue.h"
#include <mutex>

void ThreadSafeQueue::push(QByteArray&& item) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(std::move(item));
    cv.notify_one();
}

QByteArray ThreadSafeQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return !queue.empty(); });
    QByteArray item = std::move(queue.front());
    queue.pop();
    return item;
}

bool ThreadSafeQueue::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return queue.empty();
}
