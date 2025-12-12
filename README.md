# Real-Time Face Detection

This project captures video from your camera, detects human faces in real-time using Dlib's frontal face detector, and draws green rectangles around detected faces.

## Author

Radithole Maripa

## System Requirements

To build and run this project on Windows, you must have the following installed:

- **MinGW-w64** (64-bit GCC compiler for Windows)  
- **OpenCV** (precompiled or compiled for Windows)  
- **Dlib** (C++ library for machine learning and computer vision)  

> Note: OpenCV uses BSD 3-Clause License, Dlib uses Boost Software License.  
> If redistributing their binaries or source, include the respective license notices.

## Required Files
- **shape_preditor_68_face_landmarks.dat** - Dlib's facial landmark predictor model.
This file is required for detecting key facial points (eyes, nose, mouth. jawline) 
after a face is detected.

## Build on Windows

> Important: Update the paths in the `Makefile` to match where OpenCV and Dlib are installed on your system.  
> Example lines in `Makefile` (replace with your paths):

```make
OPENCV_DIR := C:/path/to/your/opencv
DLIB_DIR := C:/path/to/your/dlib
```

1. Open **Command Prompt** in the project folder.
2. Run:
```make
mingw32-make
```
- This will compile the source files and create the executable FaceDetect.exe.

3. To clean build artifacts:
```make
mingw32-make clean
```

## Run Instructions
1. Ensure the **face predictor model file(shape_predictor_68_face_landmarks.dat)** 
is in the same directory as the executable or update the path in your code.
2. In the Command Line, run:
```make
FaceDetect
```
- A window will open showing the camera feed. Detected faces will have green rectangles and facial landmarks will be marked.
- Press q to quit the program.
