#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <time.h>

#include "include/comparator.h"
#include "include/skiplist.h"
using namespace skiplist;

#define NUM_THREADS 2
#define TEST_COUNT 100000

typedef KeyValue<int, std::string> Key;
Arena arena;
KeyComparator<Key> cmp;
SkipList<Key, KeyComparator<Key>> skipList(cmp, &arena);

void insertElement(int tid) {
  std::cout << tid << std::endl;  
  int tmp = TEST_COUNT / NUM_THREADS; 
  for (int i = tid * tmp, count = 0; count < tmp; i++) {
      count++;
      skipList.Insert(Key(rand() % TEST_COUNT, "a")); 
  }
}

void getElement(int tid) {
  std::cout << tid << std::endl;  
  int tmp = TEST_COUNT/NUM_THREADS; 
  for (int i = tid * tmp, count = 0; count < tmp; i++) {
      count++;
      skipList.Contains(Key(rand() % TEST_COUNT, "")); 
  }
}


int main() {
  srand(time(NULL));  
  {
    std::thread threads[NUM_THREADS];
    int i;

    // static_assert(2, "aaaa");

    auto start = std::chrono::high_resolution_clock::now();

    for (i = 0; i < NUM_THREADS; i++) {
        std::cout << "main() : creating thread, " << i << std::endl;
        threads[i] = std::thread(insertElement, i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    auto finish = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "insert elapsed:" << elapsed.count() << std::endl;
  }

  // 从这里开始的部分，您可以参考上述代码将其改写为使用线程来执行 getElement 函数
  {
    std::thread threads[NUM_THREADS];
    int i;

    auto start = std::chrono::high_resolution_clock::now();

    for (i = 0; i < NUM_THREADS; i++) {
        std::cout << "main() : creating thread, " << i << std::endl;
        threads[i] = std::thread(getElement, i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    auto finish = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "get elapsed:" << elapsed.count() << std::endl;
    }

    return 0;
}