#ifndef __OEM_IO_Ctrl_H__
#define __OEM_IO_Ctrl_H__


#define KBDIO_BASE 0x800 

#define KBD_IOCTL_GET_INPUT_MODE \
        CTL_CODE(FILE_DEVICE_UNKNOWN,KBDIO_BASE, METHOD_BUFFERED, FILE_ANY_ACCESS)
   
#define KBD_IOCTL_SET_INPUT_MODE \
        CTL_CODE(FILE_DEVICE_UNKNOWN,(KBDIO_BASE+1), METHOD_BUFFERED, FILE_ANY_ACCESS)
  
#define KBD_IOCTL_GET_KEYPAD_STATE \
        CTL_CODE(FILE_DEVICE_UNKNOWN,(KBDIO_BASE+2), METHOD_BUFFERED, FILE_ANY_ACCESS)
   
#define KBD_IOCTL_SET_KEYPAD_STATE \
        CTL_CODE(FILE_DEVICE_UNKNOWN,(KBDIO_BASE+3), METHOD_BUFFERED, FILE_ANY_ACCESS)        
        
typedef struct _KeypadLock
{   	           
    DWORD  VK_Code;
    BOOL   LockState;
}KeypadLock, *lpKeypadLock;        
                                    
                                    
#endif //__OEM_IO_Ctrl_H__                 
                                    
                                    