## 基于SkipList 的 KV 存储引擎

## 编译运行
```
mkdir build && cd build && cmake .. && make
```

## 代码文件
- src/test.cc 包含catch2的测试
- src/include/skiplist.h 包含跳表的实现
- src/include/key_value.h 包含自定义Key-Value

## 已完成
- [x] 跳表实现
- [x] catch2单元测试
- [x] 支持自定义类型和比较器
- [x] 自定义比较器只支持原生类型，若要支持其他类型可参考src/include/comparator.h 定制比较器

## TODO
- [] 并发及测试

## Acknowledgements
A large number of implements references [leveldb](https://github.com/google/leveldb)
