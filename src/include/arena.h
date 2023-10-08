// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "utils.h"

SKIPLIST_START

class Arena {
 public:
  Arena();

  NON_COPYABLE(Arena)
  Arena& operator=(const Arena&) = delete;

  ~Arena();

  // Return a pointer to a newly allocated memory block of "bytes" bytes.
  auto Allocate(size_t bytes) -> char*;

  // Allocate memory with the normal alignment guarantees provided by malloc.
  auto AllocateAligned(size_t bytes) -> char*;

  // Returns an estimate of the total memory usage of data allocated
  // by the arena.
  auto MemoryUsage() const -> size_t {
    return memory_usage_.load(std::memory_order_relaxed);
  }

 private:
  auto AllocateFallback(size_t bytes) -> char*;
  auto AllocateNewBlock(size_t block_bytes) -> char*;

  // Allocation state
  char* alloc_ptr_;
  size_t alloc_bytes_remaining_;

  // Array of new[] allocated memory blocks
  std::vector<char*> blocks_;

  // Total memory usage of the arena.
  //
  // TODO(costan): This member is accessed via atomics, but the others are
  //               accessed without any locking. Is this OK?
  std::atomic<size_t> memory_usage_;
};

inline auto Arena::Allocate(size_t bytes) -> char* {
  // The semantics of what to return are a bit messy if we allow
  // 0-byte allocations, so we disallow them here (we don't need
  // them for our internal use).
  assert(bytes > 0);
  if (bytes <= alloc_bytes_remaining_) {
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
  }
  return AllocateFallback(bytes);
}

SKIPLIST_END  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_ARENA_H_
