#include "../include/lfu_cache.h"

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

bool LFUCache::doesMapHaveKey(int key, MAP_CAT cat) {
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
	if(map_value[node->next->key].count > count+1) return false;

	return true;
}

bool LFUCache::isNodeEqualMapNode(KeyNode *node, int count) {
	return map_node[count] == node;
}

bool LFUCache::isCountEqualMapValueCount(int key, int count) {
	return map_value[key].count == count;
}

bool LFUCache::isMapValueFull() {
    return map_value.size() == capacity;
}

bool LFUCache::isKeyEqualMapNodeKey(int count, int key) {
    return map_node[count]->key == key;
}

string LFUCache::get(int key) {
	if(key < 0) return NULL;
	if(capacity == 0) return NULL;
	if(!doesMapHaveKey(key, MAP_CAT_VALUE)) return NULL;

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

	if(!doesMapHaveKey(key_count+1, MAP_CAT_NODE)) {
		if(doesMapHaveKey(key_count, MAP_CAT_NODE)) {
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
		map_node.insert(make_pair(key_count+1, key_node));
	} else {
		dll.add_next(map_node[key_count+1], key_node);
		map_node[key_count+1] = key_node;
	}

	map_value[key].count++;

	return map_value[key].value;
}

void LFUCache::put(int key, string value) {
	if(capacity == 0) return;
	if(doesMapHaveKey(key, MAP_CAT_VALUE)) {
		map_value[key].value = value;
		get(key);
		return;
	}

    if(isMapValueFull()) {
		int lfu_node_key = lfuNode->key;
		int lfu_node_count = map_value[lfu_node_key].count;

        if(doesMapHaveKey(lfu_node_count, MAP_CAT_NODE)
            && isKeyEqualMapNodeKey(lfu_node_count, lfu_node_key)) {
			map_node.erase(lfu_node_count);
		}
		map_value.erase(lfu_node_key);
		KeyNode *temp_lfu_node = lfuNode;
		lfuNode = lfuNode->next;
		dll.distract(temp_lfu_node);
		delete(temp_lfu_node);
	}

	KeyNode *n = new KeyNode(key);
	map_value.insert(make_pair(key, CacheValue(value, n)));

    if(!doesMapHaveKey(1, MAP_CAT_NODE)) {
        if(lfuNode)
            dll.add_before(lfuNode, n);
        lfuNode = n;
        map_node.insert(make_pair(1, n));
    } else {
        dll.add_next(map_node[1], n);
        map_node[1] = n;
    }

    if(map_value[lfuNode->key].count > 1) {
        lfuNode = map_node[1];
    }
}
