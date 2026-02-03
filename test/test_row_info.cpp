#include <gtest/gtest.h>

#include "rows/input_rows.hpp"
#include "rows/output_rows.hpp"

TEST(RowInfoTest, Type1ToString) {
    Type1RowInfo row(540, "alice"); // 09:00 1 alice
    EXPECT_EQ(row.get_time(), 540);
    EXPECT_EQ(row.get_user_name(), "alice");
    EXPECT_EQ(row.to_string(), "09:00 1 alice");
}

TEST(RowInfoTest, Type2ToString) {
    Type2RowInfo row(600, "bob", 2); // 10:00 2 bob 2
    EXPECT_EQ(row.get_time(), 600);
    EXPECT_EQ(row.get_user_name(), "bob");
    EXPECT_EQ(row.get_table_id(), 2);
    EXPECT_EQ(row.to_string(), "10:00 2 bob 2");
}

TEST(RowInfoTest, Type11ToString) {
    Type11RowInfo row(1140, "charlie"); // 19:00 11 charlie
    EXPECT_EQ(row.to_string(), "19:00 11 charlie");
}

TEST(RowInfoTest, Type13ToString) {
    Type13RowInfo row(540, "ClientUnknown");
    EXPECT_EQ(row.to_string(), "09:00 13 ClientUnknown");
}
