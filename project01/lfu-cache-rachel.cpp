#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>  
#include <string>  
#include <iterator>

using namespace std;  
using myset = std::unordered_set<int>;  

class LFUCache {
public:
  int capacity;
  int min;
  unordered_map<int, int> map;
  unordered_map<int, int> countMap;
  unordered_map<int, std::unordered_set<int> > orderMap; 

  LFUCache(int _capacity) {
    capacity = _capacity;
    min = 0;    
  }

  void printset(const myset& s) {  
    for (const auto& iter : s) {  
        cout << " [" << iter << "]\n";  
    }  
    cout << endl << "size() == " << s.size() << endl << endl;  
  }  
    
  int get(int key) {
    
    return key;        
  }  
    
  void put(int key, int value) {
    int size = map.size();

    if(size >= capacity) {
      
      auto orderObj = orderMap.find(min);
      unordered_set<int> removedSet = orderObj->second;

      auto it = removedSet.begin();
      int removedKey = *it;
      
      map.erase(removedKey);
      countMap.erase(removedKey);
      removedSet.erase(removedKey);
    } 

    map[key] = value;

    if(countMap.count(key) == 1) {
      countMap[key] = countMap[key] + 1;
    } else {
      countMap[key] = 1;
    }
    int count = countMap[key];

    
    if(orderMap.count(count) <= 0) {
      unordered_set<int> newSet;
      newSet.insert(key);
      orderMap[count] = newSet;
    } else {
      auto orderObj = orderMap.find(count);
      unordered_set<int> currentSet = orderObj->second;
      currentSet.insert(key);
    }

    unordered_set<int> prevSet;
    
    if(count > 1) {
      auto prevOrderObj = orderMap.find(count - 1);
      prevSet = prevOrderObj->second;
      prevSet.erase(key);
    }

    if(prevSet.size() <= 0 || min > 0) {
      min = count;
    }
  }
};

int main() {
  LFUCache cache(2);
  cache.put(1, 1);
  cache.put(2, 2);
  cache.put(3, 3);
  cache.put(4, 4);
  
  return 0;
}
