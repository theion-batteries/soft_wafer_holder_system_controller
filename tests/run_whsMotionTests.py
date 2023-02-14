import subprocess
import os

# Get the directory containing the Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the relative path to another file

# Replace with the path to your test executable
test_executable = os.path.join(script_dir, "../../../../../build/Debug/", "whsTests.exe")
# Run the Connect test
connect_test_args = [test_executable, "--gtest_filter=LinearMotionTest.Connect"]
connect_test_result = subprocess.run(connect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if connect_test_result.returncode == 0:
    print("Connect test passed!")
else:
    print("Connect test failed:")
    print(connect_test_result.stdout.decode())
    print(connect_test_result.stderr.decode())

# Run the IsConnected test
isconnected_test_args = [test_executable, "--gtest_filter=LinearMotionTest.IsConnected"]
isconnected_test_result = subprocess.run(isconnected_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the IsConnected test
if isconnected_test_result.returncode == 0:
    print("IsConnected test passed!")
else:
    print("IsConnected test failed:")
    print(isconnected_test_result.stdout.decode())
    print(isconnected_test_result.stderr.decode())
# Run the MoveHome test
movehome_test_args = [test_executable, "--gtest_filter=LinearMotionTest.MoveHome"]
movehome_test_result = subprocess.run(movehome_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the MoveHome test
if movehome_test_result.returncode == 0:
    print("MoveHome test passed!")
else:
    print("MoveHome test failed:")
    print(movehome_test_result.stdout.decode())
    print(movehome_test_result.stderr.decode())

# Run the getSpeed test
getspeed_test_args = [test_executable, "--gtest_filter=LinearMotionTest.getSpeed"]
getspeed_test_result = subprocess.run(getspeed_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the getSpeed test
if getspeed_test_result.returncode == 0:
    print("getSpeed test passed!")
else:
    print("getSpeed test failed:")
    print(getspeed_test_result.stdout.decode())
    print(getspeed_test_result.stderr.decode())

# Run the setSpeed test
setspeed_test_args = [test_executable, "--gtest_filter=LinearMotionTest.setSpeed"]
setspeed_test_result = subprocess.run(setspeed_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the setSpeed test
if setspeed_test_result.returncode == 0:
    print("setSpeed test passed!")
else:
    print("setSpeed test failed:")
    print(setspeed_test_result.stdout.decode())
    print(setspeed_test_result.stderr.decode())

# Run the getPosition test
getposition_test_args = [test_executable, "--gtest_filter=LinearMotionTest.getPosition"]
getposition_test_result = subprocess.run(getposition_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the getPosition test
if getposition_test_result.returncode == 0:
    print("getPosition test passed!")
else:
    print("getPosition test failed:")
    print(getposition_test_result.stdout.decode())
    print(getposition_test_result.stderr.decode())

# Run the MoveCenter test
movecenter_test_args = [test_executable, "--gtest_filter=LinearMotionTest.MoveCenter"]
movecenter_test_result = subprocess.run(movecenter_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the MoveCenter test
if movecenter_test_result.returncode == 0:
    print("MoveCenter test passed!")
else:
    print("MoveCenter test failed:")
    print(movecenter_test_result.stdout.decode())
    print(movecenter_test_result.stderr.decode())

# Run the MoveDownTo test
movedownto_test_args = [test_executable, "--gtest_filter=LinearMotionTest.MoveDownTo"]
movedownto_test_result = subprocess.run(movedownto_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the MoveDownTo test
if movedownto_test_result.returncode == 0:
    print("MoveDownTo test passed!")
else:
    print("MoveDownTo test failed:")
    print(movedownto_test_result.stdout.decode())
    print(movedownto_test_result.stderr.decode())

# Run the MoveUpTo test
moveupto_test_args = [test_executable, "--gtest_filter=LinearMotionTest.MoveUpTo"]
moveupto_test_result = subprocess.run(moveupto_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the MoveUpTo test
if moveupto_test_result.returncode == 0:
    print("MoveUpTo test passed!")
else:
    print("MoveUpTo test failed:")
    print(moveupto_test_result.stdout.decode())
    print(moveupto_test_result.stderr.decode())
