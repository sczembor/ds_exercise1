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
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

//STRUCTS ----------------------------
struct msgs {
    char  key[255];
    char  val1[255];
    char  val2[10];
    char   val3[10];
};

//MAIN -------------------------------- Funkce dělat -> otevřu, pošlu msg, zavřu
int main (int argc, char **argv)
{
    struct msgs mes1;
    char client_queue_name [64];
    mqd_t qd_server, qd_client;
    
    sprintf (client_queue_name, "/client-queue-%d", getpid ());
    
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
        printf("Please insert  the key:  (up to 255 char and confirm with /0 \n");
        n = readLine(0, buffer, MAX_MSG_SIZE);
        strcpy(mes1.key,buffer);
        printf("Please insert  value1:  (up to 255 char and confirm with /0 \n");
        n = readLine(0, buffer, MAX_MSG_SIZE);
        strcpy(mes1.val1,buffer);
        printf("Please insert  value2:  (1 integer confirm with /0 \n");
        strcpy(mes1.val2,buffer);
        printf("Please insert  the key:  (1 float confirm with /0 \n");
        strcpy(mes1.val3,buffer);
        if (n!=-1){
            int msg;
            msg=mq_send(qd_server,(const char *)&msg,MSG_BUFFER_SIZE,0);
            printf("Message sent: %s\n",buffer);
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

