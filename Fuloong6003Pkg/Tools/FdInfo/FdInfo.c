/*
Copyright - 

Revision:
    2010.04.13 - Source created         - wang.qch

NOTE:  This tool is MIPS specific
       Used to adjust the region ZeroVector of FV 
       which aligns at the boot address:0xbfc00000

       There is a premise for using this tool:
       that the SecCore resides in the FV which start at offset 0x0 of the FD file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FdInfo.h"

#define MAX_FV_NUM             16
#define MAX_FFS_NUM            32
#define MAX_SECTION_NUM		   32
#define MAX_TYPE_STR_LENGTH    96



typedef struct _SECTION_INFO{
    struct _SECTION_INFO    *Next;
    UINTN           		Offset;         //from this ffs file
    UINTN					SectionSize;
}SECTION_INFO;

typedef struct _FFS_FILE_INFO{
    struct _FFS_FILE_INFO   *Next;          //Linked list, NULL terminated.
    UINTN           		Offset;         //from this fv
    UINTN					SectionCount;
	UINTN					FfsSize;
    SECTION_INFO    		*SectionInfoList;       //First node of the linked list, NULL means none.
}FFS_FILE_INFO;
#define	GET_FFS_INFO_FROM_SEC_INFO(pSecInfo) (FFS_FILE_INFO *)((UINTN)&pSecInfo - (UINTN)&(((FFS_FILE_INFO *)0)->SectionInfoList))


typedef struct _FV_INFO{
    struct _FV_INFO     *Next;          //Linked list, NULL terminated.
    UINTN           	Offset;         //from this fd
    UINTN				FileCount;
	UINTN				FvSize;
    FFS_FILE_INFO   	*FileInfoList;   //first node of the linked list, NULL means none.
}FV_INFO;
#define	GET_FV_INFO_FROM_FFS_INFO(pFfsInfo) (FV_INFO *)((UINTN)&pFfsInfo - (UINTN)&(((FV_INFO *)0)->FileInfoList))

typedef struct{
	UINTN		FvCount;
	UINTN		FdSize;		//unit byte
	FV_INFO		*FvInfoList;
}FD_INFO;
#define	GET_FD_INFO_FROM_FV_INFO(pFvInfo)	(FD_INFO *)((UINTN)&pFvInfo - (UINTN)&(((FD_INFO *)0)->FvInfoList))

FD_INFO 	*gFdInfo 		= NULL;
UINTN    	gFileSize 		= 0;



FV_INFO *AllocSecInfoNode(void){
	SECTION_INFO *SecInfo;
	
	SecInfo = (SECTION_INFO *)malloc(sizeof(SECTION_INFO));
	if(SecInfo == NULL){
		printf("\nOut of resoruces");
		goto _EXIT_POINT;
	}
	memset(SecInfo, 0, sizeof(SECTION_INFO));

_EXIT_POINT:	
	return SecInfo;
}


FV_INFO *AllocFfsInfoNode(void){
	FFS_FILE_INFO *FfsInfo;
	
	FfsInfo = (FV_INFO *)malloc(sizeof(FFS_FILE_INFO));
	if(FfsInfo == NULL){
		printf("\nOut of resoruces");
		goto _EXIT_POINT;
	}
	memset(FfsInfo, 0, sizeof(FFS_FILE_INFO));

_EXIT_POINT:	
	return FfsInfo;
}

BOOLEAN AllocFvInfoNode(FV_INFO **FvInfo){
	BOOLEAN	Status;
	*FvInfo = (FV_INFO *)malloc(sizeof(FV_INFO));
	if(*FvInfo == NULL){
		printf("\nOut of resoruces");
		Status = FALSE;
		goto _EXIT_POINT;
	}
	memset(*FvInfo, 0, sizeof(FV_INFO));
	Status = TRUE;
_EXIT_POINT:	
	return Status;
}



FV_INFO *AllocFvInfoNode_bak(void){
	FV_INFO *FvInfo;
	
	FvInfo = (FV_INFO *)malloc(sizeof(FV_INFO));
	if(FvInfo == NULL){
		printf("\nOut of resoruces");
		goto _EXIT_POINT;
	}
	memset(FvInfo, 0, sizeof(FV_INFO));

_EXIT_POINT:	
	return FvInfo;
}

/*
Parameter:
	Ffs		- pointer to buffer contain the whole Ffs image data.
*/
BOOLEAN	ConstructSectionInfo(UINT8 *Ffs, SECTION_INFO *SectionList){
	BOOLEAN Status;
	FFS_FILE_INFO				*FfsInfo;
	EFI_FFS_FILE_HEADER			*FfsHdr;
	EFI_COMMON_SECTION_HEADER	*SecHdr;
	EFI_COMMON_SECTION_HEADER2	*SecHdr2;
	SECTION_INFO				*pSecInfo;
	SECTION_INFO				*cpSecInfo;
	UINT8						*Ptr;
	UINTN						SectionIndex;
	UINTN						SectionSize;

	SecHdr		= NULL;
	SecHdr2		= NULL;
	Ptr			= NULL;
	FfsHdr		= NULL;

	FfsInfo 	= GET_FFS_INFO_FROM_SEC_INFO(SectionList);
	FfsHdr		= (EFI_FFS_FILE_HEADER *)Ffs;
	Ptr			= Ffs;
	Ptr			+= (FfsHdr->Attributes & FFS_ATTRIB_LARGE_FILE)?sizeof(EFI_FFS_FILE_HEADER2):sizeof(EFI_FFS_FILE_HEADER);

	for(SectionIndex = 0; SectionIndex < MAX_SECTION_NUM; SectionIndex ++){
		Ptr = (UINT8 *)((UINTN)(Ptr + 0x3) & (~0x3));
		SecHdr	= (EFI_COMMON_SECTION_HEADER *)Ptr;
		if(SectionList == NULL){
			SectionList = AllocSecInfoNode();
			if(SectionList == NULL){
				Status = FALSE;
				goto _EXIT_POINT;
			}
			SectionList->Offset	= (UINTN)(Ptr - Ffs);
			cpSecInfo = SectionList;
		}
		else{
			pSecInfo = AllocSecInfoNode();
			if(pSecInfo == NULL){
				Status = FALSE;
				goto _EXIT_POINT;
			}
			pSecInfo->Offset = (UINTN)(Ptr - Ffs);
			cpSecInfo->Next	= pSecInfo;
			cpSecInfo = pSecInfo;
		}
		if((SecHdr->Size[0] == 0xFF)&&	(SecHdr->Size[0] == 0xFF)&&(SecHdr->Size[0] == 0xFF)){
			SecHdr2	= (EFI_COMMON_SECTION_HEADER2 *)SecHdr;
			SectionSize = (UINTN)SecHdr2->ExtendedSize;
		}
		else{
			SectionSize = (UINTN)(SecHdr->Size[0]) | (UINTN)((SecHdr->Size[1])<<8) |(UINTN)((SecHdr->Size[2])<<16);
		}
		cpSecInfo->SectionSize = SectionSize;
		Ptr += SectionSize;
		
		if(Ptr >= (Ffs + FfsInfo->FfsSize))
			break;
	}

	FfsInfo->SectionCount = SectionIndex;
	
	Status = TRUE;
_EXIT_POINT:
	return Status;
}



