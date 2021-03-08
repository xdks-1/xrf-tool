#pragma once
#include <stdio.h>
#include <intrin.h>
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <vector>
#include <psapi.h>
#include <SubAuth.h>
#include <lmcons.h>
#include <TlHelp32.h>

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
