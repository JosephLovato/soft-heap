#pragma once
#include <algorithm>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <vector>

#include "node.hpp"
#include "soft_heap.hpp"
#include "tree.hpp"

[[nodiscard]] auto generate_rand(int n) noexcept {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 1);  // 1,2,...,size-1
  std::shuffle(v.begin(), v.end(), std::mt19937(std::random_device()()));
  return v;
}

[[nodiscard]] auto rand_int(int start, int stop) noexcept {
  auto generator = std::mt19937(std::random_device()());
  auto dist = std::uniform_int_distribution<int>(start, stop);
  return dist(generator);
}

void test_STL(int size) {
  auto rand = generate_rand(size);
  auto min_queue = std::priority_queue<int>(rand.begin(), rand.end());
  for ([[maybe_unused]] auto&& x : rand) {
    min_queue.top();
    min_queue.pop();
  }
};

void test_Soft_Heap_Extract(int size, int eps) {
  auto rand = generate_rand(size);
  auto soft_heap = soft_heap::SoftHeap<int, std::list<int>>(
      rand.begin(), rand.end(), 1.0 / eps);
  for ([[maybe_unused]] auto&& x : rand) {
    soft_heap.ExtractMin();
  }
};

int main(int argc, char* argv[]) {
  if (argc == 2) {
    std::cout << "STL\n";
    test_STL(std::stoi(argv[1]));
  } else {
    std::cout << "Soft Heap\n";
    test_Soft_Heap_Extract(std::stoi(argv[1]), std::stoi(argv[2]));
  }

  return 0;
}