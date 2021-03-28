//
//  lib.c
//  
//
//  Created by Jan Ferbr on 18/03/2021.
//
#include <unistd.h>
#include <errno.h>
#include "lib.h"

struct msgs{
    int type;
    char* key;
    char* val1;
    int val2;
    float val3;
    char queue_name[64];
    struct msgs* pNext;
};

int init(mqd_t qd_server,mqd_t qd client){
    struct msgs msg;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    msg.type=1;
    
    int msg;
    printf("Sending message\n");
    msg=mq_send(qd_server,(const char *)&mes1,sizeof(mes1)+1,0);
    printf ("Client: message sent: %s,%s,%d,%f\n",&mes1.key, &mes1.val1, mes1.val2, mes1.val3);
    if (msg < 0) {
        perror("Error in sending msg");
        exit(1);
    }
    int mes_rec=0;
    while(mes_rec==0){
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
            if (i==m-1){
                mes_rec=1;
            }
        }
    }
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
