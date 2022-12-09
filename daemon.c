//
// Created by tumpek on 12/28/21.
//

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>

#define MAX_BRIGHTNESS 24000
#define UNIT MAX_BRIGHTNESS/10
#define READ_BYTES 5
#define QUEUE_NAME "/foggyossag"

int helyertekek[5] = {10000, 1000, 100, 10, 1};

int brightUp();
int brightDown();

void mySigHandler(int signal){
    if(mq_unlink(QUEUE_NAME) < 0){
        char error[30];
        snprintf(error, 20, "\ncant delete queue:\n%d", errno);
        write(1, error, strlen(error));
    }
    write(1, "\n", 1);
    exit(0);
}

void print(unsigned int number){
    char string[10];
    snprintf(string, 10, "%d\n", number);
    write(STDOUT_FILENO, string, strlen(string));
}

void error(){
    char error[10];
    snprintf(error, 10, "%d\n-1\n", errno);
    write(STDERR_FILENO, error, strlen(error));
}

int main(){

    if(signal(SIGINT, mySigHandler) < 0){
        write(1, "cant handle sigint\n", 19);
        return -1;
    }
    if(signal(SIGTERM, mySigHandler) < 0){
        write(1, "cant handle sigterm\n", 20);
        return -1;
    }

    mqd_t message_queue;
    ssize_t received;
    char message[10];

    struct mq_attr attr;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 3;

    message_queue = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, S_IRWXG | S_IRWXU, &attr);
    if(fchown(message_queue, getuid(), 1002) < 0){
        error();
        return 1;
    }
    if(message_queue < 0){
        error();
        return 2;
    }

    seteuid(getuid());

    while(1){
        received = mq_receive(message_queue, message, 3, NULL);
        if(received < 1){
            char error[10];
            snprintf(error, 10, "%d\n-3\n", errno);
            write(STDERR_FILENO, error, strlen(error));
            continue;
        }

        if(message[0] == 'u')
            brightUp();
        else if(message[0] == 'd')
            brightDown();
        else{
            write(STDERR_FILENO, "Unexpected message\n", 19);
            return -1;
        }

    }

    return 0;
}

int brightUp() {
    int bright_file;
    char contents[READ_BYTES+1];
    char read_bytes;
    int read_number;
    int result_number;
    char result[READ_BYTES+2];
    bright_file = open("/sys/class/backlight/intel_backlight/brightness", O_RDWR);

    if(bright_file < 0) {
        write(STDERR_FILENO, "-1\n", 3);
        return -1;
    }


    // Undefined behaviour, but tests show that it discards the higher bits, which is good
    read_bytes = read(bright_file, contents, READ_BYTES+1);
    read_number = 0;
    for (char i = 0; i < read_bytes-1; ++i) {
        read_number += (int)(contents[i]-'0') * helyertekek[i+READ_BYTES-(read_bytes-1)];
    }

    result_number = read_number + UNIT;
    if(result_number <= 0 || result_number > MAX_BRIGHTNESS) {
#if DEBUG==1
        char kalap[50];
        write(1, "1\n", 2);
        snprintf(kalap, 50, "%d\n%d\n", result_number, read_number);
        write(1, kalap, strlen(kalap));
#endif
        return -1;
    }

    snprintf(result, READ_BYTES+2, "%d\n", result_number);
    lseek(bright_file, 0, SEEK_SET);
    size_t otlet = strlen(result);
    write(bright_file, result, otlet);
    close(bright_file);
#if DEBUG==1
    write(1, result, otlet);
#endif
    return 0;
}

int brightDown() {
    int bright_file;
    char contents[READ_BYTES+1];
    char read_bytes;
    int read_number;
    int result_number;
    char result[READ_BYTES+2];
    bright_file = open("/sys/class/backlight/intel_backlight/brightness", O_RDWR);

    if(bright_file < 0) {
        write(STDERR_FILENO, "-1\n", 3);
        return -1;
    }


    // Undefined behaviour, but tests show that it discards the higher bits, which is good
    read_bytes = read(bright_file, contents, READ_BYTES+1);
    read_number = 0;
    for (char i = 0; i < read_bytes-1; ++i) {
        read_number += (int)(contents[i]-'0') * helyertekek[i+READ_BYTES-(read_bytes-1)];
    }

    result_number = read_number - UNIT;
    if(result_number <= 0 || result_number > MAX_BRIGHTNESS) {
#if DEBUG==1
        char kalap[50];
        write(1, "1\n", 2);
        snprintf(kalap, 50, "%d\n%d\n", result_number, read_number);
        write(1, kalap, strlen(kalap));
#endif
        return -1;
    }

    snprintf(result, READ_BYTES+2, "%d\n", result_number);
    lseek(bright_file, 0, SEEK_SET);
    size_t otlet = strlen(result);
    write(bright_file, result, otlet);
    close(bright_file);
#if DEBUG==1
    write(1, result, otlet);
#endif
    return 0;
}