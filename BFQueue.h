#include <cstring>
#include <atomic>
#include <vector>
#include <memory>

template <typename T>
class BFQueue {

    std::allocator<T> alloc;

    public:

    alignas(64) std::atomic<size_t> tail{0};
    size_t head_cache{};
    alignas(64) std::atomic<size_t> head{0};
    size_t tail_cache{};

    alignas(64) size_t capacity{};
    alignas(64) T* bfqueue;

    BFQueue(size_t initial_capacity): capacity(next_power_of_two(initial_capacity)) {
        bfqueue = alloc.allocate(capacity);
        memset(bfqueue, 0, capacity);
    }

    ~BFQueue() {
        alloc.deallocate(bfqueue, capacity);
    }

    void push(T t) noexcept {
        emplace(std::move(t));
    }

    bool try_push(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    void pop() noexcept {
        if(front() != nullptr) {
            size_t t = tail.load(std::memory_order_relaxed);
            tail.store(t + 1, std::memory_order_release);
        }
    }

    void emplace(T &t) noexcept {
        if(!filled()) {
            size_t h = head.load(std::memory_order_relaxed);
            bfqueue[h & (capacity - 1)] = t;
            head.store(h + 1, std::memory_order_release);
        }
    }

    void emplace(T &&t) noexcept {
        if(!filled()) {
            size_t h = head.load(std::memory_order_relaxed);
            bfqueue[h & (capacity - 1)] = std::move(t);
            head.store(h + 1, std::memory_order_release);
        }
    }

    bool try_emplace(T t) noexcept {
        if(filled()) return false;
        push(std::move(t));
        return true;
    }

    T* front() noexcept {
        size_t t = tail.load(std::memory_order_relaxed);
        if(t == head_cache)
            head_cache = head.load(std::memory_order_acquire);
        return (t == head_cache) ? nullptr : &bfqueue[t & (capacity - 1)];
    }

    size_t size() {
        size_t t = tail.load(std::memory_order_acquire);
        size_t h = head.load(std::memory_order_acquire);
        return (h - t);
    }

    bool empty() {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        return (t == h);
    }

    bool filled() {
        size_t h = head.load(std::memory_order_relaxed);
        if(h - tail_cache == capacity)
            tail_cache = tail.load(std::memory_order_acquire);
        return (h - tail_cache == capacity);
    }

    size_t next_power_of_two(size_t n) {
        return n == 0 ? 1 : 1 << (32 - __builtin_clz(n - 1));
    }
};


