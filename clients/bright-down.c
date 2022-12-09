//
// Created by tumpek on 12/28/21.
//

#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define QUEUE_NAME "/foggyossag"

int main(){
    mqd_t message_queue;
    const char* const message = "d";
    message_queue = mq_open(QUEUE_NAME, O_WRONLY);
    if(message_queue < 0){
        char error[30];
        snprintf(error, 30, "Bright-up: -1\n%d\n", errno);
        write(STDERR_FILENO, error, strlen(error));
        return -1;
    }

    if(mq_send(message_queue, message, 1, 19) < 0){
        char error[30];
        snprintf(error, 30, "Bright-up: -2\n%d\n", errno);
        write(STDERR_FILENO, error, strlen(error));
        return -1;
    }

    return 0;
}


