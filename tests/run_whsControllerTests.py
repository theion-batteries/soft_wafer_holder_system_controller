import subprocess
import os

# Get the directory containing the Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the relative path to another file

# Replace with the path to your test executable
test_executable = os.path.join(script_dir, "../../../../../build/Debug/", "whsTests.exe")

# Run the Connect test
connect_test_args = [test_executable, "--gtest_filter=WhsControllerTest.Connect"]
connect_test_result = subprocess.run(connect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if connect_test_result.returncode == 0:
    print("Connect test passed!")
else:
    print("Connect test failed:")
    #print(connect_test_result.stdout.decode())
    #print(connect_test_result.stderr.decode())

# Run the Disconnect test
disconnect_test_args = [test_executable, "--gtest_filter=WhsControllerTest.Disconnect"]
disconnect_test_result = subprocess.run(disconnect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Disconnect test
if disconnect_test_result.returncode == 0:
    print("Disconnect test passed!")
else:
    print("Disconnect test failed:")
    #print(disconnect_test_result.stdout.decode())
    #print(disconnect_test_result.stderr.decode())

# Run the IsConnected test
isconnected_test_args = [test_executable, "--gtest_filter=WhsControllerTest.IsConnected"]
isconnected_test_result = subprocess.run(isconnected_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the IsConnected test
if isconnected_test_result.returncode == 0:
    print("IsConnected test passed!")
else:
    print("IsConnected test failed:")
    #print(isconnected_test_result.stdout.decode())
    #print(isconnected_test_result.stderr.decode())

# Run the move_down_until_data_availble test
move_down_until_data_availble_test_args = [test_executable, "--gtest_filter=WhsControllerTest.move_down_until_data_availble"]
move_down_until_data_availble_test_result = subprocess.run(move_down_until_data_availble_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the move_down_until_data_availble test
if move_down_until_data_availble_test_result.returncode == 0:
    print("move_down_until_data_availble test passed!")
else:
    print("move_down_until_data_availble test failed:")
    #print(move_down_until_data_availble_test_result.stdout.decode())
    #print(move_down_until_data_availble_test_result.stderr.decode())

# Run the move_down_to_surface test
move_down_to_surface_test_args = [test_executable, "--gtest_filter=WhsControllerTest.move_down_to_surface"]
move_down_to_surface_test_result = subprocess.run(move_down_to_surface_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the move_down_to_surface test
if move_down_to_surface_test_result.returncode == 0:
    print("move_down_to_surface test passed!")
else:
    print("move_down_to_surface test failed:")
    #print(move_down_to_surface_test_result.stdout.decode())
    #print(move_down_to_surface_test_result.stderr.decode())

# Run the deep_wafer_holder_desired_thickness test
deep_wafer_holder_desired_thickness_test_args = [test_executable, "--gtest_filter=WhsControllerTest.deep_wafer_holder_desired_thickness"]
deep_wafer_holder_desired_thickness_test_result = subprocess.run(deep_wafer_holder_desired_thickness_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the deep_wafer_holder_desired_thickness test
if deep_wafer_holder_desired_thickness_test_result.returncode == 0:
    print("deep_wafer_holder_desired_thickness test passed!")
else:
    print("deep_wafer_holder_desired_thickness test failed:")
    #print(deep_wafer_holder_desired_thickness_test_result.stdout.decode())
    #print(deep_wafer_holder_desired_thickness_test_result.stderr.decode())

# Run the monitor_and_calibrate test
monitor_and_calibrate_args = [test_executable, "--gtest_filter=WhsControllerTest.monitor_and_calibrate"]
monitor_and_calibrate_result = subprocess.run(monitor_and_calibrate_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the monitor_and_calibrate test
if monitor_and_calibrate_result.returncode == 0:
    print("monitor_and_calibrate test passed!")
else:
    print("monitor_and_calibrate test failed:")
    #print(monitor_and_calibrate_result.stdout.decode())
    #print(monitor_and_calibrate_result.stderr.decode())

# Run the extract_move_home test
extract_move_home_args = [test_executable, "--gtest_filter=WhsControllerTest.extract_move_home"]
extract_move_home_result = subprocess.run(extract_move_home_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the extract_move_home test
if extract_move_home_result.returncode == 0:
    print("extract_move_home test passed!")
else:
    print("extract_move_home test failed:")
    #print(extract_move_home_result.stdout.decode())
    #print(extract_move_home_result.stderr.decode())

# Run the reload_config_file test
reload_config_file_args = [test_executable, "--gtest_filter=WhsControllerTest.reload_config_file"]
reload_config_file_result = subprocess.run(reload_config_file_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the reload_config_file test
if reload_config_file_result.returncode == 0:
    print("reload_config_file test passed!")
else:
    print("reload_config_file test failed:")
    #print(reload_config_file_result.stdout.decode())
    #print(reload_config_file_result.stderr.decode())

# Run the get_mover_status test
get_mover_status_args = [test_executable, "--gtest_filter=WhsControllerTest.get_mover_status"]
get_mover_status_result = subprocess.run(get_mover_status_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the get_mover_status test
if get_mover_status_result.returncode == 0:
    print("get_mover_status test passed!")
else:
    print("get_mover_status test failed:")
    #print(get_mover_status_result.stdout.decode())
    #print(get_mover_status_result.stderr.decode())

# Run the get_sensor_status test
get_sensor_status_args = [test_executable, "--gtest_filter=WhsControllerTest.get_sensor_status"]
get_sensor_status_result = subprocess.run(get_sensor_status_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the get_sensor_status test
if get_sensor_status_result.returncode == 0:
    print("get_sensor_status test passed!")
else:
    print("get_sensor_status test failed:")
    #print(get_sensor_status_result.stdout.decode())
    #print(get_sensor_status_result.stderr.decode())

# Run the get_sensor_status test
get_sensor_values_args = [test_executable, "--gtest_filter=WhsControllerTest.get_sensor_values"]
get_sensor_values_result = subprocess.run(get_sensor_values_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the get_sensor_values test
if get_sensor_values_result.returncode == 0:
    print("get_sensor_values test passed!")
else:
    print("get_sensor_values test failed:")
    #print(get_sensor_values_result.stdout.decode())
    #print(get_sensor_values_result.stderr.decode())

# Run the get_sensor_status test
get_axis_position_args = [test_executable, "--gtest_filter=WhsControllerTest.get_axis_position"]
get_axis_position_result = subprocess.run(get_axis_position_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the get_axis_position test
if get_axis_position_result.returncode == 0:
    print("get_axis_position test passed!")
else:
    print("get_axis_position test failed:")
    #print(get_axis_position_result.stdout.decode())
    #print(get_axis_position_result.stderr.decode())