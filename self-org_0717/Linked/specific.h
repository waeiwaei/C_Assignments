#pragma once


// Define the SOLLTYPE macro
#define SOLLTYPE "MTF" // Replace "MTF" with the appropriate self-organizing policy used in your implementation

typedef struct Node{
	char* data;
	int frequency;
	struct Node* next;
}Node;

struct soll{
	Node* head;
	Node* tail;
	orgtype type;
	int count;
};

bool frequency_reorg(soll* s, Node* current, Node* previous);
bool transpose_reorg(soll* s, Node* current, Node* previous);
bool mtf_reorg(soll* s, Node* current, Node* previous);


