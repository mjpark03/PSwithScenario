#include <iostream>
#include <unordered_map>
#include <list>

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
	void add_before(KeyNode *std, KeyNode *newBefore) {
		newBefore->next = std;
		if(std->prev) {
			newBefore->prev = std->prev;
			newBefore->prev->next = newBefore;
		}

		std->prev = newBefore;
	}

	void add_next(KeyNode *std, KeyNode *newNext) {
		newNext->prev = std;
		if(std->next) {
			newNext->next = std->next;
			newNext->next->prev = newNext;
		}

		std->next = newNext;
	}

	void distract(KeyNode *std) {
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
};

class CacheValue {
public:
	int value;
	int count;
	KeyNode *node;

	CacheValue() {
		this->value = 0;
		this->count = 0;
		this->node = NULL;
	}

	CacheValue(int value, KeyNode *node) {
		this->value = value;
		this->count = 1;
		this->node = node;
	}
};

KeyNode *head;

class LFUCache {
private:
	unordered_map<int, CacheValue> map_value;
	unordered_map<int, KeyNode*> map_node;
	uint32_t capacity;
	Doubly_Llinked dll;
public:
	LFUCache(int capacity) {
		this->capacity = capacity;
		head = NULL;
	}

	int get(int key) {
		if(this->capacity == 0) return -1;
		if(map_value.find(key) == map_value.end()) {
			return -1;
		} else {
			int count = map_value[key].count;
			KeyNode *n = map_value[key].node;

			if(n == head) {
				if(n->next) {
					if(map_value[n->next->key].count <= count+1) {
						head = n->next;
					}
				}
			}

			if(map_node.find(count) != map_node.end() && map_node[count] == n) {
				if(n->prev) {
					map_node[count] = n->prev;
				} else {
					map_node.erase(count);
				}
			}

			KeyNode *temp_n_prev = NULL;
			KeyNode *temp_n_next = NULL;
			if(n) {
				temp_n_prev = n->prev;
				temp_n_next = n->next;
				dll.distract(n);
			}

			if(map_node.find(count+1) == map_node.end()) {
				if(map_node.find(count) != map_node.end() && map_node[count] != n) {
					dll.add_next(map_node[count], n);
				} else {
					if(temp_n_prev != NULL) {
						dll.add_next(temp_n_prev, n);
						if(map_value[temp_n_prev->key].count == count) {
							map_node[count] = temp_n_prev;
						} else {
							map_node.erase(count);
						}
					} else if(temp_n_next != NULL) {
						dll.add_before(temp_n_next, n);
						map_node.erase(count);
					}
				}
				map_node.insert(make_pair(count +1, n));
			} else {
				dll.add_next(map_node[count+1], n);
				map_node[count+1] = n;
			}

			map_value[key].count++;

			return map_value[key].value;
		}
	}

	void put(int key, int value) {
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
};

int main(void) {
    return 0;
}
