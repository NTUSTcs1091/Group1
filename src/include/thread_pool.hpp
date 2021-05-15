/*
 * MIT License
 * Copyright (c) 2021 Yen Hao, Chen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _GROUP1_THREAD_POOL_H_
#define _GROUP1_THREAD_POOL_H_
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

/**
 * @brief
 * This class provides a thread pool, which can initialize several threads,
 * Use dispatch() or execute() to wake up the thread, which will dispatch the work itself.
 * can also use abort() to discard all existing work.
 * The members of this class will be synchronized through the mutex, so they are thread-safe
 *
 * @example use ThreadPool
 *
 * @code
 * void do_something(int a, int b) {
 *  return a + b;
 * }
 *
 * int main() {
 *   auto pool = std::make_unique<ThreadPool>(10);
 *   auto result1 = pool->dispatch(std::bind(do_something, 10, 20));
 *   auto result2 = pool->dispatch([] {
 *       std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *       std::cout << "OK!" << std::endl;
 *   });
 *   std::cout << "result 1 :" << result1.get() << std::endl; // output: "resut 1 :30"
 *   result2.get();
 *   return 0;
 * }
 */
class ThreadPool {
  using Task = std::function<void()>;

 public:
  ThreadPool(ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&) = delete;

  /**
   * Set available_ to the value of max(1, poolSize), and initialize processing_ to true.
   * Next, according to available_, create the same number of threads and push them to workers_.
   */
  explicit ThreadPool(uint16_t poolSize);

  /**
   * When the ThreadPool object is deconstructed, make sure that each thread executes join() function.
   */
  ~ThreadPool();

  /**
   * Encapsulate the parameter as a Smart pointer of type Task through std::bind,
   * because it is not sure when the task will be completed, the future is used to encapsulate the return value.
   * The smart pointer is used to ensure that when the job is executed, it has not been recycled by the system.
   * The encapsulated smart pointer will be "push" into jobs_, waiting for worker_ to take out and execute.
   * Jobs created using dispatch follow the FIFO mode.
   */
  template <typename Fn, typename... Args>
  auto dispatch(Fn&& func, Args&&... args) -> std::future<decltype(func(args...))>;

  /**
   * Its function is similar to dispatch(),
   * the difference is that dispatch uses emplace_back() to put the job,
   * Execute uses emplace_front() to put the job.
   * In other words, when a higher priority task is to be executed, you can use execute() to dispatch
   */
  template <typename Fn, typename... Args>
  auto execute(Fn&& func, Args&&... args) -> std::future<decltype(func(args...))>;

  /**
   * abort() will set the relevant condition_variable,
   * so that worker_ suspends taking out jobs, and clears the jobs queue.
   * This method will cause jobs that have not yet been executed to be abandoned.
   */
  void abort();

  /**
   * Return the number of jobs that have not been executed
   */
  size_t remainJobSize() const;

  /**
   * Get the number of idle workers
   */
  size_t avaliableWorkerSize() const;

 private:
  /**
   * In constructive execution, call this method.
   * If processing_ is true, Enter an infinite loop, each iteration will block the thread.
   * Only when cv_ executes notify, the thread is awakened, and a job is taken out for execution,
   * and available_ is decremented at the same time, and available_ is incremented when the task ends.
   */
  void initThread();

  /* Thread queue, when dispatch or execute call, will be woken up, take out the elements of jobs_ and execute */
  std::deque<std::thread> workers_;

  /* Work queue, when dispatch or execute call, put a Task */
  std::deque<Task> jobs_;

  /* Mutual exclusion lock, when the elements of jobs_ are put in or out, to ensure synchronization between threads */
  std::mutex mutex_;

  /**
   * Conditional variables,
   * dispatch() or execute() will execute notify_one(), and deconstructed will call notify_all(),
   * wait until notified, if the conditional `processing_ || !jobs_.empty()` is true Wake up thread;
   * Otherwise, continue to block the thread.
   */
  std::condition_variable cv_;

  /**
   * Only change when constructive, deconstructive or abort() is called.
   * dispatch() and execute() will check whether the variable is true before putting the job into jobs_
   * When abort() is called or deconstruct, it is changed to false, and it is forbidden to put new jobs into jobs_
   */
  std::atomic<bool> processing_;

  /**
   * When workers_ is scheduled, the value is decremented; after job execution is completed, the value is incremented.
   * If there is no job in jobs_, this value is equivalent to pool size
   */
  std::atomic<uint16_t> available_;
};
#endif  //_GROUP1_THREAD_POOL_H_
