#include <queue>
#include <mutex>
#include <cmath>
#include <atomic>
#include <vector>

template <typename T>
class BFQueue {

    public:
    std::vector<T> bfqueue;
    alignas(64) std::atomic<int> tail{0};
    alignas(64) std::atomic<int> head{0};
    int capacity{};

    BFQueue(size_t initial_capacity) : bfqueue(initial_capacity), capacity(initial_capacity) {}

    void push(T t) noexcept {
        int next = head.load(std::memory_order_relaxed) % capacity;
        bfqueue[next] = std::move(t);
        head.fetch_add(1, std::memory_order_release);
    }

    bool try_push(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    void pop() noexcept {
        int next = tail.load(std::memory_order_relaxed) % capacity;
        tail.fetch_add(1, std::memory_order_release);
    }

    void emplace(T t) noexcept {
        push(std::move(t));
    }

    bool try_emplace(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    T* front() noexcept {
        int t = tail.load(std::memory_order_relaxed) % capacity;
        int h = head.load(std::memory_order_acquire) % capacity;
        if(t == h) return nullptr;
        return &bfqueue[t];
    }

    int size() {
        int t = tail.load(std::memory_order_relaxed);
        int h = head.load(std::memory_order_acquire);
        return std::abs(t - h) % capacity;
    }

    bool empty() {
        int t = tail.load(std::memory_order_relaxed);
        int h = head.load(std::memory_order_acquire);
        if(t == h) return true;
        return false;
    }

    bool filled() {
        int t = tail.load(std::memory_order_relaxed);
        int h = head.load(std::memory_order_acquire);
        if(std::abs(t - h) == capacity) return true;
        return false;
    }
};
