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
  KeyValue(const KeyType &key) {
    value_ = key_ = key;
  }
  KeyValue(const ValueType &value) {
    value_ = key_ = value;
  }

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

  auto operator==(const KeyValue &other) const noexcept -> bool {
    return this->key_ == other.key_;
  }

  auto operator<(const KeyValue &other) const noexcept -> bool {
    return this->key_ < other.key_;
  }

  auto operator>(const KeyValue &other) const noexcept -> bool {
    return this->key_ > other.key_;
  }

 public:
  auto GetKey() const noexcept -> KeyType {
    return key_;
  }

  auto GetValue() const noexcept -> ValueType {
    return value_;
  }

  auto SetKey(KeyType key) noexcept -> void {
    key_ = key;
  }

  auto SetValue(ValueType value) noexcept -> void {
    value_ = value;
  }
  
 private:
  KeyType key_;
  ValueType value_;
};

SKIPLIST_END

#endif