#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>

#define DIGIT_BLANK 0xFF //Setting to display a blank instead of a number
#define DIGIT_DASH  0xFE //Setting to display a - instead of a number

/* This struct defines the clock-aware settings provided to the lower-level LED
   driver. In this settings block, each available digit of the clock is specified
   from 0-9, but it could also be DIGIT_BLANK (to show nothing) or DIGIT_DASH (to
   show -). The alarm indicator and the colon are displayed if enabled. */
typedef struct {
    uint8_t hour[2];
    uint8_t minute[2];
    bool    alarm_enabled;
    bool    show_colon;
} display_setting_t;

/* Updates the display to the provided settings. Assume it returns FALSE on success or TRUE otherwise.
   This is not a particularly useful error as it doesn't provide any indication for what was invalid,
   so in an actual implementation it could provide an error code instead. The most likely error would
   be an invalid set of inputs. */
bool display_update(display_setting_t *setting);

#endif