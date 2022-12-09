#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>

#define MAX_BRIGHTNESS 24000
#define UNIT MAX_BRIGHTNESS/10
#define READ_BYTES 5
#define QUEUE_NAME "/foggyossag"

// Lul ennek semmi értelme

int helyertekek[5] = {10000, 1000, 100, 10, 1};

int brightUp();
int brightDown();
void* messageClient();




int main() {
    mqd_t message_queue;
    pthread_t client_thread;
    ssize_t received;
    char message[50];

    struct mq_attr attr;
    attr.mq_maxmsg = 2;
    attr.mq_msgsize = 10;

    message_queue = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, S_IRWXU, &attr);
    if(message_queue < 0){
        char error[7];
        snprintf(error, 7, "%d\n-1\n", errno);
        write(1, error, 7);
        return -1;
    }
    if(pthread_create(&client_thread, NULL, messageClient, NULL) != 0) {
        write(1, "-2\n", 3);
        return -2;
    }

    received = mq_receive(message_queue, message, 10, NULL);
    if(received < 0){
        char error[7];
        snprintf(error, 7, "%d\n-3\n", errno);
        write(1, error, strlen(error));
        //return -3;
    }
    write(1, message, received);

    if(mq_close(message_queue) < 0){
        write(1, "-5\n", 3);
        return -5;
    }

    if(mq_unlink(QUEUE_NAME) < 0){
        write(1, "-4\n", 3);
        return -4;
    }

    return 0;
}

void* messageClient(){
    mqd_t message_queue;

    message_queue = mq_open(QUEUE_NAME, O_WRONLY);
    if(message_queue < 0){
        write(1, "child-1\n", 8);
        pthread_exit(1);
    }

    if(mq_send(message_queue, "bright-upp", 10, 19) < 0){
        char error[20];
        snprintf(error, 20, "child:\n%d\n-2\n", errno);
        write(1, error, strlen(error));
        pthread_exit(2);
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
        write(1, "-1\n", 3);
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
        char kalap[50];
        write(1, "1\n", 2);
        snprintf(kalap, 50, "%d\n%d\n", result_number, read_number);
        write(1, kalap, strlen(kalap));
        return 1;
    }

    snprintf(result, READ_BYTES+2, "%d\n", result_number);
    lseek(bright_file, 0, SEEK_SET);
    size_t otlet = strlen(result);
    write(bright_file, result, otlet);
    close(bright_file);
    write(1, result, otlet);
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
        write(1, "-1\n", 3);
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
        char kalap[50];
        write(1, "1\n", 2);
        snprintf(kalap, 50, "%d\n%d\n", result_number, read_number);
        write(1, kalap, strlen(kalap));
        return 1;
    }

    snprintf(result, READ_BYTES+2, "%d\n", result_number);
    lseek(bright_file, 0, SEEK_SET);
    size_t otlet = strlen(result);
    write(bright_file, result, otlet);
    close(bright_file);
    write(1, result, otlet);
    return 0;
}
