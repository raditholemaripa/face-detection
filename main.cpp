/***************************************************************
 * Project: Real-Time Face Detection using OpenCV and Dlib
 * Author: Radithole Maripa
 * Date: 12/09/2025
 * Description:
 *   This program captures video from the default camera,
 *   detects human faces in real-time using Dlib's 
 *   frontal face detector, and draws green rectangles,
 *   it also detects the facial landmarks
 *   around detected faces. Press 'q' to exit the program.
 * Dependencies:
 *   - OpenCV (for video capture and image display)
 *   - Dlib (for face detection)
 ***************************************************************/

#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing/frontal_face_detector.h>  // Dlib face detector
#include <dlib/image_processing.h>                        // Dlib image processing utilities
#include <dlib/opencv.h>                                  // To convert OpenCV image to Dlib format
#include <opencv2/opencv.hpp>                             // OpenCV main header
#include <iostream>                                       // For console output
#include <numeric>
#include <filesystem>
#include <exception>

using namespace std;

// Detect a smile using landmark points and calculating the smile ratio
string detectSmile(const dlib::full_object_detection& shape);

int main(int  argc, char** argv)
{
    // Open the default camera (camera index 0)
    cv::VideoCapture video(0);
    cv::Mat frame;                                         // Matrix to store video frames

    if (!video.isOpened()) {
        cout << "Could not open camera\n";  			   // Print error if camera is not accessible
        return -1;
    }

    // Initialize Dlib's frontal face detector
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    // Load the 68-point face landmark model.
    // Prefer a copy next to the executable (CMake copies it there at build time),
    // falling back to the current working directory.
    std::filesystem::path modelPath = "shape_predictor_68_face_landmarks.dat";
    if (argc > 0 && argv[0] != nullptr) {
        std::filesystem::path exeDir = std::filesystem::path(argv[0]).parent_path();
        std::filesystem::path candidate = exeDir / "shape_predictor_68_face_landmarks.dat";
        if (std::filesystem::exists(candidate)) {
            modelPath = candidate;
        }
    }

    if (!std::filesystem::exists(modelPath)) {
        std::cerr << "Landmark model not found: " << modelPath << "\n"
                  << "Place shape_predictor_68_face_landmarks.dat next to the executable.\n";
        return -1;
    }

    dlib::shape_predictor shapePredictor;
    try {
        dlib::deserialize(modelPath.string()) >> shapePredictor;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load landmark model: " << e.what() << "\n";
        return -1;
    }

    while (true)
    {
        video >> frame;          // Capture a new frame
        if (frame.empty())       // Stop if the frame is empty
            break;

        // Convert OpenCV frame to Dlib image format
        dlib::cv_image<dlib::bgr_pixel> cimg(frame);

        // Detect faces in the frame
        std::vector<dlib::rectangle> faces = detector(cimg);

        // Draw rectangles around detected faces
        for (const auto& face : faces) {
            cv::rectangle(
                frame,
                cv::Point(face.left(), face.top()),    	// Top-left corner
                cv::Point(face.right(), face.bottom()),	// Bottom-right corner
                cv::Scalar(0, 255, 0),                	// Green color
                2                                      // Thickness
            );

            // Get landmarks
            dlib::full_object_detection shape = shapePredictor(cimg, face);

            // Draw each landmark point
            for(auto i = 0; i < shape.num_parts(); i++) {
                auto point = shape.part(i);
                cv::circle(frame, cv::Point(point.x(), point.y()), 2, cv::Scalar(255, 0, 0), -1);
            }

            string smileText = detectSmile(shape);
            cv::putText(frame, smileText, cv::Point(face.left(), face.top() - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
        }

        // Display the frame with rectangles
        cv::imshow("Face Detection", frame);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q')
            break;
    }

    return 0;  // End program
}

// Function definition
string detectSmile(const dlib::full_object_detection& shape)
{
    cv::Point leftMouth(shape.part(48).x(), shape.part(48).y());
    cv::Point rightMouth(shape.part(54).x(), shape.part(54).y());
    cv::Point topMouth(shape.part(51).x(), shape.part(51).y());
    cv::Point bottomMouth(shape.part(57).x(), shape.part(57).y());

    double mouthWidth  = cv::norm(rightMouth - leftMouth);
    double mouthHeight = std::max(cv::norm(bottomMouth - topMouth), 1e-6);
    double smileRatio  = mouthWidth / mouthHeight;

    static std::vector<double> lastRatios;
    lastRatios.push_back(smileRatio);
    if(lastRatios.size() > 5) lastRatios.erase(lastRatios.begin());

    double avgRatio = std::accumulate(lastRatios.begin(), lastRatios.end(), 0.0) / lastRatios.size();

    return (avgRatio > 3.15) ? "Smiling :)" : "Neutral";
}