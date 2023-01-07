#include "serializer.h"

/**
 * @brief Check if the ring buffer is empty.
 * @param s The serializer.
 * @return True if the ring buffer is empty, false otherwise.
 */
static bool is_ring_buffer_empty(Serializer *s) { return s->head == s->tail; }

/**
 * @brief Check if the ring buffer is full.
 * @param s The serializer.
 * @return True if the ring buffer is full, false otherwise.
 */
static bool is_ring_buffer_full(Serializer *s) {
  return (s->head + 1) % s->depth == s->tail;
}

/**
 * @brief Initialize a serializer.
 * @param serializer The serializer.
 * @param depth The depth of the buffer
 * @param max_line_size The maximum size of a string in the serializer.
 */
void serializer_init(Serializer *serializer, int depth, int max_line_size) {
  // clear pointers
  serializer->head = 0;
  serializer->tail = 0;

  // set buffer descriptor
  serializer->depth = depth;
  serializer->max_line_size = max_line_size;

  // allocate memory for buffer
  serializer->_buffer = malloc(depth * max_line_size * sizeof(char));
}

/**
 * @brief Free a serializer.
 * @param serializer The serializer.
 */
void serializer_free(Serializer *serializer) { free(serializer->_buffer); }

/**
 * @brief Get a pointer to the next available string in the serializer.
 * @param serializer The serializer.
 * @return A pointer for writing a string.
 */
char *serializer_get_write_ptr(Serializer *ser) {
  if (is_ring_buffer_full(ser)) {
    return NULL;
  }
  // get pointer to free string
  int index = ser->head * ser->max_line_size;
  char *ptr = ser->_buffer + index;
  return ptr;
}

/**
 * @brief Commit a string to the serializer.
 * @param serializer The serializer.
 * @param size The size of the string.
 */
void serializer_commit_line(Serializer *ser, uint8_t size) {
  // add null terminator
  int index = ser->head * ser->max_line_size + size;
  ser->_buffer[index] = '\0';

  // increment head
  ser->head = (ser->head + 1) % ser->depth;
}

/**
 * @brief Get a pointer to the next string in the serializer.
 * @param serializer The serializer.
 * @return A pointer to the next string in the serializer.
 */
char *serializer_get_read_ptr(Serializer *ser) {
  if (is_ring_buffer_empty(ser)) {
    return NULL;
  }
  // get pointer to string
  int index = ser->tail * ser->max_line_size;
  char *data = ser->_buffer + index;

  // increment tail
  ser->tail = (ser->tail + 1) % ser->depth;
  return data;
}
