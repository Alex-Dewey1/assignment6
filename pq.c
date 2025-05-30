/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Alexander Dewey
 * Email: deweyal@oregonstate.edu
 */

#include <stdlib.h>
#include "dynarray.h"
#include "pq.h"
#include "assert.h"
#include <stdio.h>
/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 * in addition, you want to define an element struct with both data and priority, 
 * corresponding to the elements of the priority queue. 
 */
struct pq{
	struct dynarray* heap;
};

struct pq_element{
	int priority;
	void* data;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* new_pq = malloc(sizeof(struct pq));
	new_pq->heap = dynarray_create();
	return new_pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	dynarray_free(pq->heap);
	free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  return dynarray_length(pq->heap) == 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, higher priority
 * values are given precedent, and higher place in the queue.  In other words, the
 * element in the priority queue with the highest priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   data - the data value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, higher priority values
 *     should correspond to the first elements.  In other words,
 *     the element in the priority queue with the highest priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* data, int priority) {
	struct pq_element* new_element = malloc(sizeof(struct pq_element));
	new_element->data = data;
	new_element->priority = priority;
	dynarray_insert(pq->heap, -1, new_element);

	int current_index = dynarray_length(pq->heap) - 1;
	while(current_index > 0){
		int parent_index = (current_index - 1) / 2;
		struct pq_element* parent_element = (struct pq_element*) dynarray_get(pq->heap, parent_index);
		struct pq_element* current_element = (struct pq_element*) dynarray_get(pq->heap, current_index);

		if(current_element->priority > parent_element->priority){
			struct pq_element* temp = (struct pq_element*) dynarray_get(pq->heap, parent_index);
			dynarray_set(pq->heap, parent_index, current_element);
			dynarray_set(pq->heap, current_index, temp);
			current_index = parent_index;
		}else{
			break;
		}
	}
}


/*
 * This function should return the data of the first element in a priority
 * queue, i.e. the data associated with the element with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the data of the first item in pq, i.e. the item with
 *   max priority value.
 */
void* pq_max(struct pq* pq) {
	if(dynarray_length(pq->heap) == 0){
		return NULL;
	}
	struct pq_element* root_element = (struct pq_element*) dynarray_get(pq->heap, 0);
	return root_element->data;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with highest priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with highest priority value.
 */
int pq_max_priority(struct pq* pq) {
	if(dynarray_length(pq->heap) == 0){
		return 0;
	}
	struct pq_element* root_element = (struct pq_element*) dynarray_get(pq->heap, 0);
	return root_element->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with highest priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   highest priority value.
 */
void* pq_max_dequeue(struct pq* pq) {
    printf("%4d", dynarray_length(pq->heap));
	if(dynarray_length(pq->heap) == 0){
		return NULL;
	}
	struct pq_element* root_element = (struct pq_element*) dynarray_get(pq->heap, 0);
	struct pq_element* last_element = (struct pq_element*) dynarray_get(pq->heap, -1);
	dynarray_set(pq->heap, 0, last_element);
	dynarray_remove(pq->heap, -1);

	void* data_to_return = root_element->data;
	free(root_element);

	if(dynarray_length(pq->heap) == 1 || dynarray_length(pq->heap) == 0){
		return data_to_return;
	}

	int current_index = 0;
	while(1){
		int left_index = 2 * current_index + 1;
		int right_index = 2 * current_index + 2;
		int largest_child_index = current_index;
  		
		assert(current_index < dynarray_length(pq->heap) || current_index == -1);
		struct pq_element* current_element = (struct pq_element*) dynarray_get(pq->heap, current_index);

		if(left_index < dynarray_length(pq->heap)){
			assert(left_index < dynarray_length(pq->heap) || left_index == -1);
			struct pq_element* left_element = (struct pq_element*) dynarray_get(pq->heap, left_index);
			if(left_element->priority > current_element->priority){
				largest_child_index = left_index;
			}
		}

		if(right_index < dynarray_length(pq->heap)){
			assert(right_index < dynarray_length(pq->heap) || right_index == -1);
			struct pq_element* right_element = (struct pq_element*) dynarray_get(pq->heap, right_index);
			assert(largest_child_index < dynarray_length(pq->heap) || largest_child_index == -1);
			struct pq_element* largest_priority = (struct pq_element*) dynarray_get(pq->heap, largest_child_index);
			if(right_element->priority > largest_priority->priority){
				largest_child_index = right_index;
			}
		}

		if(largest_child_index != current_index){
			struct pq_element* temp = (struct pq_element*) dynarray_get(pq->heap, largest_child_index);
			dynarray_set(pq->heap, largest_child_index, current_element);
			dynarray_set(pq->heap, current_index, temp);
			current_index = largest_child_index;
		}else{
			break;
		}


	}
	return data_to_return;

}
