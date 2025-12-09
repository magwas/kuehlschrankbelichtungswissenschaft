#include "gtest/gtest.h"
#include "MessageQueueTestFixture.h"

TEST_F(MessageQueueTest, add_increases_the_tail) {
    when(QUEUE_IS_EMPTY);
    systemQueue.send(&(testMessages[0]));
    ASSERT_EQ(1, getTail());
}

TEST_F(MessageQueueTest, add_leaves_the_head_alone) {
    when(QUEUE_IS_EMPTY);
    systemQueue.send(&(testMessages[0]));
    ASSERT_EQ(0, getHead());
}

TEST_F(MessageQueueTest, add_sets_the_type_correctly) {
    when(QUEUE_IS_EMPTY);
    systemQueue.send(&(testMessages[1]));
    ASSERT_EQ(MessageType::Serial,get(0).type);
}

TEST_F(MessageQueueTest, add_sets_the_payload_correctly) {
    when(QUEUE_IS_EMPTY);
    systemQueue.send(&(testMessages[1]));
    char * payload = get(0).payload;
    ASSERT_STREQ("message two",payload);
}

TEST_F(MessageQueueTest, add_correctly_wraps_when_reaches_table_end) {
    when(BEFORE_WRAPAROUND);
    systemQueue.send(&(testMessages[0]));
    ASSERT_EQ(0, getTail());
}
