#include "stdafx.h"
#include "resource.h"
#include "KBHook.h"

BOOL CALLBACK  KeyHookDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void FuncKeyHandle(DWORD VKCode,DWORD KEY_MESSAGE);

int WINAPI WinMain(	HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPTSTR    lpCmdLine,
			int       nCmdShow)
{
	HWND hwnd;
	MSG  msg;

	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);
	
	if(FindWindow(NULL,KEY_HOOK_WND))
	{
		return 0;
	}	
	RETAILMSG (1, (TEXT("*** Start Key Hook ***\r\n")));   
	       
	//creat the main window	        
	hwnd =(HWND) CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_FnKey_Dlg), NULL, KeyHookDlgProc, (LPARAM)hInstance);
       
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}

BOOL CALLBACK KeyHookDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	KBHook m_KBHook;
	PKBDLLHOOKSTRUCT KBScan;
	HWND hWnd;
	static BOOL HookState = FALSE;

	switch (message)
	{
		case WM_INITDIALOG:
			SetWindowText(hDlg,KEY_HOOK_WND);
			ShowWindow(hDlg, SW_HIDE);
			HookState = m_KBHook.StartKBHook(hDlg,(HINSTANCE)lParam );
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDCANCEL) || (LOWORD(wParam)==IDOK))
				ShowWindow(hDlg, SW_HIDE);
			return TRUE;

        case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;

		case WM_KBHOOK_NOTIFY:
			KBScan = (PKBDLLHOOKSTRUCT) lParam;
			RETAILMSG (1, (TEXT("OnKBHook (vkCode:%x scanCode:%x event:%x)\r\n"),KBScan->vkCode,KBScan->scanCode,wParam));
			hWnd = GetForegroundWindow();
			if(hWnd)
				PostMessage(hWnd, WM_KEY_EVENT_NOTIFY,KBScan->vkCode, wParam);
			//======================================
			//Function Key --> Quick Launch
			//======================================
			//FuncKeyHandle(KBScan->vkCode,wParam);
			return TRUE;  		          
	             		         	                	         
		case KEY_HOOK_OFF:
			if (HookState)
				m_KBHook.StopKBHook();
			return TRUE;

		case KEY_HOOK_ON:
			if (HookState == FALSE) 
				HookState = m_KBHook.StartKBHook(hDlg,(HINSTANCE)lParam );
			return TRUE;
	}
	
	return FALSE;
}

void FuncKeyHandle(DWORD VKCode,DWORD KEY_MESSAGE)
{
	UNREFERENCED_PARAMETER(VKCode);
	UNREFERENCED_PARAMETER(KEY_MESSAGE);
	
	
}

