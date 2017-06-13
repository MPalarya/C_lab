#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

// ##################################
// #   Internal Private Functions   #
// ##################################
void insertAfter(LinkedListNode* existingNode, LinkedListNode* newNode) {
	if (existingNode->next)
		existingNode->next->prev = newNode;
	newNode->next = existingNode->next;
	newNode->prev = existingNode;
	existingNode->next = newNode;
}

void insertBefore(LinkedListNode* existingNode, LinkedListNode* newNode) {
	if (existingNode->prev)
		existingNode->prev->next = newNode;
	newNode->prev = existingNode->prev;
	newNode->next = existingNode;
	existingNode->prev = newNode;
}

int isEmpty(LinkedList* list) {
	return (list->head == NULL);
}

LinkedListNode* getNode(LinkedList* list, int index) {
	int i;
	LinkedListNode* node = NULL;

	for (i = 0, node = list->head; node; i++, node = node->next) {
		if (i == index)
			return node;
	}
	return NULL;
}

LinkedListNode* newListNode(int value) {
	LinkedListNode* node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	node->next = node->prev = NULL;
	node->value = value;
	return node;
}

// #################################
// #   External Public Functions   #
// #################################
LinkedList* newList() {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

void add(LinkedList* list, int value) {
	LinkedListNode* newNode = newListNode(value);
	LinkedListNode* curNode = NULL;

	if (isEmpty(list))
		list->head = newNode;

	else {
		// reach to end of the list
		for (curNode = list->head; curNode->next; curNode = curNode->next);
		insertAfter(curNode, newNode);
	}
}

void addToStart(LinkedList* list, int value) {
	LinkedListNode* newNode = newListNode(value);

	if (!isEmpty(list))
		insertBefore(list->head, newNode);

	list->head = newNode;
}

EResult addAfter(LinkedList* list, int newValue, int existingValue) {
	LinkedListNode* curNode = NULL;
	LinkedListNode* newNode = NULL;

	for (curNode = list->head; curNode; curNode = curNode->next) {
		if (curNode->value == existingValue) {
			newNode = newListNode(newValue);
			insertAfter(curNode, newNode);
			return SUCCESS;
		}
	}
	return FAIL;
}

EResult removeNode(LinkedList* list, int index) {
	LinkedListNode* node = getNode(list, index);
	if (!node)
		return FAIL;

	if (node->prev)
		node->prev->next = node->next;

	else // removing head -> index = 0 
		list->head = node->next;

	if (node->next)
		node->next->prev = node->prev;

	free(node);
	return SUCCESS;
}

void freeList(LinkedList* list) {
	LinkedListNode* curNode = list->head;
	LinkedListNode* nextNode = NULL;

	while (curNode) {
		nextNode = curNode->next;
		free(curNode);
		curNode = nextNode;
	}
	free(list);
}

int findIndex(LinkedList* list, int value) {
	int i;
	LinkedListNode* curNode = NULL;

	for (i = 0, curNode = list->head; curNode; curNode = curNode->next, i++) {
		if (curNode->value == value)
			return i;
	}
	return -1;
}

void printList(LinkedList* list) {
	LinkedListNode* curNode = NULL;
	printf("[");

	for (curNode = list->head; curNode; curNode = curNode->next) {
		if (curNode != list->head)
			printf(", ");
		printf("%d", curNode->value);
	}

	printf("]\n");
}
