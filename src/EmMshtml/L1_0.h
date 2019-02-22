#include "API.h"
#ifndef CONST_VTABLE
#define CONST_VTABLE /* Make lpVtbl a pointer to const struct */
#endif
#pragma warning(push, 3)
#include <objbase.h>
#include <ExDisp.h> /* for IWebBrowser2 */
#include <ExDispid.h> /* for DISPID_DOCUMENTCOMPLETE */
#include <MsHTML.h> /* for IHTMLDocument2 */
#include <MsHtmHst.h> /* for IDocHostUIHandler */
#pragma warning(pop)

#define MY_CAT_(a,b)  a ## b
#define MY_CAT(a,b)  MY_CAT_(a,b)
#define MY_SafeFree(p,fn)  if (p) { fn(p); (p) = NULL; }
#define MY_SafeFreeCOM(p)  MY_SafeFree(p, p->lpVtbl->Release)
#define MY_SafeFreeSA(sa)  MY_SafeFree(sa, SafeArrayDestroy)

#define MySelf  EmMshtmlServerSelf
#define MY_0B  MySelf * const pSelf
#define MY_ImplFakeRefCount  MY__BdSelf; pSelf; return 1

typedef struct IEmMshtmlServer I_Self;
typedef struct SEmMshtmlServer S_Self;
typedef struct MySelf MySelf;
typedef struct MyComUnknown MyComUnknown;
typedef struct MyDocHostUIHandler MyDocHostUIHandler;
typedef struct MyOleClientSite MyOleClientSite;
typedef struct MyOleInPlaceSite MyOleInPlaceSite;
typedef struct MyWebBrowserEvents MyWebBrowserEvents;

struct SEmMshtmlServer {
	struct MyComUnknown {
		IUnknown I;
		MySelf *pSelf;
	} ComUnknown;
	struct MyDocHostUIHandler {
		IDocHostUIHandler I;
		MySelf *pSelf;
	} DocHostUIHandler;
	struct MyOleClientSite {
		IOleClientSite I;
		MySelf *pSelf;
	} OleClientSite;
	struct MyOleInPlaceSite {
		IOleInPlaceSite I;
		MySelf *pSelf;
	} OleInPlaceSite;
	struct MyWebBrowserEvents {
		union {
			IUnknown Unknown;
			DWebBrowserEvents2 WebBrowserEvents;
		} I;
		MySelf *pSelf;
	} WebBrowserEvents;
#define MY_PC  (0, pSelf->S.pClient)
	IEmMshtmlClient *pClient;
#define MYwbo  (0, pSelf->S.pWB_Obj)
#define MY_wbo (0, pSelf->S.pWB_Obj->lpVtbl)
	IOleObject *pWB_Obj;
#define MYwb  (0, pSelf->S.pWB)
#define MY_wb (0, pSelf->S.pWB->lpVtbl)
	IWebBrowser2 *pWB;
#define MYwbi  (0, pSelf->S.pWB_IPO)
#define MY_wbi (0, pSelf->S.pWB_IPO->lpVtbl)
	IOleInPlaceObject *pWB_IPO;
#define MYwbia  (0, pSelf->S.pWB_IPAO)
#define MY_wbia (0, pSelf->S.pWB_IPAO->lpVtbl)
	IOleInPlaceActiveObject *pWB_IPAO;
#define MYwbcpc  (0, pSelf->S.pWB_CPC)
#define MY_wbcpc (0, pSelf->S.pWB_CPC->lpVtbl)
	IConnectionPointContainer *pWB_CPC;
#define MYwbcp  (0, pSelf->S.pWB_CP)
#define MY_wbcp (0, pSelf->S.pWB_CP->lpVtbl)
	IConnectionPoint *pWB_CP;
	BOOL oleInit;
	LONG oleRefCount;
	DWORD wb_ConnectionPointCookie;
	HWND wb_hwndParent;
	EmMshtml_UI_Options UI_Options;
	BSTR loadhtmlasync_html;
	void *loadhtmlasync_user;
};

struct MySelf {
	I_Self I;
	S_Self S;
};

static void My__DelSelf(MY_0B);
static HRESULT My__QI(MY_0B, IID const *pIID, void **ppObj);
static ULONG My__AddRef(MY_0B);
static ULONG My__Release(MY_0B);

