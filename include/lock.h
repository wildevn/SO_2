#ifndef LOCK_H
#define LOCK_H

typedef struct process {
	void * thread;
	struct process * next_thread;
}process_t;

typedef struct {
	enum {
	      UNLOCKED,
	      LOCKED,
	} status;

	process_t * list;
} lock_t;

void lock_init(lock_t *);
void lock_acquire(lock_t *, void *);
void lock_release(lock_t *);

void block(void *);
void unblock(lock_t *);

#endif                          /* LOCK_H */
