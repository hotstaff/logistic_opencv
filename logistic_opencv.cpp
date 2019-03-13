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


/*default options*/
#define AI 3.5			/*start value of a*/
#define AF 4.0			/*end value of a*/
#define SKIP 8			/*dots density = 1/SKIP*/
#define WIDTH 26500		/*image size  width*/
#define HEIGHT 20960		/*image size height*/
#define ITERATION 10000		/*iteration counter of logistic function*/

/*default defines*/
#define COLOR_R 0		/*R*/
#define COLOR_G 255		/*G*/
#define COLOR_B 0		/*B*/
#define FILENAME "output.png"	/*output file name (.png,.jpg)*/

#define remain_time(full_count, current_count, spent_time)              \
        ((full_count - current_count) * (spent_time / current_count))   

struct calc_setting {
        unsigned int width = WIDTH;
        unsigned int height = HEIGHT;
        uint8_t color_r = COLOR_R;
        uint8_t color_g = COLOR_G;
        uint8_t color_b = COLOR_B;
        double ai = AI;
        double af = AF;
        unsigned int skip = SKIP;
        unsigned int iteration = ITERATION;
};


/**
 * check_setting() - Check setting.
 * 
 * Returns:
 *  true: sucess
 *  false: fail
 */
int check_setting(const struct calc_setting *setting) {
        if (
                setting->ai < 0 || setting->ai > 4
                || setting->ai >= setting->af || setting->af > 4
                || setting->skip < 1 || setting->skip >= setting->width
                || setting->width <= 0
                || setting->height <= 0
                || setting->iteration <= 0
        ) {
                return false;
        } 
        return true;
}


/**
 * time_elapsed() - Return time elapsed.
 */
double time_elapsed(unsigned long long int *cv_start_count, double cv_time_freq)
{
        return (cvGetTickCount() - *cv_start_count) / (1000 * cv_time_freq);
}



double logistic(double *a, double x, unsigned int i)
{       
        while (--i) {
                x = (*a) * x * (1 - x);
        }
        return x;
}



int main (int argc, char **argv)
{
        IplImage *img = 0;
        struct calc_setting setting;

        /*timer frequency*/
        const double cv_time_freq = cvGetTickFrequency() * 1000;
        
        unsigned int i = 0; 	/*iteration counter*/
        unsigned int x, y;	/*image coordinates*/
        double a, x0;		/*function coordinates*/
        double aplus; 
        double x0plus;
        unsigned long long int time_start;
	unsigned long int tmp_pixel;

        if (argc > 1 && argc != 7) {
                printf("usage: ./logistic_opencv AI(0 <= ai <= 4) AF(ai < af <= 4) SKIP WIDTH HEIGHT ITERATION\n\n");
                return -1;
        }

        if (argc == 7) {
                sscanf(argv[1], "%lf", &setting.ai);
                sscanf(argv[2], "%lf", &setting.af);
                sscanf(argv[3], "%u", &setting.skip);
                sscanf(argv[4], "%u", &setting.width);
                sscanf(argv[5], "%u", &setting.height);
                sscanf(argv[6], "%u", &setting.iteration);
        }

        if (!check_setting(&setting)) {
                printf("parameter error.\n\n");
                return -1;
        };

        printf("a value region: %lf to %lf, skip: %u, size: %ux%u, iteration: %u\n",
                                        setting.ai,
                                        setting.af,
                                        setting.skip,
                                        setting.width,
                                        setting.height,
                                        setting.iteration);

        /*initialize an image*/
        img = cvCreateImage(
                cvSize(setting.width, setting.height),
                                         IPL_DEPTH_8U,
                                                    3);
        if (img == 0) 
                return -1;
        cvZero(img);

        aplus = (setting.af - setting.ai) * (1.0 / setting.width);
        x0plus = (1.0 / setting.height) * setting.skip;
        time_start = cvGetTickCount();

        for (a = setting.ai; a < setting.af; a += aplus) {

                for (x0 = 0; x0 < 1; x0 += x0plus) {
                        y = (unsigned int) setting.height * logistic(&a, x0, setting.iteration);
                        if (y == 0)
                                continue;
                        x = (unsigned int) setting.width * (a - setting.ai) / (setting.af - setting.ai);

                        tmp_pixel = img->widthStep * (setting.height - y) + (x * 3);
                        img->imageData[tmp_pixel] = setting.color_r;
                        img->imageData[tmp_pixel + 1] = setting.color_g;
                        img->imageData[tmp_pixel + 2] = setting.color_b;
                }

                i++;
                /*print remaining time*/
		printf("%f, Remaining time: %.1f[s]\n", 
                       a,
                       remain_time(setting.width,
                                   i,
                                   time_elapsed(&time_start, cv_time_freq)));

        }

        printf("Time:  %.1f[s]\n", time_elapsed(&time_start, cv_time_freq));
        cvSaveImage(FILENAME, img, 0);
        cvReleaseImage(&img);

        return 0;
}
