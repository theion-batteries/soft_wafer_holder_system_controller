#include <gtest/gtest.h>

#include "whs_controller.h"

class WhsControllerTest : public ::testing::Test {
protected:
    whs_controller whsControl ;

    void SetUp() override {
       whsControl.connect_controller();
    }

    void TearDown() override {
       whsControl.disconnect_controller();
    }
};

TEST_F(WhsControllerTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.connect_controller());
}
TEST_F(WhsControllerTest, Disconnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.disconnect_controller());
}
TEST_F(WhsControllerTest, IsConnected) {
    EXPECT_TRUE(whsControl.get_whs_controller_status());
}

TEST_F(WhsControllerTest, move_down_until_data_availble) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.move_down_until_data_availble());
}

TEST_F(WhsControllerTest, move_down_to_surface) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.move_down_to_surface());
}
TEST_F(WhsControllerTest, deep_wafer_holder_desired_thickness) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.deep_wafer_holder_desired_thickness());
}

TEST_F(WhsControllerTest, monitor_and_calibrate) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.monitor_and_calibrate());
}
TEST_F(WhsControllerTest, extract_move_home) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.extract_move_home());
}


TEST_F(WhsControllerTest, reload_config_file) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, whsControl.reload_config_file());
}


TEST_F(WhsControllerTest, get_mover_status) {
    EXPECT_TRUE( whsControl.get_mover_status());
}
TEST_F(WhsControllerTest, get_sensor_status) {
    EXPECT_TRUE( whsControl.get_sensor_status());
}

TEST_F(WhsControllerTest, get_sensor_values) {
     EXPECT_EQ(0, whsControl.get_sensor_values());
}
TEST_F(WhsControllerTest, get_axis_position) {
     EXPECT_EQ(0, whsControl.get_axis_position());
}
