#include <iostream>
#include <unordered_map>

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

void Doubly_Llinked::add_before(KeyNode *std, KeyNode *newBefore) {
	if(!std || !newBefore) return;

	newBefore->next = std;
	if(std->prev) {
		newBefore->prev = std->prev;
		newBefore->prev->next = newBefore;
	}

	std->prev = newBefore;
}

void Doubly_Llinked::add_next(KeyNode *std, KeyNode *newNext) {
	if(!std || !newNext) return;

	newNext->prev = std;
	if(std->next) {
		newNext->next = std->next;
		newNext->next->prev = newNext;
	}

	std->next = newNext;
}

void Doubly_Llinked::distract(KeyNode *std) {
	if(!std) return;

	if(std->next && std->prev) {
		std->next->prev = std->prev;
		std->prev->next = std->next;
	}
	else if(std->next) {
		std->next->prev = NULL;
	} else if(std->prev) {
		std->prev->next = NULL;
	}

	std->prev = NULL;
	std->next = NULL;
}

class CacheValue {
public:
	int value;
	int count;
	KeyNode *node;

	CacheValue() {}
	CacheValue(int value, KeyNode *node) : value(value), node(node), count(1) {}
};

class LFUCache {
private:
	unordered_map<int, CacheValue> map_value;
	unordered_map<int, KeyNode*> map_node;
	Doubly_Llinked dll;
	KeyNode *lfuNode;
	int capacity;

	enum MAP_CAT{
		MAP_CAT_NODE,
		MAP_CAT_VALUE
	};

	bool doesMapHasKey(int key, MAP_CAT cat);
	bool isNodeEqualLfuNode(KeyNode *node);
	bool isLfuNodeCountLessThanNextNodeCount(KeyNode *node, int count);
	bool isNodeEqualMapNode(KeyNode *node, int count);
	bool isCountEqualMapValueCount(int key, int count);
public:
	LFUCache(int capacity) : capacity(capacity), lfuNode(NULL) {}
	int get(int key);
	void put(int key, int value);
};

bool LFUCache::doesMapHasKey(int key, MAP_CAT cat) {
	if(cat == MAP_CAT_NODE) {
		return map_node.find(key) != map_node.end();
	} else {
		return map_value.find(key) != map_value.end();
	}
}

bool LFUCache::isNodeEqualLfuNode(KeyNode *node) {
	if(!lfuNode) return false;
	if(node != lfuNode) return false;

	return true;
}

bool LFUCache::isLfuNodeCountLessThanNextNodeCount(KeyNode *node, int count) {
	if(!node->next) return false;
	if(map_value[node->next->key].count > count + 1) return false;

	return true;
}

bool LFUCache::isNodeEqualMapNode(KeyNode *node, int count) {
	return map_node[count] == node;
}

bool LFUCache::isCountEqualMapValueCount(int key, int count) {
	return map_value[key].count == count;
}

int LFUCache::get(int key) {
	if(key < 0) return -1;
	if(capacity == 0) return -1;
	if(!doesMapHasKey(key, MAP_CAT_VALUE)) return -1;

	int key_count = map_value[key].count;
	KeyNode *key_node = map_value[key].node;

	if(isNodeEqualLfuNode(key_node) && isLfuNodeCountLessThanNextNodeCount(key_node, key_count))
		lfuNode = key_node->next;
	if(isNodeEqualMapNode(key_node, key_count)) {
		if(key_node->prev) {
			map_node[key_count] = key_node->prev;
		} else {
			map_node.erase(key_count);
		}
	}


	KeyNode *temp_n_prev = NULL;
	KeyNode *temp_n_next = NULL;
	if(key_node) {
		temp_n_prev = key_node->prev;
		temp_n_next = key_node->next;
		dll.distract(key_node);
	}

	if(!doesMapHasKey(key_count + 1, MAP_CAT_NODE)) {
		if(doesMapHasKey(key_count, MAP_CAT_NODE)) {
			dll.add_next(map_node[key_count], key_node);
		} else if(temp_n_prev) {
			dll.add_next(temp_n_prev, key_node);
			if(isCountEqualMapValueCount(temp_n_prev->key, key_count)) {
				map_node[key_count] = temp_n_prev;
			} else {
				map_node.erase(key_count);
			}
		} else if(temp_n_next) {
			dll.add_before(temp_n_next, key_node);
			map_node.erase(key_count);
		}
		
		map_node.insert(make_pair(key_count + 1, key_node));
	} else {
		dll.add_next(map_node[key_count + 1], key_node);
		map_node[key_count + 1] = key_node;
	}

	map_value[key].count++;

	return map_value[key].value;
}

void LFUCache::put(int key, int value) {
	if(this->capacity == 0) return;
	if(map_value.find(key) != map_value.end()) {
		map_value[key].value = value;
		get(key);
		return;
	}

	if(map_value.size() == this->capacity) {
		int head_key = lfuNode->key;
		int head_count = map_value[head_key].count;

		if(map_node.find(head_count) != map_node.end() && map_node[head_count]->key == head_key) {
			map_node.erase(head_count);
		}
		map_value.erase(head_key);
		KeyNode *temp_head = lfuNode;
		lfuNode = lfuNode->next;
		dll.distract(temp_head);
		delete(temp_head);
	}

	KeyNode *n = new KeyNode(key);
	map_value.insert(make_pair(key, CacheValue(value, n)));

		if(map_node.find(1) == map_node.end()) { //Not found
			if(lfuNode)
				dll.add_before(lfuNode, n);
			lfuNode = n;
			map_node.insert(make_pair(1, n));
		} else { //Found
			dll.add_next(map_node[1], n);
			map_node[1] = n;
		}

		if(map_value[lfuNode->key].count > 1) {
			lfuNode = map_node[1];
		}
	}
