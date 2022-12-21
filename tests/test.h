#include <gtest/gtest.h>
#include "linear_motion.h"


// Demonstrate some basic assertions.
TEST(LinearMotion, connectTest) {
  // expect connect
  linear_motion mover;
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, mover.connect());
  // Expect equality.
}