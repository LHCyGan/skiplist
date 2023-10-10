#ifndef SRC_INCLUDE_UTILS_H
#define SRC_INCLUDE_UTILS_H

#define SKIPLIST_START namespace skiplist {    \

#define SKIPLIST_END }

#define NON_COPYABLE(class_name)                \
    class_name(const class_name & ) = delete;

#define NON_MOVEABLE(class_name)                \
    class_name(const class_name && ) = delete;

#define TEMPLATE_KEY_CMP template <typename Key, class Comparator>

#endif