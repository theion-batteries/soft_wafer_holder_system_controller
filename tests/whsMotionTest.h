#include <gtest/gtest.h>
#include "linear_motion.h"

// Demonstrate some basic assertions.
TEST(whsLinearMotionOBj, connect) {
  // expect connect
  linear_motion mover;
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
}
// Demonstrate some basic assertions.
TEST(whsLinearMotionPtr, connect) {
  // expect connect
  linear_motion mover;
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, mover.connect());
  Iaxis_motion* moverPtr = new linear_motion();
  EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_error, moverPtr->connect());
  delete moverPtr;
}
// Demonstrate some basic assertions.
TEST(whsLinearMotionPtr, deletePtr) {
  // expect connect
 std::unique_ptr< Iaxis_motion> moverPtr = std::make_unique< linear_motion>();
   moverPtr.reset();
  EXPECT_EQ(moverPtr, nullptr);
}