#ifndef SRC_INCLUDE_SKIPLIST_H
#define SRC_INCLUDE_SKIPLIST_H

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "arena.h"
#include "random.h"
#include "key_value.h"

SKIPLIST_START

TEMPLATE_KEY_CMP
class SkipList {
 private:
  struct Node;

 public:
  // Create a new SkipList object that will use "cmp" for comparing keys,
  // and will allocate memory using "*arena".  Objects allocated in the arena
  // must remain allocated for the lifetime of the skiplist object.
  explicit SkipList(Comparator cmp, Arena *arena);

  NON_COPYABLE(SkipList);

  SkipList & operator=(const SkipList &) = delete;

  // Insert key into the list.
  // REQUIRES: nothing that compares equal to key is currently in the list.
  void Insert(const Key &key);

  // Returns true iff an entry that compares equal to key is in the list.
  auto Contains(const Key &key) const noexcept -> bool;

  // Display
  void Display() noexcept;

  // Dump File
  void DumpFile(const std::string filename) noexcept;

  // Iteration over the contents of a skip list
  class Iterator {
   public:
    // Initialize an iterator over the specified list.
    // The returned iterator is not valid.
    explicit Iterator(const SkipList *list);

    // Returns true iff the iterator is positioned at a valid node.
    auto Valid() const noexcept -> bool;

    // Returns the key at the current position.
    // REQUIRES: Valid()
    auto key() const noexcept -> const Key &;

    // Advances to the next position.
    // REQUIRES: Valid()
    void Next();

    // Advances to the previous position.
    // REQUIRES: Valid()
    void Prev();

    // Advance to the first entry with a key >= target
    void Seek(const Key &target);

    void SeekToFirst();

    // Position at the first entry in list.
    // Final state of iterator is Valid() iff list is not empty.
    void SeekToLast();
    
   private:
    const SkipList *list_;
    Node *node_;
  };

 private:
  enum { kMaxHeight = 12 };

  inline auto GetMaxHeight() const noexcept -> int {
    return max_height_.load(std::memory_order_relaxed);
  }

  auto NewNode(const Key &key, int height) -> Node *;
  auto RandomHeight() -> int;
  auto Equal(const Key &a, const Key &b) const -> bool {
    // return a.GetKey() == b.GetKey();
    return (compare_(a, b) == 0);
  }

  // Return true if key is greater than the data stored in "n"
  auto KeyIsAfterNode(const Key &key, Node *n) const -> bool;
  
  // Return the earliest node that comes at or after key.
  // Return nullptr if there is no such node.
  //
  // If prev is non-null, fills prev[level] with pointer to previous
  // node at "level" for every level in [0..max_height_-1].
  auto FindGreaterOrEqual(const Key &key, Node **prev) const -> Node *;
  
  // Return the latest node with a key < key.
  // Return head_ if there is no such node.
  auto FindLessThan(const Key &key) const -> Node *;
  
  // Return the last node in the list.
  // Return head_ if list is empty.
  auto FindLast() const -> Node *;

  // Immutable after construction
  Comparator const compare_;
  Arena *const arena_;  // Arena used for allocations of nodes

  Node *const head_;
  
  // Modified only by Insert().  Read racily by readers, but stale
  // values are ok.
  std::atomic<int> max_height_;  // Height of the entire list
  
  // Read/written only by Insert().
  Random rnd_;

  // file operator
  std::ofstream file_writer_;
  std::ifstream file_reader_;
};

// Implementation details follow
TEMPLATE_KEY_CMP
struct SkipList<Key, Comparator>::Node {
  explicit Node(const Key &k) : key(k) {}

  Key const key;

  // Accessors/mutators for links.  Wrapped in methods so we can
  // add the appropriate barriers as necessary.
  auto Next(int n) -> Node * {
    assert(n >= 0);
    // Use an 'acquire load' so that we observe a fully initialized
    // version of the returned Node.
    return next_[n].load(std::memory_order_acquire);
  }

  void SetNext(int n, Node *x) {
    assert(n >= 0);
    // Use a 'release store' so that anybody who reads through this
    // pointer observes a fully initialized version of the inserted node.
    next_[n].store(x, std::memory_order_release);
  }

  // No-barrier variants that can be safely used in a few locations.
  auto NoBarrier_Next(int n) -> Node * {
    assert(n >= 0);
    return next_[n].load(std::memory_order_relaxed);
  }

  void NoBarrier_SetNext(int n, Node *x) {
    assert(n >= 0);
    next_[n].store(x, std::memory_order_relaxed);
  }

 private:
  // Array of length equal to the node height.  next_[0] is lowest level link.
  std::atomic<Node *> next_[1];
};

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::NewNode(const Key &key, int height) 
    -> typename SkipList<Key, Comparator>::Node * {
  char *const node_memory = arena_->AllocateAligned(sizeof(Node) + sizeof(std::atomic<Node *>) * (height - 1));
  return new (node_memory) Node(key);
}

TEMPLATE_KEY_CMP
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList *list) {
  list_ = list;
  node_ = nullptr;
}

TEMPLATE_KEY_CMP
inline auto SkipList<Key, Comparator>::Iterator::Valid() const noexcept -> bool {
  return node_ != nullptr;
}

TEMPLATE_KEY_CMP
inline auto SkipList<Key, Comparator>::Iterator::key() const noexcept -> const Key & {
  assert(Valid());
  return node_->key;
}

TEMPLATE_KEY_CMP
inline void SkipList<Key, Comparator>::Iterator::Next() {
  assert(Valid());
  node_ = node_->Next(0);
}

