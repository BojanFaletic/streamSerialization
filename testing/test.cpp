#include "serializer.h"
#include <gtest/gtest.h>

TEST(Serializer, init) {
  Serializer s;
  serializer_init(&s);
  EXPECT_EQ(s.head, 0);
  EXPECT_EQ(s.tail, 0);
}

TEST(Serializer, push) {
  Serializer s;
  serializer_init(&s);
  char *ptr = serializer_get_write_ptr(&s);
  sprintf(ptr, "Hello World");
  serializer_commit_line(&s, strlen(ptr));
  EXPECT_EQ(s.head, 1);
  EXPECT_EQ(s.tail, 0);
}

TEST(Serializer, pop) {
  Serializer s;
  serializer_init(&s);
  char *ptr = serializer_get_write_ptr(&s);
  sprintf(ptr, "Hello World");
  serializer_commit_line(&s, strlen(ptr));
  char *line = serializer_get_read_ptr(&s);
  EXPECT_EQ(s.head, 1);
  EXPECT_EQ(s.tail, 1);
  EXPECT_STREQ(line, "Hello World");
}

TEST(Serializer, multiple) {
  Serializer s;
  serializer_init(&s);
  char *ptr = serializer_get_write_ptr(&s);
  if (ptr != NULL) {
    sprintf(ptr, "Hello World 1");
    serializer_commit_line(&s, strlen(ptr));
  }
  ptr = serializer_get_write_ptr(&s);
  if (ptr != NULL) {
    sprintf(ptr, "Hello World 2");
    serializer_commit_line(&s, strlen(ptr));
  }
  ptr = serializer_get_write_ptr(&s);
  if (ptr != NULL) {
    sprintf(ptr, "Hello World 3");
    serializer_commit_line(&s, strlen(ptr));
  }
  EXPECT_STREQ(serializer_get_read_ptr(&s), "Hello World 1");
  EXPECT_STREQ(serializer_get_read_ptr(&s), NULL);
  EXPECT_STREQ(serializer_get_read_ptr(&s), NULL);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}