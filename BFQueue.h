#include <queue>
#include <mutex>
#include <atomic>
#include <vector>

template <typename T>
class BFQueue {

    public:
    std::vector<T> bfqueue;
    alignas(64) std::atomic<int> start{0};
    alignas(64) std::atomic<int> end{0};
    int capacity{};

    BFQueue(size_t initial_capacity) : bfqueue(initial_capacity), capacity(initial_capacity) {}

    void push(T t) noexcept {
        int next = end.load(std::memory_order_relaxed) % capacity;
        bfqueue[next] = std::move(t);
        end.store(end.load(std::memory_order_relaxed) + 1, std::memory_order_release);
    }

    void pop() noexcept {
        start.fetch_add(1, std::memory_order_release);
    }

    void emplace(T t) noexcept {
        push(std::move(t));
    }

    T* front() noexcept {
        int s = start.load(std::memory_order_relaxed);
        int e = end.load(std::memory_order_acquire);
        if(s == e) return nullptr;
        return &bfqueue[s];
    }
};
