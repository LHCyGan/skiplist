## 基于SkipList 的 KV 存储引擎

## 编译运行
```
mkdir build && cd build && cmake .. && make
```

## 代码文件
- src/test.cc 包含catch2的测试
- src/include/skiplist.h 包含跳表的实现
- src/include/key_value.h 包含自定义Key-Value
- src/include/stress_test.h 插入和查找测试

# 存储引擎数据表现

## 插入操作

跳表树高：12 

采用随机插入数据测试：


|插入数据规模（万条） |耗时（秒） | 
|---|---|
|10 |0.0471331 |
|50 |0.4223 |
|100 |1.0615 |


## 取数据操作

|取数据规模（万条） |耗时（秒） | 
|---|---|
|10|0.0481944 |10|
|50|0.525651 |50|
|100|1.29762 |100|

## 已完成
- [x] 跳表实现
- [x] catch2单元测试
- [x] 支持自定义类型和比较器
- [x] 自定义比较器只支持原生类型，若要支持其他类型可参考src/include/comparator.h 定制比较器
- [x] 并发及测试

## Acknowledgements
A large number of implements references [leveldb](https://github.com/google/leveldb)
