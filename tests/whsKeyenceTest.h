#include <gtest/gtest.h>

#include "keyence_sensor.h"

class KeyenceSensorTest : public ::testing::Test {
protected:
    Idistance_sensor* dSensor ;

    void SetUp() override {
       dSensor = new keyence_sensor("127.0.0.1", 6555);
       dSensor->connect();
    }

    void TearDown() override {
       dSensor->disconnect();
        delete dSensor;
    }
};

TEST_F(KeyenceSensorTest, Connect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, dSensor->connect());
}

TEST_F(KeyenceSensorTest, Disconnect) {
    EXPECT_EQ(wgm_feedbacks::enum_sub_sys_feedback::sub_success, dSensor->disconnect());
}
TEST_F(KeyenceSensorTest, IsConnected) {
    EXPECT_TRUE(dSensor->getStatus());
}


TEST_F(KeyenceSensorTest, getMesuredValue) {
    EXPECT_EQ(0, dSensor->getMesuredValue());
}
