#include <gtest/gtest.h>

#include "linear_motion.h"

class LinearMotionTest : public ::testing::Test {
protected:
    Iaxis_motion* mover_ ;

    void SetUp() override {
       mover_ = new linear_motion("127.0.0.1", 8882,10);
       mover_->connect();
    }

    void TearDown() override {
       mover_->disconnect();
        delete mover_;
    }
};

TEST_F(LinearMotionTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->connect());
}
TEST_F(LinearMotionTest, Disconnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->disconnect());
}
TEST_F(LinearMotionTest, IsConnected) {
    EXPECT_TRUE(mover_->getStatus());
}

TEST_F(LinearMotionTest, MoveHome) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_home());
}


TEST_F(LinearMotionTest, getSpeed) {
    EXPECT_EQ(800, mover_->get_speed());
}
TEST_F(LinearMotionTest, setSpeed) {
    EXPECT_EQ(sub_success, mover_->set_speed(100));
}
TEST_F(LinearMotionTest, getPosition) {
    EXPECT_EQ(-100, mover_->get_position());
}

TEST_F(LinearMotionTest, MoveCenter) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_center());
}
TEST_F(LinearMotionTest, MoveDownTo) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_down_to(100));
}
TEST_F(LinearMotionTest, MoveUpTo) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover_->move_up_to(100));
}


