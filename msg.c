#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// FIFO includes
#include <sys/stat.h>
#include <fcntl.h>

// Message Queue includes
#include <sys/ipc.h>
#include <sys/msg.h>

// Shared Memory includes
#include <sys/shm.h>

#define MSG_SIZE 100
#define SHM_KEY 1234

// Message structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    pid_t pid;
    char msg[] = "Hello IPC!";
    char buffer[MSG_SIZE];
 printf("\n--- MESSAGE QUEUE ---\n");
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    pid = fork();
    if (pid > 0) {
        struct msg_buffer msgbuf;
        msgbuf.msg_type = 1;
        strcpy(msgbuf.msg_text, msg);
        msgsnd(msgid, &msgbuf, sizeof(msgbuf.msg_text), 0);
        wait(NULL);
    } else {
        struct msg_buffer msgbuf;
        msgrcv(msgid, &msgbuf, sizeof(msgbuf.msg_text), 1, 0);
        printf("Child received via MSG QUEUE: %s\n", msgbuf.msg_text);
        msgctl(msgid, IPC_RMID, NULL);
        exit(0);
    }


