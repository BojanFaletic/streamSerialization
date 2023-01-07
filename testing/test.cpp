#include "serializer.h"
#include <gtest/gtest.h>

TEST(Serializer, init) {
  Serializer s;
  serializer_init(&s, 10, 10);
  EXPECT_EQ(s.head, 0);
  EXPECT_EQ(s.tail, 0);
}

TEST(Serializer, push) {
  Serializer s;
  serializer_init(&s, 10, 10);
  const char *data = "Hello World";
  serializer_write_data(&s, (char *)data, strlen(data));
  serializer_commit_line(&s);
  EXPECT_EQ(s.head, 1);
  EXPECT_EQ(s.tail, 0);
}

TEST(Serializer, pop) {
  Serializer s;
  serializer_init(&s, 10, 16);
  const char *data = "Hello World";
  int status;

  status = serializer_write_data(&s, (char *)data, strlen(data));
  EXPECT_EQ(status, SERIALIZER_OK);

  status = serializer_commit_line(&s);
  EXPECT_EQ(status, SERIALIZER_OK);

  char *line = serializer_read_data(&s);
  EXPECT_EQ(s.head, 1);
  EXPECT_EQ(s.tail, 1);
  EXPECT_STREQ(line, "Hello World");
}

TEST(Serializer, multiple) {
  Serializer s;
  serializer_init(&s, 2, 16);

  char *data = (char*)"Hello World 1";
  int status = serializer_write_data(&s, (char *)data, strlen(data));
  EXPECT_EQ(status, SERIALIZER_OK);
  serializer_commit_line(&s);

  data = (char*)"Hello World 2";
  serializer_write_data(&s, (char *)data, strlen(data));
  serializer_commit_line(&s);

  data = (char*)"Hello World 3";
  serializer_write_data(&s, (char *)data, strlen(data));
  serializer_commit_line(&s);

  EXPECT_STREQ(serializer_read_data(&s), "Hello World 1");
  EXPECT_STREQ(serializer_read_data(&s), NULL);
  EXPECT_STREQ(serializer_read_data(&s), NULL);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}