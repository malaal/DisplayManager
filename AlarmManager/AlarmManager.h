/***
 *
 * Public API for the Alarm Manager
 *
 * Platform-independent
 *
 ***/

#ifndef __ALARM_MANAGER_H__
#define __ALARM_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool    enabled;
    uint8_t hour;
    uint8_t minute;
} alarm_state_t;

/* Returns (via pointer) the state of the alarm time. Assumes 24-hour time.
   The alarm is enabled if the enabled bool is true, and the hour/minute fields
   will contain valid data. */
void alarmManager_getState(alarm_state_t *state);

#endif