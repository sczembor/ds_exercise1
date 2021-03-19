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
int busy;
int kill;
pthread_t thread;
pthread_attr_t thread_attr;
pthread_mutex_t mutex1;
pthread_cond_t signal1;
void manage_request (mqd_t *s) {
    kill=FALSE;
    pthread_mutex_lock(&mutex1);
    char in_buffer[MAX_MSG_SIZE];
    int n;
    mqd_t qd_server=*s;
    
    printf("thread connected as well GJ\n");
    busy=FALSE;
    pthread_cond_signal(&signal1);
    pthread_mutex_unlock(&mutex1);
    
    while (1){
        if (mq_receive (qd_server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Server: mq_receive");
            exit (1);
        }
        
        printf ("Server: message received: %s\n",in_buffer);
        
    }
}

int main(int argc, char **arv)
{
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_mutex_init(&mutex1,NULL);
    pthread_cond_init(&signal1,NULL);
    
    mqd_t qd_server, qd_client;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    if ((qd_server = mq_open ("/server-queue", O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    
    
    while(1){
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
