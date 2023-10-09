#ifndef SRC_INCLUDE_COMPARATOR_H
#define SRC_INCLUDE_COMPARATOR_H

#include "utils.h"

template <typename Key>
class KeyComparator {
 public:
  auto operator()(const Key& a, const Key& b) const -> int {
    if (a.key_ < b.key_) {
      return -1;
    } else if (a.key_ > b.key_) {
      return + 1;
    } else {
      return 0;
    }
  }
};

#endif