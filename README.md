# Real-Time Face Detection

This project captures video from your camera, detects human faces in real-time using Dlib's frontal face detector, and draws green rectangles around detected faces.

## Author

Radithole Maripa

## Dependencies

- OpenCV (Windows, MinGW)
- Dlib
- C++17 compatible compiler

> Note: OpenCV uses BSD 3-Clause License, Dlib uses Boost Software License.  
> If redistributing their binaries or source, include the respective license notices.

## Build on Windows

1. Ensure OpenCV and Dlib are installed.  
2. Update the `Makefile` paths if needed:

```make
OPENCV_DIR := C:/OpenCV/install
DLIB_DIR := C:/Dlib

