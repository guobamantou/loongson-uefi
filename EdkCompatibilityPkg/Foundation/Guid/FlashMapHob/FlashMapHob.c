/*++

Copyright (c) 2004, Intel Corporation                                                         
All rights reserved. This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  FlashMapHob.c
    
Abstract:

  GUIDs used for Flash Map HOB entries in the in the HOB list.

--*/

#include "Tiano.h"
#include EFI_GUID_DEFINITION (FlashMapHob)


EFI_GUID gEfiFlashMapHobGuid  = EFI_FLASH_MAP_HOB_GUID;

EFI_GUID_STRING (&gEfiFlashMapHobGuid, "Flash Map HOB", "Flash Map HOB GUID for HOB list");

