#pragma once

// enable C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/***** Error Code *****/
#define SERIALIZER_WRITER_FULL -2     // buffer full
#define SERIALIZER_READER_EMPTY -3    // buffer empty
#define SERIALIZER_BUFFER_OVERFLOW -4 // buffer overflow
#define SERIALIZER_MEMORY_ERROR -5    // memory error
#define SERIALIZER_OK 0               // no error

/***** Types *****/

/** @brief Serializer object */
typedef struct {
  int max_line_size; // max string size
  int depth;         // number of strings
  char *_buffer;     // buffer
  int write_index;   // index of next string to write
  int head;          // start of first string
  int tail;          // end of last string
} Serializer;

/***** Functions *****/

// initialize serializer, must be called before use
uint8_t serializer_init(Serializer *serializer, int depth, int max_line_size);
void serializer_free(Serializer *serializer);

// add string to serializer
uint8_t serializer_write_data(Serializer *serializer, char *data, int size);
uint8_t serializer_commit_line(Serializer *ser);

// remove string from serializer
char *serializer_read_data(Serializer *serializer);

// enable C++ compatibility
#ifdef __cplusplus
}
#endif