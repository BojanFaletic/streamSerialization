### StreamSerialization

This is a simple library for serializing byte streams to lines.
It is useful for reading lines from a stream, such as a serial data stream.

It is designed to be used in embedded systems, where efficiency is important.

Communication is zero-copy, meaning that no data is copied, only pointers are used.

Main features:
* Zero copy (no copying of data, only pointers are used)
* Written in C
* No dynamic memory allocation
* No dependencies


#### Example

```c
#include "serializer.h"
#include <semaphore.h>

Serial s;
sem_t sem;

void RxCallback(uint8_t *data, uint16_t len) {
  static char* line = NULL;
  static uint8_t line_len;

  if (line == NULL){
    line = serializer_get_write_ptr(&s);
    line_len = 0;
  }

  for (int i = 0; i < len; i++) {
    if (data[i] == '\n'){
      serializer_commit(&s, line_len);
      line = serializer_get_write_ptr(&s);
      line_len = 0;
      sem_post(&sem);
    }
    else {
      line[line_len] = data[i];
      line_len++;
      line_len = line_len % MAX_STRING_SIZE;
    }
  }
}

void consumer() {
  while (1) {
    sem_wait(&sem);
    char* line = serializer_get_read_ptr(&s);
    printf("Got line: %s\n", line);
  }
}

```
