/*
Copyright - 

Revision:

NOTE:  This tool is MIPS specific
       Used to adjust the region ZeroVector of FV 
       which aligns at the boot address:0xbfc00000

       There is a premise for using this tool:
       that the SecCore resides in the FV which start at offset 0x0 of the FD file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RecalculateFvZeroVector.h"

#define BACKUP_EXTENSION        ".bak.ZeroVector"
#define BRANCH_INSTRUCTION      0x10000000



/*
Discription:
    show help info about this tool 

Parameters:
Brief-Algorithm:

*/
void Usage(void){
    printf("\nRecalculateFvZeroVector:");
    printf("\n\tStore a JMP instruction in a FV.ZeroVector for JUMPing to the SEC Core Entry Point");
    printf("\nUsage:");
    printf("\n\tRecalculateFvZeroVector FdFileName");
}


/*
Discription:
    Find the executing-entry-point of SEC-CORE-TE image 

Parameters:    FileBuffer          - Pointer, points to mem-buffer which holds an FV in which  
                          the SEC-CORE-TE image is supposed to reside.
    Offset              - Pointer, points to a sizeof(UINTN) bytes location holding 
                          the offset of the entry point from FileBuffer

Brief-Algorithm:
    1.find the the ffs file which is of FFS_SECURITY_CORE type
    2.find the TE_SECTION in this ffs file
    3.read the ENTRY_POINT value in TE_HEADER
    4.return offset value of ENTRY_POINT from the beginning of this FV
    
*/
BOOLEAN FindSecCoreTeImage(CHAR8 *FileBuffer, UINTN *Offset){
    EFI_FIRMWARE_VOLUME_HEADER          *FvHdr;
    EFI_FIRMWARE_VOLUME_EXT_HEADER      *FvExtHdr;
    EFI_FFS_FILE_HEADER                 *FfsHdr;
    EFI_FFS_FILE_HEADER2                *FfsHdr2;
    EFI_COMMON_SECTION_HEADER           *ComSectionHdr;
    EFI_TE_IMAGE_HEADER                 *TeImageHdr;    
    CHAR8                               *Ptr;
    CHAR8                               *FfsEnd;
    UINTN                               FfsSize;
    UINTN                               SectionSize;


    //
    //looking for the Sec Core FFS in this FV
    Ptr     =   FileBuffer;
    FvHdr   =   (EFI_FIRMWARE_VOLUME_HEADER *)FileBuffer;
    if(FvHdr->Signature != EFI_FVH_SIGNATURE){
        printf("\nInvalid FV data offered");
        return FALSE;
    }
    if(FvHdr->ExtHeaderOffset != 0){
        //printf("\n\tThis Fv has Extended Header");
        FvExtHdr = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)(Ptr + FvHdr->ExtHeaderOffset);
        FfsHdr = (EFI_FFS_FILE_HEADER *)(Ptr + FvHdr->ExtHeaderOffset + FvExtHdr->ExtHeaderSize);
    }
    else{
        //printf("\n\tThis Fv doesn't have Extende Header");
        FfsHdr = (EFI_FFS_FILE_HEADER *)(Ptr + FvHdr->HeaderLength);
    }
    
    Ptr = (UINT8 *)FfsHdr;
    
    do{
        Ptr = (UINT8 *)((UINTN)(Ptr + 0x7) & (~0x7));
        FfsHdr = (EFI_FFS_FILE_HEADER *)Ptr;
        if(!(FfsHdr->Attributes & FFS_ATTRIB_LARGE_FILE)){
            //printf("\n\tThis FFS file doesN'T have large file attribute set");
            FfsSize = ((UINTN)(FfsHdr->Size[0]) | (UINTN)(FfsHdr->Size[1] << 8) | (UINTN)(FfsHdr->Size[2] << 16));
        }
        else{
            //printf("\n\tThis FFS file has large file attribute set");
            FfsHdr2 = (EFI_FFS_FILE_HEADER2 *)FfsHdr;
            FfsSize= (UINTN)FfsHdr2->ExtendedSize  << 24;
        }        
        
        if(FfsHdr->Type == EFI_FV_FILETYPE_SECURITY_CORE){
            //printf("\n\tSeccore file found\n");
            break;
        }
        Ptr += FfsSize;    
        
    }while(Ptr < (FileBuffer + (UINTN)FvHdr->FvLength));
    
    //make sure we have it.
    if(FfsHdr->Type != EFI_FV_FILETYPE_SECURITY_CORE){
        printf("\nCannot find SecCore FFS in this FV");
        return FALSE;
    }
    //
    //  Sec Core FFS file found
    //  looking for SecCore Te Section
    //  we asume the SEC_CORE_FFS contains only one TE Section where the Sec Code resides.
    FfsEnd = Ptr + FfsSize;
    
    Ptr += (FfsHdr->Attributes & FFS_ATTRIB_LARGE_FILE)? sizeof(EFI_FFS_FILE_HEADER2):sizeof(EFI_FFS_FILE_HEADER);
    do{
        //here we usually always encounter an EFI_COMMON_SECTION_HEADER
        //for EFI_COMMON_SECTION_HEADER2 are for section which is larger than 0xffffff bytes
        ComSectionHdr = (EFI_COMMON_SECTION_HEADER *)Ptr;
        SectionSize =     ((UINTN)(ComSectionHdr->Size[0]))|
                          (((UINTN)(ComSectionHdr->Size[1]))<<8)|
                          (((UINTN)(ComSectionHdr->Size[2]))<<16);
        
        if(ComSectionHdr->Type == EFI_SECTION_TE){
            printf("\nTe Section Found");
            break;
        }

        Ptr += SectionSize;
    }while(Ptr < FfsEnd);
    if(ComSectionHdr->Type != EFI_SECTION_TE){
        printf("\nCannot find Sec Core Te Section");
        return FALSE;
    }
    //
    //Sec Core Te Section Found
    //get the entry point of the image
    TeImageHdr = (EFI_TE_IMAGE_HEADER *)(Ptr + sizeof(EFI_TE_SECTION));
    *Offset = TeImageHdr->AddressOfEntryPoint - 
              TeImageHdr->StrippedSize        + 
              sizeof(EFI_TE_IMAGE_HEADER)     +
              sizeof(EFI_TE_SECTION)          +
              Ptr                             -
              FileBuffer;
        
    printf("\nOffset of entry point: 0x%x", *Offset);

    return TRUE;    

}

