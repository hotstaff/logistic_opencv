// SPDX-License-Identifier: MIT
/*
 *  logistic_opencv.cpp
 *
 *  This program outputs very big logistic map.
 *
 *   Required:
 *       To compile this program, opencv is required.
 *   Compile:
 *       Please run "make" in the directory to compile.
 *       
 *  Written by Hideto Manjo.
*/

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <stdio.h>

#define SKIP 8  /*dots density = 1/SKIP*/
#define AI 3.5  /*start value of a*/
#define AF 4.0  /*end value of a*/

#define WIDTH 26500  /*image size  width*/
#define HEIGHT 20960  /*image size height*/
#define COLOR_R 0  /*R*/
#define COLOR_G 255  /*G*/
#define COLOR_B 0  /*B*/
#define FILENAME "output.png" /*output file name (.png,.jpg)*/
#define ITERATION 10000 /*iteration count of logistic function*/


double logistic(double a, double x)
{
        unsigned int i = ITERATION;

        while (--i) {
                x = a * x * (1 - x);
        }
        return x;
}


double time_spent(long long unsigned int *cv_start_count, double *cv_time_freq)
{
        return (cvGetTickCount() - *cv_start_count) / (1000 * *cv_time_freq);
}


int main (int argc, char **argv)
{
        IplImage *img = 0;
        unsigned int i = 0;  /*iteration counter*/
        unsigned int x, y;  /*image coordinates*/
        unsigned int width = WIDTH;
        unsigned int height = HEIGHT;

        long long unsigned int time_start, time_lap_start; /*timer count*/
        double cv_time_freq = cvGetTickFrequency() * 1000; /*timer frequency*/
        double la, lx0;  /*function coordinates*/
        double lap = (AF - AI) * (1.0 / width);  
        double lxp = (1.0 / height) * SKIP;

	double tmp_remain;
	long unsigned int tmp_pixel;

        /*initialize an image*/
        img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        if (img == 0) 
                return -1;

        cvZero(img);

        /*initialize timer*/
        time_start = cvGetTickCount();

        for (la = AI; la < AF; la += lap) {
                time_lap_start = cvGetTickCount();

                for (lx0 = 0; lx0 < 1; lx0 += lxp) {
                        y = (unsigned int) height * logistic(la, lx0);
                        if (y == 0)
                                continue;
                        x = (unsigned int) width * (la - AI) / (AF - AI);

                        tmp_pixel = img->widthStep * (height - y) + (x * 3);
                        img->imageData[tmp_pixel + 0] = COLOR_R;
                        img->imageData[tmp_pixel + 1] = COLOR_G;
                        img->imageData[tmp_pixel + 2] = COLOR_B;
                }

                i++;

                /*print remaining time*/
		tmp_remain = (width - i) * time_spent(&time_lap_start, &cv_time_freq);
		printf("%f, Remaining time: %.1f[s]\n", la, tmp_remain);

        }

        printf("Time:  %.1f[s]\n", time_spent(&time_start, &cv_time_freq));
        cvSaveImage(FILENAME, img, 0);
        cvReleaseImage(&img);

        return 0;
}
