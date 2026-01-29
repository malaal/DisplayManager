#include <RTOS.h> //Include the RTOS library
#include "AlarmManager/AlarmManager.h"
#include "DisplayManagerInternal.h"
#include "Drivers/Display.h"
#include "Drivers/RTC.h" //Assuming this exists to provide the current time

// Here's the hard-coded single digit that will be displayed
#define DIGIT_DISPLAY 5

// Number of seconds to display things that aren't the clock before going back to the clock
#define DISPLAY_MODE_TIMEOUT_S 5

task_t displayManager_task; //The OS handle to the display manager task

/*********************
 * Public Functions
 *********************/

/* Create (and start) the display manager task. This requires a pre-defined task priority and a stack size
   (or some OS default), which are not defined in this sample code. */
bool displayManager_init(void)
{
    bool ret = FALSE; //default return value is Success

    //The NULL here indicates no parameters are passed to the task function
    displayManager_task = OS_CREATE_TASK(displayManager_task, DISPLAY_MANAGER_PRIORITY, DISPLAY_MANAGER_STACK_SZ, NULL);

    //Assumes that a failure to create the task results in a NULL task handle.
    if (displayManager_task == NULL)
    {
        ret = TRUE;
    }

    return ret;

}

/* Set an event flag in the OS event flags. I am assuming that the OS ties this operation to the task handle, rather
   than defining a separate message queue. */
bool displayManager_signal(uint8_t event)
{
    //Some translation of an OS error result to an application error result might be required here.
    return OS_SET_EVENT(displayManager_task, event);
}

/*********************
 * Private Functions
 *********************/

/* This function gets the current time and updates the time in an LED display setting struct */
static void displayManager_time_to_display(display_setting_t* setting)
{
    time_t now = rtc_getTime(); //Assume this returns the current time in hours/minutes at least

    alarm_state_t alarmState;
    alarmManager_getState(&alarmState);

    setting->show_colon    = true;
    setting->alarm_enabled = alarmState.enabled;
    setting->hour[0]       = now.hour / 10;
    setting->hour[1]       = now.hour % 10;
    setting->minute[0]     = now.minute / 10;
    setting->minute[1]     = now.minute % 10;
}

/* This function gets the current alarm setting and updates the time in an LED display setting struct */
static void displayManager_alarm_to_display(display_setting_t* setting)
{
    alarm_state_t alarmState;
    alarmManager_getState(&alarmState);

    setting->show_colon    = true;
    setting->alarm_enabled = alarmState.enabled;

    if (setting->alarm_enabled)
    {
        //Alarm enabled. Show HH:MM
        setting->hour[0]   = alarmState.hour / 10;
        setting->hour[1]   = alarmState.hour % 10;
        setting->minute[0] = alarmState.minute / 10;
        setting->minute[1] = alarmState.minute % 10;
    }
    else
    {
        //Alarm disabled. Show --:--
        setting->hour[0]   = DIGIT_DASH;
        setting->hour[1]   = DIGIT_DASH;
        setting->minute[0] = DIGIT_DASH;
        setting->minute[1] = DIGIT_DASH;
    }
}

/* This function gets the current "single digit" mode setting and updates the time in an LED display setting struct */
static void displayManager_digit_to_display(display_setting_t* setting)
{
    alarm_state_t alarmState;
    alarmManager_getState(&alarmState);

    setting->show_colon    = false; //Not a time -- don't show the colon!
    setting->alarm_enabled = alarmState.enabled;
    setting->hour[0]       = DIGIT_BLANK;
    setting->hour[1]       = DIGIT_BLANK;
    setting->minute[0]     = DIGIT_BLANK;
    setting->minute[1]     = DIGIT_DISPLAY; //Show the hard-coded single digit in the right-most position
}

static void displayManager_task(void)
{
    // This enum holds the current mode (ie: state) of the DisplayManager
    enum {
        MODE_CLOCK,
        MODE_ALARM,
        MODE_DIGIT,
    } mode = MODE_CLOCK;

    // This value holds the number of seconds remaining before switching back to MODE_CLOCK
    uint8_t timeout = 0;

    // This will hold the current display setting value.
    display_setting_t setting;

    // Start the task by setting the display to show the time
    displayManager_time_to_display(&setting);
    display_update(&setting);

    // Run forever
    while (1)
    {
        /* Here we wait forever for an OS event. I am assuming two things:
           1. The event flags are automatically cleared after they are read
           2. More than one event flag can be set. If the user has managed to press both buttons simultaneously, or before this task could handle them individually, this could be a problem. In that case I am going to display the ALARM mode.
        */
        uint8_t event = OS_GET_EVENT(TIEMOUT_FOREVER);

        /* Handle button presses first to change the mode of the display.
           Note that if the user presses the button of the already-active mode, the timeout will reset.
           Also the user can switch modes at any time, without waiting for the clock to go back to clock mode.

           We're going to test button2 first and then button 1 *without* an else if; this way if both
           event bits are set, we will go into alarm mode */
        if (event & DISPLAYMANAGER_EVT_FLAG_BUTTON2)
        {
            mode = MODE_DIGIT;
            timeout = DISPLAY_MODE_TIMEOUT_S;
        }
        if (event & DISPLAYMANAGER_EVT_FLAG_BUTTON1)
        {
            mode = MODE_ALARM;
            timeout = DISPLAY_MODE_TIMEOUT_S;
        }

        /* If the event is a tick and ONLY a tick, update the display timeout. We don't want to do this
           if we just switched to a new mode, or it will be displayed 1 second less than expected. */
        if (event & DISPLAYMANAGER_EVT_FLAG_TICK == DISPLAYMANAGER_EVT_FLAG_TICK)
        {
            if (timeout > 0) {
                timeout--;
            }

            //If the timer expired, reset to the clock mode
            if (timeout == 0)
            {
                mode = MODE_CLOCK;
            }
        }

        /* Now update the clock display. */
        switch (mode)
        {
            case MODE_DIGIT:
                displayManager_digit_to_display(&setting);
                break;
            case MODE_ALARM:
                displayManager_alarm_to_display(&setting);
                break;
            case MODE_CLOCK:
            default:
                displayManager_time_to_display(&setting);
                break;
        }
        display_update(&setting);
    }
}