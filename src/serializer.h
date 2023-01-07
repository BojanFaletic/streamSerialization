#pragma once

// enable C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/***** Configuration *****/
#define MAX_STRING_SIZE 32 // max string size
#define BUFFER_SIZE 2      // number of strings

/***** Types *****/

/** @brief Serializer stuct, acts as a ring buffer.
 * @note The ring buffer is implemented as a circular array.
 * @note Each string is null terminated.
 */
typedef struct {
  uint8_t head; // start of first string
  uint8_t tail; // end of last string
} Serializer;

/***** Functions *****/

// initialize serializer
void serializer_init(Serializer *serializer);

// add string to serializer
char *serializer_get_write_ptr(Serializer *serializer);
void serializer_commit_line(Serializer *serializer, uint8_t size);

// remove string from serializer
char *serializer_get_read_ptr(Serializer *serializer);

// enable C++ compatibility
#ifdef __cplusplus
}
#endif