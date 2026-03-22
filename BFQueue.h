#include <queue>
#include <mutex>

template <typename T>
class BFQueue {

    public:
    std::queue<T> bfqueue;
    std::mutex m;

    BFQueue(const size_t capacity) { }

    void push(T t) {
        bfqueue.push(t);
    }

    void pop() {
        bfqueue.pop();
    }

    void emplace(T t) {
        bfqueue.emplace(std::move(t));
    }

    T* front() {
        if(bfqueue.empty()) return nullptr;
        return &bfqueue.front();
    }
};
