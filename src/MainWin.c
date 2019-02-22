#include "MainWin0.h"

static HWND My__Cw(void *pUser)
{
	LPCTSTR const lpszClass = TEXT("TEMP_EmbedMshtmlHello.MainWin");
	WNDCLASS wc = { 0 };
	wc.cbWndExtra = sizeof(LPVOID);
	wc.hInstance = APPhInst;
	wc.lpfnWndProc = My__WndProc;
	wc.lpszClassName = lpszClass;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	return CreateWindow(lpszClass, TEXT("EmMshtml sample"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, APPhInst, pUser);
}

void App_MainWin_ShowModal(int nCmdShow)
{
	MySelf *pSelf = NULL;
	HWND hwnd = NULL;
	MSG msg = { 0 };
	pSelf = MemAllocZero(sizeof(*pSelf));
	if (!pSelf) {
		MessageBox(NULL, TEXT("MainWin Self alloc failed."), NULL, MB_ICONERROR);
		goto eof;
	}
	hwnd = My__Cw(pSelf);
	if (!hwnd) {
		MessageBox(NULL, TEXT("MainWin window creation failed."), NULL, MB_ICONERROR);
		goto eof;
	}
	ShowWindow(hwnd, nCmdShow);
	while (IsWindow(hwnd)) {
		GetMessage(&msg, NULL, 0, 0);
		if (MY_htm &&
			MY_htm->TranslateAccel(MY_htm, &msg) == S_OK) {
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
eof:
	if (pSelf) { MemFree(pSelf); }
}

static LRESULT CALLBACK My__WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	MySelf *pSelf = NULL;
	LRESULT lResult = 0;
	BOOL overriden = FALSE;
	if (msg == WM_NCCREATE) {
		LPVOID lpSelf = ((LPCREATESTRUCT)l)->lpCreateParams;
		pSelf = (MySelf*)(LPARAM)lpSelf;
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)lpSelf);
	}
	else {
		pSelf = (MySelf*)GetWindowLongPtr(hwnd, 0);
	}
	if (msg == WM_NCCREATE) {
		pSelf->hwndSelf = hwnd;
		pSelf->hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		pSelf->EmMshtmlClient.I = s_IEmMsHtmlClient;
		pSelf->EmMshtmlClient.pSelf = pSelf;
	}
	else if (msg == WM_NCDESTROY) {
		if (pSelf->hbrBackground) {
			DeleteObject(pSelf->hbrBackground);
		}
		if (pSelf->pEmMshtmlServer) {
			Del_EmMshtmlServer(pSelf->pEmMshtmlServer);
		}
		ZeroMemory(pSelf, sizeof(*pSelf));
	}
	else if (msg == WM_CREATE) {
		if (My_OnCreate(pSelf) == FALSE) {
			overriden = TRUE; lResult = -1;
		}
	}
	else if (msg == WM_CTLCOLORSTATIC) {
		overriden = TRUE;
		SetBkMode((HDC)w, TRANSPARENT);
		lResult = (LRESULT)pSelf->hbrBackground;
	}
	else if (msg == WM_PAINT) {
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, pSelf->hbrBackground);
		EndPaint(hwnd, &ps);
	}
	else if (msg == WM_SIZE) {
		pSelf->cxClient = (short)LOWORD(l);
		pSelf->cyClient = (short)HIWORD(l);
		My_OnSize(pSelf);
	}
	else if (msg == WM_COMMAND) {
		My_OnCommand(pSelf, LOWORD(w), HIWORD(w));
	}
	if (!overriden) {
		lResult = CallWindowProc(DefWindowProc, hwnd, msg, w, l);
	}
	return lResult;
}

