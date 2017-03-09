# 2. Write pseudo code about problem

## Define Requirements

* get(key)
  * return value (value > 0)
  * if the key does not exist, return -1
* put(key, value)
  * set(update) or insert value
  * if capacity is full, invalidate the least frequently used item
  * if two or more keys that have the same frequency, it should invalidate the least recently used key
* Complexity
  * O(1)

## Select Data Structure and Algorithm

* Map: store key and value
* Array: record frequency for each key
* Queue: insert key in the order that get method is invoked


