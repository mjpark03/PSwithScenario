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
	KeyNode *head;
	int capacity;
public:
	LFUCache(int capacity) : capacity(capacity), head(NULL) {}
	int get(int key);
	void put(int key, int value);
};

int LFUCache::get(int key) {
	if(key < 0) return -1;
	if(capacity == 0) return -1;
	if(map_value.find(key) == map_value.end()) return -1;

	int key_count = map_value[key].count;
	KeyNode *key_node = map_value[key].node;

	if(key_node == head) {
		if(key_node->next) {
			if(map_value[key_node->next->key].count <= key_count+1) {
				head = key_node->next;
			}
		}
	}

	if(map_node.find(key_count) != map_node.end() && map_node[key_count] == key_node) {
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

	if(map_node.find(key_count+1) == map_node.end()) {
		if(map_node.find(key_count) != map_node.end() && map_node[key_count] != key_node) {
			dll.add_next(map_node[key_count], key_node);
		} else {
			if(temp_n_prev != NULL) {
				dll.add_next(temp_n_prev, key_node);
				if(map_value[temp_n_prev->key].count == key_count) {
					map_node[key_count] = temp_n_prev;
				} else {
					map_node.erase(key_count);
				}
			} else if(temp_n_next != NULL) {
				dll.add_before(temp_n_next, key_node);
				map_node.erase(key_count);
			}
		}
		map_node.insert(make_pair(key_count +1, key_node));
	} else {
		dll.add_next(map_node[key_count+1], key_node);
		map_node[key_count+1] = key_node;
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
		int head_key = head->key;
		int head_count = map_value[head_key].count;

		if(map_node.find(head_count) != map_node.end() && map_node[head_count]->key == head_key) {
			map_node.erase(head_count);
		}
		map_value.erase(head_key);
		KeyNode *temp_head = head;
		head = head->next;
		dll.distract(temp_head);
		delete(temp_head);
	}

	KeyNode *n = new KeyNode(key);
	map_value.insert(make_pair(key, CacheValue(value, n)));

		if(map_node.find(1) == map_node.end()) { //Not found
			if(head)
				dll.add_before(head, n);
			head = n;
			map_node.insert(make_pair(1, n));
		} else { //Found
			dll.add_next(map_node[1], n);
			map_node[1] = n;
		}

		if(map_value[head->key].count > 1) {
			head = map_node[1];
		}
	}
