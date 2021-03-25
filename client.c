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
    char queue_name[255];
    struct msgs* pNext;
};

//MAIN -------------------------------- Funkce dělat -> otevřu, pošlu msg, zavřu
int main (int argc, char **argv)
{
    struct msgs mes1;
    char client_queue_name [64];
    mqd_t qd_server, qd_client;
    
    //sprintf (client_queue_name, "/client-queue");
    printf("Please input a unique name for the client queue in the form of /queue_name :\n");
    scanf("%s", &mes1.queue_name);
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
        scanf("key:%s value1:%s value2:%i value3:%f", &mes1.key, &mes1.val1, &mes1.val2, &mes1.val3);
        if (n!=-1){
            int msg;
            printf("Sending message\n");
            msg=mq_send(qd_server,(const char *)&mes1,sizeof(mes1)+1,0);
            printf ("Client: message sent: %s,%s,%i,%f\n",&mes1.key, &mes1.val1, mes1.val2, mes1.val3);
            if (msg < 0) {
                perror("Error in sending msg");
                exit(1);
            }
            int m=attr.mq_curmsgs;
            for (int i=0;i<m;i++){
                struct msgs in_buffer;
                if (mq_receive (qd_client, (char*)&in_buffer, MAX_MSG_SIZE, NULL) == -1) {
                    perror ("Server: mq_receive");
                    exit (1);
                }
                printf ("Client: message received: %s,%s,%i,%f\n",&in_buffer.key, &in_buffer.val1, in_buffer.val2, in_buffer.val3);
            }
        }
    }
    
    if (mq_unlink (client_queue_name) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: bye\n");
    
    return 0;
}

