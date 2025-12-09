#include <gtest/gtest.h>
#include "MessageQueueTestFixture.h"


TEST_F(MessageQueueTest, register_with_empty_list_registers_the_right_type) {
    when(NO_LISTENERS);
    systemQueue.registerListener(entries[0].type,entries[0].listener);
    ASSERT_EQ(MessageType::Button,getListeners()[0].type);
}

TEST_F(MessageQueueTest, register_with_empty_list_registers_the_right_listener) {
    when(NO_LISTENERS);
    systemQueue.registerListener(entries[0].type,entries[0].listener);
    ASSERT_EQ(&listener1,getListeners()[0].listener);
}

TEST_F(MessageQueueTest, register_with_nonempty_list_registers_the_right_type) {
    when(ONE_LISTENER);
    systemQueue.registerListener(entries[0].type,entries[0].listener);
    ASSERT_EQ(MessageType::Button,getListeners()[1].type);
}

TEST_F(MessageQueueTest, register_with_nonempty_list_registers_the_right_listener) {
    when(ONE_LISTENER);
    systemQueue.registerListener(entries[0].type,entries[0].listener);
    ASSERT_EQ(&listener1,getListeners()[1].listener);
}
