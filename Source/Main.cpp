#include "XRF.h"
#include "Flash/XBFS.h"

#define CMD_ALL				"cinfo, dispflash, readflash, dumpflash, whoami, terminate"

#define CMD_CONSOLE_INFO	"cinfo"
#define CMD_DISPFLASH		"dispflash"
#define CMD_READFLASH		"readflash"
#define CMD_DUMPFLASH		"dumpflash"
#define CMD_WHOAMI		"whoami"
#define CMD_TERMINATE		"terminate"

int main(int argc, char *argv[])
{
	EnableDebugPriv();
	printf("[XRF] Hello!\n");

	if (argc < 2) {
		printf("[XRF] No parameters passed!\n\n");
		printf("[XRF] Commands: %s", CMD_ALL);
		return 1;
	}

	if (strncmp(CMD_CONSOLE_INFO, argv[1], strlen(CMD_CONSOLE_INFO)) == 0) {
		PrintConsoleRevision();
		PrintDevkitType();
	}
	else if (strncmp(CMD_DISPFLASH, argv[1], strlen(CMD_DISPFLASH)) == 0) {
		printf("List of XBFS Entires:\n");
		for (int i = 0; i < sizeof(XBFS_ENTRIES) / sizeof(char*); ++i) {
			printf("XBFS Entry: %s\n", XBFS_ENTRIES[i]);
		}
	}
	else if (strncmp(CMD_READFLASH, argv[1], strlen(CMD_READFLASH)) == 0) {
		if (argc < 3) {
			printf("Read Flash: Please supply a filename!\n");
			printf("Example: readflash sp_s.cfg D:\\DevelopmentFiles\n");
			return 3;
		}
		printf("WARNING! REQUIRES ADMINISTRATOR PRIVILEGES!\n");
		XBFS::Read(argv[2], argv[3], false);
	}
	else if (strncmp(CMD_DUMPFLASH, argv[1], strlen(CMD_DUMPFLASH)) == 0) {
		if (argc < 2) {
			printf("Dump Flash: Please supply a destination file name to dump to!\n");
			return 3;
		}
		printf("WARNING! REQUIRES ADMINISTRATOR PRIVILEGES!\n");
		XBFS::Read("", argv[2], true);
	}
	else if (strncmp(CMD_WHOAMI, argv[1], strlen(CMD_WHOAMI)) == 0) {
		PrintCurrentUser();
	}
	else if (strncmp(CMD_TERMINATE, argv[1], strlen(CMD_TERMINATE)) == 0) {
		if(argc < 3) {
			printf("Please supply processname to terminate!\n");
			return 3;
		}
		TerminateProcessEx(argv[2], 0);
	}
	else {
		printf("[XRF] Unknown command passed.\n");
		return 2;
	}
	return 0;
}
