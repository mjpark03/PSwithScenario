/*
 * main.cpp
 *
 *  Created on: 2017. 3. 22.
 *      Author: taeyeopkim
 */




#include <iostream>
#include <ext/hash_map>

using namespace std;
using namespace __gnu_cxx;

class KeyNode {
public:
	int key;
	KeyNode *prev;
	KeyNode *next;

	KeyNode(int key) {
		this->key = key;
		this->next = NULL;
		this->prev = NULL;
	}
};

KeyNode *head;

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
		if(std->prev)
			std->prev->next = std->next;
		if(std->next)
			std->next->prev = std->prev;

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

class LFUCache {
private:
	hash_map<int, CacheValue> map_value;
	hash_map<int, KeyNode*> map_node;
	int capacity;
	Doubly_Llinked dll;
public:
    LFUCache(int capacity) {
    	this->capacity = capacity;
    }

    int get(int key) {
    	if(map_value.find(key) == map_value.end()) {
    		return -1;
    	} else {
    		int count = map_value[key].count;
    		KeyNode *n = map_value[key].node;

			if(map_node[count] == n) {
				if(n->prev) {
					map_node[count] = n->prev;
				} else {
					map_node.erase(count);
				}
			}
			dll.distract(n);

    		if(map_node.find(count+1) == map_node.end()) {
    		} else {
        		dll.add_next(map_node[count+1], n);
        		map_node[count+1] = n;
    		}

    		map_value[key].count++;
    		return map_value[key].value;
    	}
    }

    void put(int key, int value) {
    	if(map_value.size() == this->capacity) {
    		int head_key = head->key;
    		int head_count = map_value[head_key].count;

    		if(map_node[head_count]->key == head->key) {
    			map_node.erase(head_key);
    		}
    		map_value.erase(head_key);
    		head = head->next;
    		delete(head->prev);
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
    }
};

int main(void) {
	cout << "Start LFUCache!" << endl;

	LFUCache lfucache(3);

	lfucache.put(1, 1);
	lfucache.put(2, 2);
	lfucache.put(3, 3);

	cout << lfucache.get(1) << " " << lfucache.get(2) << " " << lfucache.get(3) << endl;

	return 0;
}

