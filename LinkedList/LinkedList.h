#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef enum { SUCCESS, FAIL } EResult;

typedef struct linkedListNode {
	int value;
	struct linkedListNode* next;
	struct linkedListNode* prev;
} LinkedListNode;

typedef struct linkedList {
	LinkedListNode* head;
} LinkedList;

LinkedList* newList();

void add(LinkedList* list, int value);
void addToStart(LinkedList* list, int value);
EResult addAfter(LinkedList* list, int newValue, int existingValue);

EResult removeNode(LinkedList* list, int index);
void freeList(LinkedList* list);

int findIndex(LinkedList* list, int value);
void printList(LinkedList* list);

#endif // !LINKED_LIST_H