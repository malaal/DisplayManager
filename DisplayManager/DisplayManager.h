/***
 *
 * Public API for the Display Manager
 *
 * Platform-independent
 *
 ***/

#ifndef __DISPLAY_MANAGER_H__
#define __DISPLAY_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>

/* Start the DisplayManager task
   Returns FALSE on success and TRUE on error. */
bool displayManager_Init(void);

/* Signal the 1-second tick to DisplayManager. This must be called every second to ensure proper operation of the module.
   Returns FALSE on success and TRUE on error. */
bool displayManager_signal_tick(void);

/* Signal that button1 has been pressed. In this case, this button displays the alarm state.
   Returns FALSE on success and TRUE on error. */
bool displayManager_signal_button1_pressed(void);

/* Signal that button2 has been pressed. In this case, this button displays a (fixed) single digit number.
   Returns FALSE on success and TRUE on error. */
bool displayManager_signal_button2_pressed(void);

#endif