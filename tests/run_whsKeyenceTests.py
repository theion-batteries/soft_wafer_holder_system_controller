import subprocess
import os

# Get the directory containing the Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the relative path to another file

# Replace with the path to your test executable
test_executable = os.path.join(script_dir, "../../../../../build/Debug/", "whsTests.exe")

# Run the Connect test
connect_test_args = [test_executable, "--gtest_filter=KeyenceSensorTest.Connect"]
connect_test_result = subprocess.run(connect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if connect_test_result.returncode == 0:
    print("Connect test passed!")
else:
    print("Connect test failed:")
    print(connect_test_result.stdout.decode())
    print(connect_test_result.stderr.decode())

# Run the Disconnect test
disconnect_test_args = [test_executable, "--gtest_filter=KeyenceSensorTest.Disconnect"]
disconnect_test_result = subprocess.run(disconnect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Disconnect test
if disconnect_test_result.returncode == 0:
    print("Disconnect test passed!")
else:
    print("Disconnect test failed:")
    print(disconnect_test_result.stdout.decode())
    print(disconnect_test_result.stderr.decode())

# Run the IsConnected test
isconnected_test_args = [test_executable, "--gtest_filter=KeyenceSensorTest.IsConnected"]
isconnected_test_result = subprocess.run(isconnected_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the IsConnected test
if isconnected_test_result.returncode == 0:
    print("IsConnected test passed!")
else:
    print("IsConnected test failed:")
    print(isconnected_test_result.stdout.decode())
    print(isconnected_test_result.stderr.decode())

# Run the getMesuredValue test
getmesuredvalue_test_args = [test_executable, "--gtest_filter=KeyenceSensorTest.getMesuredValue"]
getmesuredvalue_test_result = subprocess.run(getmesuredvalue_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the getMesuredValue test
if getmesuredvalue_test_result.returncode == 0:
    print("getMesuredValue test passed!")
else:
    print("getMesuredValue test failed:")
    print(getmesuredvalue_test_result.stdout.decode())
    print(getmesuredvalue_test_result.stderr.decode())
