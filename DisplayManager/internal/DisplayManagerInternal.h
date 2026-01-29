/***
 *
 * Internals for the Display Manager
 *
 * Largely platform-independent but relies on the system's RTOS. Some kind of OS abstraction layer could be
 * used to remove even that dependency as well
 *
 ***/

#ifndef __DISPLAY_MANAGER_INTERNAL_H__
#define __DISPLAY_MANAGER_INTERNAL_H__

#include <stdint.h>
#include <stdbool.h>

/* These are the Event flags which are used to inform the DisplayManager task to wake and act. */
#define DISPLAYMANAGER_EVT_FLAG_TICK    0x01
#define DISPLAYMANAGER_EVT_FLAG_BUTTON1 0x02
#define DISPLAYMANAGER_EVT_FLAG_BUTTON2 0x04

/* Initialize the DisplayManager task.
   Returns FALSE on success or TRUE otherwise. */
bool displayManager_init(void);

/* Sends an OS signal to the DisplayManager task.
   Returns FALSE on success or TRUE otherwise.
   In this case, a "failure" could be that the underlying RTOS event queue is full, an error that definitely requires further analysis. */
bool displayManager_signal(uint8_t event);

#endif