/*
Discription:     

Parameters:

Brief-Algorithm:

*/
UINT16 
CalculateSum16 (
    UINT16             *Buffer,
    UINTN               Length
){
  UINT16    Sum;
  UINTN     Count;
  UINTN     Total;

  if( (Buffer == NULL)||
        (((UINTN)Buffer & 0x1) != 0)||
        ((Length & 0x1) != 0)
    ){
    printf("\nError in 'CalculateSum16' - invalid parameters");
    exit (-1);
  }
  
  Total = Length / sizeof (*Buffer);
  for (Sum = 0, Count = 0; Count < Total; Count++) {
    Sum = (UINT16) (Sum + *(Buffer + Count));
  }
  return Sum;
}


/*
Discription:     

Parameters:

Brief-Algorithm:

*/
UINT16
CalculateCheckSum16 (
    UINT16             *Buffer,
    UINTN               Length
){
  UINT16     CheckSum;

  CheckSum = CalculateSum16 (Buffer, Length);

  //
  // Return the checksum based on 2's complement.
  return (UINT16) (0x10000 - CheckSum);
}


/*
Discription:     
        Update the region 'ZeroVector' of EFI_FIRMWARE_VOLUME_HEADER
        with 'JUMP' instruction for jumping to the entry point of the PE imag
        
Parameters:
        FileBuffer      - Buffer contains the whole data of a BIOS FD image.
        Value           - Destination of the 'JUMP' instruction.
                          (Offset of the entry point from the very beginning of the FD image)
Brief-Algorithm:

*/
BOOLEAN UpdateFvZeroVector(CHAR8 *FileBuffer, UINTN Value){

    UINT32    BranchInstruction;
    UINT16    Offset;
    UINT16    ChkSum;
    //Value = (Offset + sizeof(INSTRUCTION)) << 2  
    // deduce:
    //Offset = (Value >> 2) - 4
    Offset = ((UINT16)Value >> 2) - 1;
    BranchInstruction = BRANCH_INSTRUCTION;
    BranchInstruction |= Offset;

    printf("\nBranchInstruction : 0x%8x",(UINT32)BranchInstruction);
    memset(FileBuffer, 0, 0x16);
    FileBuffer[0] = (UINT8)BranchInstruction;
    FileBuffer[1] = (UINT8)(BranchInstruction >> 8);
    FileBuffer[2] = (UINT8)(BranchInstruction >> 16);
    FileBuffer[3] = (UINT8)(BranchInstruction >> 24);

    //
    //update checksum of 'ZeroVector' to ensure the total chesum of FvHdr is '0'
    //Of the 16bytes in 'ZeroVector[]', at most 14byts can be used for the 
    //storing of an RESET_VECTOR_INSTRUCTION, the remained 2bytes==16bits are reserved
    //for storing checksum
    ChkSum = CalculateCheckSum16((UINT16 *)FileBuffer, 16-2);//16 is the length of 'ZeroVector'
    FileBuffer[14] = (UINT8)ChkSum;
    FileBuffer[15] = (UINT8)(ChkSum >> 8);

    return TRUE;
}


