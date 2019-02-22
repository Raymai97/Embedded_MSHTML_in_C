#include "L1_0.h"

HRESULT New_EmMshtmlServer(
	IEmMshtmlServer **pp_I_Self, IEmMshtmlClient *pClient)
{
	HRESULT hr = 0;
	MySelf *pSelf = NULL;
	if (!pClient) {
		hr = E_INVALIDARG; goto eof;
	}
	pSelf = MemAllocZero(sizeof(*pSelf));
	if (!pSelf) {
		hr = E_OUTOFMEMORY; goto eof;
	}
	pSelf->S.pClient = pClient;
	pSelf->I = s_IEmMshtmlServer;
	pSelf->S.ComUnknown.I.lpVtbl = &s_IComUnknown;
	pSelf->S.ComUnknown.pSelf = pSelf;
	pSelf->S.DocHostUIHandler.I.lpVtbl = &s_IDocHostUIHandler;
	pSelf->S.DocHostUIHandler.pSelf = pSelf;
	pSelf->S.OleClientSite.I.lpVtbl = &s_IOleClientSite;
	pSelf->S.OleClientSite.pSelf = pSelf;
	pSelf->S.OleInPlaceSite.I.lpVtbl = &s_IOleInPlaceSite;
	pSelf->S.OleInPlaceSite.pSelf = pSelf;
	pSelf->S.WebBrowserEvents.I.WebBrowserEvents.lpVtbl = &s_IWebBrowserEvents;
	pSelf->S.WebBrowserEvents.pSelf = pSelf;
	pSelf->S.oleInit = SUCCEEDED(OleInitialize(NULL));

	hr = CoCreateInstance(&CLSID_WebBrowser, NULL, CLSCTX_INPROC_SERVER,
		&IID_IOleObject, &pSelf->S.pWB_Obj);
	if (FAILED(hr)) goto eof;
	if (!MYwbo) { hr = E_UNEXPECTED; goto eof; }
	
	hr = MY_wbo->SetClientSite(MYwbo, &pSelf->S.OleClientSite.I);
	if (FAILED(hr)) goto eof;

	hr = MY_wbo->QueryInterface(MYwbo, &IID_IWebBrowser2,
		&pSelf->S.pWB);
	if (FAILED(hr)) goto eof;
	if (!MYwb) { hr = E_UNEXPECTED; goto eof; }

	hr = MY_wbo->QueryInterface(MYwbo, &IID_IOleInPlaceObject,
		&pSelf->S.pWB_IPO);
	if (FAILED(hr)) goto eof;
	if (!MYwbi) { hr = E_UNEXPECTED; goto eof; }

	hr = MY_wbo->QueryInterface(MYwbo, &IID_IOleInPlaceActiveObject,
		&pSelf->S.pWB_IPAO);
	if (FAILED(hr)) goto eof;
	if (!MYwbia) { hr = E_UNEXPECTED; goto eof; }

	hr = MY_wbo->QueryInterface(MYwbo, &IID_IConnectionPointContainer,
		&pSelf->S.pWB_CPC);
	if (FAILED(hr)) goto eof;
	if (!MYwbcpc) { hr = E_UNEXPECTED; goto eof; }

	hr = MY_wbcpc->FindConnectionPoint(MYwbcpc, &DIID_DWebBrowserEvents2,
		&pSelf->S.pWB_CP);
	if (FAILED(hr)) goto eof;
	if (!MYwbcp) { hr = E_UNEXPECTED; goto eof; }
	
	hr = MY_wbcp->Advise(MYwbcp, &pSelf->S.WebBrowserEvents.I.Unknown,
		&pSelf->S.wb_ConnectionPointCookie);
	if (FAILED(hr)) goto eof;

	*pp_I_Self = &pSelf->I;
eof:
	if (FAILED(hr)) {
		MY_SafeFree(pSelf, My__DelSelf);
	}
	return hr;
}

