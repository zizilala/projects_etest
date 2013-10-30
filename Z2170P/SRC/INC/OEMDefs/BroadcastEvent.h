
#ifndef __Broadcast_Event_H__
#define __Broadcast_Event_H__

//WM_APP (0x8000 ~ 0xBFFF) 

#define SYS_SUSPEND_NOTIFY				TEXT("SYSTEM/SUSPEND")
#define SYS_STANDBY_NOTIFY				TEXT("SYSTEM/STANDBY")
#define SYS_EMERGENCY_SHUTDOWN_NOTIFY	TEXT("SYSTEM/EMERGENCY")

#define BT_RDY_NOTIFY               	TEXT("BT/Ready/Notification")

#define BCR_LED_NOTIFY              	TEXT("BCR_LED/Notification")
#define BCR_START_SCAN_NOTIFY			TEXT("BCR/Start/Scan/Notification")
#define BCR_STOP_SCAN_NOTIFY			TEXT("BCR/Stop/Scan/Notification")

#define KEYPAD_BACKLIGHT_NOTIFY			TEXT("Keypad/UserActivity")
#define GWE_USER_ACTIVE_NOTIFY			TEXT("PowerManager/ActivityTimer/UserActivity")
#define LCD_BACKLIGHT_ACTIVE_NOTIFY		TEXT("BklActive")


#define WM_MEMORY_LOW_NOTIFY	(WM_APP + 110)
#define KEY_HOOK_OFF			(WM_APP + 164)
#define KEY_HOOK_ON				(WM_APP + 165)
#define WM_BARCODE_NOTIFY		(WM_APP + 180)
#define WM_SYSTEM_NOTIFY		(WM_APP + 198) 

#endif
