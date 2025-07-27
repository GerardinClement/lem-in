#include "lem_in.h"

void init_queue(t_queue *queue) {
    if (queue) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
}

int enqueue(t_queue *queue, int data) {
    if (!queue)
        return 0;

    t_node_queue *new_node = (t_node_queue *)malloc(sizeof(t_node_queue));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        return 0;
    }
    new_node->data = data;
    new_node->next = NULL;

    if (queue->head == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->size++;
    
    free(new_node);
    return 1;
}

int dequeue(t_queue *queue, int *data_out) {
    if (!queue || queue->head == NULL) {
        return 0;
    }

    t_node_queue *temp = queue->head;

    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    queue->size--;
    *data_out = temp->data;
    free(temp);
    return 1;
}

int is_queue_empty(t_queue *queue) {
    return (queue == NULL || queue->head == NULL);
}

int queue_size(t_queue *queue) {
    return (queue ? queue->size : 0);
}

void destroy_queue(t_queue *queue) {
    if (!queue)
        return;

    t_node_queue *current = queue->head;
    t_node_queue *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}