void Del_EmMshtmlServer(IEmMshtmlServer *p_I_Self)
{
	MY_1BdSelf;
	My__DelSelf(pSelf);
}

static void My__DelSelf(MY_0B)
{
	if (pSelf->S.wb_ConnectionPointCookie) {
		MY_wbcp->Unadvise(MYwbcp, pSelf->S.wb_ConnectionPointCookie);
		pSelf->S.wb_ConnectionPointCookie = 0;
	}
	MY_SafeFreeCOM(pSelf->S.pWB_CP);
	MY_SafeFreeCOM(pSelf->S.pWB_CPC);
	MY_SafeFreeCOM(pSelf->S.pWB_IPAO);
	MY_SafeFreeCOM(pSelf->S.pWB_IPO);
	MY_SafeFreeCOM(pSelf->S.pWB);
	MY_SafeFreeCOM(pSelf->S.pWB_Obj);
	if (pSelf->S.oleInit) {
		OleUninitialize();
		pSelf->S.oleInit = FALSE;
	}
	MemFree(pSelf);
}

static HRESULT My__QI(MY_0B, IID const *pIID, void **ppObj)
{
	if (IsEqualIID(pIID, &IID_IUnknown)) {
		*ppObj = &pSelf->S.ComUnknown.I;
	}
	else if (IsEqualIID(pIID, &IID_IDocHostUIHandler)) {
		*ppObj = &pSelf->S.DocHostUIHandler.I;
	}
	else if (IsEqualIID(pIID, &IID_IOleClientSite)) {
		*ppObj = &pSelf->S.OleClientSite.I;
	}
	else if (IsEqualIID(pIID, &IID_IOleInPlaceSite)) {
		*ppObj = &pSelf->S.OleInPlaceSite.I;
	}
	else if (IsEqualIID(pIID, &DIID_DWebBrowserEvents2)) {
		*ppObj = &pSelf->S.WebBrowserEvents.I;
	}
	else {
		*ppObj = NULL;
		return E_NOTIMPL;
	}
	return S_OK;
}

static ULONG My__AddRef(MY_0B)
{
	pSelf->S.oleRefCount += 1;
	return pSelf->S.oleRefCount;
}

static ULONG My__Release(MY_0B)
{
	if (pSelf->S.oleRefCount > 0) {
		pSelf->S.oleRefCount -= 1;
	}
	return pSelf->S.oleRefCount;
}



static HRESULT MyNewBSTR(BSTR *pbstr, PCWSTR pszSrc)
{
	BSTR bs = SysAllocString(pszSrc);
	if (!bs) { return E_OUTOFMEMORY; }
	*pbstr = bs;
	return S_OK;
}

static void MyDelBSTR(BSTR bstr)
{
	SysFreeString(bstr);
}

static HRESULT My_NavToUrl(MY_0B,
	PCWSTR pszUrl)
{
	HRESULT hr = 0;
	BSTR bsUrl = NULL;
	VARIANT vt0 = { VT_EMPTY };
	hr = MyNewBSTR(&bsUrl, pszUrl);
	if (FAILED(hr)) goto eof;
	hr = MY_wb->Navigate(MYwb, bsUrl, &vt0, &vt0, &vt0, &vt0);
eof:
	MY_SafeFree(bsUrl, MyDelBSTR);
	return hr;
}

