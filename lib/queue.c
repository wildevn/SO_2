#include <queue.h>

/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. You  are free to  choose your own  implementation. You
  may need to define additional functions if you want to implement the
  extra-point functionality.
 */
// TODO: inicializes a queue
void queue_init(node_t * queue)
{
  queue->thread_id = -1; // Cabeça
  queue->tcb = NULL;
  queue->prox = NULL;
}

// TODO: returns the first element of the queue
node_t *dequeue(node_t * queue)
{
  node_t q;

  if(is_empty(queue->prox) == 0) {
    q = queue->prox;
    queue->prox = q->prox;
    return q;
  }
	return NULL;
}

// TODO: inserts a node in a queue
void enqueue(node_t * queue, node_t * item)
{
  node_t *q;
  
  item->prox = NULL;
  if(is_empty(queue->prox) == 0) { // Verifies if the line is empty
    q = queue->prox;
    while(is_empty(q->prox) == 0)
      q = q->prox;
    q->prox = item;
    return;
  }
  queue->prox = item;
}

// TODO: checks if a queue is empty
int is_empty(node_t *queue)
{
  if(queue == NULL)
    return 1;
	return 0;
}



