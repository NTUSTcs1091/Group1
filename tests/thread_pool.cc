#include "include/thread_pool.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

TEST(ThreadPoolTest, Basic) {
  ThreadPool tp(5);
  EXPECT_EQ(tp.avaliableWorkerSize(), 5);
  EXPECT_EQ(tp.remainJobSize(), 0);
  tp.abort();
}

TEST(ThreadPoolTest, Concurrency) {
  std::mutex mutex;
  std::condition_variable cv;
  auto delay = [](int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
  ThreadPool tp(5);

  EXPECT_EQ(tp.avaliableWorkerSize(), 5);
  EXPECT_EQ(tp.remainJobSize(), 0);

  /* Add 3 tasks */
  for (int i = 0; i < 3; ++i) {
    tp.dispatch([&] {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock);
    });
  }
  delay(10);
  EXPECT_EQ(tp.avaliableWorkerSize(), 2);
  EXPECT_EQ(tp.remainJobSize(), 0);

  /* Add 5 tasks */
  for (int i = 0; i < 5; ++i) {
    tp.dispatch([&] {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock);
    });
  }
  delay(10);

  EXPECT_EQ(tp.avaliableWorkerSize(), 0);
  EXPECT_EQ(tp.remainJobSize(), 3);
  while (tp.avaliableWorkerSize() != 5) {
    cv.notify_all();
  }

  delay(10);
  EXPECT_GT(tp.avaliableWorkerSize(), 0);
  EXPECT_LT(tp.remainJobSize(), 3);
  tp.abort();
  cv.notify_all();
}

TEST(ThreadPoolTest, Scheduler) {
  std::mutex mutex;
  std::condition_variable cv1, cv2;
  int result = 0;
  auto delay = [](int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };
  ThreadPool tp(1);

  /* Fill up workers */
  tp.dispatch([&] {
    std::unique_lock<std::mutex> lock(mutex);
    cv1.wait(lock);
  });
  delay(25);

  tp.dispatch([&] {
    std::unique_lock<std::mutex> lock(mutex);
    cv2.wait(lock);
    result = 20;
  });

  tp.execute([&] {
    std::unique_lock<std::mutex> lock(mutex);
    cv2.wait(lock);
    result = 10;
  });

  cv1.notify_one();

  delay(25);
  cv2.notify_one();

  delay(10);
  /* execute method has higher priority  */
  EXPECT_EQ(result, 10);

  while (tp.remainJobSize() > 0 || tp.avaliableWorkerSize() < 1) {
    cv1.notify_one();
    cv2.notify_one();
  }

  tp.abort();
}
