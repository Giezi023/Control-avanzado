#ifndef ENCODER_H
#define ENCODER_H

#include "driver/pulse_cnt.h"

void encoder_init(void);
int encoder_get_count(void);
float encoder_get_rpm(void);

#endif