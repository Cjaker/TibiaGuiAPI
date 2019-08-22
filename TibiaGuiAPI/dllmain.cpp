// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "WinApi.h"
#include <sstream>

DWORD baseAddr = 0x0;

// offsets
DWORD allocBytesOffset = 0x266BC3;
DWORD initializeGUICompositeOffset = 0x165AB0;
DWORD setWidthHeightOffset = 0x165B50; // unstable
DWORD createGUITextRectangleOffset = 0xBE380;

// struct sizes
DWORD GUIDecorateLineSize = 0x28;
DWORD GUICompositeSize = 0x2C;
DWORD GUILinkSize = 0x3C;
DWORD GUITextRectangleSize = 0x64;
DWORD GUIFrameWindowSize = 0x68;
DWORD GUIModalDialogSize = 0xCC;


// alloc bytes
// return address to allocated block
void* allocBytes(uint32_t bytesCount) {
	void* val = (void*)0x0;

	_asm {
		push bytesCount
		call allocBytesOffset
		add esp, 4
		mov val, eax
	}

	return val;
}

// initialize GUICompositeObject
// return initialized object addr
void* initializeGUICompositeObject(void* object) {
	void* val = (void*)0x0;
	_asm {
		mov ecx, object
		call initializeGUICompositeOffset
		mov val, eax
	}

	return val;
}

// set width/height of GUICompositeObject (maybe for anothers objects too)
// return nothing?
void setWidthHeight(void* object, uint32_t width, uint32_t height) {
	_asm {
		push height
		push width
		mov ecx, object
		call setWidthHeightOffset
		add esp, 8
	}
}

// create object of type: GUITextRectangle
void createGUITextRectangle(void* GUITextRectangleObject, int a1, int a2, int a3, int a4, int a5, const char* text, int a7, int a8) {
	_asm {
		push a8
		push a7
		push text
		push a5
		push a4
		push a3
		push a2
		push a1
		mov ecx, GUITextRectangleObject
		call createGUITextRectangleOffset
		add esp, 32
	}
}

// create the info window
void createInfoWindow() {
	void* GUICompositeObject = allocBytes(GUICompositeSize);
	initializeGUICompositeObject(GUICompositeObject);
	void* GUITextRectangleObject = allocBytes(GUITextRectangleSize);
	const char* text = "Tibia Windows Client\nVersion 11\nCopyright (C) 2002-2016\nCipSoft GmbH\nAll rights reserved.";
	createGUITextRectangle(GUITextRectangleObject, 4, 0xB0, 0xB0, 0xB0, 1, text, 0, 0);
	void* GUILinkObject = allocBytes(GUILinkSize);
	// not done
}

void initialize() {
	baseAddr = WinApi::GetProcessBaseAddress(GetCurrentProcessId());
	// initialize offsets
	allocBytesOffset += baseAddr;
	initializeGUICompositeOffset += baseAddr;
	setWidthHeightOffset += baseAddr;
	createGUITextRectangleOffset += baseAddr;

	// do the first test
	createInfoWindow();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		initialize();
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

