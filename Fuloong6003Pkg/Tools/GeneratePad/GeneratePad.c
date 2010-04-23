#include <stdio.h>
#include <string.h>

#define PAD_FILE_EXT 		".pad"
#define PAD_DATA_DEFAULT	0xff
#define STATUS_ERROR        1
#define STATUS_SUCCESS      0

void Usage(void){
    printf("\nTool used to generate a pad file which is filled with the specified data");
    printf("\nUsage:");
    printf("\n\tGeneratePad [-d Data] -f FileName -s FileSize");
    printf("\nwhere:");
    printf("\n\t-d Data : specifies what the file will be filled with, \n\t\tData should be a byte value in HEX format");
    printf("\n\t-f Filename: the PAD file name");
    printf("\n\t-s FileSize: the PAD file size");
    printf("\n");
}

int main(int Argc, char *Argv[]){
    unsigned int    PadSize;
    unsigned int    PadSizeSpecified;
    char            *PadFileName;
    unsigned char   *PadData;
    unsigned int    TmpData;
    unsigned int    PadDataSpecified;
    unsigned int    RStatus;
    FILE            *fp;
    
    
    //
    //prepare varibales
    PadDataSpecified = 0;
    PadSizeSpecified = 0;
    PadData = NULL;
    TmpData = PAD_DATA_DEFAULT;
    RStatus = STATUS_SUCCESS;
    PadFileName = NULL;
    fp  = NULL;
    
    //
    //parse options
    if(Argc < 5){
        printf("\nInvalid parameters\n");
        Usage();
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;
    }
    
    do{
        Argc --;
        Argv ++;
        if((Argc <= 0) || (Argv == NULL)){
            break;
        }
        if((strcmp("-d", Argv[0])==0) || (strcmp("/d", Argv[0])==0)){            
            Argc --;
            Argv ++;
            if((Argc <= 0) || (Argv == NULL)){
                printf("\nInvalid parameters when parsing -d option\n");
                Usage();
                RStatus = STATUS_ERROR;
                goto _EXIT_POINT;
            }
            sscanf(Argv[0], "%x", &TmpData);
            PadDataSpecified = 1;
            continue;
        }
        else if((strcmp("-f", Argv[0])==0) || (strcmp("/f", Argv[0])==0)){
            Argc --;
            Argv ++;
            if((Argc <= 0) || (Argv == NULL)){
                printf("\nInvalid parameters when parsing -f option\n");
                Usage();
                RStatus = STATUS_ERROR;
                goto _EXIT_POINT;
            }
            PadFileName = Argv[0];
            continue;
        }
        else if((strcmp("-s", Argv[0])==0) || (strcmp("/s", Argv[0])==0)){
            Argc --;
            Argv ++;
            if((Argc <= 0) || (Argv == NULL)){
                printf("\nInvalid parameters when parsing -s option\n");
                Usage();
                RStatus = STATUS_ERROR;
                goto _EXIT_POINT;
            }
            sscanf(Argv[0], "%x", &PadSize);
            PadSizeSpecified = 1;
            continue;
        }
        else{
            printf("\nUnknow option %s\n", Argv[0]);
            Usage();
            RStatus = STATUS_ERROR;
            goto _EXIT_POINT;
        }        
    }while((Argc > 0) && (Argv[0] != NULL));
    
    //
    //check if all options needed are valued
    if(PadSizeSpecified == 0){
        printf("\nThe size of the pad file should be specified wiht -s option");
        Usage();
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;
    }
    if(PadDataSpecified == 0){
        printf("\nDefault pad data : 0x%x : used\n", PAD_DATA_DEFAULT);
    }
    if(PadFileName == NULL){
        printf("\nPad file name must be specified with -f option");
        Usage();
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;
    }
    
    
    //
    //do the real work
    fp = NULL;
    fp = fopen(PadFileName, "r");
    if(fp){
        printf("\nPad file : %s : exist, overwrite...\n", PadFileName);
        fclose(fp);
    }
    
    fp = NULL;
    fp = fopen(PadFileName, "wb");
    if(fp == NULL){
        printf("\nOpen file : %s : error\n", PadFileName);
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;
    }

    PadData = NULL;
    PadData = (unsigned char *)malloc(PadSize);
    if(PadData == NULL){
        printf("\nOut of resources\n");
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;
    }
    memset(PadData, TmpData, PadSize);
    
    if (PadSize != fwrite(PadData, sizeof(unsigned char), PadSize, fp)){
        printf("\nWrite data to : %s : error\n", PadFileName);
        RStatus = STATUS_ERROR;
        goto _EXIT_POINT;    
    }
    else{
        printf("\nPad file : %s : generated successfully\n", PadFileName);
        RStatus = STATUS_SUCCESS;
    }
       
    //
    //exit point
_EXIT_POINT:
    if(fp != NULL){
        fclose(fp);
        fp = NULL;
    }
    if(PadData != NULL){
        free((void *)PadData);
        PadData = NULL;
    }
    return RStatus;
}
