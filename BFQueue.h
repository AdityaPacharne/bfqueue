#include <queue>
#include <mutex>

template <typename T>
class BFQueue {

    public:
    std::queue<T> bfqueue;
    std::mutex m;

    BFQueue(const size_t capacity) { }

    void push(T t) {
        std::lock_guard<std::mutex> lk(m);
        bfqueue.push(t);
    }

    void pop() {
        std::lock_guard<std::mutex> lk(m);
        bfqueue.pop();
    }

    void emplace(T t) {
        std::lock_guard<std::mutex> lk(m);
        bfqueue.emplace(std::move(t));
    }

    T* front() {
        std::lock_guard<std::mutex> lk(m);
        if(bfqueue.empty()) return nullptr;
        return &bfqueue.front();
    }
};
