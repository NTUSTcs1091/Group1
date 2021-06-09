#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include "thread_pool.hpp"

TEST(ThreadPool, constructor) {
    ThreadPool tp1(3);
    EXPECT_EQ(3 , tp1.avaliableWorkerSize());
    EXPECT_EQ(0 , tp1.remainJobSize());

    ThreadPool tp2(0);
    EXPECT_EQ(1 , tp2.avaliableWorkerSize());
    EXPECT_EQ(0 , tp2.remainJobSize());
    tp1.abort();
    tp2.abort();

    EXPECT_EQ(0 , tp1.avaliableWorkerSize());
    EXPECT_EQ(0 , tp2.avaliableWorkerSize());
}

TEST(ThreadPool, scheduler) {
    ThreadPool tp(3);

    auto getNumber = []{
        return 10;
    };

    auto result1 = tp.dispatch(getNumber);
    auto result2 = tp.dispatch([]{
        return 1.5;
    });

    EXPECT_EQ(10, result1.get());
    EXPECT_FLOAT_EQ(1.5, result2.get());
}

TEST(ThreadPool, parallel) {
    ThreadPool tp(5);

    auto delayTime = std::chrono::milliseconds(200);
    auto delaySomeTime = [&delayTime]{
        std::this_thread::sleep_for(delayTime);
        return true;
    };
    EXPECT_EQ(0, tp.remainJobSize());

    auto res1 = tp.dispatch(delaySomeTime);
    EXPECT_EQ(4, tp.avaliableWorkerSize());
    
    auto res2 = tp.dispatch(delaySomeTime);
    EXPECT_EQ(3, tp.avaliableWorkerSize());

    auto res3 = tp.dispatch(delaySomeTime);
    EXPECT_EQ(2, tp.avaliableWorkerSize());

    res1.get();
    res2.get();
    res3.get();
    EXPECT_EQ(5, tp.avaliableWorkerSize());
}