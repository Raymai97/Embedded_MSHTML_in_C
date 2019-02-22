#include "Base.h"

AppGlobalProp x_AppGlobalProp;

void AppInit(HINSTANCE hInstApp)
{
	AppGlobalProp *p = &x_AppGlobalProp;
	assert(p->hadInit == FALSE);
	p->hadInit = TRUE;
	p->hProcessHeap = GetProcessHeap();
	p->hInstApp = hInstApp;
}

void * MemAllocZero(SIZE_T cb)
{
	return HeapAlloc(APPhHeap, HEAP_ZERO_MEMORY, cb);
}

void MemFree(void *ptr)
{
	HeapFree(APPhHeap, 0, ptr);
}
