#pragma once
#include <Windows.h>

void * MemAllocZero(SIZE_T cb);
void MemFree(void *ptr);

typedef struct IEmMshtmlServer IEmMshtmlServer;
typedef struct IEmMshtmlClient IEmMshtmlClient;

HRESULT New_EmMshtmlServer(IEmMshtmlServer **pp_I_Self, IEmMshtmlClient *pClient);
void Del_EmMshtmlServer(IEmMshtmlServer *p_I_Self);

typedef struct EmMshtml_UI_Options EmMshtml_UI_Options;
struct EmMshtml_UI_Options {
	BOOL no3dBorder : 1;
	BOOL noScroll : 1;
};

struct IEmMshtmlServer {
#define H__B  IEmMshtmlServer * const p_I_Self

	HRESULT(*InitPlaceOn)(H__B,
		HWND hwndParent);

	HRESULT(*SetXywh)(H__B,
		int x, int y, int w, int h);
	
	HRESULT(*TranslateAccel)(H__B,
		LPMSG lpMsg);

	/*
		Invokes OnNavigateXXX and OnDocumentComplete.
	*/
	HRESULT(*Navigate)(H__B,
		PCWSTR pszUrl);

	/*
		Invokes OnLoadHtmlComplete.
	*/
	HRESULT(*LoadHtmlAsync)(H__B,
		PCWSTR pszHtml, void *pUser);

	HRESULT(*GoBack)(H__B);
	HRESULT(*GoForward)(H__B);
	HRESULT(*Refresh)(H__B);

	/*
		Take effects only when navigate or refresh (newer IE).
	*/
	void(*Get_UI_Options)(H__B, EmMshtml_UI_Options *pOpt);
	void(*Set_UI_Options)(H__B, EmMshtml_UI_Options const *pOpt);

#undef H__B
};

struct IEmMshtmlClient {
#define H__B  IEmMshtmlClient * const p_I_Self

	/* Beyond this line are optional: */

	void(*OnDocumentComplete)(H__B,
		PCWSTR pszUrl);
	void(*OnLoadHtmlComplete)(H__B,
		HRESULT hResult,
		void *pUser);

	void(*OnNavigating)(H__B,
		PCWSTR pszUrl,
		PCWSTR pszTargetFrameName,
		PCWSTR pszHeaders,
		BOOL *pCancel);
	void(*OnNavigateComplete)(H__B,
		PCWSTR pszUrl);
	void(*OnNavigateError)(H__B,
		PCWSTR pszUrl,
		PCWSTR pszTargetFrameName,
		LONG statusCode,
		BOOL *pCancel);

	void(*OnDownloadBegin)(H__B);
	void(*OnDownloadEnd)(H__B);
	void(*OnDownloadProgressChange)(H__B,
		LONG cbReceived,
		LONG cbTotal);

	void(*OnTitleChange)(H__B,
		PCWSTR pszNewTitle);

#undef H__B
};
