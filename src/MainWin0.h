#include "MainWin.h"
#include "EmMshtml/API.h"
#include <stdarg.h>

static int MyColonExHandler(char, PCSTR, int, void *, void *);
static int MyColonEx_wide(int mode, void **ppsz);

#define RAYMAI_PRINTF_MALLOC0  MemAllocZero
#define RAYMAI_PRINTF_MEMFREE  MemFree
#define RAYMAI_PRINTF_CHARTYPE  TCHAR
#define RAYMAI_PRINTF_ENABLE_ASCII_FMTSTR
#define RAYMAI_PRINTF_ENABLE_COLONEX
#define RAYMAI_PRINTF_ENABLE_LONGLONG
#define RAYMAI_PRINTF_COLONEX_HANDLER  MyColonExHandler
#include <raymai/RayPrinf.h>

#define MY_0B  MySelf * const pSelf

typedef struct MySelf MySelf;
typedef struct MyEmMshtmlClient MyEmMshtmlClient;
struct MySelf {
	HWND hwndSelf;
	HBRUSH hbrBackground;
	int cxClient, cyClient;
#define MY_htm  (0, pSelf->pEmMshtmlServer)
	IEmMshtmlServer *pEmMshtmlServer;
#define MY_EmMshtml_B  IEmMshtmlClient * const p_I
#define MY_EmMshtml_BdSelf  MySelf * const pSelf = ((MyEmMshtmlClient*)p_I)->pSelf
	struct MyEmMshtmlClient {
		IEmMshtmlClient I;
		MySelf *pSelf;
	} EmMshtmlClient;
};



#define MY__B  MY_EmMshtml_B

static void MyEmMshtml_OnDocumentComplete(MY__B,
	PCWSTR pszUrl);
static void MyEmMshtml_OnLoadHtmlComplete(MY__B,
	HRESULT hResult,
	void *pUser);

static void MyEmMshtml_OnNavigating(MY__B,
	PCWSTR pszUrl,
	PCWSTR pszTargetFrameName,
	PCWSTR pszHeaders,
	BOOL *pCancel);
static void MyEmMshtml_OnNavigateComplete(MY__B,
	PCWSTR pszUrl);
static void MyEmMshtml_OnNavigateError(MY__B,
	PCWSTR pszUrl,
	PCWSTR pszTargetFrameName,
	LONG statusCode,
	BOOL *pCancel);

static void MyEmMshtml_OnDownloadBegin(MY__B);
static void MyEmMshtml_OnDownloadEnd(MY__B);
static void MyEmMshtml_OnDownloadProgressChange(MY__B,
	LONG cbReceived,
	LONG cbTotal);

static void MyEmMshtml_OnTitleChange(MY__B,
	PCWSTR pszNewTitle);

static IEmMshtmlClient const s_IEmMsHtmlClient = {
	MyEmMshtml_OnDocumentComplete,
	MyEmMshtml_OnLoadHtmlComplete,
	MyEmMshtml_OnNavigating,
	MyEmMshtml_OnNavigateComplete,
	MyEmMshtml_OnNavigateError,
	MyEmMshtml_OnDownloadBegin,
	MyEmMshtml_OnDownloadEnd,
	MyEmMshtml_OnDownloadProgressChange,
	MyEmMshtml_OnTitleChange
};
#undef MY__B



static HWND My__Cw(void *pUser);
static LRESULT CALLBACK My__WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static BOOL My_OnCreate(MY_0B);
static void My_OnSize(MY_0B);
static void My_OnCommand(MY_0B, int id, int nCode);

static void My_ShowMenu_LoadHtml(MY_0B);
static void My_LoadHtml1(MY_0B);

static void My_ShowMenu_Go(MY_0B);
static void My_GotoBlank(MY_0B);
static void My_GotoNavCanceled(MY_0B);
static void My_GotoDiskC(MY_0B);
static void My_GotoEnteredUrl(MY_0B);

static void My_Get_UI_Options(MY_0B);
static void My_Set_UI_Options(MY_0B);

static void __cdecl My_AppendLog(MY_0B, PCSTR pszFmt, ...);
static void My_MoveCtl(MY_0B, int id, int x, int y, int w, int h);

enum {
	MYID__First = 99,
	btnBack,
	btnForward,
	btnRefresh,
	btnLoadHtml,
	cmdLoadHtml1,
	txtEnteredUrl,
	btnGo,
	cmdGotoBlank,
	cmdGotoNavCanceled,
	cmdGotoDiskC,
	cmdGotoEnteredURL,
	chkNo3dBorder,
	chkNoScroll,
	btnSetUIOpt,
	btnGetUIOpt,
	lbxLog
};

