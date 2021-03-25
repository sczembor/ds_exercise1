//
//  Client.c
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
#define MAX_MSG_SIZE 1024
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

//STRUCTS ----------------------------
struct msgs{
    int type;
    char* key;
    char* val1;
    int val2;
    float val3;
    char queue_name[64];
    struct msgs* pNext;
};

//MAIN -------------------------------- Funkce dělat -> otevřu, pošlu msg, zavřu
int main (int argc, char **argv)
{
    struct msgs mes1;
    char client_queue_name [64];
    mqd_t qd_server, qd_client;
    
    sprintf (&mes1.queue_name, "/client_num-%d", getpid ());
    sprintf (client_queue_name, "%s",&mes1.queue_name);
    
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    printf("opening client queue\n");
    if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    
    printf("opening server queue\n");
    if ((qd_server = mq_open ("/server-queue", O_WRONLY)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }
    
    char buffer[MAX_MSG_SIZE],buffer1[MAX_MSG_SIZE];
    int err = 0;
    int n;
    while (err != -1) {
        printf("1. init()\n2.set_value()\n3.get_value()\n4.modify_value()\n5.delete_key()\n6.exsist()\n7.num_items()\nChoose one option(ex. \"1\"):");
        scanf("%i", &mes1.type);
        switch (mes1.type) {
            case 1://init
                printf("message sent");
                break;
            case 2://set_value
                printf("key:");
                scanf("%s",&mes1.key);
                printf("value1:");
                scanf("%s",&mes1.val1);
                printf("value2:");
                scanf("%d",&mes1.val2);
                printf("value3:");
                scanf("%f",&mes1.val3);
                break;
            case 3://get_value
                printf("key:");
                scanf("%s",&mes1.key);
            case 4://modify value
                printf("key:");
                scanf("%s",&mes1.key);
                printf("value1:");
                scanf("%s",&mes1.val1);
                printf("value2:");
                scanf("%d",&mes1.val2);
                printf("value3:");
                scanf("%f",&mes1.val3);
            case 5://delete_key
                printf("key:");
                scanf("%s",&mes1.key);
            case 6://exsist
                printf("key:");
                scanf("%s",&mes1.key);
            case 7://num_items
                break;
            default:
                perror ("Client: Invalid Arguemnt(function)");
                exit (1);
                break;
        }
        //scanf("%s %s %i %f", &mes1.key, &mes1.val1, &mes1.val2, &mes1.val3);
        if (n!=-1){
            int msg;
            printf("Sending message\n");
            msg=mq_send(qd_server,(const char *)&mes1,sizeof(mes1)+1,0);
            printf ("Client: message sent: %s,%s,%d,%f\n",&mes1.key, &mes1.val1, mes1.val2, mes1.val3);
            if (msg < 0) {
                perror("Error in sending msg");
                exit(1);
            }
            if (mq_getattr(qd_client, &attr) == -1)
                perror("mq_getattr");
            int m=attr.mq_curmsgs;
            
            //printf("the number of new messages is %i",m);
            
            for (int i=0;i<m;i++){
                struct msgs in_buffer;
                if (mq_receive (qd_client, (char*)&in_buffer, MAX_MSG_SIZE, NULL) == -1) {
                    perror ("Server: mq_receive");
                    exit (1);
                }
                printf ("Client: message received: type:%i, %s,%s,%i,%f\n",in_buffer.type, &in_buffer.key, &in_buffer.val1, in_buffer.val2, in_buffer.val3);
            }
        }
    }
    
    if (mq_unlink (client_queue_name) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    mq_close(client_queue_name);
    printf ("Client: bye\n");
    
    return 0;
}

