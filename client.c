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
#define MAX_MESSAGES 50
#define MAX_MSG_SIZE 1024
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

//STRUCTS ----------------------------
struct msgs {
    char  key[10];
    char  val1[10];
    int   val2;
    float  val3;
};

//MAIN -------------------------------- Funkce dělat -> otevřu, pošlu msg, zavřu
int main (int argc, char **argv)
{
    struct msgs mes1;
    char client_queue_name [64];
    mqd_t qd_server, qd_client;
    
    sprintf (client_queue_name, "/client-queue");
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Client: mq_open (client)");
        exit (1);
    }
    
    if ((qd_server = mq_open ("/server-queue", O_WRONLY)) == -1) {
        perror ("Client: mq_open (server)");
        exit (1);
    }
    
    char buffer[MAX_MSG_SIZE],buffer1[MAX_MSG_SIZE];
    int err = 0;
    int n;
    while (err != -1) {
        scanf("%s %s %i %f", mes1.key, mes1.val1, &mes1.val2, &mes1.val3);
        if (n!=-1){
            int msg;
            msg=mq_send(qd_server,(const char *)&mes1,sizeof(mes1)+1,0);
            printf("Message sent: %s\n",mes1);
            if (msg < 0) {
                perror("Error in sending msg");
                exit(1);
            }
        }
    }
    
    
    if (mq_close (qd_client) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }
    
    if (mq_unlink (client_queue_name) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    printf ("Client: bye\n");
    
    return 0;
}

