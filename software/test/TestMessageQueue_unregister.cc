#include <gtest/gtest.h>
#include "MessageQueueTestFixture.h"

TEST_F(MessageQueueTest, unregister_with_a_single_item_list_sets_head_to_null) {
    when(ONE_LISTENER);
    systemQueue.unregisterListener(&(entries[0]));
    ASSERT_EQ(NULL,getListeners());
}

TEST_F(MessageQueueTest, unregister_of_the_first_item_sets_head_to_the_second_item) {
    when(TWO_LISTENERS);
    systemQueue.unregisterListener(&(entries[0]));
    ASSERT_EQ(&(entries[1]),getListeners());
}

TEST_F(MessageQueueTest, unregister_of_the_second_item_sets_next_of_first_item_to_null) {
    when(TWO_LISTENERS);
    systemQueue.unregisterListener(&(entries[1]));
    ASSERT_EQ(NULL,entries[0].next);
}
