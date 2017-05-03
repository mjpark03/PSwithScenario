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

## Example

<img src="https://cloud.githubusercontent.com/assets/5056158/23822590/77d6d2f8-0692-11e7-990d-1988a85157b8.jpg" width="400">

## Plan Detail

1. Refactoring algorithm & Adding test code ( ~ 5/7)
2. Implementing chatting program using LFU cache w/ Scala ( ~ 5/16)
3. Applying CERT to chatting program ( ~ 5/24)

