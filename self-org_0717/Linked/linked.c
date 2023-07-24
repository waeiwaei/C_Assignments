#include "../soll.h"
#include "../ADTs/General/general.h"
#include "specific.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#define MAX_STR_LEN 10000


char* strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char* new_str = (char*)ncalloc(len + 1, sizeof(char)); // +1 for null-terminator

    strcpy(new_str, str);
    new_str[len] = '\0'; // Add the null-terminator at the end

    return new_str;
}

/*
Create an empty soll. Can be of type:
'none'      : Unsorted linked list.
'mtf'       : When element is accessed it is moved to the front.
'transpose' : When element is accessed it is moved one place closer to the front.
*/


soll* soll_init(orgtype type) {

    soll* s = (soll*)ncalloc(1, sizeof(soll));

    s->head = NULL;
    s->tail = NULL;
    s->type = type;
    s->count = 0;

    return s;
}



// Helper function to check if the input is a valid C-style string
bool is_valid_c_string(char* str) {
    if (str == NULL) {
        return false;
    }

    // Check if the string is null-terminated
    for (size_t i = 0; str[i] != '\0'; i++) {
        // Optionally, you can perform additional checks here
        // to ensure that str contains valid characters.
        // For example, you can check if the characters are within a certain range.
    }

    return true;
}


void soll_add(soll* s, char* str) {
    if (s == NULL || str == NULL || strlen(str) == 0) {
        return;
    }

    // Create a new node and set its value
    Node* newNode = (Node*)ncalloc(1, sizeof(Node));
    newNode->data = strdup(str); // Deep copy the string
    newNode->count = 1;
    newNode->next = NULL;

    if (s->head == NULL && s->tail == NULL) {
        // If the list is empty, update both head and tail pointers
        s->head = newNode;
        s->tail = newNode;
    } else {
        // If the list is not empty, update the tail pointer and link nodes
        s->tail->next = newNode;
        s->tail = newNode;
    }

    // Increment the count of elements
    s->count++;
}




bool soll_remove(soll* s, char* str) {
    if (s == NULL || s->head == NULL) {
        return false;  // Empty list or invalid input, nothing to remove
    }

    Node* current = s->head;
    Node* previous = NULL;

    // Traverse the list to find the node containing the string
    while (current != NULL && strcmp(current->data, str) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return false;  // String not found in the list
    }

    // Remove the node from the list
    if (previous == NULL) {
        // If the first node matches, update the head
        s->head = current->next;
    } else {
        // Update the next pointer of the previous node
        previous->next = current->next;
    }

    // Update the tail if the removed node was the last node
    if (current == s->tail) {
        s->tail = previous;
    }

    free(current->data);  // Free the memory of the string data
    free(current);        // Free the memory of the node


    return true;  // Successfully removed the node
}



bool soll_isin(soll* s, char* str, long* cnt) {

    if (s == NULL || str == NULL || cnt == NULL) {
        return false;  // Invalid input or empty list, element not found
    }

    Node* current = s->head;
    Node* previous = NULL;

    // Traverse the list to find the node containing the string
    while (current != NULL && strcmp(current->data, str) != 0) {
        previous = current;
        current = current->next;
        (*cnt)++;  // Increment the pointer-chase count (only if the element is not found)
    }

    if (current == NULL) {
        return false;  // String not found in the list
    }


    // Increment the frequency of the accessed element
    current->count++;

    // Reorganize the list based on s->type
    if (s->type == mtf) {
        // Move the accessed element to the front of the list
        if (previous != NULL) {
            previous->next = current->next;

            if(current->next == NULL){
                s->tail = previous;
            }

            current->next = s->head;
            s->head = current;
        }

    } else if (s->type == transpose) {
        // Move the accessed element one position closer to the front
        if (previous != NULL) {
            // This block executes when the accessed element is not already at the front
            previous->next = current->next;

            if (current->next == NULL) {
                // Update the tail to the correct new tail node
                s->tail = previous;
            }

            current->next = previous;
            if (s->head == previous) {
                s->head = current;
            } else {
                Node* prev_prev = s->head;
                while (prev_prev->next != previous) {
                    prev_prev = prev_prev->next;
                }
                prev_prev->next = current; // This updates the prev_prev->next pointer when moving the element closer to the front
            }
        } 
    }
    return true;  // Element found in the list
}





int soll_freq(soll* s, char* str) {
    if (s == NULL || str == NULL) {
        return 0;  // Invalid input or empty list, frequency is zero
    }

    Node* current = s->head;

    // Traverse the list to find the node containing the string
    while (current != NULL && strcmp(current->data, str) != 0) {
        current = current->next;
    }

    // If the element is found, return its frequency; otherwise, return 0
    return (current != NULL) ? current->count : 0;

}





//1 -> 2 -> 3
int soll_size(soll* s) {
    if (s == NULL || s->head == NULL) {
        return 0;  // Empty list, size is zero
    }

    int count = 0;
    Node* current = s->head;

    // Traverse the list and count the number of elements
    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;

}


void soll_tostring(soll* s, char* str) {
    if (s == NULL || str == NULL) {
        on_error("Unable to generate string for SOLL\n");
    }

    Node* current = s->head;
    int offset = 0;

    // Traverse the list and append each element's data and frequency to the string
    while (current != NULL) {
        int written = snprintf(str + offset, MAX_STR_LEN - offset, "%s(%d)", current->data, current->count);
        if (written < 0 || written >= (MAX_STR_LEN - offset)) {
            // Handle potential error or buffer overflow
            return;
        }
        offset += written;

        // If there are more elements, append the separator "|"
        if (current->next != NULL) {
            written = snprintf(str + offset, MAX_STR_LEN - offset, "|");
            if (written < 0 || written >= (MAX_STR_LEN - offset)) {
                // Handle potential error or buffer overflow
                return;
            }
            offset += written;
        }

        current = current->next;
    }
}


void strfree(char* str) {
    if (str != NULL) {
        free(str);
    }
}




// Clears up all space used
bool soll_free(soll* s) {
    if (s == NULL) {
        return true;  // Invalid input, nothing to free
    }

    Node* current = s->head;
    Node* next;

    // Traverse the list and free each node and its data
    while (current != NULL) {
        next = current->next;
        strfree(current->data);  // Free the data string
        free(current);        // Free the node itself
        current = next;
    }

    // Reset the head and tail pointers to NULL
    s->head = NULL;
    s->tail = NULL;

    free(s);

    return true;
}