/*
Parameter:
	Fv		- pointer to buffer contain the whole Fv image data.
*/
BOOLEAN ConstructFfsInfo(UINT8 *Fv, FFS_FILE_INFO *FfsList){
	BOOLEAN	Status;
	EFI_FFS_FILE_HEADER    			*FfsHdr;
    EFI_FFS_FILE_HEADER2    		*FfsHdr2;
    EFI_FIRMWARE_VOLUME_EXT_HEADER	*FvExtHdr;
	EFI_FIRMWARE_VOLUME_HEADER		*FvHdr;
	FFS_FILE_INFO					*pFileInfo;
	FFS_FILE_INFO					*cpFileInfo;
	FV_INFO							*FvInfo;

	UINTN		FileIndex;
	UINTN		CurrentLeng;
	UINT8		*Ptr;

	FfsHdr		= NULL;
	FfsHdr2		= NULL;
	FvExtHdr	= NULL;
	Ptr			= NULL;
	FvHdr		= (EFI_FIRMWARE_VOLUME_HEADER *)Fv;
	Ptr			= Fv;
	
	FvInfo = GET_FV_INFO_FROM_FFS_INFO(FfsList);
	
	if(FvHdr->ExtHeaderOffset != 0){
        FvExtHdr = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)(Ptr + FvHdr->ExtHeaderOffset);
        FfsHdr = (EFI_FFS_FILE_HEADER *)(Ptr + FvHdr->ExtHeaderOffset + FvExtHdr->ExtHeaderSize);
    }
    else{
        FfsHdr = (EFI_FFS_FILE_HEADER *)(Ptr + FvHdr->HeaderLength);
    }
	
    Ptr = (UINT8 *)FfsHdr;    
	for(FileIndex = 0; FileIndex < MAX_FFS_NUM; FileIndex ++){
		Ptr = (UINT8 *)((UINTN)(Ptr + 0x7) & (~0x7));
    	FfsHdr = (EFI_FFS_FILE_HEADER *)Ptr;
		if(FfsList == NULL){
			FfsList = AllocFfsInfoNode();
			if(FfsList == NULL){
				Status = FALSE;
				goto _EXIT_POINT;
			}
			FfsList->Offset	= (UINTN)(Ptr - Fv);
			cpFileInfo = FfsList;
		}
		else{
			pFileInfo = AllocFfsInfoNode();
			if(pFileInfo == NULL){
				Status = FALSE;
				goto _EXIT_POINT;
			}
			pFileInfo->Offset = (UINTN)(Ptr - Fv);
			cpFileInfo->Next	= pFileInfo;
			cpFileInfo = pFileInfo;
		}
        if(!(FfsHdr->Attributes & FFS_ATTRIB_LARGE_FILE)){
            CurrentLeng = ((UINTN)(FfsHdr->Size[0]) | (UINTN)(FfsHdr->Size[1] << 8) | (UINTN)(FfsHdr->Size[2] << 16));
        }
        else{
            FfsHdr2 = (EFI_FFS_FILE_HEADER2 *)FfsHdr;
            CurrentLeng= (UINTN)FfsHdr2->ExtendedSize  << 24;
        }
		cpFileInfo->FfsSize = CurrentLeng;
		ConstructSectionInfo(Ptr, cpFileInfo->SectionInfoList);
		if(FALSE == Status)
			goto _EXIT_POINT;
		
		Ptr += CurrentLeng;
		if(Ptr >= (Fv + FvInfo->FvSize))
			break;		
	}	
	FvInfo->FileCount = FileIndex;
	
	Status = TRUE;
