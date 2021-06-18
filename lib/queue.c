#include <queue.h>
#include <stdio.h>

// Inicializes a queue
void queue_init(node_t *queue)
{
  queue->thread_id = -1; // Cabeça
  queue->thread_tcb = NULL;
  queue->prox = NULL;
}

// Returns the first element of the queue
node_t *dequeue(node_t *queue)
{
  node_t *q;

  if(is_empty(queue) == 0) {
    q = queue->prox;
    queue->prox = q->prox;
    q->prox = NULL;
    return q;
  }
	return NULL;
}

// Inserts a node in a queue
void enqueue(node_t *queue, node_t *node)
{
  node_t *q;
  node->prox = NULL;
  if(is_empty(queue) == 0) { // Verifies if the line is empty .. 
    q = queue->prox;
    while(is_empty(q) == 0)
      q = q->prox;
    q->prox = node;
    return;
  }
  if(queue == NULL)
    return;
  else
    queue->prox = node;
  return;
}

// Checks if a queue is empty
int is_empty(node_t *queue)
{
  if(queue == NULL)
    return -1;
  if(queue->prox == NULL)
    return 1;
	return 0;
}
