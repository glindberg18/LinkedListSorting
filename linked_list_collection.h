/*
Greeley Lindberg
10/30/19, hw6
Description: Implementation of Collection using linked lists
*/

#ifndef LINKED_LIST_COLLECTION_H
#define LINKED_LIST_COLLECTION_H

#include <vector>
#include <algorithm>
#include "collection.h"


template <typename K, typename V>
class LinkedListCollection : public Collection<K,V> {
public:

	// create an empty linked list
	LinkedListCollection();

	// copy a linked list
	LinkedListCollection(const LinkedListCollection<K,V>& rhs);

	// assign a linked list
	LinkedListCollection<K,V>& operator =(const LinkedListCollection<K,V>& rhs);

	// delete a linked list
	~LinkedListCollection();

	// insert a key-value pair into the collection
	void insert(const K& key, const V& val);

	// remove a key-value pair from the collection
	void remove(const K& key);

	// find the value associated with the key
	bool find(const K& key, V& val) const;

	// find the keys associated with the range
	void find(const K& k1, const K& k2, std::vector<K>& keys) const;

	// return all keys in the collection
	void keys(std::vector<K>& keys) const;

	// return collection keys in sorted order
	void sort(std::vector<K>& keys) const;

	// return the number of keys in collection
	int size() const;

	// in place sorting
	void insertion_sort();
	void merge_sort();
	void quick_sort();

private:
	// linked list node structure
	struct Node {
		K key;
		V value;
		Node* next;
	};
	Node* head; 		// pointer to first list node
	Node* tail; 		// pointer to last list node
	int length; 		// number of linked list nodes in list

	// helper to delete linked list
	void make_empty();

	// merge sort helper (see course notes for details)
	Node* merge_sort(Node* left , int len);

	// quick sort helper (see course notes for details)
	Node* quick_sort(Node* start , int len);
};

template <typename K, typename V>
LinkedListCollection<K,V>::LinkedListCollection() : head(nullptr), tail(nullptr), length(0) {}

template <typename K, typename V>
LinkedListCollection<K,V>::LinkedListCollection(const LinkedListCollection<K,V>& rhs): head(nullptr), tail(nullptr), length(0) {
	Node* ptr = rhs.head;
	while (ptr != nullptr) {
		insert(ptr->key, ptr->value);
		ptr = ptr->next;
	}
	
}

