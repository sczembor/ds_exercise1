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
#include "lib.h"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10
#define TRUE 1
#define FALSE 0

//STRUCTS ----------------------------
struct msgs {
    char  key[255];
    char  val1[255];
    char  val2[255];
    char   val3[255];
};

//GLOBALS -----------------------------
int busy;
int kill;
pthread_t thread;
pthread_attr_t thread_attr;
pthread_mutex_t mutex1;
pthread_cond_t signal1;

//FUNCTIONS  DECLARATIONS  -------------
void manage_request (mqd_t *s) {
    kill=FALSE;
    pthread_mutex_lock(&mutex1);
    struct msgs in_buffer;
    int n;
    mqd_t qd_server=*s;
    
    printf("thread connected as well GJ\n");
    busy=FALSE;
    pthread_cond_signal(&signal1);
    pthread_mutex_unlock(&mutex1);
    
    while (1){
        if (mq_receive (qd_server, (char*)&in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Server: mq_receive");
            exit (1);
        }
        
        printf ("Server: message received: %s,%s,%s,%s\n",in_buffer.key, in_buffer.val1, in_buffer.val2, in_buffer.val3);
        
    }
}

//MAIN --------------------------------------
int main(int argc, char **arv)
{
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&mutex1,NULL);
    pthread_cond_init(&signal1,NULL);
    
    mqd_t qd_server, qd_client;
    
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
    
    
    while(1){
        printf("creating  thread\n");
        pthread_create(&thread,&thread_attr,manage_request,&qd_server); //HERE!!!!!
        pthread_mutex_lock(&mutex1);
        while(busy==TRUE){
            pthread_cond_wait(&mutex1,&signal1);
        }
        pthread_mutex_unlock(&mutex1);
        busy=TRUE;
        if (kill==TRUE){
            exit(1);
        }
    }
    
    return 0;
}
