#include <gtest/gtest.h>
#include "MessageQueueTestFixture.h"


TEST_F(MessageQueueTest, register_with_empty_list_sets_the_listener_head) {
    when(NO_LISTENERS);
    systemQueue.registerListener(&(entries[0]));
    ASSERT_EQ(&(entries[0]),getListeners());
}

TEST_F(MessageQueueTest, register_with_empty_list_sets_the_entry_next_to_null) {
    when(NO_LISTENERS);
    systemQueue.registerListener(&(entries[0]));
    ASSERT_EQ(NULL,entries[0].next);
}

TEST_F(MessageQueueTest, register_sets_the_next_of_last_entry) {
    when(ONE_LISTENER);
    systemQueue.registerListener(&(entries[1]));
    ASSERT_EQ(&(entries[1]),entries[0].next);
}

TEST_F(MessageQueueTest, register_sets_the_next_of_the_entry_null) {
    when(ONE_LISTENER);
    systemQueue.registerListener(&(entries[1]));
    ASSERT_EQ(NULL,entries[1].next);
}