_EXIT_POINT:
	return Status;
}

/*
Parameter:
	Fd		- pointer to buffer contain the whole FD image data.
*/
BOOLEAN	ConstructFvInfo(CHAR8 *FdImage, FV_INFO *FvList){
	CHAR8						*Ptr;
	EFI_FIRMWARE_VOLUME_HEADER	*FvHdr;
	FV_INFO						*pFvInfo;
	FV_INFO						*cpFvInfo;
	UINTN						FvIndex;
	BOOLEAN						Status;
	
	Ptr 		= NULL;
	FvHdr		= NULL;
	pFvInfo		= NULL;
	cpFvInfo	= NULL;
	
	//for(Ptr = FdImage; FvIndex < MAX_FV_NUM; FvIndex ++){
	//	FvHdr = (EFI_FIRMWARE_VOLUME_HEADER *)Ptr;
	//	if(FvHdr->Signature != EFI_FVH_SIGNATURE)
    //        break;
		//if(FvList == NULL){
		//	//FvList = AllocFvInfoNode();
		//	AllocFvInfoNode(&FvList);
		//	if(FvList == NULL){
		//		Status = FALSE;
		//		goto _EXIT_POINT;
		//	}
		//	FvList->Offset	= (UINTN)(Ptr - FdImage);
		//	cpFvInfo = FvList;
		//}
		//else{
		//	//pFvInfo = AllocFvInfoNode();
		//	AllocFvInfoNode(&pFvInfo);
		//	if(pFvInfo == NULL){
		//		Status = FALSE;
		//		goto _EXIT_POINT;
		//	}
		//	pFvInfo->Offset = (UINTN)(Ptr - FdImage);
		//	cpFvInfo->Next	= pFvInfo;
		//	cpFvInfo = pFvInfo;
		//}

		//cpFvInfo->FvSize = (UINTN)FvHdr->FvLength;
		//Status = ConstructFfsInfo(Ptr, cpFvInfo->FileInfoList);
		//if(FALSE == Status)
		//	goto _EXIT_POINT;
		
		//Ptr += cpFvInfo->FvSize;
		//if(Ptr >= FdImage + gFileSize)
		//	break;
	//}
	//gFdInfo->FvCount = FvIndex;
	Status = TRUE;
_EXIT_POINT:
	return Status;
}

