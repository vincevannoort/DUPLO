/*! @file
 * \brief Implementation of a Queue
 * \section queue Queue
 */

#define QUEUESIZE 100
typedef struct {
    int a[QUEUESIZE];
    int head;
    int tail;
} Queue;

/*!
 * \brief Initialize queue
 *
 * \param[in] Queue pq, queue struct to initialize
 */
void init_queue(Queue* pq){
    pq->head = -1;
    pq->tail = 0;
}

/*!
 * \brief Enqueue an data item in the queue
 *
 * \param[in] Queue pq, queue struct to queue in
 * \param[in] int data, data to queue in struct
 */
void enqueue(Queue* pq, int data){
    if (pq->tail != pq->head){
        // if queue was empty set head to 0
        if(pq->head == -1)
            pq->head = 0;

        // enqueue data
        pq->a[pq->tail] = data;

        // add +1 to tail
        pq->tail = ++(pq->tail) % QUEUESIZE;
    }
}

/*!
 * \brief Dequeue an data item in the queue
 *
 * \param[in] Queue pq, queue struct to dequeue out
 * \param[in] int data, data to queue in struct
 *
 * \return int, 0 if queue is empty, data if queue is not empty
 */
int dequeue(Queue* pq){
    if (pq->head >= 0){
        int data_dequeue = pq->a[pq->head];

        // dequeue data
        pq->head = ++(pq->head) % QUEUESIZE;

        if(pq->head == pq->tail){
            pq->head = -1;
            pq->tail = 0;
        }
        return data_dequeue;
    }
    return 0;
}
