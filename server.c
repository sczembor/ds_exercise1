//
//  Server.c
//  
//
//  Created by Jan Ferbr on 18/03/2021.
//

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "lib.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main(int argc, char **arv)
{
    mqd_t qd_server, qd_client;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    if ((qd_server = mq_open ("example-server", O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit (1);
    }
    
    int mq_close (SERVER_QUEUE_NAME);
    
    return 1;
}