static HRESULT My_SetCurrDocHtml(MY_0B,
	BSTR bstrHtml)
{
	HRESULT hr = 0;
	IDispatch *pDisp = NULL;
	IHTMLDocument2 *pDoc = NULL;
	SAFEARRAYBOUND saBound = { 1 };
	SAFEARRAY *pSA = NULL;
	VARIANT *pVT = NULL;
	
	hr = MY_wb->get_Document(MYwb, &pDisp);
	if (FAILED(hr)) goto eof;
	if (!pDisp) { hr = E_UNEXPECTED; goto eof; }

	hr = pDisp->lpVtbl->QueryInterface(pDisp, &IID_IHTMLDocument2, &pDoc);
	if (FAILED(hr)) goto eof;
	if (!pDoc) { hr = E_UNEXPECTED; goto eof; }

	pSA = SafeArrayCreate(VT_VARIANT, 1, &saBound);
	if (!pSA) {
		hr = E_OUTOFMEMORY; goto eof;
	}
	hr = SafeArrayAccessData(pSA, &pVT);
	if (FAILED(hr)) goto eof;
	if (!pVT) { hr = E_UNEXPECTED; goto eof; }

	pVT->vt = VT_BSTR;
	pVT->bstrVal = bstrHtml;
	hr = pDoc->lpVtbl->write(pDoc, pSA);
	if (FAILED(hr)) goto eof;
	hr = pDoc->lpVtbl->close(pDoc);
eof:
	/* SafeArrayDestroy destroy its elements automatically. */
	/* In this case, the BSTR was provided by outsider, we */
	/* don't want SafeArray destroy it, so we set to NULL. */
	if (pVT) { pVT->bstrVal = NULL; }
	MY_SafeFreeSA(pSA);
	MY_SafeFreeCOM(pDoc);
	MY_SafeFreeCOM(pDisp);
	return hr;
}



static HRESULT MyAPI_InitPlaceOn(MY_1B,
	HWND hwndParent)
{
	MY_1BdSelf;
	pSelf->S.wb_hwndParent = hwndParent;
	return MY_wbo->DoVerb(MYwbo, OLEIVERB_SHOW, NULL, &pSelf->S.OleClientSite.I, 0, NULL, NULL);
}
static HRESULT MyAPI_SetXywh(MY_1B,
	int x, int y, int w, int h)
{
	MY_1BdSelf;
	RECT r; SetRect(&r, x, y, x + w, y + h);
	return MY_wbi->SetObjectRects(MYwbi, &r, &r);
}
static HRESULT MyAPI_TranslateAccel(MY_1B,
	LPMSG lpMsg)
{
	MY_1BdSelf;
	return MY_wbia->TranslateAccelerator(MYwbia, lpMsg);
}

static HRESULT MyAPI_Navigate(MY_1B,
	PCWSTR pszUrl)
{
	MY_1BdSelf;
	return My_NavToUrl(pSelf, pszUrl);
}
static HRESULT MyAPI_LoadHtmlAsync(MY_1B,
	PCWSTR pszHtml, void *pUser)
{
	MY_1BdSelf;
	HRESULT hr = 0;
	BSTR bsHtml = NULL;
	if (pSelf->S.loadhtmlasync_html) {
		return E_ACCESSDENIED;
	}
	hr = MyNewBSTR(&bsHtml, pszHtml);
	if (FAILED(hr)) {
		return hr;
	}
	pSelf->S.loadhtmlasync_html = bsHtml;
	pSelf->S.loadhtmlasync_user = pUser;
	hr = My_NavToUrl(pSelf, L"about:blank");
	if (FAILED(hr)) {
		pSelf->S.loadhtmlasync_html = NULL;
		pSelf->S.loadhtmlasync_user = NULL;
		MY_SafeFree(bsHtml, MyDelBSTR);
	}
	return hr;
}

static HRESULT MyAPI_GoBack(MY_1B)
{
	MY_1BdSelf;
	return MY_wb->GoBack(MYwb);
}
static HRESULT MyAPI_GoForward(MY_1B)
{
	MY_1BdSelf;
	return MY_wb->GoForward(MYwb);
}
static HRESULT MyAPI_Refresh(MY_1B)
{
	MY_1BdSelf;
	return MY_wb->Refresh(MYwb);
}

