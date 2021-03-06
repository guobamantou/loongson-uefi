;------------------------------------------------------------------------------
; Include file for X64 MpFuncs.asm
;
; Copyright (c) 2009, Intel Corporation
; All rights reserved. This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;------------------------------------------------------------------------------

VacantFlag             Equ   00h
NotVacantFlag          Equ   0ffh

LockLocation                  equ        RendezvousFunnelProcEnd - RendezvousFunnelProcStart
StackStartAddressLocation     equ        LockLocation + 08h
StackSizeLocation             equ        LockLocation + 10h
CProcedureLocation            equ        LockLocation + 18h
GdtrLocation                  equ        LockLocation + 20h
BufferStartLocation           equ        LockLocation + 2Ch
Cr3OffsetLocation             equ        LockLocation + 30h

;-------------------------------------------------------------------------------
