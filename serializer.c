#include "serializer.h"

// ring buffer, each string is null terminated
static char buffer[BUFFER_SIZE][MAX_STRING_SIZE];

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
  return (s->head + 1) % BUFFER_SIZE == s->tail;
}

/**
 * @brief Initialize the serializer.
 * @param serializer The serializer.
 */
void serializer_init(Serializer *serializer) {
  // clear pointers
  serializer->head = 0;
  serializer->tail = 0;
}

/**
 * @brief Get a pointer to the next available string in the serializer.
 * @param serializer The serializer.
 * @return A pointer for writing a string.
 */
char *serializer_get_write_ptr(Serializer *serializer) {
  if (is_ring_buffer_full(serializer)) {
    return NULL;
  }
  // get pointer to free string
  char *ptr = buffer[serializer->head];
  return ptr;
}

/**
 * @brief Commit a string to the serializer.
 * @param serializer The serializer.
 * @param size The size of the string.
 */
void serializer_commit_line(Serializer *serializer, uint8_t size) {
  // add null terminator
  buffer[serializer->head][size] = '\0';

  // increment head
  serializer->head = (serializer->head + 1) % BUFFER_SIZE;
}

/**
 * @brief Get a pointer to the next string in the serializer.
 * @param serializer The serializer.
 * @return A pointer to the next string in the serializer.
 */
char *serializer_get_read_ptr(Serializer *serializer) {
  if (is_ring_buffer_empty(serializer)) {
    return NULL;
  }
  // get pointer to string
  char *data = buffer[serializer->tail];

  // increment tail
  serializer->tail = (serializer->tail + 1) % BUFFER_SIZE;
  return data;
}
