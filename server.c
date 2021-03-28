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
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 2048
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
    char queue_name[64];
    struct Element* pNext;
    };

//GLOBALS -----------------------------
int busy;
int writing;
pthread_t thread;
pthread_attr_t thread_attr;
pthread_mutex_t mutex1,mutex2;
pthread_cond_t signal1,signal2;
struct Element* pHead = NULL;

//FUNCTIONS  DECLARATIONS  -------------
int addNode(char* key, char* value1, int* value2, float* value3);
int modifyNode(char* key, char* value1, int* value2, float* value3);
int deleteList(void);
int searchList(char* key);
int deleteElement(char* key);
struct Element* getValue(char* key);
int numElements(void);



void manage_request (mqd_t *s) {
    
    printf("thread running\n");
    struct Element in_buffer;
    //int n;
    pthread_mutex_lock(&mutex1);
    printf("mutex locked by thread\n");
    mqd_t qd_server=*s;
    if (mq_receive (qd_server, (char*)&in_buffer, MAX_MSG_SIZE, NULL) == -1) {
        perror ("Server: mq_receive");
        exit (1);
    }
    printf ("Server: message recived: type:%i, %s,%s,%i,%f\n",in_buffer.type,&in_buffer.key, &in_buffer.value1, in_buffer.value2, in_buffer.value3);
    //char* key=in_buffer.key;
    if(in_buffer.type == 1){
        in_buffer.type = deleteList();
    }else if(in_buffer.type == 2){
        if(searchList(&in_buffer.key)==0){
            printf("im am in if statement for 2");
            in_buffer.type = addNode(&in_buffer.key,&in_buffer.value1,&in_buffer.value2,&in_buffer.value3);
            printf("after invoke of addNode");
        }else{
            in_buffer.type = -1;
        }
    }else if(in_buffer.type == 3){
        if(searchList(&in_buffer.key)==1){
            struct Element* tmp = getValue(in_buffer.key);
            in_buffer.value1 = tmp->value1;
            in_buffer.value2 = tmp->value2;
            in_buffer.value3 = tmp->value3;
            in_buffer.type = 0;
        }else{
            in_buffer.type = -1;
        }
    }else if(in_buffer.type == 4){
        in_buffer.type = modifyNode(&in_buffer.key, &in_buffer.value1,&in_buffer.value2,&in_buffer.value3);
    }else if(in_buffer.type == 5){
        //in_buffer.type = deleteElement(&in_buffer.key);
        while(pHead)
        {
            printf("key:%s\nvalue2:%d",pHead->key,pHead->value2);
            pHead = pHead->pNext;
        }
    }else if(in_buffer.type == 6){
        printf("in buffer key przed wejsciem do funkcji %s\n",&in_buffer.key);
        in_buffer.type = searchList(&in_buffer.key);
        printf("zwrociło się z funkcji to: %d",in_buffer.type);
    }else if(in_buffer.type == 7){
        in_buffer.type = numElements();
    }else{
        in_buffer.type = -1;
        printf("Wrong argument");
    }
    printf ("2Server: message sent back: type:%i, %s,%s,%i,%f\n",in_buffer.type,&in_buffer.key, &in_buffer.value1, in_buffer.value2, in_buffer.value3);
    mqd_t qd_client;
    //printf("in_buffer.queue_name is %s\n",in_buffer.queue_name);
    if ((qd_client = mq_open (in_buffer.queue_name, O_WRONLY)) == -1) {
        perror ("Client: mq_open (client)");
        exit (1);
    }
    int msg=mq_send(qd_client,(const char *)&in_buffer,MAX_MSG_SIZE,0);
    if (msg < 0) {
        perror("Error in sending msg");
        exit(1);
    }
    mq_close(qd_client);
    busy = FALSE;
    pthread_cond_signal(&signal1);
    pthread_mutex_unlock(&mutex1);
    
    printf("mutex unlocked by thread\n");
    printf("exiting thread!\n");
    pthread_exit(NULL);
}

//MAIN --------------------------------------
int main(int argc, char **arv)
{
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    pthread_cond_init(&signal1,NULL);
    pthread_cond_init(&signal2,NULL);
    
    mqd_t qd_server;
    
    //sev.sigev_notify = SIGEV_NONE;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    int clear = mq_unlink("/server-queue");
    printf("clearing queue results:%d", clear);
    
    printf("opening queue\n");
    if ((qd_server = mq_open ("/server-queue", O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    
    while(1){
        if (mq_getattr(qd_server, &attr) == -1)
            perror("mq_getattr");
        
        //printf("number of messages in queue is %i\n",attr.mq_curmsgs);
        //wait(1000);
        
        int m=attr.mq_curmsgs;
        for (int i=0;i<m;i++){
            if (mq_getattr(qd_server, &attr) == -1)
                perror("mq_getattr");
            printf("number of messages in queue is %i\n",attr.mq_curmsgs);
            printf("creating thread because buffer not empty\n");
            pthread_create(&thread,&thread_attr,manage_request,&qd_server);
            usleep(500000);
            pthread_mutex_lock(&mutex1);
            printf("mutex locked by main\n");
            while (busy==TRUE){
                //printf("Waiting for signal by main\n");
                pthread_cond_wait(&mutex1,&signal1);
            }
            busy=TRUE;
            pthread_mutex_unlock(&mutex1);
            printf("mutex unlocked by main\n");
            printf("busy changing to true\n");
        }
    }
    return 0;
}

int addNode(char* key, char* value1, int* value2, float* value3)
{
    struct Element* new = (struct Element*)malloc(sizeof(struct Element));
    printf("im am in addNode function");
    new->key = key;
    new->value1 = value1;
    new->value2 = *value2;
    new->value3 = *value3;
    new->pNext = pHead;
    pHead = new;
    return 0;
}
int deleteList()
{
    struct Element* tmp = NULL;
    while(pHead != NULL){
        tmp = pHead->pNext;
        free(pHead);
        pHead = tmp;
    }
    return 0;
}
int searchList(char* wtf)
{
    struct Element* tmp = pHead;
    printf("all good\n");
    while(tmp != NULL)
    {
        printf("im in searchlist while loop\n");
        printf("wartość key:%s\n", wtf);
        printf("wartość tmp->key:%s\n",tmp->key);
        printf("im in searchlist while loop\n");
        int i = strcmp(wtf, tmp->key);
        printf("wartosc porównania: %d\n",i);
        if(strcmp(wtf, tmp->key) == 0)
            return 1;
        tmp = tmp->pNext;
    }
    return 0;//element does not exsist
}
struct Element* getValue(char* key)
{
    struct Element* tmp = pHead;
    while(tmp != NULL)
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
    while(tmp != NULL)
    {
        if(!strcmp(key, tmp->key))
        {
            tmp->value1 = value1;
            tmp->value2 = *value2;
            tmp->value3 = *value3;
            return 0;
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
            if(prev!=NULL)
                prev->pNext = tmp->pNext;
            else
                pHead = tmp->pNext;
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
    return num;
}