static void MyAPI_Set_UI_Options(MY_1B,
	EmMshtml_UI_Options const *pOpt)
{
	MY_1BdSelf;
	pSelf->S.UI_Options = *pOpt;
}
static void MyAPI_Get_UI_Options(MY_1B,
	EmMshtml_UI_Options *pOpt)
{
	MY_1BdSelf;
	*pOpt = pSelf->S.UI_Options;
}



#define MY__B  MY_ComUnknown_B
#define MY__BdSelf  MY_ComUnknown_BdSelf

static HRESULT WINAPI MyComUnknown_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj)
{
	MY__BdSelf; return My__QI(pSelf, pIID, ppObj);
}
static ULONG WINAPI MyComUnknown_AddRef(MY__B)
{
	MY__BdSelf; return My__AddRef(pSelf);
}
static ULONG WINAPI MyComUnknown_Release(MY__B)
{
	MY__BdSelf; return My__Release(pSelf);
}
#undef MY__B
#undef MY__BdSelf



#define MY__B  MY_DocHostUIHandler_B
#define MY__BdSelf  MY_DocHostUIHandler_BdSelf

static HRESULT WINAPI MyDocHostUIHandler_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj)
{
	MY__BdSelf; return My__QI(pSelf, pIID, ppObj);
}
static ULONG WINAPI MyDocHostUIHandler_AddRef(MY__B)
{
	MY__BdSelf; return My__AddRef(pSelf);
}
static ULONG WINAPI MyDocHostUIHandler_Release(MY__B)
{
	MY__BdSelf; return My__Release(pSelf);
}

