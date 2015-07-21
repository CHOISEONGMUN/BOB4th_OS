// OS_SharedMemoryExam.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//


/**----------------------------------------------------------------------------
*
*-----------------------------------------------------------------------------
* All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
*-----------------------------------------------------------------------------
*
**---------------------------------------------------------------------------*/

// read
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366542(v=vs.85).aspx

#include "stdafx.h"
#include "mmio.h"
#include "StopWatch.h"
#include "FileIoHelper.h"
#include <stdio.h>
#include <stdlib.h>


/*/
�����ϴ� ����
�������Ѷ�

create filemapping�� Ű ����Ʈ��
����¡ ũ�⸦ �����϶�
file io helper class�� �ִ�.
*/
int _tmain(int argc, _TCHAR* argv[])
{

	StopWatch stopwatch;
	/*
	_ASSERTE(create_very_big_file(L"big.txt", 400));

	StopWatch sw;
	sw.Start();
	_ASSERTE(file_copy_using_read_write(L"big.txt", L"big2.txt"));
	sw.Stop();
	print("file_copy_using_read_write info] time elapsed = %f\n", sw.GetDurationSecond());



	sw.Start();
	_ASSERTE(file_copy_using_memory_map(L"big.txt", L"big2.txt"));
	sw.Stop();
	print("file_copy_using_read_write info] time elapsed = %f\n", sw.GetDurationSecond());
	*/

	FileIoHelper f_helper, src_f_helper, des_f_helper;
	LARGE_INTEGER file_size, offset;
	LARGE_INTEGER size;

	memset(&offset, 0, 8);
	file_size.QuadPart = 1024 * 1024 * 1024;
	file_size.HighPart = 1;

	
	f_helper.FIOCreateFile(L"D:\\test.txt", file_size);
	f_helper.FIOClose();
	
	
	src_f_helper.FIOpenForRead(L"D:\\test.txt");
	des_f_helper.FIOCreateFile(L"D:\\test2.txt", file_size);
	size.QuadPart = 512 * 1024 * 1024;

	StopWatch sw;

	sw.Start();
	char *buffer = new char[size.QuadPart];
	while (offset.QuadPart < file_size.QuadPart){
		if (file_size.QuadPart - offset.QuadPart < size.QuadPart){
			src_f_helper.FIOReadFromFile(offset, file_size.QuadPart - offset.QuadPart, (PUCHAR)buffer);
			des_f_helper.FIOWriteToFile(offset, file_size.QuadPart - offset.QuadPart, (PUCHAR)buffer);
			break;
		}
		else{
			src_f_helper.FIOReadFromFile(offset, size.QuadPart, (PUCHAR)buffer);
			des_f_helper.FIOWriteToFile(offset, size.QuadPart, (PUCHAR)buffer);
			offset.QuadPart += size.QuadPart;
		}
	}
	sw.Stop();
	printf("�� �ɸ��� �ð��� : %0.5lf���Դϴ�.", sw.GetDurationSecond());
	
	return 0;
}

