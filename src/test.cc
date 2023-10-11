#include <cstdint>
// #define CATCH_CONFIG_MAIN

#include "include/catch.h"
#include "include/skiplist.h"
#include "include/key_value.h"
#include "include/comparator.h"


SKIPLIST_START

// typedef uint64_t Key;

// struct Comparator {
//   int operator()(const Key& a, const Key& b) const {
//     if (a < b) {
//       return -1;
//     } else if (a > b) {
//       return +1;
//     } else {
//       return 0;
//     }
//   }
// };

// TEST_CASE("Simple Test Case", "[SkipList]") {
//   Arena arena;
//   Comparator cmp;
//   SkipList<Key, Comparator> list(cmp, &arena);
//   REQUIRE(!list.Contains(10) == true);

//   SkipList<Key, Comparator>::Iterator iter(&list);
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToFirst();
//   REQUIRE_FALSE(iter.Valid());
//   iter.Seek(100);
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToLast();
//   REQUIRE_FALSE(iter.Valid());
// }

// TEST_CASE("Test", "[SkipList2]") {
//   const int N = 2000;
//   const int R = 5000;
//   Random rnd(1000);
//   std::set<Key> keys;
//   Arena arena;
//   Comparator cmp;
//   SkipList<Key, Comparator> list(cmp, &arena);
//   for (int i = 0; i < N; i++) {
//     Key key = rnd.Next() % R;
//     if (keys.insert(key).second) {
//       list.Insert(key);
//     }
//   }

//   for (int i = 0; i < R; i++) {
//     if (list.Contains(i)) {
//       REQUIRE(keys.count(i) == 1);
//     } else {
//       REQUIRE(keys.count(i) == 0);
//     }
//   }

//   // Simple iterator tests
//   {
//     SkipList<Key, Comparator>::Iterator iter(&list);
//     REQUIRE_FALSE(iter.Valid());

//     iter.Seek(0);
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.begin()) == iter.key());

//     iter.SeekToFirst();
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.begin()) == iter.key());

//     iter.SeekToLast();
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.rbegin()) == iter.key());
//   }

//   // Forward iteration test
//   for (int i = 0; i < R; i++) {
//     SkipList<Key, Comparator>::Iterator iter(&list);
//     iter.Seek(i);

//     // Compare against model iterator
//     std::set<Key>::iterator model_iter = keys.lower_bound(i);
//     for (int j = 0; j < 3; j++) {
//       if (model_iter == keys.end()) {
//         REQUIRE_FALSE(iter.Valid());
//         break;
//       } else {
//         REQUIRE_FALSE(!iter.Valid());
//         REQUIRE(*model_iter == iter.key());
//         ++model_iter;
//         iter.Next();
//       }
//     }
//   }

//   // Backward iteration test
//   {
//     SkipList<Key, Comparator>::Iterator iter(&list);
//     iter.SeekToLast();

//     // Compare against model iterator
//     for (std::set<Key>::reverse_iterator model_iter = keys.rbegin();
//          model_iter != keys.rend(); ++model_iter) {
//       REQUIRE_FALSE(!iter.Valid());
//       REQUIRE(*model_iter == iter.key());
//       iter.Prev();
//     }
//     REQUIRE_FALSE(iter.Valid());
//   }
// }

/**
=======================================================
测试KeyValue
=======================================================
**/

// typedef KeyValue<int, std::string> Key;

// TEST_CASE("Test KeyValue", "[SkipList KeyValue]") {
//   Arena arena;
//   KeyComparator<Key> cmp;
//   SkipList<Key, KeyComparator<Key>> list(cmp, &arena);
//   list.Insert(Key(10, "9999"));
//   REQUIRE(list.Contains(Key(10, "9999")) == true);

//   SkipList<Key, KeyComparator<Key>>::Iterator iter(&list);
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToFirst();
//   REQUIRE_FALSE(!iter.Valid());
//   iter.Seek(Key(100, "9999"));
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToLast();
//   REQUIRE_FALSE(!iter.Valid());
// }

// TEST_CASE("Simple Test Case", "[SkipList]") {
//   Arena arena;
//   KeyComparator<Key> cmp;
//   SkipList<Key, KeyComparator<Key>> list(cmp, &arena);
//   REQUIRE(!list.Contains(Key(10)) == true);

//   SkipList<Key, KeyComparator<Key>>::Iterator iter(&list);
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToFirst();
//   REQUIRE_FALSE(iter.Valid());
//   iter.Seek(Key(100));
//   REQUIRE_FALSE(iter.Valid());
//   iter.SeekToLast();
//   REQUIRE_FALSE(iter.Valid());
// }

// TEST_CASE("Test", "[SkipList2]") {
//   const int N = 2000;
//   const int R = 5000;
//   Random rnd(1000);
//   std::set<Key> keys;
//   Arena arena;
//   KeyComparator<Key> cmp;
//   SkipList<Key, KeyComparator<Key>> list(cmp, &arena);
//   for (int i = 0; i < N; i++) {
//     Key key (rnd.Next() % R, std::to_string(rnd.Next() % R));
//     if (keys.insert(key).second) {
//       list.Insert(key);
//     }
//   }

//   for (int i = 0; i < R; i++) {
//     if (list.Contains(i)) {
//       REQUIRE(keys.count(i) == 1);
//     } else {
//       REQUIRE(keys.count(i) == 0);
//     }
//   }

//   // Simple iterator tests
//   {
//     SkipList<Key, KeyComparator<Key>>::Iterator iter(&list);
//     REQUIRE_FALSE(iter.Valid());

//     iter.Seek(0);
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.begin()) == iter.key());

//     iter.SeekToFirst();
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.begin()) == iter.key());

//     iter.SeekToLast();
//     REQUIRE_FALSE(!iter.Valid());
//     REQUIRE(*(keys.rbegin()) == iter.key());
//   }

//   // Forward iteration test
//   for (int i = 0; i < R; i++) {
//     SkipList<Key, KeyComparator<Key>>::Iterator iter(&list);
//     iter.Seek(i);

//     // Compare against model iterator
//     std::set<Key>::iterator model_iter = keys.lower_bound(i);
//     for (int j = 0; j < 3; j++) {
//       if (model_iter == keys.end()) {
//         REQUIRE_FALSE(iter.Valid());
//         break;
//       } else {
//         REQUIRE_FALSE(!iter.Valid());
//         REQUIRE(*model_iter == iter.key());
//         ++model_iter;
//         iter.Next();
//       }
//     }
//   }

//   // Backward iteration test
//   {
//     SkipList<Key, KeyComparator<Key>>::Iterator iter(&list);
//     iter.SeekToLast();

//     // Compare against model iterator
//     for (std::set<Key>::reverse_iterator model_iter = keys.rbegin();
//          model_iter != keys.rend(); ++model_iter) {
//       REQUIRE_FALSE(!iter.Valid());
//       REQUIRE(*model_iter == iter.key());
//       iter.Prev();
//     }
//     REQUIRE_FALSE(iter.Valid());
//   }
// }

/**
====================================================
并发测试
====================================================
**/

SKIPLIST_END