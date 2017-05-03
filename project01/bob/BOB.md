
# Requirement

+ get(key)
  + get the value(always positive)
  + if no exist return -1
+ put(key, value)
  + set or insert the value
  + if reach its capacity, invalidate the least frequently used item
  + if there is a tie of the least values, invalidate the least recently used item

# Design data structure

+ Hashmap1: Store key and class
  + key: int
  + class:
    + value: int
    + address(each linked list's address): int
+ Hashmap2: Store count and address
  + count: int
  + address(the most recent address of each count): linked list


# Example


<img src="https://cloud.githubusercontent.com/assets/16299919/23830796/5cfaeec8-0756-11e7-9371-95d54a287fab.PNG" width="400">


# Psuedo code

<img src="https://cloud.githubusercontent.com/assets/16299919/23939595/cc876956-09a4-11e7-9426-9bda312275b7.JPG" width="400">
<img src="https://cloud.githubusercontent.com/assets/16299919/23939596/ce02b5f6-09a4-11e7-800b-aa44335c9ffe.JPG" width="400">

# Troubleshooting

+ [sublime g++ version](https://codeconnect.wordpress.com/2015/10/11/building-c11-code-in-sublime-text-3/)
+ LeetCode only accept <unordered_map>, no <hash_map>
