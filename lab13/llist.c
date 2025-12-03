/**
 * File: llist.c
 * Purpose: create a linked list implementation
 * Author: Sean Balbale
 * Date: 12/3/2025
 */

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct Node{
    int data;               // data stored in node
    struct Node *next;      // pointer to next node
} NODE;


struct Node *createNode(int data);
void insertNode(struct Node *head, int data);
void printList(struct Node *head);
void removeNode(struct Node *head);
void freeList(struct Node *head);


/**
 * Creates a new linked list node with 'data' using malloc and
 * returns a pointer to the newly allocated node, or NULL if 
 * malloc failed.
 */
struct Node *createNode(int data){
    struct Node *a = (struct Node *)malloc(sizeof(struct Node));
    if (a == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    a->data = data;
    a->next = NULL;
    return a;
}

/**
 * Inserts a new node with 'data' at the head of the list
 */
void insertNode(struct Node *head, int data){
    struct Node *a = createNode(data);
    struct Node *curr = head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = a;

}
/**
 * Prints all elements in the linked list pointed by 'head'
 */
void printList(struct Node *head){
    struct Node *a = head;
    while(a != NULL){
        printf("%d ",a->data);
        a = a->next;
    }
    printf("\n");
}

/**
 * Removes the node at the head of the list
 */
void removeNode(struct Node *head){
    *head = *(head->next);
}

/**
 * Frees all memory allocated for the linked list pointed by 'head'
 */
void freeList(struct Node *head){
    struct Node *a = head;
    struct Node *temp = a->next;
    while(a->next!= NULL){
        free(a);
        a = temp;
        temp = a->next;
    }
}

int main() {
    
    // Read the number of integers
    printf("Enter an array of integers: \n");
    
    int data;
    scanf("%d", &data);
    struct Node *head = createNode(data);
    while (scanf("%d", &data) != EOF) {
        insertNode(head,data);
    }

    printList(head);

    removeNode(head);

    printList(head);

    freeList(head);

    return 0;
}