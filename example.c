#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "serializer.h"

// fake serial port
uint8_t sendBuffer[] =
    "Long line for testing\nShort line\nTest\nToo long line for testing\n";
static uint8_t send_ptr = 0;

uint8_t recvByte(void) {
  uint8_t tmp = sendBuffer[send_ptr++];
  send_ptr %= sizeof(sendBuffer) - 1;
  return tmp;
}

// panic (for testing)
void panic(void) {
  fprintf(stderr, "PANIC\n");
  exit(1);
}

Serializer s;
sem_t sem;

// fake interrupt handler
void RxInterruptHandler(void) {
  char byte = recvByte();

  if (byte != '\n') {
    // write byte
    serializer_write_data(&s, &byte, 1);
  } else {
    // commit line
    serializer_commit_line(&s);

    // signal consumer
    sem_post(&sem);
  }
}

// simulate RX interrupt
void produce(void) {
  while (1) {
    RxInterruptHandler();

    // simulate 100Hz
    usleep(10000);
  }
}

// consumer
void consume(void) {
  while (1) {
    // wait for signal
    sem_wait(&sem);

    // get line
    char *line = serializer_read_data(&s);
    if (line == NULL) {
      fprintf(stderr, "line is NULL\n");
      panic();
    }
    printf("%s\n", line);

    // free line
    serializer_free_line(&s);
  }
}

int main() {
  // init semaphore
  sem_init(&sem, 0, 0);

  // init serializer
  serializer_init(&s, 2, 32);

  // threads
  pthread_t producer;
  pthread_t consumer;

  // start threads
  pthread_create(&producer, NULL, (void *)produce, NULL);
  pthread_create(&consumer, NULL, (void *)consume, NULL);

  // wait for threads
  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  // free serializer
  serializer_free(&s);
  return 0;
}