#include <gtest/gtest.h>

#include "repository.hpp"
#include "start_info.hpp"
#include <memory>

class RepositoryTest : public ::testing::Test {
  protected:
    void SetUp() override {
        StartInfo info;
        info.set_tables_cnt(3);
        info.set_start_time(540); // 09:00
        info.set_end_time(1140);  // 19:00
        info.set_hour_price(10);
        repo = std::make_unique<Repository>(info);
    }

    std::unique_ptr<Repository> repo;
};

TEST_F(RepositoryTest, AddRemoveCurrentUser) {
    EXPECT_FALSE(repo->is_user_currently_in("alice"));

    repo->add_current_user("alice");
    EXPECT_TRUE(repo->is_user_currently_in("alice"));

    repo->remove_current_user("alice");
    EXPECT_FALSE(repo->is_user_currently_in("alice"));
}

TEST_F(RepositoryTest, WaitingQueue) {
    EXPECT_FALSE(repo->has_waiting_users());

    repo->add_waiting_user("alice");
    EXPECT_TRUE(repo->has_waiting_users());
    EXPECT_EQ(repo->get_next_waiting_user(), "alice");

    repo->add_waiting_user("bob");
    EXPECT_EQ(repo->get_next_waiting_user(), "alice");

    repo->remove_waiting_user();
    EXPECT_EQ(repo->get_next_waiting_user(), "bob");
}

TEST_F(RepositoryTest, QueueFullCheck) {
    EXPECT_FALSE(repo->is_queue_full());

    repo->add_waiting_user("alice");
    EXPECT_FALSE(repo->is_queue_full());

    repo->add_waiting_user("bob");
    EXPECT_FALSE(repo->is_queue_full());

    repo->add_waiting_user("charlie");
    EXPECT_TRUE(repo->is_queue_full()); // 3 tables = max 3 in queue
}

TEST_F(RepositoryTest, TableOperations) {
    EXPECT_TRUE(repo->is_table_free(1));
    EXPECT_TRUE(repo->is_table_free(2));
    EXPECT_TRUE(repo->is_table_free(3));

    repo->mark_table_occupied(1);

    EXPECT_FALSE(repo->is_table_free(1));
    EXPECT_TRUE(repo->is_table_free(2));
}

TEST_F(RepositoryTest, HasFreeTables) {
    EXPECT_TRUE(repo->has_free_tables());

    repo->mark_table_occupied(1);
    repo->mark_table_occupied(2);
    repo->mark_table_occupied(3);

    EXPECT_FALSE(repo->has_free_tables());
}

TEST_F(RepositoryTest, UserTableMapping) {
    EXPECT_FALSE(repo->has_user_table("alice"));

    repo->add_user_table("alice", 1);
    EXPECT_TRUE(repo->has_user_table("alice"));
    EXPECT_EQ(repo->get_user_table_id("alice"), 1);

    repo->remove_user_table("alice");
    EXPECT_FALSE(repo->has_user_table("alice"));
}

TEST_F(RepositoryTest, CalculateTotalPrice) {
    // 0-59 minutes -> 1 hour
    EXPECT_EQ(repo->calculate_total_price(1), 10);
    EXPECT_EQ(repo->calculate_total_price(59), 10);

    // 60 minutes -> 1 hour
    EXPECT_EQ(repo->calculate_total_price(60), 10);

    // 61-119 minutes -> 2 hours
    EXPECT_EQ(repo->calculate_total_price(61), 20);
    EXPECT_EQ(repo->calculate_total_price(119), 20);

    // 120 minutes -> 2 hours
    EXPECT_EQ(repo->calculate_total_price(120), 20);
}

TEST_F(RepositoryTest, IsUserWaiting) {
    EXPECT_FALSE(repo->is_user_waiting("alice"));

    repo->add_waiting_user("alice");
    EXPECT_TRUE(repo->is_user_waiting("alice"));

    repo->add_waiting_user("bob");
    EXPECT_TRUE(repo->is_user_waiting("alice"));
    EXPECT_TRUE(repo->is_user_waiting("bob"));

    repo->remove_waiting_user(); // removes alice (FIFO)
    EXPECT_FALSE(repo->is_user_waiting("alice"));
    EXPECT_TRUE(repo->is_user_waiting("bob"));
}
