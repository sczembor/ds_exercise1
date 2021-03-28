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
#define MAX_MSG_SIZE 2048
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
    
    
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    
    
    char buffer[MAX_MSG_SIZE],buffer1[MAX_MSG_SIZE];
    int err = 0;
    int n,check;
    while (err != -1) {
        printf("1. init()\n2.set_value()\n3.get_value()\n4.modify_value()\n5.delete_key()\n6.exsist()\n7.num_items()\nChoose one option(ex. \"1\"):");
        scanf("%i", &mes1.type);
        //printf("opening server queue\n");
        if ((qd_server = mq_open ("/server-queue", O_WRONLY)) == -1) {
            perror ("Client: mq_open (server)");
            exit (1);
        }
        //printf("opening client queue\n");
        sprintf (&mes1.queue_name, "/client_num-%d", getpid ());
        sprintf (client_queue_name, "%s",mes1.queue_name);
        if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
            perror ("Client: mq_open (client)");
            exit (1);
        }
        //printf("mes1.queue_name is %s\n",mes1.queue_name);

        switch (mes1.type) {
            case 1://init
                //printf("message sent");  //init function in the library
                check=init(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 2://set_value
                check=set_value(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 3://get_value
                check=get_value(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 4://modify value
                check=modify_value(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 5://delete_key
                check=delete_key(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 6://exsist
                check=exist(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            case 7://num_items
                check=num_items(qd_server,qd_client,mes1,attr);
                printf("function returned:%i\n",check);
                break;
            default:
                perror ("Client: Invalid Arguemnt(function)");
                exit (1);
                break;
        }
        mq_close(qd_server);
        mq_close(qd_client);
        //ENDIN HERE ------------------------------------------------
    }
    
    if (mq_unlink (client_queue_name) == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    mq_close(client_queue_name);
    printf ("Client: bye\n");
    
    return 0;
}

