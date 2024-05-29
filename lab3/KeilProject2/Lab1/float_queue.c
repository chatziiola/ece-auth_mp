#include "float_queue.h"
#include <stdlib.h>

int float_queue_init(FloatQueue *queue, uint32_t size) {
	queue->data = (float*)malloc(sizeof(float) * size);
	queue->head = 0;
	queue->tail = 0;
	queue->size = size;
	
	// If malloc returns NULL (0) the allocation has failed.
	return queue->data != 0;
}

int float_queue_enqueue(FloatQueue *queue, float item) {
	if (!float_queue_is_full(queue)) {
		queue->data[queue->tail++] = item;
		queue->tail %= queue->size;
		return 1;
	} else {
		return 0;
	}
}

int float_queue_dequeue(FloatQueue *queue, float *item) {
	if (!float_queue_is_empty(queue)) {
		*item = queue->data[queue->head++];
		queue->head %= queue->size;
		return 1;
	} else {
		return 0;
	}
}

int float_queue_is_full(FloatQueue *queue) {
	return ((queue->tail + 1) % queue->size) == queue->head;
}

int float_queue_is_empty(FloatQueue *queue) {
	return queue->tail == queue->head;
}

// *******************************ARM University Program Copyright © ARM Ltd 2016*************************************   
