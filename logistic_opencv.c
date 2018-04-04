/*-------------------------------------------------------------
  logistic_opencv.c :This program outputs very big logistic map.

    Required:
        To compile this program, opencv is required.
    Compile:
        Please run "make" in the directory to compile.
        
  Written by Hideto Manjo.
---------------------------------------------------------------*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#define SKIP 8  /*dots density = 1/SKIP*/
#define AI 3.5  /*start value of a*/
#define AF 4.0  /*end value of a*/

#define WIDTH 26500  /*image size  width*/
#define HEIGHT 20960  /*image size height*/
#define FILENAME "output.png" /*output file name (.png,.jpg)*/


double logistic(double a, double x)
{
        int i = 100000;
        while (--i) {
        x = a * x * (1 - x);
    }
    return x;
}


int main (int argc, char **argv)
{

        int x, y;  /*image coordinates*/
        int width = WIDTH, height = HEIGHT;
        IplImage *img = 0;
        double c, f;  /*timer*/

        /*initialize an image*/
        img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        if (img == 0) 
            return -1;

        cvZero(img);

        /*initialize timer*/
        f = cvGetTickFrequency() * 1000;
        c = cvGetTickCount();

        /*initialize calculation*/
        double la, lx0; /*function coordinates*/
        int i = 0;

        double lap = (AF - AI) * (1.0 / width);
        double lxp = (1.0 / height) * SKIP;

        for (la = AI; la < AF; la += lap)
        {
                d = cvGetTickCount();

                for (lx0 = 0; lx0 < 1; lx0 += lxp)
                {

                        y = (int) height * logistic(la, lx0);
                        if (y == 0)
                            continue;
                        x = (int) width * (la - AI) / (AF - AI);

                        int a = img->widthStep * (height - y) + (x * 3);
                        img->imageData[a + 0] = 0;
                        img->imageData[a + 1] = 255;
                        img->imageData[a + 2] = 0;
                }

                i++;
                printf("%f ,Remaining time:  %.1f[s]\n", la, (width - i) * (cvGetTickCount() - d) / (1000 * f));
        }

        printf("Time:  %.1f[s]\n", (cvGetTickCount() - c) / (1000 * f));
        cvSaveImage(FILENAME, img, 0);
        cvReleaseImage(&img);

        return 0;
}