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

    unordered_set<int> initSet;
    orderMap[1] = initSet;
  }

  void printset(const myset& s) {  
    for (const auto& iter : s) {  
        cout << " [" << iter << "]\n";  
    }  
    cout << endl << "size() == " << s.size() << endl << endl;  
  }  
    
  int get(int key) {

    if(map.count(key) != 1)
      return -1;

    int count = countMap[key];
    countMap[key] = count + 1;

    auto prevOrderObj = orderMap.find(count);
    unordered_set<int> prevSet = prevOrderObj->second;
    prevSet.erase(key);

    if(count == min && prevSet.size() <= 0)
      min = min + 1;

    if(orderMap.count(count + 1) <= 0) {
      unordered_set<int> newSet;
      orderMap[count] = newSet;
    }

    auto currentOrderObj = orderMap.find(count + 1);
    unordered_set<int> currentSet = currentOrderObj->second;
    currentSet.insert(key);
    
    return map.find(key)->second;        
  }  
    
  void put(int key, int value) {
    int size = map.size();

    if(map.count(key) == 1) {
      map[key] = value;
      get(key);
      return;
    }

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
    countMap[key] = 1;
    min = 1;
    auto newOrderObj = orderMap.find(min);
    unordered_set<int> set = newOrderObj->second;
    set.insert(key);
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
