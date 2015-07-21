// memory_mapped_io_exam.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Strsafe.h>
#include <crtdbg.h>
#include <string>
/*
----ReadFile() api 를 통해서 파일 읽기
----Memory Mapped I / O 를 이용해서 파일 읽기
+ 현재디렉토리\bob.txt, 현재디렉토리\bob2.txt 삭제
ps.mmio 샘플 코드는 http ://pastebin.com/cLQmPb6m 에 있습니다. 참고하세요.
*/

typedef unsigned long int 	uint32_t;


bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}

void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
bool create_bob_txt()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	// 파일에 데이터 쓰기
	DWORD bytes_written = 0;
	wchar_t string_buf[1024];
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}


	if (!WriteFile(file_handle, string_buf, wcslen(string_buf), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 영어로 쓰기
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"All work and no play makes jack a dull boy.")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	if (!WriteFile(file_handle, string_buf, wcslen(string_buf), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}




	char string_bufa[1024];
	if (!SUCCEEDED(StringCbPrintfA(
		string_bufa,
		sizeof(string_bufa),
		"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	if (!WriteFile(file_handle, string_bufa, strlen(string_bufa), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 영어로 쓰기
	if (!SUCCEEDED(StringCbPrintfA(
		string_bufa,
		sizeof(string_bufa),
		"All work and no play makes jack a dull boy.")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	if (!WriteFile(file_handle, string_bufa, strlen(string_bufa), &bytes_written, NULL))
	{
		print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// 파일 닫기
	CloseHandle(file_handle);
	return true;

}

/*
bool read_file_using_memory_map()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}
}


// 경로를 만들어 주고 파일을 생성
HANDLE create_file()
{
	// current dir \\ test.txt 파일명 생성
	wchar_t file_name[260];
	wchar_t *buf = NULL;

	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	HANDLE file_handle = CreateFileW(
		(LPCWSTR)file_name,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		print("err, CreateFile(%ws) failed, gle = %u", file_name, GetLastError());
		return false;
	}

	return file_handle;
}



bool ReadMappingIO(HANDLE file_handle, wchar_t *file_name)
{
	// check file size
	//
	LARGE_INTEGER fileSize;
	if (TRUE != GetFileSizeEx(file_handle, &fileSize))
	{
		print("err, GetFileSizeEx(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

    // [ WARN ]
    //
    // 4Gb 이상의 파일의 경우 MapViewOfFile()에서 오류가 나거나
    // 파일 포인터 이동이 문제가 됨
    // FilIoHelperClass 모듈을 이용해야 함
    //
    _ASSERTE(fileSize.HighPart == 0);
        if (fileSize.HighPart > 0)
        {
        print("file size = %I64d (over 4GB) can not handle. use FileIoHelperClass",
            fileSize.QuadPart);
                CloseHandle(file_handle);
        return false;
        }
 
    DWORD file_size = (DWORD)fileSize.QuadPart;
    HANDLE file_map = CreateFileMapping(
                            file_handle,
                            NULL,
                            PAGE_READONLY,
                            0,
                            0,
                            NULL
                            );
    if (NULL == file_map)
    {
        print("err, CreateFileMapping(%ws) failed, gle = %u", file_name, GetLastError());
        CloseHandle(file_handle);
        return false;
    }
 
    PCHAR file_view = (PCHAR) MapViewOfFile(
                                    file_map,
                                    FILE_MAP_READ,
                                    0,
                                    0,
                                    0
                                    );
    if(file_view == NULL)
    {
        print("err, MapViewOfFile(%ws) failed, gle = %u", file_name, GetLastError());
       
        CloseHandle(file_map);
        CloseHandle(file_handle);
        return false;
    }  
 
    // do some io
    char a = file_view[0];  // 0x d9
    char b = file_view[1];  // 0xb3
 
 
 
    // close all
    UnmapViewOfFile(file_view);
    CloseHandle(file_map);
    CloseHandle(file_handle);
    return true;
}
*/


bool read_file_using_memory_map()
{
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

	// current dir \\ test.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	/*if (true == is_file_existsW(file_name))
	{
	::DeleteFileW(file_name);
	}*/




	HANDLE file_handle = CreateFileW(
		(LPCWSTR)file_name,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		print("err, CreateFile(%ws) failed, gle = %u", file_name, GetLastError());
		return false;
	}

	// check file size
	//
	LARGE_INTEGER fileSize;
	if (TRUE != GetFileSizeEx(file_handle, &fileSize))
	{
		print("err, GetFileSizeEx(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	// [ WARN ]
	//
	// 4Gb 이상의 파일의 경우 MapViewOfFile()에서 오류가 나거나
	// 파일 포인터 이동이 문제가 됨
	// FilIoHelperClass 모듈을 이용해야 함
	//
	_ASSERTE(fileSize.HighPart == 0);
	if (fileSize.HighPart > 0)
	{
		print("file size = %I64d (over 4GB) can not handle. use FileIoHelperClass",
			fileSize.QuadPart);
		CloseHandle(file_handle);
		return false;
	}

	DWORD file_size = (DWORD)fileSize.QuadPart;
	HANDLE file_map = CreateFileMapping(
		file_handle,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
		);
	if (NULL == file_map)
	{
		print("err, CreateFileMapping(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
		);
	if (file_view == NULL)
	{
		print("err, MapViewOfFile(%ws) failed, gle = %u", file_name, GetLastError());

		CloseHandle(file_map);
		CloseHandle(file_handle);
		return false;
	}

	// do some io

	char a = file_view[0];  // 0x d9
	char b = file_view[1];  // 0xb3

	for (int i = 0; i < file_size; i++){
		printf("%c", file_view[i]);
	}

	// close all
	UnmapViewOfFile(file_view);
	CloseHandle(file_map);
	CloseHandle(file_handle);
	return true;
}



#define MAX_LINE 1024 
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE file_handle;
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	char temp_str[MAX_LINE];


	buflen = GetCurrentDirectoryW(buflen, buf);
	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);

	GetCurrentDirectoryW(buflen, buf);

	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, can not create file name");
		return false;
	}

	wchar_t file_name2[260];
	if (!SUCCEEDED(StringCbPrintfW(
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, can not create file name");
		free(buf);
		return false;
	}


	file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	DWORD bytes_written = 0;
	wchar_t string_buf[1024] = { 0 };
	if (!SUCCEEDED(StringCbPrintfW(
		string_buf,
		sizeof(string_buf),
		L"A안녕BCD안녕EFGHI안녕DSFS안녕FERWE안녕RWEF안녕SDFS")))
	{
		print("err, can not create data to write.");
		CloseHandle(file_handle);
		return false;
	}

	int utf8_string_len = WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, NULL, 0, NULL, NULL);
	char *utf8_string = (char*)malloc(utf8_string_len * sizeof(char));
	RtlZeroMemory(utf8_string, utf8_string_len);
	WideCharToMultiByte(CP_UTF8, 0, string_buf, -1, utf8_string, utf8_string_len, NULL, NULL);

	if (!WriteFile(file_handle, utf8_string, utf8_string_len, &bytes_written, NULL))
	{
		printf("err, WriteFile() failed. gle = 0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}


	free(buf); buf = NULL;
	memset(utf8_string, 0, utf8_string_len);
	CloseHandle(file_handle);


	//CopyFile
	CopyFile(file_name, file_name2, TRUE);


	HANDLE fh_read = CreateFileW(L"bob.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	DWORD copy_len;
	ReadFile(fh_read, utf8_string, utf8_string_len, &copy_len, NULL);

	HANDLE file_map = CreateFileMapping(
		fh_read,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
	);

	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
	);

	memset(string_buf, 0, sizeof(string_buf));
	int read_len;
	read_len = MultiByteToWideChar(CP_UTF8, 0, file_view, -1, NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, file_view, -1, string_buf, read_len);

	read_len = WideCharToMultiByte(CP_ACP, 0, string_buf, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, string_buf, -1, utf8_string, read_len, NULL, NULL);
	print("문자열 : %s \n", utf8_string);

	return 0;
}