TEMPLATE_KEY_CMP
inline void SkipList<Key, Comparator>::Iterator::Prev() {
  // Instead of using explicit "prev" links, we just search for the
  // last node that falls before key.
  assert(Valid());
  node_ = list_->FindLessThan(node_->key);
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

TEMPLATE_KEY_CMP
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key &target) {
  node_ = list_->FindGreaterOrEqual(target, nullptr);
}

TEMPLATE_KEY_CMP
inline void SkipList<Key, Comparator>::Iterator::SeekToFirst() {
  node_ = list_->head_->Next(0);
}

TEMPLATE_KEY_CMP
inline void SkipList<Key, Comparator>::Iterator::SeekToLast() {
  node_ = list_->FindLast();
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::RandomHeight() -> int {
  // Increase height with probability 1 in kBranching
  static const unsigned int kBranching = 4;
  int height = 1;
  while (height < kMaxHeight && rnd_.OneIn(kBranching)) {
    height ++;
  }
  assert(height > 0);
  assert(height <= kMaxHeight);
  return height;
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::KeyIsAfterNode(const Key &key, Node *n) const -> bool {
  // null n is considered infinite
  return (n != nullptr) && (compare_(n->key, key) < 0);
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::FindGreaterOrEqual(const Key &key, Node **prev) const -> 
                                                  SkipList<Key, Comparator>::Node * {
  Node *x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    Node *next = x->Next(level);
    if (KeyIsAfterNode(key, next)) {
      // Keep searching in this list
      x = next;
    } else {
      if (prev != nullptr) prev[level] = x;
      if (level == 0) {
        return next;
      } else {
        // Switch to next list
        level --;
      }
    }
  }
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::FindLessThan(const Key &key) const -> SkipList<Key, Comparator>::Node * {
  Node *x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    assert(x == head_ || compare_(x->key, key) < 0);
    Node *next = x->Next(level);
    if (next == nullptr || compare_(next->key, key) >= 0) {
      if (level == 0) {
        return x;
      } else {
        // Switch to next list
        level --;
      }
    } else {
      x = next;
    }
  }
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::FindLast() const -> SkipList<Key, Comparator>::Node * {
  Node *x = head_;
  int level = GetMaxHeight() - 1;
  while (true) {
    Node *next = x->Next(level);
    if (next == nullptr) {
      if (level == 0) {
        return x;
      } else {
        // Switch to next list
        level --;
      }
    } else {
      x = next;
    }
  }
}

TEMPLATE_KEY_CMP
SkipList<Key, Comparator>::SkipList(Comparator cmp, Arena *arena)
                                    : compare_(cmp),
                                      arena_(arena),
                                      head_(NewNode(Key(), kMaxHeight)),
                                      max_height_(1),
                                      rnd_(0xdeadbeef) {
  for (int i = 0; i < kMaxHeight; i ++) {
    head_->SetNext(i, nullptr);
  }

}

TEMPLATE_KEY_CMP
void SkipList<Key, Comparator>::Insert(const Key &key) {
  // TODO(opt): We can use a barrier-free variant of FindGreaterOrEqual()
  // here since Insert() is externally synchronized.
  Node *prev[kMaxHeight];
  Node *x = FindGreaterOrEqual(key, prev);

  // Our data structure does not allow duplicate insertion
  assert(x == nullptr || !Equal(key, x->key));

  int height = RandomHeight();
  if (height > GetMaxHeight()) {
    for (int i = GetMaxHeight(); i < height; i ++) {
      prev[i] = head_;
    }
    // It is ok to mutate max_height_ without any synchronization
    // with concurrent readers.  A concurrent reader that observes
    // the new value of max_height_ will see either the old value of
    // new level pointers from head_ (nullptr), or a new value set in
    // the loop below.  In the former case the reader will
    // immediately drop to the next level since nullptr sorts after all
    // keys.  In the latter case the reader will use the new node.
    max_height_.store(height, std::memory_order_relaxed);
  }

  x = NewNode(key, height);
  for (int i = 0 ;i < height; i ++) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "x" in prev[i].
    x->NoBarrier_SetNext(i, prev[i]->NoBarrier_Next(i));
    prev[i]->SetNext(i, x);
  }
}

TEMPLATE_KEY_CMP
auto SkipList<Key, Comparator>::Contains(const Key &key) const noexcept -> bool {
  Node *x = FindGreaterOrEqual(key, nullptr);
  if (x != nullptr && Equal(key, x->key)) {
    return true;
  } else {
    return false;
  }
}

TEMPLATE_KEY_CMP
void SkipList<Key, Comparator>::Display() noexcept {
  std::cout << "SkipList Display:" << std::endl;
  Node *x = head_;
  for (int i = 0; i < max_height_; i ++) {
    std::cout << "Level " << i << ": ";
    Node* next = x->Next(i);
    while (next != nullptr) {
      std::cout << next->key.GetKey() << ": " << next->key.GetValue() << ";" << '\t';
      next = next->Next(i);
    }
    std::cout << std::endl;
  }
}

TEMPLATE_KEY_CMP
void SkipList<Key, Comparator>::DumpFile(const std::string filename) noexcept {
  std::cout << "dump_file-----------------" << std::endl;
  file_writer_.open(filename);
  Node *x = head_->Next(0);

  while (x != nullptr) {
    file_writer_ << x->key.GetKey() << ": " << x->key.GetValue() << "\n";
    x = x->Next(0);
  }

  file_writer_.flush();
  file_writer_.close();

  std::cout << "dump finished" << std::endl;
}

SKIPLIST_END

#endif