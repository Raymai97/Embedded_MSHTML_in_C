#pragma once
#include <assert.h>
#include <Windows.h>

typedef struct AppGlobalProp AppGlobalProp;
struct AppGlobalProp {
	BOOL hadInit;
	HANDLE hProcessHeap;
	HINSTANCE hInstApp;
};
extern AppGlobalProp x_AppGlobalProp;

#define APPhInst  (0, x_AppGlobalProp.hInstApp)
#define APPhHeap  (0, x_AppGlobalProp.hProcessHeap)

void AppInit(HINSTANCE hInstApp);

void * MemAllocZero(SIZE_T cb);
void MemFree(void *ptr);
