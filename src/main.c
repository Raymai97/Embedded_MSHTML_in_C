#include "MainWin.h"

int AppMain(HINSTANCE hInstApp, int nCmdShow)
{
	AppInit(hInstApp);
	App_MainWin_ShowModal(nCmdShow);
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpCmdl, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstPrev);
	UNREFERENCED_PARAMETER(lpCmdl);
	return AppMain(hInst, nCmdShow);
}

void RawMain(void)
{
	HINSTANCE const hInst = GetModuleHandle(NULL);
	int nCmdShow = SW_SHOWNORMAL;
	STARTUPINFO si = { sizeof(si) };
	GetStartupInfo(&si);
	if (si.dwFlags & STARTF_USESHOWWINDOW) {
		nCmdShow = si.wShowWindow;
	}
	ExitProcess(AppMain(hInst, nCmdShow));
}
