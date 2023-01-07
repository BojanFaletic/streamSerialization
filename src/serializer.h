#pragma once

// enable C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/***** Types *****/

/** @brief Serializer object */
typedef struct {
  int max_line_size; // max string size
  int depth;         // number of strings
  char *_buffer;     // buffer
  uint8_t head;      // start of first string
  uint8_t tail;      // end of last string
} Serializer;

/***** Functions *****/

// initialize serializer, must be called before use
void serializer_init(Serializer *serializer, int depth, int max_line_size);
void serializer_free(Serializer *serializer);


// add string to serializer
char *serializer_get_write_ptr(Serializer *serializer);
void serializer_commit_line(Serializer *serializer, uint8_t size);

// remove string from serializer
char *serializer_get_read_ptr(Serializer *serializer);

// enable C++ compatibility
#ifdef __cplusplus
}
#endif