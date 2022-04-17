/* Header file for the simple circular queue example */
#ifndef __QUEUE_H__
#define __QUEUE_H__

struct JOB{
    int jobid;
    char *commands[1000];
	char command[1000];
    int status;
	char job_start_time[1000];
	char job_end_time[1000];
};

typedef struct _queue {
	int size;    /* maximum size of the queue */
	struct JOB** buffer; /* queue buffer */
	int start;   /* index to the start of the queue */
	int end;     /* index to the end of the queue */
	int count;   /* no. of elements in the queue */
} queue;

queue *queue_init(int n);
struct JOB* queue_delete(queue *q);
int queue_insert(queue *q, struct JOB *item);
void queue_display(queue *q);
void queue_destroy(queue *q);

#endif
