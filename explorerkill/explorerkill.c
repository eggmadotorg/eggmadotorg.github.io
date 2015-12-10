#include <windows.h>
#include <Tlhelp32.h>

void killexplorer() {
	char buf[200];
	HANDLE hp;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	PROCESSENTRY32 pe32 = {0};

	if( hProcessSnap == INVALID_HANDLE_VALUE ) {
		wsprintf(buf, "CreateToolhelp32Snapshot failed: %d", GetLastError());
		MessageBox( 0, buf, "Error", MB_OK );
		return;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if( Process32First( hProcessSnap, &pe32 ) ) { 
		do {
			if(!lstrcmpi(pe32.szExeFile,"explorer.exe")) {
				hp = OpenProcess (PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
				if(!TerminateProcess(hp, 0)) {
					wsprintf(buf,"TerminateProcess failed: %d",GetLastError());
					MessageBox( 0, buf, "Error", MB_OK );
				}
				CloseHandle(hp);
			}
		} while( Process32Next( hProcessSnap, &pe32 ) );
	}
	CloseHandle( hProcessSnap );
}

// Here is a small program by Jeffrey Richter...
// Written 2000 Jeffrey Richter
LRESULT CALLBACK LowLevelKeyboardProc( int nCode,WPARAM wParam,LPARAM lParam) {
	PKBDLLHOOKSTRUCT p;
	if (nCode == HC_ACTION && (wParam==WM_KEYDOWN||wParam==WM_KEYUP)) {
		p = ( PKBDLLHOOKSTRUCT ) lParam;
		if(p->vkCode == VK_SCROLL) {
			if(wParam==WM_KEYDOWN)
				killexplorer();
			return 1;
		}
	}
	return CallNextHookEx( NULL, nCode, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hinstExe, HINSTANCE o, PTSTR pszCmdLine, int oo) {
	 HHOOK hhkLowLevelKybd	= SetWindowsHookEx( WH_KEYBOARD_LL,LowLevelKeyboardProc,hinstExe, 0 ); 
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

	 //MessageBox( NULL,"Click \"Ok\" to quit","Scroll Lock = Kill Explorer", MB_OK ); 


	 //UnhookWindowsHookEx(hhkLowLevelKybd); 

	 //return(0);
}
