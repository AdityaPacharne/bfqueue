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

    BFQueue(int initial_capacity):
        bfqueue(next_power_of_two(initial_capacity)),
        capacity(next_power_of_two(initial_capacity))
    {}

    void push(T t) noexcept {
        int next = head.load(std::memory_order_relaxed) & (capacity - 1);
        bfqueue[next] = std::move(t);
        head.fetch_add(1, std::memory_order_release);
    }

    bool try_push(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    void pop() noexcept {
        int next = tail.load(std::memory_order_relaxed) & (capacity - 1);
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
        int t = tail.load(std::memory_order_relaxed) & (capacity - 1);
        int h = head.load(std::memory_order_acquire) & (capacity - 1);
        if(t == h) return nullptr;
        return &bfqueue[t];
    }

    int size() {
        int t = tail.load(std::memory_order_relaxed);
        int h = head.load(std::memory_order_acquire);
        return std::abs(t - h) & (capacity - 1);
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

    uint32_t next_power_of_two(uint32_t n) {
        return n == 0 ? 1 : 1 << (32 - __builtin_clz(n - 1));
    }
};
