//
// Created by tumpek on 12/9/22.
//

#include "client_common.h"

#define QUEUE_NAME "/foggyossag"
#define ERROR_BUFF_SIZE 30

// Has no significance I just chose one
#define MSG_PRIORITY 19

mqd_t create_ma_queue(){
    return mq_open(QUEUE_NAME, O_WRONLY);
}

int send_msg_queue(mqd_t message_queue, const char* message){
    return mq_send(message_queue, message, 1, MSG_PRIORITY);
}

void on_queue_error(const char* activity) {
    char error[ERROR_BUFF_SIZE];
    snprintf(error, ERROR_BUFF_SIZE, "Bright-up: %s: \n%d\n", activity, errno);
    write(STDERR_FILENO, error, strlen(error));
}

void client_code(const char *message) {
    mqd_t message_queue;
    message_queue = create_ma_queue();
    if(message_queue < 0){
        on_queue_error("creating queue failed");
        exit(1);
    }

    if(send_msg_queue(message_queue, message) < 0){
        on_queue_error("sending message failed");
        exit(1);
    }
}
