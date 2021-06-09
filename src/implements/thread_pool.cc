#include "include/thread_pool.hpp"
#include <iostream>

ThreadPool::ThreadPool(uint16_t poolSize) {
  auto max_concurrency = std::thread::hardware_concurrency();
  uint16_t size_ = poolSize > 0 ? poolSize : 1;
  const ushort THREAD_SCALE_LIMIT = 5;

  // Prevent generating too many threads
  if(size_ > max_concurrency * THREAD_SCALE_LIMIT)
    size_ = max_concurrency * THREAD_SCALE_LIMIT;
  available_.store(size_);
  processing_.store(true);
  for (int i = 0; i < size_; ++i) {
    workers_.emplace_back(std::bind(&ThreadPool::initThread, this));
  }
}

ThreadPool::~ThreadPool() {
  if(processing_.load()) {
    abort();
  }
}

void ThreadPool::initThread() {
  while (processing_.load()) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [this] {
        // if pool is running and exists pending jobs, stop blocking thread
        bool condition = !jobs_.empty() || !processing_.load();
        return condition;
      });
      if (!processing_.load() || jobs_.empty()) return;

      task = std::move(jobs_.front());
      jobs_.pop_front();
    }
    --available_;
    task();
    ++available_;
  }
}

void ThreadPool::abort() {
  processing_.store(false);
  cv_.notify_all();

  for(auto it = workers_.begin(); it != workers_.end(); ++it)
      if(it->joinable()) it->join();
  {
    std::lock_guard<std::mutex> lock(mutex_);
    while(!jobs_.empty())
        jobs_.pop_back();
  }
  
  available_.store(0);
}

size_t ThreadPool::remainJobSize() const { return jobs_.size(); }

size_t ThreadPool::avaliableWorkerSize() const { return available_.load(); }
