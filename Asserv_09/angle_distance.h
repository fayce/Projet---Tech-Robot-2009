#ifndef _ANGLE_DISTANCE_H_
#define _ANGLE_DISTANCE_H_

#include "types.h"
 
#include <string.h>
#include "stdio.h"

struct rs_wheels {
    int32_t left;
    int32_t right;
};


struct rs_polar {
    int32_t distance;
    int32_t angle;
};
/**
 * convert the values of wheels encoders (left, right) into (distance,
 * angle)
 */
struct rs_polar rs_get_polar_from_wheels(struct rs_wheels w);

/**
 * convert (distance, angle) into (left, right)
 */
struct rs_wheels rs_get_wheels_from_polar(struct rs_polar p);


#endif
