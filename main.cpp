#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void help(char* progName)
{
    cout << endl
         <<  "Obrabotka isobrajeniya primenyaya maski"
         << endl
         << progName << " [image_name -- /home/beka/image/1.jpg] [G -- grayscale] "        << endl << endl;
}


void Sharpen(const Mat& myImage,Mat& Result);

int main( int argc, char* argv[])
{
    help(argv[0]);
    const char* filename = argc >=2 ? argv[1] : "/home/beka/image/1.jpg";

    Mat I, J, K;

    if (argc >= 3 && !strcmp("G", argv[2]))
        I = imread( filename, CV_LOAD_IMAGE_GRAYSCALE);
    else
        I = imread( filename, CV_LOAD_IMAGE_GRAYSCALE);

    namedWindow("Ishodnoe isobr", WINDOW_AUTOSIZE);
    namedWindow("Resultat", WINDOW_AUTOSIZE);

    imshow("Ishodnoe isobr", I);
    double t = (double)getTickCount();

    Sharpen(I, J);

    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Resultat svoego metoda v sek: " << t << endl;

    imshow("Resultat", J);
    waitKey(0);

    Mat kern = (Mat_<char>(3,3) <<  1, 0, -1,
                                    0, 0, 0,
                                    1, 0, 1);
    t = (double)getTickCount();
    filter2D(I, K, I.depth(), kern );
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "Resultat gotovoi funkzii v sek:      " << t << endl;

    imshow("Resultat", K);

    waitKey(0);
    return 0;
}
void Sharpen(const Mat& myImage,Mat& Result)
{
    CV_Assert(myImage.depth() == CV_8U);  // принимать только Uchar изображения

    const int nChannels = myImage.channels();
    Result.create(myImage.size(),myImage.type());

    for(int j = 1 ; j < myImage.rows-1; ++j)
    {
        const uchar* previous = myImage.ptr<uchar>(j - 1);
        const uchar* current  = myImage.ptr<uchar>(j    );
        const uchar* next     = myImage.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);

        for(int i= nChannels;i < nChannels*(myImage.cols-1); ++i)
        {
            *output++ = saturate_cast<uchar>(5*current[i]
                                             -current[i-nChannels] - current[i+nChannels] - previous[i] - next[i]);
        }
    }

    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows-1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols-1).setTo(Scalar(0));
}