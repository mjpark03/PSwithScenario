#include <unordered_map>
#include <unordered_set>

class LFUCache {
public:
  int capacity;
  std::unordered_map<int, int> map;
  std::unordered_map<int, int> countMap;
  std::unordered_map<int, std::unordered_set<int> > orderMap;

  LFUCache(int capacity) {
    this -> capacity = capacity;
  }

  int get(int key) {
    auto value = map.find(key);
    if(value == map.end()) {
      return 0;
    }

    return 0;
  }

  void put(int key, int value) {
    (void)key;
    (void)value;
  }
};

int main() {
  return 0;
}
