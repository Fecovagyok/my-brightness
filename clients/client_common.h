//
// Created by tumpek on 12/9/22.
//

#ifndef MYFIRST_CLIENT_COMMON_H
#define MYFIRST_CLIENT_COMMON_H

#include <mqueue.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

mqd_t create_ma_queue();
void on_queue_error();
void send_msg_queue(mqd_t);

#endif //MYFIRST_CLIENT_COMMON_H
