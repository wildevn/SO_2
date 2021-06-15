#ifndef LOCK_H
#define LOCK_H

typedef struct {
	enum {
	      UNLOCKED,
	      LOCKED,
	} status;

  void *lock_queue;
} lock_t;

void lock_init(lock_t *);
void lock_acquire(lock_t *);
void lock_release(lock_t *);

#endif                          /* LOCK_H */
