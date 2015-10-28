#include <stdio.h>
//#include "wmc_file_data.h"
U8_WMC FileData[];
extern U32_WMC FileData_pos;
int fileread(void* DstBuf,int element_size,int count ,FILE *fileHandele)
{
	int read_size = 0;
	if (fileHandele == NULL)
	{
		return 0;
	}
//	printf("%d %d \n",count,FileData_pos);
	read_size = element_size*count;
	memcpy(DstBuf,&FileData[FileData_pos],read_size);
	FileData_pos += read_size;
//	printf("%d\n",FileData_pos);
	if (FileData_pos >= 243)
	{
//		printf("error\n");
	}

	return count;

}


int fileseek(FILE * File,long Offset, int Origin)
{
	if (File == NULL)
	{
		return 0;
	}
	switch(Origin)
	{
		case SEEK_CUR:
            FileData_pos = FileData_pos + Offset;
			break;
		case SEEK_END:
            FileData_pos = sizeof(FileData) + Offset;
			break;
		case SEEK_SET:
			FileData_pos = Offset;
			break;
	}
	return 0;

}
