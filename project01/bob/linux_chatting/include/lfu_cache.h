#ifndef __LFU_CACHE__
#define __LFU_CACHE__

#include <unordered_map>
#include <vector>

using namespace std;
using namespace __gnu_cxx;


class KeyNode {
public:
	int key;
	KeyNode *prev;
	KeyNode *next;

	KeyNode(int key) : key(key), prev(NULL), next(NULL) {}
};

class Doubly_Llinked {
public:
	void add_before(KeyNode *std, KeyNode *newBefore);
	void add_next(KeyNode *std, KeyNode *newNext);
	void distract(KeyNode *std);
};

class CacheValue {
public:
	vector<string> value;
	int count;
	KeyNode *node;

	CacheValue() {}
	CacheValue(vector<string> value, KeyNode *node) : value(value), count(1), node(node){}
};

class LFUCache {
private:
	unordered_map<int, CacheValue> map_value;
	unordered_map<int, KeyNode*> map_node;
	Doubly_Llinked dll;
	KeyNode *lfuNode;
	size_t capacity;

	enum MAP_CAT{
		MAP_CAT_NODE,
		MAP_CAT_VALUE
	};

	bool doesMapHaveKey(int key, MAP_CAT cat);
	bool isNodeEqualLfuNode(KeyNode *node);
	bool isLfuNodeCountLessThanNextNodeCount(KeyNode *node, int count);
	bool isNodeEqualMapNode(KeyNode *node, int count);
	bool isCountEqualMapValueCount(int key, int count);
    bool isMapValueFull();
    bool isKeyEqualMapNodeKey(int count, int key);
public:
	LFUCache(int capacity) : lfuNode(NULL), capacity(capacity) {}
	vector<string> get(int key);
	void put(int key, string value);
};



#endif
