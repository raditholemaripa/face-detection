cmake_minimum_required(VERSION 3.16)

project(FaceDetect
    VERSION 1.0.0
    DESCRIPTION "Real-time face and smile detection with OpenCV and Dlib"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- Dependencies ----
find_package(OpenCV REQUIRED)

find_package(dlib CONFIG QUIET)
if(NOT dlib_FOUND)
    find_package(dlib REQUIRED)
endif()

if(NOT TARGET dlib::dlib)
    add_library(dlib::dlib INTERFACE IMPORTED)
    target_include_directories(dlib::dlib INTERFACE ${dlib_INCLUDE_DIRS})
    target_link_libraries(dlib::dlib INTERFACE ${dlib_LIBRARIES})
endif()

message(STATUS "OpenCV version: ${OpenCV_VERSION}")

# ---- Target ----
add_executable(FaceDetect main.cpp)

target_link_libraries(FaceDetect PRIVATE
    ${OpenCV_LIBS}
    dlib::dlib
)

# ---- Runtime model file handling ----
set(MODEL_FILE "${CMAKE_CURRENT_SOURCE_DIR}/shape_predictor_68_face_landmarks.dat")
if(EXISTS "${MODEL_FILE}")
    add_custom_command(TARGET FaceDetect POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MODEL_FILE}"
            "$<TARGET_FILE_DIR:FaceDetect>/shape_predictor_68_face_landmarks.dat"
        COMMENT "Copying landmark model next to executable"
    )
else()
    message(WARNING
        "shape_predictor_68_face_landmarks.dat not found in source root. "
        "Download it before running (see README).")
endif()

install(TARGETS FaceDetect RUNTIME DESTINATION bin)