static BOOL My_OnCreate(MY_0B)
{
	LPCTSTR pszErrSite = NULL;
	HRESULT hr = 0;
	hr = New_EmMshtmlServer(&pSelf->pEmMshtmlServer, &pSelf->EmMshtmlClient.I);
	if (FAILED(hr)) {
		pszErrSite = TEXT("New_EmMshtmlServer");
		goto eof;
	}
	hr = MY_htm->InitPlaceOn(MY_htm, pSelf->hwndSelf);
	if (FAILED(hr)) {
		pszErrSite = TEXT("htm->InitPlaceOn");
		goto eof;
	}

#define NEW_CTL(Id, Cls, Tx, St, StEx, X, Y, W, H) \
	CreateWindowEx(StEx, TEXT(Cls), TEXT(Tx), WS_CHILD | WS_VISIBLE | St, \
		X, Y, W, H, pSelf->hwndSelf, (HMENU)(WPARAM)(Id), NULL, NULL)

#define NEW_BTN(Id, Tx, X, Y, W, H) \
	NEW_CTL(Id, "Button", Tx, 0, 0, X, Y, W, H)

#define NEW_CHK(Id, Tx, X, Y, W, H) \
	NEW_CTL(Id, "Button", Tx, BS_AUTOCHECKBOX, 0, X, Y, W, H)

#define NEW_EDT(Id, St, X, Y, W, H) \
	NEW_CTL(Id, "Edit", "", St | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE, X, Y, W, H)

#define NEW_LBX(Id, St, X, Y, W, H) \
	NEW_CTL(Id, "ListBox", "", St | WS_VSCROLL, WS_EX_CLIENTEDGE, X, Y, W, H)

	NEW_BTN(btnBack, "Back", 10, 10, 50, 40);
	NEW_BTN(btnForward, "Forw", 60, 10, 50, 40);
	NEW_BTN(btnRefresh, "Refresh", 110, 10, 100, 40);
	NEW_BTN(btnLoadHtml, "LoadHtml", 210, 10, 100, 40);
	NEW_EDT(txtEnteredUrl, 0, 310, 15, 200, 30);
	NEW_BTN(btnGo, "Go", 510, 10, 40, 40);

	NEW_CHK(chkNo3dBorder, "No 3D border", 10, 50, 130, 40);
	NEW_CHK(chkNoScroll, "No Scroll", 140, 50, 110, 40);
	NEW_BTN(btnSetUIOpt, "Set UI", 250, 50, 80, 40);
	NEW_BTN(btnGetUIOpt, "Get UI", 330, 50, 80, 40);

	NEW_LBX(lbxLog, LBS_NOINTEGRALHEIGHT | LBS_NOTIFY, 0, 0, 0, 0);

#undef NEW_CTL
#undef NEW_BTN
#undef NEW_CHK
#undef NEW_EDT

eof:
	if (FAILED(hr)) {
		TCHAR sz[99] = { 0 };
		wsprintf(sz, TEXT("%s return 0x%.8X \n"), pszErrSite, hr);
		MessageBox(NULL, sz, NULL, MB_ICONERROR);
	}
	return SUCCEEDED(hr);
}

static void My_OnSize(MY_0B)
{
	RECT const reserved = { 10, 110, 10, 10 };
	int const cxMax = pSelf->cxClient - reserved.right - reserved.left;
	int const cyMax = pSelf->cyClient - reserved.bottom - reserved.top;
	struct { int x, y, w, h; } Htm, Log;
	Htm.x = reserved.left;
	Htm.y = reserved.top;
	Htm.w = cxMax;
	Htm.h = cyMax / 2;
	Log.x = Htm.x;
	Log.y = Htm.y + Htm.h;
	Log.w = Htm.w;
	Log.h = cyMax - Htm.h;
	if (MY_htm) {
		MY_htm->SetXywh(MY_htm, Htm.x, Htm.y, Htm.w, Htm.h);
	}
	My_MoveCtl(pSelf, lbxLog, Log.x, Log.y, Log.w, Log.h);
}

static void My_OnCommand(MY_0B, int id, int nCode)
{
	if (id == btnBack) {
		MY_htm->GoBack(MY_htm);
	}
	else if (id == btnForward) {
		MY_htm->GoForward(MY_htm);
	}
	else if (id == btnRefresh) {
		MY_htm->Refresh(MY_htm);
	}
	else if (id == btnLoadHtml) {
		My_ShowMenu_LoadHtml(pSelf);
	}
	else if (id == cmdLoadHtml1) {
		My_LoadHtml1(pSelf);
	}
	else if (id == btnGo) {
		My_ShowMenu_Go(pSelf);
	}
	else if (id == cmdGotoDiskC) {
		My_GotoDiskC(pSelf);
	}
	else if (id == cmdGotoBlank) {
		My_GotoBlank(pSelf);
	}
	else if (id == cmdGotoNavCanceled) {
		My_GotoNavCanceled(pSelf);
	}
	else if (id == cmdGotoEnteredURL) {
		My_GotoEnteredUrl(pSelf);
	}
	else if (id == btnGetUIOpt) {
		My_Get_UI_Options(pSelf);
	}
	else if (id == btnSetUIOpt) {
		My_Set_UI_Options(pSelf);
	}
	else if (id == lbxLog) {
		if (nCode == LBN_DBLCLK) {
			if (MessageBox(pSelf->hwndSelf, TEXT("Clear Log?"),
				TEXT(""), MB_ICONQUESTION | MB_YESNO) == IDYES) {
				HWND const hCtl = GetDlgItem(pSelf->hwndSelf, lbxLog);
				SendMessage(hCtl, LB_RESETCONTENT, 0, 0);
			}
		}
	}
}

