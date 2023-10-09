#ifndef SRC_INCLUDE_KEY_TYPE_H
#define SRC_INCLUDE_KEY_TYPE_H

#include <string>
#include <list>

#include "utils.h"

SKIPLIST_START

template<typename KeyType, typename ValueType>
class KeyValue {
 public:

  KeyValue() : key_(0), value_(0) {}
  KeyValue(const KeyType &key, const ValueType &value) : key_(key), value_(value) {}

  ~KeyValue() = default;

  KeyValue(const KeyValue &other) {
    key_ = other.key_;
    value_ = other.value_;
  }
  KeyValue(const KeyValue &&other) {
    key_ = std::move(other.key_);
    value_ = std::move(other.value_);
  }

  auto operator=(const KeyValue &other) const noexcept -> KeyValue & {
    if (this != &other) {
      key_ = other.key_;
      value_ = other.value_;
    }

    return *this;
  }
  auto operator=(const KeyValue &&other) const noexcept -> KeyValue & {
    if (this != &other) {
      key_ = std::move(other.key_);
      value_ = std::move(other.value_);
    }

    return *this;
  }

 public:
  KeyType key_;
  ValueType value_;
};




SKIPLIST_END

#endif