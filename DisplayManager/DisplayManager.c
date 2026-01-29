#include "DisplayManager.h"
#include "internal/DisplayManagerInternal.h"

bool displayManager_init(void)
{
    //Calls the internal init function
    return displayManager_init();
}

bool displayManager_signal_tick(void)
{
    return displayManager_signal(DISPLAYMANAGER_EVT_FLAG_TICK);
}

bool displayManager_signal_button1_pressed(void)
{
    return displayManager_signal(DISPLAYMANAGER_EVT_FLAG_BUTTON1);
}

bool displayManager_signal_button2_pressed(void)
{
    return displayManager_signal(DISPLAYMANAGER_EVT_FLAG_BUTTON2);
}
