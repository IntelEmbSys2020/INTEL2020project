#ifndef __DECTECTION_H__
#define __DECTECTION_H__

#include "fianl.h"

#define MIN_CENTER_DIS 100

double getDistance (Point pointO,Point pointA);
Mat DetectMark(Mat image,flag_detection_t &td);

#endif