static void My_ShowMenu_LoadHtml(MY_0B)
{
	RECT rc = { 0 };
	HMENU hMn = CreatePopupMenu();
	GetWindowRect(GetDlgItem(pSelf->hwndSelf, btnLoadHtml), &rc);
	AppendMenu(hMn, 0, cmdLoadHtml1, TEXT("1) Hello World"));
	TrackPopupMenu(hMn, TPM_LEFTALIGN, rc.left, rc.bottom, 0, pSelf->hwndSelf, NULL);
	DestroyMenu(hMn);
}
static void My_LoadHtml1(MY_0B)
{
	PCWSTR const pszHtml =
		L"<html><body>"
		L"<h1> Hello world! </h1>"
		L"</body></html>";
	MY_htm->LoadHtmlAsync(MY_htm, pszHtml, NULL);
}

static void My_ShowMenu_Go(MY_0B)
{
	RECT rc = { 0 };
	HMENU hMn = CreatePopupMenu();
	GetWindowRect(GetDlgItem(pSelf->hwndSelf, btnGo), &rc);
	AppendMenu(hMn, 0, cmdGotoEnteredURL, TEXT("Entered URL"));
	AppendMenu(hMn, 0, cmdGotoBlank, TEXT("about:blank"));
	AppendMenu(hMn, 0, cmdGotoNavCanceled, TEXT("about:NavigationCanceled"));
	AppendMenu(hMn, 0, cmdGotoDiskC, TEXT("C:\\"));
	SetMenuDefaultItem(hMn, cmdGotoEnteredURL, MF_BYCOMMAND);
	TrackPopupMenu(hMn, TPM_LEFTALIGN, rc.left, rc.bottom, 0, pSelf->hwndSelf, NULL);
	DestroyMenu(hMn);
}
static void My_GotoBlank(MY_0B)
{
	MY_htm->Navigate(MY_htm, L"about:blank");
}
static void My_GotoNavCanceled(MY_0B)
{
	MY_htm->Navigate(MY_htm, L"about:NavigationCanceled");
}
static void My_GotoDiskC(MY_0B)
{
	MY_htm->Navigate(MY_htm, L"file:///C:/");
}
static void My_GotoEnteredUrl(MY_0B)
{
	HWND const hCtl = GetDlgItem(pSelf->hwndSelf, txtEnteredUrl);
	HRESULT hr = 0;
	int cch = 0; PTSTR psz = NULL;
	int cchW = 0; PWSTR pszW = NULL;
	cch = GetWindowTextLength(hCtl);
	psz = MemAllocZero(sizeof(TCHAR) * (cch + 1));
	if (!psz) {
		MessageBox(hCtl, TEXT("URL alloc failed."), NULL, MB_ICONERROR);
		goto eof;
	}
	GetWindowText(hCtl, psz, cch + 1);
#ifndef UNICODE
	cchW = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	pszW = MemAllocZero(sizeof(WCHAR) * cchW);
	if (!pszW) {
		MessageBox(hCtl, TEXT("A2W alloc failed."), NULL, MB_ICONERROR);
		goto eof;
	}
	MultiByteToWideChar(CP_ACP, 0, psz, -1, pszW, cchW);
	hr = MY_htm->Navigate(MY_htm, pszW);
#else
	hr = MY_htm->Navigate(MY_htm, psz);
#endif
eof:
	if (pszW) { MemFree(pszW); }
	if (psz) { MemFree(psz); }
}

