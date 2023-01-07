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
 * @param depth The depth of the serializer.
 * @param max_line_size The maximum size of a line.
 * @return 0 on success, -1 on failure.
 */
uint8_t serializer_init(Serializer *serializer, int depth, int max_line_size) {
  // set buffer descriptor
  serializer->max_line_size = max_line_size;
  serializer->depth = depth;

  // allocate memory for buffer
  serializer->_buffer = malloc(depth * max_line_size * sizeof(char));
  if (serializer->_buffer == NULL) {
    return SERIALIZER_MEMORY_ERROR;
  }

  // clear pointers
  serializer->head = 0;
  serializer->tail = 0;
  serializer->write_index = 0;

  return SERIALIZER_OK;
}

/**
 * @brief Free a serializer.
 * @param serializer The serializer.
 */
void serializer_free(Serializer *serializer) { free(serializer->_buffer); }

/**
 * @brief Get a pointer to the next free string in the serializer.
 * @param serializer The serializer.
 * @return A pointer to the next free string in the serializer.
 */
static char *serializer_get_write_ptr(Serializer *ser) {
  if (is_ring_buffer_full(ser)) {
    return NULL;
  }
  // get pointer to free string
  int index = ser->head * ser->max_line_size;
  char *ptr = ser->_buffer + index;

  return ptr;
}

/**
 * @brief Write data to the serializer.
 * @param serializer The serializer.
 * @return A pointer to the next string in the serializer.
 */
uint8_t serializer_write_data(Serializer *serializer, char *data, int size) {
  char *ptr = serializer_get_write_ptr(serializer);
  if (ptr == NULL) {
    return SERIALIZER_WRITER_FULL;
  }

  // check if we have enough space
  if (serializer->write_index + size > serializer->max_line_size) {
    serializer->write_index = 0;
    return SERIALIZER_BUFFER_OVERFLOW;
  }

  // write data
  memcpy(ptr + serializer->write_index, data, size);
  serializer->write_index += size;
  return SERIALIZER_OK;
}

/**
 * @brief Commit a line to the serializer.
 * @param serializer The serializer.
 * @return 0 on success, -1 on failure.
 */
uint8_t serializer_commit_line(Serializer *ser) {
  if (is_ring_buffer_full(ser)) {
    return SERIALIZER_WRITER_FULL;
  }

  // add null terminator
  int index = ser->head * ser->max_line_size + ser->write_index;
  ser->_buffer[index] = '\0';

  // increment head
  ser->head = (ser->head + 1) % ser->depth;
  ser->write_index = 0;
  return SERIALIZER_OK;
}

/**
 * @brief Read a line from the serializer.
 * @param serializer The serializer.
 * @return A pointer to the read line.
 */
char *serializer_read_data(Serializer *ser) {
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
