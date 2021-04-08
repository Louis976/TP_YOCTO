cmake_minimum_required(VERSION 3.10)
project (gyro)
add_executable(gyro gyro.c)
install(TARGETS gyro RUNTIME DESTINATION /usr/bin)
