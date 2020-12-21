#include "B-spline.h"

float b_spline(float x)
{
    float abs_x = fabs(x);
    if (abs_x < 1) {
        return abs_x*abs_x*(0.5 * abs_x - 1) + 2 / 3.0;
    }
    else if (abs_x < 2) {
        return -1.0 * pow(abs_x, 3) / 6.0 + pow(x, 2) - 2 * abs_x + 4 / 3.0;
    }
    else {
        return 0.0f;
    }
}

float D_spline_dx(float x) {
    float abs_x = fabs(x);
    float sign = x < 0 ? -1 : 1;
    if (abs_x < 1) {
        return 3.0 * x * abs_x / 2.0 - 2 * x;
    }
    else if (abs_x < 2) {
        return -0.5 * x * abs_x + 2 * x - 2 * sign;
    }
    else {
        return 0.0f;
    }
}
