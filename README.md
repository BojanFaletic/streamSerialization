# StreamSerialization

This is a simple library for serializing byte streams to lines.
It is useful for reading lines from a stream, such as a serial data stream.

It is designed to be used in embedded systems, where efficiency is important.

Communication is zero-copy, meaning that no data is copied, only pointers are used.

Main features:
* Zero copy (no copying of data, only pointers are used)
* Written in C
* Single dynamic memory allocation
* No dependencies

## Warning
This library is not thread safe. It is up to the user to ensure that only one thread is reading and writing to the serializer at a time.


## Configuration
The serializer is configured during initialization by passing a pointer to a `Serializer` struct, and a buffer to use for the serializer.

```c
Serial s;
int main() {
  serializer_init(&s, 2, 32);
  // ..
  serializer_free(&s);
  return 0;
}
```

The first argument is a pointer to the `Serializer` struct. This is used to store the state of the serializer. The second argument is the number of lines to store in the buffer. The third argument is the maximum length of a line.

## Example

Simple example of how to use the serializer. The serializer is used to read lines from a serial port, and print them to the console.

```c
#include "serializer.h"
#include <semaphore.h>

Serial s;
sem_t sem;

void RxCallback(uint8_t *data, uint16_t len) {
  for (int i = 0; i < len; i++) {
    if (data[i] == '\n'){
      // Write newline to serializer
      serializer_commit_line(&s);

      // Signal consumer thread that a line is ready
      sem_post(&sem);
    }
    else {
      // Write data to serializer
      serializer_write_data(&s, data+i, 1);
    }
  }
}

void consumer() {
  while (1) {
    // Wait for a line to be ready
    sem_wait(&sem);

    // Read line from serializer
    char* line = serializer_read_data(&s);
    printf("Got line: %s\n", line);
  }
}
```

Full example can be found `example.c`.

## Running the example

To run the example, you need to have cmake and make installed.

```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ ./serialize
```