BOOLEAN ConstructFdInfo(CHAR8 *FdImage){
	BOOLEAN	Status;
	gFdInfo = NULL;
	gFdInfo = (FD_INFO *)malloc(sizeof(FD_INFO));
	if(gFdInfo == NULL){
		printf("\nOut of resource");
		Status = FALSE;
		goto _EXIT_POINT;
	}
	memset(&gFdInfo, 0,sizeof(FD_INFO));
	Status = ConstructFvInfo(FdImage, gFdInfo->FvInfoList);

_EXIT_POINT:
	return Status;
}

void DestructSectionInfo(SECTION_INFO  *SecInfo){
	free(SecInfo);
}
void DestructFfsInfo(FFS_FILE_INFO *FileInfo){
	SECTION_INFO	*cpSecInfo;
	SECTION_INFO	*npSecInfo;
	cpSecInfo = FileInfo->SectionInfoList;
	while(cpSecInfo != NULL){
		npSecInfo = cpSecInfo->Next;
		DestructSectionInfo(cpSecInfo);
		cpSecInfo = npSecInfo;
	}
	free(FileInfo);
}
void DestructFvInfo(FV_INFO *FvInfo){
	FFS_FILE_INFO	*cpFileInfo;
	FFS_FILE_INFO	*npFileInfo;
	cpFileInfo = FvInfo->FileInfoList;
	while(cpFileInfo != NULL){
		npFileInfo = cpFileInfo->Next;
		DestructFfsInfo(cpFileInfo);
		free(cpFileInfo);
		cpFileInfo = npFileInfo;
	}
	free(FvInfo);
}
void DestructFdInfo(void){
	FV_INFO		*cpFvInfo;
	FV_INFO		*npFvInfo;
	if(gFdInfo != NULL){
		cpFvInfo = gFdInfo->FvInfoList;
		while(cpFvInfo != NULL){
			npFvInfo = cpFvInfo->Next;
			DestructFvInfo(cpFvInfo);
			free(cpFvInfo);
			cpFvInfo = npFvInfo;		
		}
		free(gFdInfo);
		gFdInfo = NULL;
	}
}


void ShowFdInfo(void){
	printf("\nkdkdkdkd");
	
}

void Usage(){
}
int main(int Argc, char *Argv[]){
    FILE     	*fp;
    UINTN    	Status;
	CHAR8 		*FileBuffer;
	
	fp				= NULL;
	FileBuffer 		= NULL;
    
    if (Argc <=1 ){
		printf("\nInvalid parameter");
        Usage();
		Status = 1;
        goto _EXIT_POINT;
    }

	fp = NULL;
    fp = fopen(Argv[1], "r+b");
    if (fp == NULL){
        printf("\nCannot open file: %s, operation aborted", Argv[1]);
        Status = 1;
        goto _EXIT_POINT;
    }
    fseek(fp, 0L, SEEK_END);
    gFileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

	FileBuffer = NULL;
    FileBuffer = (CHAR8 *)malloc(gFileSize);
    if (FileBuffer == NULL){
        printf("\nOut of resources.");
        Status = 1;
        goto _EXIT_POINT;
    }
	printf("\n-----------000033333");
    if(gFileSize != fread(FileBuffer, 1, gFileSize, fp)){
        printf("\nRead file error.", gFileSize);
        Status = 1;
        goto _EXIT_POINT;
    }
	printf("\n----------999999");

    fclose(fp);
    fp = NULL;
	
	printf("\n----------001");
	
	Status = ConstructFdInfo(FileBuffer);
	printf("\n----------002");
	if(FALSE == Status)
		goto _EXIT_POINT;
	
	//ShowFdInfo();
    Status = 0;
    
_EXIT_POINT:
    if(fp != NULL){
        fclose(fp);
        fp = NULL;
        //printf("\nFile Closed\n");
    }
	DestructFdInfo();
    if(FileBuffer != NULL){
        free(FileBuffer);
        FileBuffer = NULL;
        //printf("\nBuffer Freed\n");
    }
    printf("\nreturn status : %x",Status);
	printf("\n");
    return Status;    
}