static HRESULT MyNewBSTR(BSTR *pbstr, PCWSTR pszSrc);
static void MyDelBSTR(BSTR bstr);

static HRESULT My_NavToUrl(MY_0B,
	PCWSTR pszUrl);

static HRESULT My_SetCurrDocHtml(MY_0B,
	BSTR bstrHtml);



#define MY_1B  I_Self * const p_I_Self
#define MY_1BdSelf  MySelf * const pSelf = (MySelf*)p_I_Self

static HRESULT MyAPI_InitPlaceOn(MY_1B,
	HWND hwndParent);
static HRESULT MyAPI_SetXywh(MY_1B,
	int x, int y, int w, int h);
static HRESULT MyAPI_TranslateAccel(MY_1B,
	LPMSG lpMsg);

static HRESULT MyAPI_Navigate(MY_1B,
	PCWSTR pszUrl);
static HRESULT MyAPI_LoadHtmlAsync(MY_1B,
	PCWSTR pszHtml, void *pUser);

static HRESULT MyAPI_GoBack(MY_1B);
static HRESULT MyAPI_GoForward(MY_1B);
static HRESULT MyAPI_Refresh(MY_1B);

static void MyAPI_Set_UI_Options(MY_1B,
	EmMshtml_UI_Options const *pOpt);
static void MyAPI_Get_UI_Options(MY_1B,
	EmMshtml_UI_Options *pOpt);

static IEmMshtmlServer const s_IEmMshtmlServer = {
	MyAPI_InitPlaceOn,
	MyAPI_SetXywh,
	MyAPI_TranslateAccel,
	MyAPI_Navigate,
	MyAPI_LoadHtmlAsync,
	MyAPI_GoBack,
	MyAPI_GoForward,
	MyAPI_Refresh,
	MyAPI_Get_UI_Options,
	MyAPI_Set_UI_Options
};



#define MY_ComUnknown_B  IUnknown *p_I_This
#define MY_ComUnknown_BdSelf  MySelf * const pSelf = ((MyComUnknown*)p_I_This)->pSelf
#define MY__B  MY_ComUnknown_B

static HRESULT WINAPI MyComUnknown_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj);
static ULONG WINAPI MyComUnknown_AddRef(MY__B);
static ULONG WINAPI MyComUnknown_Release(MY__B);

static IUnknownVtbl const s_IComUnknown = {
	MyComUnknown_QueryInterface,
	MyComUnknown_AddRef,
	MyComUnknown_Release
};
#undef MY__B



#define MY_DocHostUIHandler_B  IDocHostUIHandler *p_I_This
#define MY_DocHostUIHandler_BdSelf  MySelf * const pSelf = ((MyDocHostUIHandler*)p_I_This)->pSelf
#define MY__B  MY_DocHostUIHandler_B

static HRESULT WINAPI MyDocHostUIHandler_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj);
static ULONG WINAPI MyDocHostUIHandler_AddRef(MY__B);
static ULONG WINAPI MyDocHostUIHandler_Release(MY__B);

static HRESULT WINAPI MyDocHostUIHandler_ShowContextMenu(MY__B,
	DWORD dwID,
	POINT *ppt,
	IUnknown *pcmdtReserved,
	IDispatch *pdispReserved);
static HRESULT WINAPI MyDocHostUIHandler_GetHostInfo(MY__B,
	DOCHOSTUIINFO *pInfo);
static HRESULT WINAPI MyDocHostUIHandler_ShowUI(MY__B,
	DWORD dwID,
	IOleInPlaceActiveObject *pActiveObject,
	IOleCommandTarget *pCommandTarget,
	IOleInPlaceFrame *pFrame,
	IOleInPlaceUIWindow *pDoc);
static HRESULT WINAPI MyDocHostUIHandler_HideUI(MY__B);
static HRESULT WINAPI MyDocHostUIHandler_UpdateUI(MY__B);
static HRESULT WINAPI MyDocHostUIHandler_EnableModeless(MY__B,
	BOOL fEnable);
static HRESULT WINAPI MyDocHostUIHandler_OnDocWindowActivate(MY__B,
	BOOL fActivate);
static HRESULT WINAPI MyDocHostUIHandler_OnFrameWindowActivate(MY__B,
	BOOL fActivate);
