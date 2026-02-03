#include <gtest/gtest.h>

#include "repository.hpp"
#include "rows/input_rows.hpp"
#include "start_info.hpp"
#include <memory>

class EventLogicTest : public ::testing::Test {
  protected:
    void SetUp() override {
        StartInfo info;
        info.set_tables_cnt(2);
        info.set_start_time(540); // 09:00
        info.set_end_time(1140);  // 19:00
        info.set_hour_price(10);
        repo = std::make_unique<Repository>(info);
    }

    std::unique_ptr<Repository> repo;
};

TEST_F(EventLogicTest, Type1ClientArrival) {
    Type1RowInfo row(540, "alice");
    row.do_step(*repo);

    EXPECT_TRUE(repo->is_user_currently_in("alice"));
}

TEST_F(EventLogicTest, Type1NotOpenYetBefore) {
    Type1RowInfo row(539, "alice"); // Before 09:00
    row.do_step(*repo);

    EXPECT_FALSE(repo->is_user_currently_in("alice"));
    // Should generate Type13 error
}

TEST_F(EventLogicTest, Type1NotOpenYetAfter) {
    Type1RowInfo row(1140, "alice"); // At closing time
    row.do_step(*repo);

    EXPECT_FALSE(repo->is_user_currently_in("alice"));
}

TEST_F(EventLogicTest, Type1YouShallNotPass) {
    repo->add_current_user("alice");

    Type1RowInfo row(600, "alice");
    row.do_step(*repo);

    EXPECT_TRUE(repo->is_user_currently_in("alice"));
}

TEST_F(EventLogicTest, Type2ClientSitsDown) {
    repo->add_current_user("alice");

    Type2RowInfo row(600, "alice", 1);
    row.do_step(*repo);

    EXPECT_TRUE(repo->has_user_table("alice"));
    EXPECT_EQ(repo->get_user_table_id("alice"), 1);
    EXPECT_FALSE(repo->is_table_free(1));
}

TEST_F(EventLogicTest, Type2PlaceIsBusy) {
    repo->add_current_user("alice");
    repo->add_current_user("bob");

    Type2RowInfo row1(600, "alice", 1);
    row1.do_step(*repo);

    Type2RowInfo row2(610, "bob", 1);
    row2.do_step(*repo);

    // Bob should not be at table 1
    EXPECT_FALSE(repo->has_user_table("bob"));
}

TEST_F(EventLogicTest, Type2UnknownTable) {
    repo->add_current_user("alice");

    Type2RowInfo row(600, "alice", 99); // Table 99 doesn't exist
    row.do_step(*repo);

    EXPECT_FALSE(repo->has_user_table("alice"));
}

TEST_F(EventLogicTest, Type3ClientWaiting) {
    repo->add_current_user("alice");
    repo->add_current_user("bob");

    // Fill all tables
    Type2RowInfo row1(600, "alice", 1);
    row1.do_step(*repo);
    Type2RowInfo row2(610, "bob", 2);
    row2.do_step(*repo);

    // Charlie arrives and wants to wait
    repo->add_current_user("charlie");
    Type3RowInfo row3(620, "charlie");
    row3.do_step(*repo);

    EXPECT_TRUE(repo->is_user_waiting("charlie"));
}

TEST_F(EventLogicTest, Type4ClientLeaves) {
    repo->add_current_user("alice");

    Type2RowInfo row1(600, "alice", 1);
    row1.do_step(*repo);

    Type4RowInfo row2(700, "alice");
    row2.do_step(*repo);

    EXPECT_FALSE(repo->is_user_currently_in("alice"));
    EXPECT_TRUE(repo->is_table_free(1));
}

TEST_F(EventLogicTest, Type4SeatsWaitingClient) {
    repo->add_current_user("alice");
    repo->add_current_user("bob");

    // Alice sits at table 1
    Type2RowInfo row1(600, "alice", 1);
    row1.do_step(*repo);

    // Bob waits (needs all tables occupied first)
    repo->add_current_user("charlie");
    Type2RowInfo row2(610, "charlie", 2);
    row2.do_step(*repo);

    repo->add_waiting_user("bob");

    // Alice leaves
    Type4RowInfo row3(700, "alice");
    row3.do_step(*repo);

    // Bob should be seated at table 1
    EXPECT_TRUE(repo->has_user_table("bob"));
    EXPECT_EQ(repo->get_user_table_id("bob"), 1);
}