static HRESULT WINAPI MyDocHostUIHandler_ShowContextMenu(MY__B,
	DWORD dwID,
	POINT *ppt,
	IUnknown *pcmdtReserved,
	IDispatch *pdispReserved)
{
	UNREFERENCED_PARAMETER((p_I_This, dwID, ppt, pcmdtReserved, pdispReserved));
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_GetHostInfo(MY__B,
	DOCHOSTUIINFO *pInfo)
{
	MY__BdSelf;
	EmMshtml_UI_Options const *po = &pSelf->S.UI_Options;
	ZeroMemory(pInfo, sizeof(*pInfo));
	pInfo->cbSize = sizeof(*pInfo);
	if (po->no3dBorder) {
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
	}
	if (po->noScroll) {
		pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	}
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_ShowUI(MY__B,
	DWORD dwID,
	IOleInPlaceActiveObject *pActiveObject,
	IOleCommandTarget *pCommandTarget,
	IOleInPlaceFrame *pFrame,
	IOleInPlaceUIWindow *pDoc)
{
	UNREFERENCED_PARAMETER((p_I_This, dwID, pActiveObject, pCommandTarget, pFrame, pDoc));
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_HideUI(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_UpdateUI(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_EnableModeless(MY__B,
	BOOL fEnable)
{
	UNREFERENCED_PARAMETER((p_I_This, fEnable));
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_OnDocWindowActivate(MY__B,
	BOOL fActivate)
{
	UNREFERENCED_PARAMETER((p_I_This, fActivate));
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_OnFrameWindowActivate(MY__B,
	BOOL fActivate)
{
	UNREFERENCED_PARAMETER((p_I_This, fActivate));
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_ResizeBorder(MY__B,
	LPCRECT prcBorder,
	IOleInPlaceUIWindow *pUIWindow,
	BOOL fRameWindow)
{
	UNREFERENCED_PARAMETER((p_I_This, prcBorder, pUIWindow, fRameWindow));
	return S_OK;
}
static HRESULT WINAPI MyDocHostUIHandler_TranslateAccelerator(MY__B,
	LPMSG lpMsg,
	const GUID *pguidCmdGroup,
	DWORD nCmdID)
{
	UNREFERENCED_PARAMETER((p_I_This, lpMsg, pguidCmdGroup, nCmdID));
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_GetOptionKeyPath(MY__B,
	LPOLESTR *pchKey,
	DWORD dw)
{
	UNREFERENCED_PARAMETER((p_I_This, pchKey, dw));
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_GetDropTarget(MY__B,
	IDropTarget *pDropTarget,
	IDropTarget **ppDropTarget)
{
	UNREFERENCED_PARAMETER((p_I_This, pDropTarget));
	*ppDropTarget = NULL;
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_GetExternal(MY__B,
	IDispatch **ppDispatch)
{
	UNREFERENCED_PARAMETER(p_I_This);
	*ppDispatch = NULL;
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_TranslateUrl(MY__B,
	DWORD dwTranslate,
	LPWSTR pchURLIn,
	LPWSTR *ppchURLOut)
{
	UNREFERENCED_PARAMETER((p_I_This, dwTranslate, pchURLIn));
	*ppchURLOut = NULL;
	return S_FALSE;
}
static HRESULT WINAPI MyDocHostUIHandler_FilterDataObject(MY__B,
	IDataObject *pDO,
	IDataObject **ppDORet)
{
	UNREFERENCED_PARAMETER((p_I_This, pDO));
	*ppDORet = NULL;
	return S_FALSE;
}
#undef MY__B
#undef MY__BdSelf



#define MY__B  MY_OleClientSite_B
#define MY__BdSelf  MY_OleClientSite_BdSelf

static HRESULT WINAPI MyOleClientSite_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj)
{
	MY__BdSelf; return My__QI(pSelf, pIID, ppObj);
}
static ULONG WINAPI MyOleClientSite_AddRef(MY__B)
{
	MY__BdSelf; return My__AddRef(pSelf);
}
static ULONG WINAPI MyOleClientSite_Release(MY__B)
{
	MY__BdSelf; return My__Release(pSelf);
}

static HRESULT WINAPI MyOleClientSite_SaveObject(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleClientSite_GetMoniker(MY__B,
	DWORD dwAssign,
	DWORD dwWhichMoniker,
	IMoniker **ppmk)
{
	UNREFERENCED_PARAMETER((p_I_This, dwAssign, dwWhichMoniker));
	*ppmk = NULL;
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleClientSite_GetContainer(MY__B,
	IOleContainer **ppContainer)
{
	UNREFERENCED_PARAMETER(p_I_This);
	*ppContainer = NULL;
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleClientSite_ShowObject(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleClientSite_OnShowWindow(MY__B,
	BOOL fShow)
{
	UNREFERENCED_PARAMETER((p_I_This, fShow));
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleClientSite_RequestNewObjectLayout(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return E_NOTIMPL;
}

#undef MY__B
#undef MY__BdSelf



#define MY__B  MY_OleInPlaceSite_B
#define MY__BdSelf  MY_OleInPlaceSite_BdSelf

static HRESULT WINAPI MyOleInPlaceSite_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj)
{
	MY__BdSelf; return My__QI(pSelf, pIID, ppObj);
}
static ULONG WINAPI MyOleInPlaceSite_AddRef(MY__B)
{
	MY__BdSelf; return My__AddRef(pSelf);
}
static ULONG WINAPI MyOleInPlaceSite_Release(MY__B)
{
	MY__BdSelf; return My__Release(pSelf);
}

static HRESULT WINAPI MyOleInPlaceSite_GetWindow(MY__B,
	HWND *pHwnd)
{
	MY__BdSelf;
	*pHwnd = pSelf->S.wb_hwndParent;
	return S_OK;
}
static HRESULT WINAPI MyOleInPlaceSite_ContextSensitiveHelp(MY__B,
	BOOL fEnterMode)
{
	UNREFERENCED_PARAMETER((p_I_This, fEnterMode));
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleInPlaceSite_CanInPlaceActivate(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_OK;
}
static HRESULT WINAPI MyOleInPlaceSite_OnInPlaceActivate(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_OK;
}
static HRESULT WINAPI MyOleInPlaceSite_OnUIActivate(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_OK;
}
static HRESULT WINAPI MyOleInPlaceSite_GetWindowContext(MY__B,
	IOleInPlaceFrame **ppFrame,
	IOleInPlaceUIWindow **ppDoc,
	LPRECT lprcPosRect,
	LPRECT lprcClipRect,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	UNREFERENCED_PARAMETER((p_I_This, lprcPosRect, lprcClipRect));
	*ppFrame = NULL;
	*ppDoc = NULL;
	ZeroMemory(lpFrameInfo, sizeof(*lpFrameInfo));
	lpFrameInfo->cb = sizeof(*lpFrameInfo);
	return S_OK;
}
static HRESULT WINAPI MyOleInPlaceSite_Scroll(MY__B,
	SIZE scrollExtent)
{
	UNREFERENCED_PARAMETER((p_I_This, scrollExtent));
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleInPlaceSite_OnUIDeactivate(MY__B,
	BOOL fUndoable)
{
	UNREFERENCED_PARAMETER((p_I_This, fUndoable));
	return S_FALSE;
}
static HRESULT WINAPI MyOleInPlaceSite_OnInPlaceDeactivate(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return S_FALSE;
}
static HRESULT WINAPI MyOleInPlaceSite_DiscardUndoState(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleInPlaceSite_DeactivateAndUndo(MY__B)
{
	UNREFERENCED_PARAMETER(p_I_This);
	return E_NOTIMPL;
}
static HRESULT WINAPI MyOleInPlaceSite_OnPosRectChange(MY__B,
	LPCRECT lprcPosRect)
{
	UNREFERENCED_PARAMETER((p_I_This, lprcPosRect));
	return E_NOTIMPL;
}
#undef MY__B
#undef MY__BdSelf


#define MY__B  MY_WebBrowserEvents_B
#define MY__BdSelf  MY_WebBrowserEvents_BdSelf

static HRESULT WINAPI MyWebBrowserEvents_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj)
{
	MY__BdSelf;
	return My__QI(pSelf, pIID, ppObj);
}
static ULONG WINAPI MyWebBrowserEvents_AddRef(MY__B)
{
	MY__BdSelf;
	return My__AddRef(pSelf);
}
static ULONG WINAPI MyWebBrowserEvents_Release(MY__B)
{
	MY__BdSelf;
	return My__Release(pSelf);
}

static HRESULT WINAPI MyWebBrowserEvents_GetTypeInfoCount(MY__B,
	UINT *pctinfo)
{
	UNREFERENCED_PARAMETER((p_I_This, pctinfo));
	return E_NOTIMPL;
}
static HRESULT WINAPI MyWebBrowserEvents_GetTypeInfo(MY__B,
	UINT iTInfo,
	LCID lcid,
	ITypeInfo **ppTInfo)
{
	UNREFERENCED_PARAMETER((p_I_This, iTInfo, lcid));
	*ppTInfo = NULL;
	return E_NOTIMPL;
}
static HRESULT WINAPI MyWebBrowserEvents_GetIDsOfNames(MY__B,
	IID const *pIID,
	LPOLESTR *rgszNames,
	UINT cNames,
	LCID lcid,
	DISPID *rgDispId)
{
	UNREFERENCED_PARAMETER((p_I_This, pIID, rgszNames, cNames, lcid, rgDispId));
	return E_NOTIMPL;
}
static HRESULT WINAPI MyWebBrowserEvents_Invoke(MY__B,
	DISPID dispIdMember,
	IID const *pIID,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS *pDispParams,
	VARIANT *pVarResult,
	EXCEPINFO *pExcepInfo,
	UINT *puArgErr)
{
	typedef VARIANT_BOOL VBOO;
	enum { vbTrue = VARIANT_TRUE, vbFalse = VARIANT_FALSE };
	MY__BdSelf;
	VARIANT * const params = pDispParams->rgvarg;
	UNREFERENCED_PARAMETER((pIID, lcid, wFlags, pVarResult, pExcepInfo, puArgErr));
	/*
		Parameters are in reverse order.
		Example: DocumentComplete(pDispatch, URL)
		params[0] for URL, params[1] for pDispatch
	*/
	if (dispIdMember == DISPID_DOCUMENTCOMPLETE) {
		BSTR const Url = params[0].pvarVal->bstrVal;
		if (pSelf->S.loadhtmlasync_html) {
			HRESULT hResult = My_SetCurrDocHtml(pSelf,
				pSelf->S.loadhtmlasync_html);
			if (MY_PC->OnLoadHtmlComplete) {
				MY_PC->OnLoadHtmlComplete(MY_PC,
					hResult, pSelf->S.loadhtmlasync_user);
			}
			MY_SafeFree(pSelf->S.loadhtmlasync_html, MyDelBSTR);
		}
		else if (MY_PC->OnDocumentComplete) {
			MY_PC->OnDocumentComplete(MY_PC, Url);
		}
	}
	else if (dispIdMember == DISPID_BEFORENAVIGATE2) {
		BSTR const Url = params[5].pvarVal->bstrVal;
		/* Skip Flags */
		BSTR const TargetFrameName = params[3].pvarVal->bstrVal;
		/* Skip PostData */
		BSTR const Headers = params[1].pvarVal->bstrVal;
		VBOO * const pCancel = &params[0].pvarVal->boolVal;
		BOOL bCancel = (*pCancel == vbTrue);
		if (pSelf->S.loadhtmlasync_html) {
		}
		else if (MY_PC->OnNavigating) {
			MY_PC->OnNavigating(MY_PC,
				Url, TargetFrameName, Headers, &bCancel);
		}
		*pCancel = bCancel ? vbTrue : vbFalse;
	}
	else if (dispIdMember == DISPID_NAVIGATECOMPLETE2) {
		BSTR const Url = params[0].pvarVal->bstrVal;
		if (pSelf->S.loadhtmlasync_html) {
		}
		else if (MY_PC->OnNavigateComplete) {
			MY_PC->OnNavigateComplete(MY_PC, Url);
		}
	}
	else if (dispIdMember == DISPID_NAVIGATEERROR) {
		BSTR const Url = params[3].pvarVal->bstrVal;
		BSTR const TargetFrameName = params[2].pvarVal->bstrVal;
		LONG const StatusCode = params[1].pvarVal->lVal;
		VBOO * const pCancel = &params[0].pvarVal->boolVal;
		BOOL bCancel = (*pCancel == vbTrue);
		if (MY_PC->OnNavigateError) {
			MY_PC->OnNavigateError(MY_PC,
				Url, TargetFrameName, StatusCode, &bCancel);
		}
		*pCancel = bCancel ? vbTrue : vbFalse;
	}
	else if (dispIdMember == DISPID_DOWNLOADBEGIN) {
		/* No Parameter */
		if (MY_PC->OnDownloadBegin) {
			MY_PC->OnDownloadBegin(MY_PC);
		}
	}
	else if (dispIdMember == DISPID_DOWNLOADCOMPLETE) {
		/* No Parameter */
		if (MY_PC->OnDownloadEnd) {
			MY_PC->OnDownloadEnd(MY_PC);
		}
	}
	else if (dispIdMember == DISPID_PROGRESSCHANGE) {
		LONG const cbReceived = params[1].lVal;
		LONG const cbTotal = params[0].lVal;
		if (MY_PC->OnDownloadProgressChange) {
			MY_PC->OnDownloadProgressChange(MY_PC,
				cbReceived == -1 ? cbTotal : cbReceived,
				cbTotal);
		}
	}
	else if (dispIdMember == DISPID_TITLECHANGE) {
		BSTR const NewTitle = params[0].bstrVal;
		if (MY_PC->OnTitleChange) {
			MY_PC->OnTitleChange(MY_PC, NewTitle);
		}
	}
	return S_OK;
}
#undef MY__B
#undef MY__BdSelf
