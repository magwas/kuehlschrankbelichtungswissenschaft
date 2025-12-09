#include <gtest/gtest.h>
#include "MessageQueueTestFixture.h"

TEST_F(MessageQueueTest, unregister_sets_type_to_None) {
    when(ONE_LISTENER);
    systemQueue.unregisterListener(entries[0].type,entries[0].listener);
    ASSERT_EQ(MessageType::None,getListeners()[0].type);
}

TEST_F(MessageQueueTest, unregister_from_first_place_returns_zero) {
    when(ONE_LISTENER);
    ASSERT_EQ(0,systemQueue.unregisterListener(entries[0].type,entries[0].listener));
}

TEST_F(MessageQueueTest, unregister_returns_the_id_of_unregistered_listener) {
    when(TWO_LISTENERS);
    ASSERT_EQ(1,systemQueue.unregisterListener(entries[1].type,entries[1].listener));
}
