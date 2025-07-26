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
        return 0; // Échec de l'allocation
    }
    new_node->data = data;
    new_node->next = NULL;

    if (queue->head == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        queue->tail->next = new_node;
        queue->tail = new_node;       // Le nouveau nœud devient le dernier
    }
    queue->size++;
    return 1; // Succès
}

int dequeue(t_queue *queue, int *data_out) {
    if (!queue || queue->head == NULL) {
        return 0; // La file est vide
    }

    t_node_queue *temp = queue->head; // Nœud à supprimer

    queue->head = queue->head->next; // La tête avance au nœud suivant
    if (queue->head == NULL) {        // Si la file devient vide après le dequeue
        queue->tail = NULL;           // La queue doit aussi être NULL
    }
    free(temp); // Libère la mémoire du nœud supprimé
    queue->size--;
    *data_out = temp->data; // Retourne les données du nœud supprimé
    return 1; // Succès
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

// --- Exemple d'utilisation (pour tester) ---

/*
int main() {
    t_queue my_queue;
    init_queue(&my_queue);

    printf("Queue empty: %d\n", is_queue_empty(&my_queue)); // Attendu: 1
    printf("Queue size: %d\n", queue_size(&my_queue));    // Attendu: 0

    enqueue(&my_queue, 10);
    enqueue(&my_queue, 20);
    enqueue(&my_queue, 30);
    printf("Enqueued 10, 20, 30.\n");

    printf("Queue empty: %d\n", is_queue_empty(&my_queue)); // Attendu: 0
    printf("Queue size: %d\n", queue_size(&my_queue));    // Attendu: 3

    int val;
    if (dequeue(&my_queue, &val)) {
        printf("Dequeued: %d\n", val); // Attendu: 10
    }
    if (dequeue(&my_queue, &val)) {
        printf("Dequeued: %d\n", val); // Attendu: 20
    }

    printf("Queue size: %d\n", queue_size(&my_queue)); // Attendu: 1

    enqueue(&my_queue, 40);
    printf("Enqueued 40.\n");

    while (!is_queue_empty(&my_queue)) {
        if (dequeue(&my_queue, &val)) {
            printf("Dequeued: %d\n", val); // Attendu: 30, puis 40
        }
    }
    printf("All elements dequeued.\n");

    printf("Queue empty: %d\n", is_queue_empty(&my_queue)); // Attendu: 1
    printf("Queue size: %d\n", queue_size(&my_queue));    // Attendu: 0

    destroy_queue(&my_queue);
    printf("Queue destroyed.\n");

    return 0;
}
*/