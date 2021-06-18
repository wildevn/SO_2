#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <lock.h>
#include <threadu.h>
#include <thread.h>

enum {
      SPIN = FALSE,
};

// Inicializes a lock
void lock_init(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
    	l->status = UNLOCKED;
		l->list->thread = NULL;
		l->list->next_thread = NULL;
	}
}

// Acquires a  lock if  it is  available or  blocks the  thread
// otherwise
void lock_acquire(lock_t * l, void * thread)
{
	if (SPIN) {
		while (LOCKED == l->status)
			thread_yield();
		l->status = LOCKED;
	} else {
		if(l->status == UNLOCKED)
			l->status = LOCKED;
		else {
			thread_t * thread_aux = (thread_t *) thread;
			lock_t * l_aux = l;
			while(l_aux->list->next_thread != NULL)
				l_aux->list = l_aux->list->next_thread;
			l_aux->list->next_thread->thread = thread_aux;
			l_aux->list->next_thread->next_thread = NULL;
			block(thread);
		}
	}
}

// Releases a  lock  and  unlocks one  thread  from the  lock's
// blocking list
void lock_release(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		if(l->list->next_thread != NULL) {
			unblock(l);
			return;
		}
		l->status=UNLOCKED;
	}
}

// Blocks the running thread
void block(void *thread)
{
	thread_t * thread_aux;
	thread_aux = (thread_t *) thread;
	tcb_t * tcb_aux;
	tcb_aux = (tcb_t *) thread_aux->tcb;
	tcb_aux->status = BLOCKED;
	thread_yield();
}

// Unblocks  a thread that is waiting on a lock.
void unblock(lock_t *l)
{
	thread_t * thread_aux;
	tcb_t * tcb_aux;
	thread_aux = (thread_t *) l->list->next_thread->thread;
	tcb_aux = (tcb_t *) thread_aux->tcb;
	tcb_aux->status = READY;
	l->list->next_thread = l->list->next_thread->next_thread;
}
