#include <atomic>
#include <vector>

template <typename T>
class BFQueue {

    public:
    alignas(64) std::atomic<size_t> tail{0};
    alignas(64) std::atomic<size_t> head{0};
    alignas(64) size_t capacity{};
    alignas(64) std::vector<T> bfqueue;

    BFQueue(size_t initial_capacity):
        bfqueue(next_power_of_two(initial_capacity)),
        capacity(next_power_of_two(initial_capacity))
    {}

    void push(T t) noexcept {
        size_t next = head.load(std::memory_order_relaxed) & (capacity - 1);
        bfqueue[next] = std::move(t);
        head.fetch_add(1, std::memory_order_release);
    }

    bool try_push(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    void pop() noexcept {
        size_t next = tail.load(std::memory_order_relaxed) & (capacity - 1);
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
        size_t t = tail.load(std::memory_order_relaxed) & (capacity - 1);
        size_t h = head.load(std::memory_order_acquire) & (capacity - 1);
        if(t == h) return nullptr;
        return &bfqueue[t];
    }

    size_t size() {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        return (h - t) & (capacity - 1);
    }

    bool empty() {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        if(t == h) return true;
        return false;
    }

    bool filled() {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        if((h - t) == capacity) return true;
        return false;
    }

    size_t next_power_of_two(size_t n) {
        return n == 0 ? 1 : 1 << (32 - __builtin_clz(n - 1));
    }
};
