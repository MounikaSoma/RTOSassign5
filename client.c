#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//Every message has a type and text.Hence, we shall define a structure for it.
struct msg_buffer { 
    long msg_type;
    char msg_text[200];
};

int main(void)
{
    struct msg_buffer buf;
    int msg_queue_id; //ID for the message queue .
        key_t key; //a key to identify a particular queue uniquely using msgget()
    if ((key = ftok("client.c", 'C')) == -1) {
        perror("ftok");	
        exit(1);
    }

    if ((msg_queue_id = msgget(key, 0644)) == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Enter text:\n");

    buf.msg_type = getpid(); 
    printf("<number> <operator(+,-,*,/,^ only)> <number>:\n");
    while(fgets(buf.msg_text, sizeof buf.msg_text, stdin) != NULL) {
        int len = strlen(buf.msg_text);

        
        if (buf.msg_text[len-1] == '\n') buf.msg_text[len-1] = '\0';

        if (msgsnd(msg_queue_id, &buf, len+1, 0) == -1) 
            perror("msgsnd");
        if (strcmp(buf.msg_text, "exit")==0){
            perror("msgsnd");
            exit(1);
        }

    }
    
    if (msgctl(msg_queue_id, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
