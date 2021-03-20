//
//  Server.c
//  
//
//  Created by Jan Ferbr on 18/03/2021.
//

#include <mqueue.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "lib.h"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 50
#define MAX_MSG_SIZE 1024
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define TRUE 1
#define FALSE 0

//STRUCTS ----------------------------
struct Element{
    int type;
    char* key;
    char* value1;
    int value2;
    float value3;
    struct Element* pNext;
    };

//GLOBALS -----------------------------
int busy;
int i=0;
pthread_t thread[10];
pthread_attr_t thread_attr;
pthread_mutex_t mutex1,mutex2;
pthread_cond_t signal1;
struct Element* pHead = NULL;

//FUNCTIONS  DECLARATIONS  -------------
int addNode(char* key, char* value1, int* value2, float* value3);
int modifyNode(char* key, char* value1, int* value2, float* value3);
int deleteList();
int searchList(char* key);
int deleteElement(char* key);
struct Element* getValue(char* key);
int numElements();



void manage_request (mqd_t *s) {
    
    printf("thread connected as well GJ1\n");
    struct Element in_buffer;
    int n;
    
    pthread_mutex_lock(&mutex1);
    printf("thread connected as well GJ2\n");
    mqd_t qd_server=*s;
    
    printf("thread connected as well GJ3\n");
    
    if (mq_receive (qd_server, (char*)&in_buffer, MAX_MSG_SIZE, NULL) == -1) {
        perror ("Server: mq_receive");
        exit (1);
    }
    i--;
    pthread_cond_signal(&signal1);
    pthread_mutex_unlock(&mutex1);
    busy = FALSE;
    printf ("Server: message received: %s,%s,%i,%f\n",&in_buffer.key, &in_buffer.value1, in_buffer.value2, in_buffer.value3);
    printf("number of running threads is %i\nexiting thread!\n",i);
    pthread_exit(&thread[i+1]);
}

//MAIN --------------------------------------
int main(int argc, char **arv)
{
    struct sigevent sev;
    
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    pthread_cond_init(&signal1,NULL);
    
    mqd_t qd_server, qd_client;
    
    //sev.sigev_notify = SIGEV_NONE;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    
    printf("opening queue\n");
    if ((qd_server = mq_open ("/server-queue", O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    busy=TRUE;
    while(1){
        if (mq_getattr(qd_server, &attr) == -1)
            perror("mq_getattr");
        
        //printf("number of messages in queue is %i\n",attr.mq_curmsgs);
        //wait(1000);
        
        if (attr.mq_curmsgs>0 && i<10){
            printf("number of messages in queue is %i\n",attr.mq_curmsgs);
            printf("creating thread because buffer not empty\n");
            pthread_create(&thread[i],&thread_attr,manage_request,&qd_server);
            //pthread_cond_wait(&mutex2,&signal1);
            pthread_mutex_lock(&mutex1);
            i++;
            pthread_mutex_unlock(&mutex1);
            //printf("mutex1 locked in main\n");
            pthread_cond_wait(&mutex1,&signal1);
        }
    }
    return 0;
}

int addNode(char* key, char* value1, int* value2, float* value3)
{
    struct Element* new = (struct Element*)malloc(sizeof(struct Element));
    new->key = key;
    new->value1 = value1;
    new->value2 = *value2;
    new->value3 = *value3;
    new->pNext = pHead;
    pHead = new;
}
int deleteList()
{
    struct Element* tmp = NULL;
    while(pHead){
        tmp = tmp->pNext;
        free(pHead);
        pHead = tmp;
    }
}
int searchList(char* key)
{
    struct Element* tmp = pHead;
    while(tmp)
    {
        if(!strcmp(key, tmp->key))
            return 1;
        tmp = tmp->pNext;
    }
    return 0;//element does not exsist
}
struct Element* getValue(char* key)
{
    struct Element* tmp = pHead;
    while(tmp)
    {
        if(!strcmp(key, tmp->key))
            return tmp;
        tmp = tmp->pNext;
    }
    return NULL;//element does not exsist
}
int modifyNode(char* key, char* value1, int* value2, float* value3)
{
    struct Element* tmp = pHead;
    while(tmp)
    {
        if(!strcmp(key, tmp->key))
        {
            tmp->value1 = value1;
            tmp->value2 = *value2;
            tmp->value3 = *value3;
        }
        tmp = tmp->pNext;
    }
    return -1;//element does not exsist
}
int deleteElement(char* key)
{
    struct Element* prev = NULL;
    struct Element* tmp = pHead;
    while(tmp)
    {
        if(!strcmp(key, tmp->key))
        {
            if(prev)
                prev->pNext = tmp->pNext;
            free(tmp);
            return 0;
        }
        prev = tmp;
        tmp = tmp->pNext;
    }
    return -1;//element does not exsist
}
int numElements()
{
    int num = 0;
    struct Element* tmp = pHead;
    while(tmp)
    {
        num = num + 1;
        tmp = tmp->pNext;
    }
    return num;//element does not exsist
}

