#ifndef SRC_INCLUDE_KEY_TYPE_H
#define SRC_INCLUDE_KEY_TYPE_H

#include <string>
#include <list>
#include <type_traits>

#include "arena.h"
#include "utils.h"

SKIPLIST_START

template<typename KeyType, typename ValueType>
class KeyValue {
 public:

  KeyValue() {
    // if (std::is_same<KeyType, int>::value) {
    //   key_ = * new KeyType();
    // } else if (std::is_same<KeyType, double>::value) {
    //   key_  = * new KeyType();
    // } else if (std::is_same<KeyType, bool>::value) {
    //   key_  = * new KeyType();
    // } else if (std::is_same<KeyType, std::string>::value || 
    //           std::is_same<KeyType, char *>::value || std::is_same<ValueType, const char *>::value) {
    //     key_  = * new KeyType();
    // }

    // if (std::is_same<ValueType, int>::value) {
    //   value_ = * new ValueType();
    // } else if (std::is_same<ValueType, double>::value) {
    //   value_ = * new ValueType();
    // } else if (std::is_same<ValueType, bool>::value) {
    //   value_ = * new ValueType();
    // } else if (std::is_same<ValueType, std::string>::value || 
    //           std::is_same<ValueType, char *>::value || std::is_same<ValueType, const char *>::value) {
    //     value_ = * new ValueType();
    // }

    // key_ = * (new KeyType());
    // value_ = * (new ValueType());

    char * key_memory = new char[sizeof(KeyType)];
    char * value_memory = new char[sizeof(ValueType)];
    new (key_memory) KeyType();
    new (value_memory) ValueType();
  }
  KeyValue(const KeyType &key, const ValueType &value) : key_(key), value_(value) {}
  KeyValue(const KeyType &key) {
    // key_ = key;

    // if (std::is_same<ValueType, int>::value) {
    //   value_ = (int) key;
    // } else if (std::is_same<ValueType, double>::value) {
    //   value_ = (double) (key);
    // } else if (std::is_same<ValueType, bool>::value) {
    //   value_ = !!(key);
    // } else if (std::is_same<ValueType, std::string>::value || 
    //           std::is_same<ValueType, char *>::value || std::is_same<ValueType, const char *>::value) {
    //     value_ = std::to_string(key);
    // }

    // value_ = * (new ValueType());

    char * key_memory = new char[sizeof(KeyType)];
    char * value_memory = new char[sizeof(ValueType)];
    new (key_memory) KeyType(key);
    new (value_memory) ValueType();
  }
  KeyValue(const ValueType &value) {
    // value_ = value;

    // if (std::is_same<KeyType, int>::value) {
    //   key_  = (int) value;
    // } else if (std::is_same<KeyType, double>::value) {
    //   key_  = (double) (value);
    // } else if (std::is_same<KeyType, bool>::value) {
    //   key_  = !!(value);
    // } else if (std::is_same<KeyType, std::string>::value || 
    //           std::is_same<KeyType, char *>::value || std::is_same<KeyType, const char *>::value) {
    //     key_  = std::to_string(value);
    // }

    // key_ = * (new KeyType());

    char * key_memory = new char[sizeof(KeyType)];
    char * value_memory = new char[sizeof(ValueType)];
    &key_ =  new (key_memory) KeyType();
    &value_ = new (value_memory) ValueType();
  }

  ~KeyValue() {
    key_.~KeyType();
    value_.~ValueType();
  }

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