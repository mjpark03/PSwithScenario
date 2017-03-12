
#Requirement

+ get(key)
  + get the value(always positive)
  + if no exist return -1
+ put(key, value)
  + set or insert the value
  + if reach its capacity, invalidate the least frequently used item
  + if there is a tie of the least values, invalidate the least recently used item

#Design data structure
