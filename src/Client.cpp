#include <ros/ros.h>  
#include <image_transport/image_transport.h>  
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>  
#include "config.h"
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()

using namespace std;
using namespace cv;

UDPSocket sock;
int jpegqual =  ENCODE_QUALITY; // Compression Parameter

//Mat frame,send;

vector < uchar > encoded; 
clock_t last_cycle;
unsigned short servPort;
string servAddress;

void imageCallback(const sensor_msgs::ImageConstPtr& cam_image)
{
    cv_bridge::CvImagePtr cv_ptr;   

    Mat frame,send;

    static int i = 0;
    cout << "i="<< i <<endl;
    cv_ptr = cv_bridge::toCvCopy(cam_image,sensor_msgs::image_encodings::BGR8);

    if(i==0)
      last_cycle = clock();

     frame = cv_ptr->image;

    resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
    vector < int > compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(jpegqual);

    imencode(".jpg", send, encoded, compression_params);
    //  imshow("send", send);
    //  cvWaitKey(1);
    int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

    int ibuf[1];
    ibuf[0] = total_pack;
    sock.sendTo(ibuf, sizeof(int), servAddress, servPort);

    for (int i = 0; i < total_pack; i++)
        sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);

   // waitKey(FRAME_INTERVAL);

    clock_t next_cycle = clock();
    double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
    cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

    cout << next_cycle - last_cycle;
    last_cycle = next_cycle;

    if(i++ > 5)
       i = 5;

    //imshow("view",img_rgb);
    //cvWaitKey(1);
}

int main(int argc, char * argv[]) 
{
    if ((argc < 3) || (argc > 4)) // Test for correct number of arguments
    { 
        cerr << "Usage: " << argv[0] << " <Server> <Server Port>\n";
        return 0;
    }   
  
    cout << "start" << endl;

    ros::init(argc, argv, "client_node");  
    ros::NodeHandle nh;   

    image_transport::ImageTransport it(nh);  
    image_transport::Subscriber sub = it.subscribe("/usb_cam/image_raw", 1, imageCallback);

    servAddress = argv[1]; // First arg: server address
    servPort = Socket::resolveService(argv[2], "udp");
    // last_cycle = clock();

    //cv::namedWindow("send"); 
    ros::spin();  
    return 0; 
}



// int main(int argc, char * argv[]) {
//     if ((argc < 3) || (argc > 3)) { // Test for correct number of arguments
//         cerr << "Usage: " << argv[0] << " <Server> <Server Port>\n";
//         exit(1);
//     }

//     string servAddress = argv[1]; // First arg: server address
//     unsigned short servPort = Socket::resolveService(argv[2], "udp");

//     try {
//         UDPSocket sock;
//         int jpegqual =  ENCODE_QUALITY; // Compression Parameter

//         Mat frame, send;
//         vector < uchar > encoded;
//         VideoCapture cap(0); // Grab the camera
//         namedWindow("send", CV_WINDOW_AUTOSIZE);
//         if (!cap.isOpened()) {
//             cerr << "OpenCV Failed to open camera";
//             exit(1);
//         }

//         clock_t last_cycle = clock();
//         while (1) {
//             cap >> frame;
//             if(frame.size().width==0)continue;//simple integrity check; skip erroneous data...
//             resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
//             vector < int > compression_params;
//             compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
//             compression_params.push_back(jpegqual);

//             imencode(".jpg", send, encoded, compression_params);
//             imshow("send", send);
//             int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

//             int ibuf[1];
//             ibuf[0] = total_pack;
//             sock.sendTo(ibuf, sizeof(int), servAddress, servPort);

//             for (int i = 0; i < total_pack; i++)
//                 sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);

//             waitKey(FRAME_INTERVAL);

//             clock_t next_cycle = clock();
//             double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
//             cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

//             cout << next_cycle - last_cycle;
//             last_cycle = next_cycle;
//         }
//         // Destructor closes the socket

//     } catch (SocketException & e) {
//         cerr << e.what() << endl;
//         exit(1);
//     }

//     return 0;
// }