int main(int Argc, char *Argv[]){
    FILE     *fp;
    CHAR8    *FileBuffer;
    CHAR8    *BackupName;
    UINTN    FileSize;
    UINTN    SecTeOffset;
    UINTN    ReadSize;
    INTN     Status;
           
    fp = NULL;
    FileBuffer = NULL;
    BackupName = NULL;
	
	printf("\n");
    
    if (Argc <=1 ){
        Usage();
        Status = -1;
        goto _EXIT_POINT;
    }
    
    fp = fopen(Argv[1], "r+b");
    if (fp == NULL){
        printf("\nCannot open file: %s, operation aborted", Argv[1]);
        Status = -1;
        goto _EXIT_POINT;
    }

    fseek(fp, 0L, SEEK_END);
    FileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    FileBuffer = (CHAR8 *)malloc(FileSize);
    if (FileBuffer == NULL){
        printf("\nOut of resources.");
        Status = -1;
        goto _EXIT_POINT;
    }
    ReadSize = fread(FileBuffer, 1, FileSize, fp);
    if(FileSize != ReadSize){
        printf("\nRead file error. Filesize: %d\tReadSize: %d", FileSize,ReadSize);
        Status = -1;
        goto _EXIT_POINT;
    }
    
    fclose(fp);
    fp = NULL;
    
    if (!FindSecCoreTeImage(FileBuffer, &SecTeOffset)){
        printf("\nCannot find Sec Core Te Image");
        Status = -1;
        goto _EXIT_POINT;
    }
    if (!UpdateFvZeroVector(FileBuffer, SecTeOffset)){
        printf("\nUpdate Fv Zero Vector error");
        Status = -1;
        goto _EXIT_POINT;
    }

    BackupName = (CHAR8 *)malloc(strlen(Argv[1]) + strlen(BACKUP_EXTENSION));
    if (BackupName == NULL){
        printf("\nOut of resources.");
        Status = -1;
        goto _EXIT_POINT;
    }
    
    strcpy(BackupName, Argv[1]);
    strcat(BackupName,BACKUP_EXTENSION);

    fp = fopen(BackupName,"r");
    if(fp){
        fclose(fp);
        fp = NULL;
        //remove(BackupName);
        //printf("\n%s removed\n", BackupName);
    }
    else{
        rename(Argv[1], BackupName);
    }

    fp = fopen(Argv[1], "w+b");
    if (fp == NULL){
        printf("\nCannot open file: %s, operation aborted", Argv[1]);
        Status = -1;
        goto _EXIT_POINT;
    }
    //printf("\nfor debug - write file back\n");
    if(FileSize != fwrite(FileBuffer, 1, FileSize, fp)){
        printf("\nWrite file error.");
        Status = -1;
        goto _EXIT_POINT;        
    }
    
    Status = 0;
    
_EXIT_POINT:
    if(fp != NULL){
        fclose(fp);
        fp = NULL;
        //printf("\nFile Closed\n");
    }
    if(FileBuffer != NULL){
        free(FileBuffer);
        FileBuffer = NULL;
        //printf("\nBuffer Freed\n");
    }
    if(BackupName != NULL){
        free(BackupName);
        BackupName = NULL;
        //printf("\nBackupName freed\n");
    }
    printf("\nreturn status : %d",Status);
    printf("\n");
    return Status;    
}