static HRESULT WINAPI MyDocHostUIHandler_ResizeBorder(MY__B,
	LPCRECT prcBorder,
	IOleInPlaceUIWindow *pUIWindow,
	BOOL fRameWindow);
static HRESULT WINAPI MyDocHostUIHandler_TranslateAccelerator(MY__B,
	LPMSG lpMsg,
	const GUID *pguidCmdGroup,
	DWORD nCmdID);
static HRESULT WINAPI MyDocHostUIHandler_GetOptionKeyPath(MY__B,
	LPOLESTR *pchKey,
	DWORD dw);
static HRESULT WINAPI MyDocHostUIHandler_GetDropTarget(MY__B,
	IDropTarget *pDropTarget,
	IDropTarget **ppDropTarget);
static HRESULT WINAPI MyDocHostUIHandler_GetExternal(MY__B,
	IDispatch **ppDispatch);
static HRESULT WINAPI MyDocHostUIHandler_TranslateUrl(MY__B,
	DWORD dwTranslate,
	LPWSTR pchURLIn,
	LPWSTR *ppchURLOut);
static HRESULT WINAPI MyDocHostUIHandler_FilterDataObject(MY__B,
	IDataObject *pDO,
	IDataObject **ppDORet);

static IDocHostUIHandlerVtbl const s_IDocHostUIHandler = {
	MyDocHostUIHandler_QueryInterface,
	MyDocHostUIHandler_AddRef,
	MyDocHostUIHandler_Release,
	MyDocHostUIHandler_ShowContextMenu,
	MyDocHostUIHandler_GetHostInfo,
	MyDocHostUIHandler_ShowUI,
	MyDocHostUIHandler_HideUI,
	MyDocHostUIHandler_UpdateUI,
	MyDocHostUIHandler_EnableModeless,
	MyDocHostUIHandler_OnDocWindowActivate,
	MyDocHostUIHandler_OnFrameWindowActivate,
	MyDocHostUIHandler_ResizeBorder,
	MyDocHostUIHandler_TranslateAccelerator,
	MyDocHostUIHandler_GetOptionKeyPath,
	MyDocHostUIHandler_GetDropTarget,
	MyDocHostUIHandler_GetExternal,
	MyDocHostUIHandler_TranslateUrl,
	MyDocHostUIHandler_FilterDataObject
};
#undef MY__B



#define MY_OleClientSite_B  IOleClientSite *p_I_This
#define MY_OleClientSite_BdSelf  MySelf * const pSelf = ((MyOleClientSite*)p_I_This)->pSelf
#define MY__B  MY_OleClientSite_B

static HRESULT WINAPI MyOleClientSite_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj);
static ULONG WINAPI MyOleClientSite_AddRef(MY__B);
static ULONG WINAPI MyOleClientSite_Release(MY__B);

static HRESULT WINAPI MyOleClientSite_SaveObject(MY__B);
static HRESULT WINAPI MyOleClientSite_GetMoniker(MY__B,
	DWORD dwAssign,
	DWORD dwWhichMoniker,
	IMoniker **ppmk);
static HRESULT WINAPI MyOleClientSite_GetContainer(MY__B,
	IOleContainer **ppContainer);
static HRESULT WINAPI MyOleClientSite_ShowObject(MY__B);
static HRESULT WINAPI MyOleClientSite_OnShowWindow(MY__B,
	BOOL fShow);
static HRESULT WINAPI MyOleClientSite_RequestNewObjectLayout(MY__B);

static IOleClientSiteVtbl const s_IOleClientSite = {
	MyOleClientSite_QueryInterface,
	MyOleClientSite_AddRef,
	MyOleClientSite_Release,
	MyOleClientSite_SaveObject,
	MyOleClientSite_GetMoniker,
	MyOleClientSite_GetContainer,
	MyOleClientSite_ShowObject,
	MyOleClientSite_OnShowWindow,
	MyOleClientSite_RequestNewObjectLayout
};
#undef MY__B



#define MY_OleInPlaceSite_B  IOleInPlaceSite *p_I_This
#define MY_OleInPlaceSite_BdSelf  MySelf * const pSelf = ((MyOleInPlaceSite*)p_I_This)->pSelf
#define MY__B  MY_OleInPlaceSite_B

