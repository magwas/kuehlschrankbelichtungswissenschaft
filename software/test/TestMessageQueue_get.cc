#include "gtest/gtest.h"
#include <string.h>
#include "MessageQueueTestFixture.h"


TEST_F(MessageQueueTest, get_gets_the_message_with_the_right_type) {
    when(QUEUE_WITH_ONE_MEMBER);
    Message *msg = systemQueue.receive();
    ASSERT_EQ(MessageType::Button, msg->type);
}

TEST_F(MessageQueueTest, get_gets_the_message_with_the_right_payload) {
    when(QUEUE_WITH_ONE_MEMBER);
    Message *msg = systemQueue.receive();
    ASSERT_STREQ("message one", msg->payload);
}

TEST_F(MessageQueueTest, get_sets_the_head) {
    when(QUEUE_WITH_ONE_MEMBER);
    systemQueue.receive();
    ASSERT_EQ(1, getHead());
}

TEST_F(MessageQueueTest, get_leaves_the_tail_alone) {
    when(QUEUE_WITH_ONE_MEMBER);
    systemQueue.receive();
    ASSERT_EQ(1, getTail());
}

TEST_F(MessageQueueTest, get_returns_NULL_if_queue_is_empty) {
    when(QUEUE_IS_EMPTY);
    Message *msg = systemQueue.receive();
    EXPECT_EQ(NULL,msg);
}

TEST_F(MessageQueueTest, get_correctly_wraps_around) {
    when(BEFORE_HEAD_WRAPAROUND);
    systemQueue.receive();
    ASSERT_EQ(0, getHead());
}