static void My_Get_UI_Options(MY_0B)
{
	HWND const hwnd = pSelf->hwndSelf;
	EmMshtml_UI_Options o = { 0 };
	MY_htm->Get_UI_Options(MY_htm, &o);
	CheckDlgButton(hwnd, chkNo3dBorder, o.no3dBorder);
	CheckDlgButton(hwnd, chkNoScroll, o.noScroll);
}
static void My_Set_UI_Options(MY_0B)
{
	HWND const hwnd = pSelf->hwndSelf;
	EmMshtml_UI_Options o = { 0 };
	o.no3dBorder = !!IsDlgButtonChecked(hwnd, chkNo3dBorder);
	o.noScroll = !!IsDlgButtonChecked(hwnd, chkNoScroll);
	MY_htm->Set_UI_Options(MY_htm, &o);
}

static void __cdecl My_AppendLog(MY_0B, PCSTR pszFmt, ...)
{
	HWND const hCtl = GetDlgItem(pSelf->hwndSelf, lbxLog);
	va_list ap;
	va_start(ap, pszFmt);
	SendMessageA(hCtl, LB_ADDSTRING, 0, (LPARAM)pszFmt);
	SendMessage(hCtl, LB_SETCURSEL, SendMessage(hCtl, LB_GETCOUNT, 0, 0) - 1, 0);
	va_end(ap);
}

static void My_MoveCtl(MY_0B, int id, int x, int y, int w, int h)
{
	MoveWindow(GetDlgItem(pSelf->hwndSelf, id), x, y, w, h, TRUE);
}



#define MY__B  MY_EmMshtml_B
#define MY__BdSelf  MY_EmMshtml_BdSelf

static void MyEmMshtml_OnDocumentComplete(MY__B,
	PCWSTR pszUrlW)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Document Complete"
		"  URL: %:w2t:s",
		pszUrlW);
	My_AppendLog(pSelf, "");
	My_AppendLog(pSelf, "");
}
static void MyEmMshtml_OnLoadHtmlComplete(MY__B,
	HRESULT hResult,
	void *pUser)
{
	MY__BdSelf;
	UNREFERENCED_PARAMETER(pUser);
	My_AppendLog(pSelf,
		"On LoadHtml Complete"
		"  hResult: 0x%.8X",
		hResult);
}

static void MyEmMshtml_OnNavigating(MY__B,
	PCWSTR pszUrlW,
	PCWSTR pszTargetFrameNameW,
	PCWSTR pszHeadersW,
	BOOL *pCancel)
{
	MY__BdSelf;
	UNREFERENCED_PARAMETER(pCancel);
	My_AppendLog(pSelf,
		"On Navigating"
		"  URL: %:w2t:s"
		"  Target Frame Name: %:w2t:s"
		"  Headers: %:w2t:s",
		pszUrlW,
		pszTargetFrameNameW,
		pszHeadersW);
}
static void MyEmMshtml_OnNavigateComplete(MY__B,
	PCWSTR pszUrlW)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Navigate Complete"
		"  URL: %:w2t:s",
		pszUrlW);
}
static void MyEmMshtml_OnNavigateError(MY__B,
	PCWSTR pszUrlW,
	PCWSTR pszTargetFrameNameW,
	LONG statusCode,
	BOOL *pCancel)
{
	MY__BdSelf;
	UNREFERENCED_PARAMETER(pCancel);
	My_AppendLog(pSelf,
		"On Navigation Error"
		"  URL: %:w2t:s"
		"  Target Frame Name: %:w2t:s"
		"  Status Code: %ld",
		pszUrlW,
		pszTargetFrameNameW,
		statusCode);
}

static void MyEmMshtml_OnDownloadBegin(MY__B)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Download Begin (no parameter)");
}
static void MyEmMshtml_OnDownloadEnd(MY__B)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Download End (no parameter)");
}
static void MyEmMshtml_OnDownloadProgressChange(MY__B,
	LONG cbReceived,
	LONG cbTotal)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Download Progress Change"
		"  Received %ld bytes"
		"  Total %ld bytes",
		cbReceived,
		cbTotal);
}

static void MyEmMshtml_OnTitleChange(MY__B,
	PCWSTR pszNewTitleW)
{
	MY__BdSelf;
	My_AppendLog(pSelf,
		"On Title Change"
		"  NewTitle: %:w2t:s",
		pszNewTitleW);
}
