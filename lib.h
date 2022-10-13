//https://stackoverflow.com/questions/50932605/extract-all-video-frame-from-mp4-video-using-opencv-and-c

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;
int c = 0;
string int2str(int &);

int main(int argc, char **argv) {
string s;

VideoCapture cap("test.mp4"); // video
if (!cap.isOpened())  
{
    cout << "Cannot open the video file" << endl;
    return -1;
}

double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

cout << "Frame per seconds : " << fps << endl;

namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

while (1)
{
    Mat frame;
    Mat Gray_frame;
    bool bSuccess = cap.read(frame); // read a new frame from video

    if (!bSuccess) 
    {
        cout << "Cannot read the frame from video file" << endl;
        break;
    }
    s = int2str(c);
    //cout<<("%d\n",frame )<<endl;
    c++;
    imshow("MyVideo", frame); //show the frame in "MyVideo" window
    imwrite("ig" + s + ".jpg", frame);
    if (waitKey(30) == 27) //esc key
    {
        cout << "esc key is pressed by user" << endl;
        break;
    }
}

return 0;

}

//int to string function
string int2str(int &i) {
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}