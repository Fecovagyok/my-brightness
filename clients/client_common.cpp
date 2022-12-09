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

void send_msg_queue(mqd_t message_queue){

}

void on_queue_error() {
    char error[ERROR_BUFF_SIZE];
    snprintf(error, ERROR_BUFF_SIZE, "Bright-up: queue_error: \n%d\n", errno);
    write(STDERR_FILENO, error, strlen(error));
}
