#pragma once
#include "pch.h"
#include <Psapi.h>

class WinApi {
public:
	static DWORD GetProcessBaseAddress(DWORD processID);
};