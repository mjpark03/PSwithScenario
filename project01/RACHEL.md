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

* HashMap 1: store key and value
  * key: Integer
  * value: Integer
* HashMap 2: record frequency for each key
  * key: Integer
  * value: Integer, the count of key
* HashMap 3: record key set for each frequency
  * key: Integer, the count
  * value: HashSet, key set

## 

