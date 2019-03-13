/**
    CS-11 Asn 2
    singly_linked_list.c       
    Purpose: implementation for a singly linked list as an 
             assignment for 'Algorithm and Data Structures' classes
	     Wroclaw University of Science and Technology W11/PWr

    @author Eng. Andrzej Ratajski
    @version 1.1 11/03/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/**
    Data stored in NODE structure. It's defined as a structure to meet
    open-to-modification condition (if one want to change data type to,
    for egz, double here it shlould be done
 */
typedef struct {
    int info;
   
} DATA;


/**
    Basic node structure with one data attribute and pointer to next node
 */
typedef struct node {
    DATA data;
    struct node* next;
} NODE;

/**
    Comparing function for DATA structure
 */ 
_Bool compare_data(DATA data1, DATA data2) {
    return data1.info == data2.info;
}

_Bool gt_data(DATA data1, DATA data2) {
    return data1.info > data2.info;
}

/**
    Iinitialize the list with a head pointing initially to nothing 

    @param pointer to a head of a list
 */
void init(NODE** head) {
    *head = NULL;
}

int isempty(NODE** head) {
    return ((*head) == NULL) ? 1 : 0;
}


/**
    Prints every element sotred in the list from the head

    @param head of a list 
 */
void print_list(NODE* head) {
    NODE* temp;
    for (temp = head; temp; temp = temp->next)
        printf("%5d", temp->data.info);
}


void insert(NODE** head, DATA data) {
    NODE* new = (NODE*)malloc(sizeof(NODE));
    if (new == NULL) exit(1);   // no memory available
    new->data = data;
    new->next = NULL;
    
    if ( (*head)==NULL ) {
        *head = new;
    } else {
        NODE* temp = *head;
        while( temp->next != NULL ) {
            temp = temp->next;
        }
    temp->next = new;
    }
        
}


void delete(NODE** head, NODE* previous_node) {
    if (previous_node == *head) {       // when it's head to be deleted
        if (previous_node->next == NULL) {
            //printf("Closing list...\n");
            *head = NULL;
            free(previous_node);
        } else {
            *head = previous_node->next;
            free(previous_node);
        }
    } else {
        if (previous_node == NULL) {
        //printf("Can't delete an element that's not in the list!\n");
        } else {
            NODE* to_delete = previous_node->next;
            if (to_delete->next == NULL) {       // when it's the last element
                previous_node->next = NULL;
                free(to_delete);
            } else {
                previous_node->next = previous_node->next->next;
                to_delete->next = NULL;
                free(to_delete);
            }
        }
    }
}

NODE* lin_search(NODE** head, DATA data, int* ptr) {
    if ( isempty(head) ) {      // a list is empty
        return NULL;
    }
    *ptr = (*ptr) + 1;
    NODE* previous = *head;
    NODE* temp = *head;

    if ( compare_data(temp->data, data) ) {
        return *head;       // positive head-case scenario
    }
    *ptr = (*ptr) + 1;
    while( !compare_data(temp->data, data) && !(temp->next == NULL) ) {
        previous = temp;
        temp = temp->next;
        *ptr = (*ptr) + 2;
    }
    if ( compare_data(temp->data, data) ) {
        *ptr = (*ptr) + 1;
        return previous;
    } else {
        *ptr = (*ptr) + 1;
        return NULL;
    }
}

NODE* findMTF(NODE** head, DATA data, int* ptr) {
    NODE* previous = lin_search(head, data, ptr);
    
    *ptr = (*ptr) + 1;
    if ( previous != NULL ) {
        delete(head, previous);

        NODE* new_head = (NODE*)malloc(sizeof(NODE));
        if (new_head == NULL) exit(1);   // no memory available
        new_head->data = data;
        new_head->next = *head;
        return new_head;
    } else {
        return *head;
    }
}

void findTRANS(NODE** head, DATA data, int* ptr) {
    NODE* previous = lin_search(head, data, ptr);
    DATA temp_data;

    *ptr = (*ptr) + 1;
    if ( previous != NULL && previous != *head ) {
        temp_data = previous->data;
        previous->data = previous->next->data;
        previous->next->data = temp_data;
    } else {
        return;
    }
}
    

NODE *bubble_sort(NODE *head) {
    NODE *tmpPtr = head, *tmpNxt = head->next;
    DATA tmp;
    while (tmpNxt != NULL) {
        while (tmpNxt != tmpPtr) {
            if (tmpNxt->data.info < tmpPtr->data.info) {
                tmp = tmpPtr->data;
                tmpPtr->data = tmpNxt->data;
                tmpNxt->data = tmp;
            }
            tmpPtr = tmpPtr->next;
        }
        tmpPtr = head;
        tmpNxt = tmpNxt->next;
    }
    return tmpPtr;
}

int main() {
    int h=0;
    int* ptr0 = &h;
    NODE* head1;
    NODE* head2;
    DATA element;

    init(&head1);
    init(&head2);
    srand(time(NULL));
    
/*----------------------------------adding random elements-----------------------------*/ 
    for (int i = 0; i < 100; i) {
        element.info = rand()%100 + 1;
        if ( lin_search(&head1, element, ptr0) == NULL ) {
          insert(&head1, element);
          insert(&head2, element);
          i++;
          //printf("%dst element of value %2d added\n", i, element.info);
        }
    }    
    printf("\nlist1 and list2 look like:\n");
    print_list(head1);
    printf("\n");

    int h1=0, h2=0;
    int* ptr1 = (int*)malloc(sizeof(int)); 
    ptr1 = &h1;
    int* ptr2 = (int*)malloc(sizeof(int));
    ptr2 = &h2;
/*-----------------------------------first function-----------------------------------*/   
    printf("launching first function on list1...\n");
    for (int i=100; i>0; i--) {
        for (int j=1; j<=100; j++) {
            element.info = j;
            head1 = findMTF(&head1, element, ptr1); 
        }
        element.info = i;
        delete(&head1, lin_search(&head1, element, ptr1));
    }
    element.info=1;
    delete(&head1, lin_search(&head1, element, ptr1));

    printf("Checking if list1 is empty:\t");
    isempty(&head1) ? printf("empty\n") : printf("not empty\n");

/*---------------------------------second function-------------------------------------*/
    printf("launching second function on list2..\n");
    for (int i=100; i>0; i--) {
        for (int j=1; j<=100; j++) {
            element.info = j;
            findTRANS(&head2, element, ptr2); 
        }
        element.info = i;
        delete(&head2, lin_search(&head2, element, ptr2));
    }
    element.info=1;
    delete(&head2, lin_search(&head2, element, ptr2));

    printf("Checking if list2 is empty:\t");
    isempty(&head2) ? printf("empty\n") : printf("not empty\n");

    printf("func1 to func2 ratio: %d \\ %d \n", h1, h2);

    ptr1 = NULL;
    ptr2 = NULL;
    free(ptr1);
    free(ptr2);
    return (0);
}
