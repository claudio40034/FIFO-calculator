#include "Queue.h"
using namespace std;
Queue::Queue(){
}


void Queue::gainMemory(){
 if ((shm_id = shmget((key_t)1124, 2*sizeof(int), 0666)) == -1)
    perror("shmget"), exit(1);
 if ((shm_ptr = (int *)shmat(shm_id, (void*)0, 0)) == (void *)-1)
    perror("shmat failed"), exit(1);
tail = (shm_ptr+4);
i = (shm_ptr+5);
for(*i=0;*i<LIMIT;(*i)++){
queue[*i] = (shm_ptr+((*i)+6));
}
}

bool Queue::enqueue(int newElement) {
    if (*tail == LIMIT)
        return false;               // Overflow

    *queue[*tail] = newElement;
    (*tail)++;
    return true;
}


int Queue::dequeue() {
    if((*tail) == 0)
        return -1;               // Underflow

    int deleted = *queue[0];

    
    for( *i = 0; *i < (*tail)-1; (*i)++)
	*queue[*i] = *queue[(*i)+1];

    (*tail)--;
    return deleted;
}
int Queue::printHead(){
return *queue[0];
}

void Queue::printQueue() {
    if ((*tail) == 0)
        cout<<"Queue vuota!"<<endl;

    else{
      
        for ( (*i) = 0; *i <= (*tail)-1; (*i)++)
           cout<<"Element "<<(*i)+1<<": "<<*queue[*i]<<"|";
    }
}
