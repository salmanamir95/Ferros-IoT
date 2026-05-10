cmake_minimum_required(VERSION 3.16)
project(broker C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json REQUIRED)
find_package(PahoMqttCpp REQUIRED)

add_executable(broker
    src/main.cpp
    src/MQTTSubscriber.cpp
    src/MessageRouter.cpp
)

target_include_directories(broker PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(broker
    nlohmann_json::nlohmann_json
    PahoMqttCpp::paho-mqttpp3
    paho-mqtt3a
)