template <typename K, typename V>
LinkedListCollection<K,V>& LinkedListCollection<K,V>::operator =(const LinkedListCollection<K,V>& rhs) {
	Node* ptr;
	Node* next_ptr;
	if (this == &rhs)
		return *this;
	ptr = head;
	while (ptr != nullptr) {
		next_ptr = ptr->next;
		remove(ptr->key);
		ptr = next_ptr;
	}
	ptr = rhs.head;
	while (ptr != nullptr) {
		insert(ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return *this; 
}

template <typename K, typename V>
LinkedListCollection<K,V>::~LinkedListCollection() {
	Node* ptr;
	Node* next_ptr;
	ptr = head;
	while (ptr != nullptr) {
		next_ptr = ptr->next;
		remove(ptr->key);
		ptr = next_ptr;
	}
}

template <typename K, typename V>
void LinkedListCollection<K,V>::insert(const K& key, const V& val) {
	Node* ptr = new Node;
	ptr->key = key;
	ptr->value = val;
	ptr->next = nullptr;
	if (!head) 
		head = ptr;
	else 
		tail->next = ptr;
	tail = ptr;
	length++;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::remove(const K& key) {
	Node* ptr;
	Node* previous;
	if (!head)
		return;
	else {
		if (head->key == key) {
			if (head==tail)
				tail = nullptr;
			ptr = head->next;
			delete head;
			head = ptr;
			length--;
		}
		else {
			ptr = head;
			while (ptr != nullptr && ptr->key != key) {
				previous = ptr;
				ptr = ptr->next;
			}
			if (ptr)
				previous->next = ptr->next;
				if (tail==ptr)
					tail = previous;
				delete ptr;
				ptr = nullptr;
				length--;
		}
	}
}

template <typename K, typename V>
bool LinkedListCollection<K,V>::find(const K& key, V& val) const {
	Node* ptr = head;
	while (ptr != nullptr) {
		if (ptr->key == key) {
			val = ptr->value;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::find(const K& k1, const K& k2, std::vector<K>& keys) const {
	keys.clear();
	Node* ptr = head;

	while (ptr) {
		if (ptr->key >= k1 && ptr->key<=k2)
			keys.push_back(ptr->key);
		ptr = ptr->next;
	}
}

template <typename K, typename V>
void LinkedListCollection<K,V>::keys(std::vector<K>& keys) const {
	keys.clear();
	Node* ptr = head;
	int i = 0;
	while (ptr != nullptr) {
		keys.push_back(ptr->key);
		ptr = ptr->next;
		i++;
	}
}

template <typename K, typename V>
void LinkedListCollection<K,V>::sort(std::vector <K>& keys) const {
	Node* ptr = new Node;
	ptr = head;
	while (ptr != nullptr) {
		keys.push_back(ptr->key);
		ptr = ptr->next;
	}
	std::sort(keys.begin(), keys.end());
}

template <typename K, typename V>
int LinkedListCollection<K,V>::size() const {
	return length;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::insertion_sort() {
	Node* curr_node; 					// traverses through the sorted region
	Node* curr_node_previous;
	Node* right;						// the current node being sorted
	Node* right_previous = head;		// right-most node of the sorted region
	Node* next_node = head->next;		// next node to be sorted

	if (!head || !head->next)
		return;
	while (next_node) {
		curr_node = head;
		right = next_node;
		next_node = right->next;
		if (right->key < head->key)
			head = right;
		else {
			while (curr_node->next && curr_node->key < right->key) {
				curr_node_previous = curr_node;
				curr_node = curr_node->next;
			}
			curr_node_previous->next = right;
			if (right_previous->key < right->key)
				right_previous = right;
		}
		right->next = curr_node;
		right_previous->next = next_node;
	}
	tail = right_previous;
}

template <typename K,typename V>
typename LinkedListCollection<K,V>::Node*
LinkedListCollection<K,V>::merge_sort(Node* left, int len) {
	Node* right = left;
	Node* right_previous = left;
	Node* sorted = nullptr;
	Node* sorted_tail;

	if (len == 1)
		return left;
	int mid = len/2;
	int i = 0;
	while (right && i < mid) {
		right_previous = right;
		right = right->next;
		i++;
	}
	right_previous->next = nullptr;

	left = merge_sort(left, mid);
	right = merge_sort(right, len - mid);

	if (left->key < right->key) {
		sorted = left;
		left = left->next;
	}
	else {
		sorted = right;
		right = right->next;
	}
	sorted_tail = sorted;
	sorted_tail->next = nullptr;
	
	while (left && right) {
		if (left->key < right->key) {
			sorted_tail->next = left;
			left = left->next;
		}
		else {
			sorted_tail->next = right;
			right = right->next;
		}
		sorted_tail = sorted_tail->next;
		sorted_tail->next = nullptr;
	}

	if (left)
		sorted_tail->next = left;
	else if (right)
		sorted_tail->next = right;
	return sorted;
}


template <typename K, typename V>
void LinkedListCollection<K,V>::merge_sort() {
	head = merge_sort(head, length);
	Node* curr_node = head;
	for (int i = 0; i < length; i++) {
		curr_node = curr_node->next;
	}
	tail = curr_node;
}

template <typename K, typename V>
typename LinkedListCollection<K,V>::Node*
LinkedListCollection<K,V>::quick_sort(Node* start, int len) {
	Node* curr_node;
	Node* left = nullptr;
	Node* left_tail = nullptr;
	Node* right = nullptr;
	Node* right_tail = nullptr;
	Node* next_node;
	int left_len = 0;
	int right_len = 0;

	static int j = 0;
	if (len <= 1) {
		return start;
	}
	curr_node = start->next;
	if (len == 2) {
		if (curr_node->key < start->key) {
			curr_node->next = start;
			start->next = nullptr;
			start = curr_node;
		}
		return start;
	}
	next_node = start->next;
	start->next = nullptr;

	while (next_node) {
		curr_node = next_node;
		next_node = curr_node->next;
		if (curr_node->key < start->key) {
			if (!left) 
				left = curr_node;
			else 
				left_tail->next = curr_node;
			left_tail = curr_node;
			left_tail->next = nullptr;
			left_len++;
		}
		else {
			if (!right) 
				right = curr_node;
			else 
				right_tail->next = curr_node;
			right_tail = curr_node;
			right_tail->next = nullptr;
			right_len++;
		}
	}
	left = quick_sort(left, left_len);
	right = quick_sort(right, right_len);
	Node* temp;
	if (left) {
		temp = left;
		while (temp->next){
			temp = temp->next;
		}
		temp->next = start;
		head = left;
	}
	else {
		head = start;
	}
	start->next = right;
	return head;
}

template <typename K, typename V>
void LinkedListCollection<K,V>::quick_sort() {
	head = quick_sort(head, length);
	Node* curr_node = head;
	for (int i = 0; i < length; i++) {
		curr_node = curr_node->next;
	}
	tail = curr_node;
}

#endif
