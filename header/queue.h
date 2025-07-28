#ifndef QUEUE_H
# define QUEUE_H

typedef struct s_node_queue {
    int             data;       // Les données du nœud (ici, un ID de salle)
    struct          s_node_queue *next;    // Pointeur vers le nœud suivant
} t_node_queue;

// Représente la liste chaînée elle-même (la file)
typedef struct s_queue {
    t_node_queue    *head;  // Pointeur vers le premier nœud de la liste
    t_node_queue    *tail;  // Pointeur vers le dernier nœud de la liste
    int             size;   // Nombre d'éléments dans la file
}                   t_queue;

void                init_queue(t_queue *queue);
int                 enqueue(t_queue *queue, int data);
int                 dequeue(t_queue *queue, int *data_out);
int                 is_queue_empty(t_queue *queue);
void                destroy_queue(t_queue *queue);


#endif