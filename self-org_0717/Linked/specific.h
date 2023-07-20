#pragma once
#ifndef SPECIFIC_H
#define SPECIFIC_H

// Define the SOLLTYPE macro
#define SOLLTYPE "MTF" // Replace "MTF" with the appropriate self-organizing policy used in your implementation

typedef struct Node{
	char* data;
	int count;
	struct Node* next;
}Node;

struct soll{
	Node* head;
	Node* tail;
	orgtype type;
	int count;
};

#endif /* SPECIFIC_H */
