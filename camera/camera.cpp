
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

using namespace std;
#ifdef USE_OPENCV
using namespace cv;
#endif



int main(int argc, char *argv[]) {
#ifdef USE_OPENCV
    VideoCapture capture(0);
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('H','2','6','4')); 
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920); 
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080); 
    cv::Size cap_size = cv::Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH), (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    float cap_fps =  capture.get(CV_CAP_PROP_FPS);
    printf("Capture size: %ux%u\n", cap_size.width, cap_size.height);
    printf("Capture fps: %.1f\n", cap_fps);

    VideoWriter writer;
    writer.open("./out.h264", CV_FOURCC('P','I','M','1'), cap_fps, cap_size, true);
 
    while (true) {
        Mat frame;
        capture >> frame;
        writer << frame;
        imshow("camera", frame);
        int key = waitKey(10);  
        if (key == 27) {
            break;
        } else {
            printf("Input key: %d\n", key);
        }
    }
#endif
    return 0;
}