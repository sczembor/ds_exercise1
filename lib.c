//
//  lib.c
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
#include "lib.h"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 2048
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

struct msgs{
    int type;
    char* key;
    char* val1;
    int val2;
    float val3;
    char queue_name[64];
    struct msgs* pNext;
};

int init(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    
    msg.type=1;
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
            
            //printf("the number of new messages is %i",m);
            
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}

int set_value(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    msg.type=2;
    
    printf("key:");
    scanf("%s",&msg.key);
    printf("value1:");
    scanf("%s",&msg.val1);
    printf("value2:");
    scanf("%d",&msg.val2);
    printf("value3:");
    scanf("%f",&msg.val3);
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
    
}

int get_value(mqd_t qd_server,mqd_t qd_client,struct msgs msg, struct mq_attr attr){
    
    msg.type=3;
    
    printf("key:");
    scanf("%s",&msg.key);
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}

int modify_value(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    msg.type=4;
    
    printf("key:");
    scanf("%s",&msg.key);
    printf("value1:");
    scanf("%s",&msg.val1);
    printf("value2:");
    scanf("%d",&msg.val2);
    printf("value3:");
    scanf("%f",&msg.val3);
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}

int delete_key(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    msg.type=5;
    
    printf("key:");
    scanf("%s",&msg.key);
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}

int exist(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    msg.type=6;
    
    printf("key:");
    scanf("%s",&msg.key);
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}

int num_items(mqd_t qd_server,mqd_t qd_client,struct msgs msg,struct mq_attr attr){
    
    msg.type=7;
    
    int message;
    printf("Sending message\n");
    message=mq_send(qd_server,(const char *)&msg,sizeof(msg)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&msg.key, &msg.val1, msg.val2, msg.val3);
    if (message < 0) {
        perror("Error in sending msg");
        return (-1);
    }
    while(1){
        if (mq_getattr(qd_client, &attr) == -1){
            perror("mq_getattr");
            return (-1);
        }
        int m=attr.mq_curmsgs;
        
        //printf("the number of new messages is %i",m);
        
        if (m>0){
            struct msgs in_buffer;
            if (mq_receive (qd_client, (char*)&msg, MAX_MSG_SIZE, NULL) == -1) {
                perror ("Server: mq_receive");
                return (-1);
            }
            return msg.type;
        }
    }
    return -1;
}


ssize_t readLine(int fd, void *buffer, size_t n)
{
    ssize_t numRead;  /* num of bytes fetched by last read() */
    size_t totRead;      /* total bytes read so far */
    char *buf;
    char ch;
    
    
    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    buf = buffer;
    totRead = 0;
    
    for (;;) {
        numRead = read(fd, &ch, 1);    /* read a byte */
        
        if (numRead == -1) {
            if (errno == EINTR)    /* interrupted -> restart read() */
                continue;
            else
                return -1;        /* some other error */
        } else if (numRead == 0) {    /* EOF */
            if (totRead == 0)    /* no byres read; return 0 */
                return 0;
            else
                break;
        } else {            /* numRead must be 1 if we get here*/
            if (ch == '\n')
                break;
            if (ch == '\0')
                break;
            if (totRead < n - 1) {        /* discard > (n-1) bytes */
                totRead++;
                *buf++ = ch;
            }
        }
    }
    
    *buf = '\0';
    return totRead;
}
