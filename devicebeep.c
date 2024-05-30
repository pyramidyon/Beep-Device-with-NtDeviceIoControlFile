#pragma once
#pragma comment(lib, "ntdll")

#include <Windows.h>
#include <stdio.h>

typedef struct _BEEP_SET_PARAMETERS {
	ULONG Frequency;
	ULONG Duration;
} BEEP_SET_PARAMETERS, * PBEEP_SET_PARAMETERS;

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, * PLSA_UNICODE_STRING, UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


#define STATUS_SUCCESS									((NTSTATUS)0x00000000L)
#define OBJ_CASE_INSENSITIVE							0x00000040
#define OBJ_KERNEL_HANDLE								0x00000200
#define FILE_OPEN										0x00000001
#define InitializeObjectAttributes(p, n, a, r, s) \
{ \
	(p)->Length = sizeof(OBJECT_ATTRIBUTES); \
	(p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
	(p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}

VOID myRtlInitUnicodeString(OUT PUNICODE_STRING DestinationString, IN PCWSTR SourceString OPTIONAL)
{

	SIZE_T Length;

	DestinationString->MaximumLength = 0;
	DestinationString->Length = 0;
	DestinationString->Buffer = (PWSTR)SourceString;
	if (SourceString) {
		Length = wcslen(SourceString) * sizeof(WCHAR);

		if (Length >= 0xFFFF) {
			Length = 0xFFFF - sizeof(UNICODE_NULL);
		}

		DestinationString->Length = (USHORT)Length;
		DestinationString->MaximumLength = (USHORT)(Length + sizeof(UNICODE_NULL));
	}

	return;
}

int main()
{
	printf("Using CTL_CODE for beep - @Pyramidyon\r\n");

	UNICODE_STRING deviceName = { 0 };
	myRtlInitUnicodeString(&deviceName, L"\\Device\\Beep");

	OBJECT_ATTRIBUTES objectAttributes;
	InitializeObjectAttributes(&objectAttributes, &deviceName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	HANDLE hDriver;
	IO_STATUS_BLOCK IoStatus;
	NTSTATUS status = NtOpenFile(&hDriver, GENERIC_WRITE, &objectAttributes, &IoStatus, FILE_SHARE_WRITE, FILE_OPEN);

	if (status == STATUS_SUCCESS) 
	{
		printf("[+] NtOpenFile() -> \\Device\\Beep\r\n");
		ULONG IOCTL_BEEP = CTL_CODE(FILE_DEVICE_BEEP, 0, METHOD_BUFFERED, FILE_ANY_ACCESS);
		ULONG dwReturned = 0;

		BEEP_SET_PARAMETERS BeepSettings;
		BeepSettings.Duration = 10000;
		BeepSettings.Frequency = 500;

		status = NtDeviceIoControlFile(hDriver, NULL, NULL, NULL, &IoStatus, IOCTL_BEEP, &BeepSettings, sizeof(BeepSettings), &dwReturned, 0);
		if (status != 0)
		{
			printf("[+] NtDeviceIoControlFile called duration: %u ms | Frequency: %u Hz\r\n", BeepSettings.Duration, BeepSettings.Frequency);
		}
		else
		{
			printf("[-] FAILD NtDeviceIoControlFile() \r\n");
		}
		Sleep(BeepSettings.Duration);
		printf("[i] Completed, closing \r\n");
		NtClose(hDriver);
	}
	else
	{
		printf("[-] FAILD NtOpenFile() -> \\Device\\Beep\r\n");
	}

	return 0;
}
