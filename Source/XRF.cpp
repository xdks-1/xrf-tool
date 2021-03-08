#include "XRF.h"

void PrintConsoleRevision()
{
	int regs[4];

	__cpuid(regs, CPUID_HW_REV);
	int consoleRevId = regs[2] & 0xFFFF;

	char *consoleRev = (char *)calloc(1, 0x100);

	switch (consoleRevId)
	{
	case HW_REV_DURANGO:
		consoleRev = "Durango";
		break;
	case HW_REV_SILVERTON_ZORRO:
		consoleRev = "Silverton Zorro";
		break;
	case HW_REV_SILVERTON_MANDA:
		consoleRev = "Silverton Manda";
		break;
	case HW_REV_CARMEL_BASE:
		consoleRev = "Carmel Base";
		break;
	case HW_REV_CARMEL_4K:
		consoleRev = "Carmel 4K";
		break;
	case HW_REV_EDMONTON:
		consoleRev = "Edmonton";
		break;
	case HW_REV_SCORPIO:
		consoleRev = "Scorpio";
		break;
	default:
		consoleRev = "Unknown";
	}
	printf("Console Revision: %s (0x%04X)\n", consoleRev, consoleRevId);
}

void PrintDevkitType()
{
	HANDLE hHandle = CreateFileA(
		PIPE_NAME_PSPSRA,
		GENERIC_READ | GENERIC_WRITE,
		OPEN_EXISTING,
		0,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hHandle == INVALID_HANDLE_VALUE) {
		printf("Could not get handle of %s\n", PIPE_NAME_PSPSRA);
		return;
	}

	DWORD read;
	LPVOID buf[SIZE_PSPSRA_CAPABILITES];

	BOOL ret = DeviceIoControl(
		hHandle,
		IOCTL_PIPE_PSPSRA_READ_CAPABILITIES,
		NULL,
		0,
		&buf,
		SIZE_PSPSRA_CAPABILITES,
		&read,
		NULL
	);

	if (!ret) {
		printf("READ_CAPABILITIES IOCTL failed!\n");
		return;
	}

	if (read != SIZE_PSPSRA_CAPABILITES) {
		printf("Invalid size console serial returned!\n Exp: %i - Got: %i\n",
			SIZE_PSPSRA_CAPABILITES,
			read
		);
		return;
	}

	UINT16 capabilities[0x100] = { 0 };
	memcpy(&capabilities, buf, 0x100);

	UINT16 cap = 0;
	char *flavour = (char *)calloc(1, 100);

	for (int i = 0; i < 0x100; i++) {
		cap = capabilities[i];
		switch (cap) {
		case DEVKIT_SRA:
			flavour = "DEVKIT SRA";
			break;
		case DEVKIT_ERA:
			flavour = "DEVKIT ERA";
			break;
		case DEVKIT_MS:
			flavour = "DEVKIT MS";
			break;
		case DEVKIT_SP:
			flavour = "DEVKIT SP";
			break;
		}
		if (strlen(flavour) > 0)
			break;
	}

	if (strlen(flavour) <= 0)
		flavour = "RETAIL";

	printf("Console Flavour: %s\n", flavour);
}

void PrintCurrentUser()
{
	char username[UNLEN + 1];
	DWORD usernameLength = UNLEN + 1;
	GetUserNameA(username, &usernameLength);
	printf("Running as: %s", username);
}

void EnableDebugPriv()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}

BOOL TerminateProcessEx(const char* processname, UINT uExitCode)
{
	BOOL result;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_stricmp(entry.szExeFile, processname) == 0)
			{
				DWORD dwDesiredAccess = PROCESS_TERMINATE;
				BOOL  bInheritHandle = FALSE;
				HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, entry.th32ProcessID);
				if (hProcess == NULL)
					return FALSE;

				result = TerminateProcess(hProcess, uExitCode);
				CloseHandle(hProcess);
			}
		}
	}
	CloseHandle(snapshot);

	return result;
}