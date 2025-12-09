#include "gtest/gtest.h"
#include "MessageQueueTestFixture.h"
#include <stdio.h>


TEST_F(MessageQueueTest, dispatch_doesnt_execute_anything_if_listeners_is_null) {
    when(NO_LISTENERS);
    systemQueue.dispatch(&(testMessages[0]));
    ASSERT_EQ(false,listener1_ran);
    ASSERT_EQ(false,listener2_ran);
}

TEST_F(MessageQueueTest, dispatch_executes_the_matching_listener) {
    when(LISTENERS_ARE_SET_UP);
    systemQueue.dispatch(&(testMessages[0]));
    ASSERT_EQ(true,listener1_ran);
    ASSERT_EQ(false,listener2_ran);
}

TEST_F(MessageQueueTest, dispatch_executes_all_matching_listeners) {
    when(LISTENERS_ARE_SET_UP);
    systemQueue.dispatch(&(testMessages[1]));
    ASSERT_EQ(true,listener1_ran);
    ASSERT_EQ(true,listener2_ran);
}