static HRESULT WINAPI MyOleInPlaceSite_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj);
static ULONG WINAPI MyOleInPlaceSite_AddRef(MY__B);
static ULONG WINAPI MyOleInPlaceSite_Release(MY__B);

static HRESULT WINAPI MyOleInPlaceSite_GetWindow(MY__B,
	HWND *pHwnd);
static HRESULT WINAPI MyOleInPlaceSite_ContextSensitiveHelp(MY__B,
	BOOL fEnterMode);
static HRESULT WINAPI MyOleInPlaceSite_CanInPlaceActivate(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_OnInPlaceActivate(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_OnUIActivate(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_GetWindowContext(MY__B,
	IOleInPlaceFrame **ppFrame,
	IOleInPlaceUIWindow **ppDoc,
	LPRECT lprcPosRect,
	LPRECT lprcClipRect,
	LPOLEINPLACEFRAMEINFO lpFrameInfo);
static HRESULT WINAPI MyOleInPlaceSite_Scroll(MY__B,
	SIZE scrollExtent);
static HRESULT WINAPI MyOleInPlaceSite_OnUIDeactivate(MY__B,
	BOOL fUndoable);
static HRESULT WINAPI MyOleInPlaceSite_OnInPlaceDeactivate(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_DiscardUndoState(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_DeactivateAndUndo(MY__B);
static HRESULT WINAPI MyOleInPlaceSite_OnPosRectChange(MY__B,
	LPCRECT lprcPosRect);

static IOleInPlaceSiteVtbl const s_IOleInPlaceSite = {
	MyOleInPlaceSite_QueryInterface,
	MyOleInPlaceSite_AddRef,
	MyOleInPlaceSite_Release,
	MyOleInPlaceSite_GetWindow,
	MyOleInPlaceSite_ContextSensitiveHelp,
	MyOleInPlaceSite_CanInPlaceActivate,
	MyOleInPlaceSite_OnInPlaceActivate,
	MyOleInPlaceSite_OnUIActivate,
	MyOleInPlaceSite_GetWindowContext,
	MyOleInPlaceSite_Scroll,
	MyOleInPlaceSite_OnUIDeactivate,
	MyOleInPlaceSite_OnInPlaceDeactivate,
	MyOleInPlaceSite_DiscardUndoState,
	MyOleInPlaceSite_DeactivateAndUndo,
	MyOleInPlaceSite_OnPosRectChange
};
#undef MY__B



#define MY_WebBrowserEvents_B  DWebBrowserEvents2 *p_I_This
#define MY_WebBrowserEvents_BdSelf  MySelf * const pSelf = ((MyWebBrowserEvents*)p_I_This)->pSelf
#define MY__B  MY_WebBrowserEvents_B

static HRESULT WINAPI MyWebBrowserEvents_QueryInterface(MY__B,
	IID const *pIID,
	void **ppObj);
static ULONG WINAPI MyWebBrowserEvents_AddRef(MY__B);
static ULONG WINAPI MyWebBrowserEvents_Release(MY__B);

static HRESULT WINAPI MyWebBrowserEvents_GetTypeInfoCount(MY__B,
	UINT *pctinfo);
static HRESULT WINAPI MyWebBrowserEvents_GetTypeInfo(MY__B,
	UINT iTInfo,
	LCID lcid,
	ITypeInfo **ppTInfo);
static HRESULT WINAPI MyWebBrowserEvents_GetIDsOfNames(MY__B,
	IID const *pIID,
	LPOLESTR *rgszNames,
	UINT cNames,
	LCID lcid,
	DISPID *rgDispId);
static HRESULT WINAPI MyWebBrowserEvents_Invoke(MY__B,
	DISPID dispIdMember,
	IID const *pIID,
	LCID lcid,
	WORD wFlags,
	DISPPARAMS *pDispParams,
	VARIANT *pVarResult,
	EXCEPINFO *pExcepInfo,
	UINT *puArgErr);

static DWebBrowserEvents2Vtbl const s_IWebBrowserEvents = {
	MyWebBrowserEvents_QueryInterface,
	MyWebBrowserEvents_AddRef,
	MyWebBrowserEvents_Release,
	MyWebBrowserEvents_GetTypeInfoCount,
	MyWebBrowserEvents_GetTypeInfo,
	MyWebBrowserEvents_GetIDsOfNames,
	MyWebBrowserEvents_Invoke
};
#undef MY__B
