#include <Strsafe.h>
#include <crtdbg.h>
#include <string>
#include <safeint.h>
#include <stdio.h>
#include <Windows.h>
#include <io.h>

typedef struct map_context
{
	HANDLE  handle;
	DWORD   size;
	HANDLE  map;
	PCHAR   view;
}*pmap_context;


typedef long uint32_t;

bool is_file_existsW(_In_ const wchar_t* file_path);
void print(_In_ const char* fmt, _In_ ...);
bool create_bob_txt();
bool read_file_using_memory_map();
bool create_very_big_file(_In_ const wchar_t* file_path, _In_ uint32_t size_in_mb);
pmap_context open_map_context(_In_ const wchar_t* file_path);
pmap_context create_map_context(_In_ const wchar_t* file_path, _In_ uint32_t file_size);
void close_map_context(_In_ pmap_context ctx);
bool file_copy_using_read_write(_In_ const wchar_t* src_file, _In_ const wchar_t* dst_file);
bool file_copy_using_memory_map(_In_ const wchar_t* src_file, _In_ const wchar_t* dst_file);