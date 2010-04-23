[Defines]
   PLATFORM_NAME                  = Fuloong6003Pkg
   PLATFORM_GUID                  = 649bdf96-aab2-4888-986a-f1aee4fdd4bf
   PLATFORM_VERSION               = 0.1
   DSC_SPECIFICATION              = 0x00010005
   OUTPUT_DIRECTORY               = Build/Fuloong6003
   SUPPORTED_ARCHITECTURES        = MIPS64
   BUILD_TARGETS                  = DEBUG|RELEASE
   SKUID_IDENTIFIER               = DEFAULT
   FLASH_DEFINITION               = Fuloong6003Pkg/Fuloong6003Pkg.fdf
   DEFINE TARGET_HACK             = DEBUG

[LibraryClasses.common]
   BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
   BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
   PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
   PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
   PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  
   PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
   PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf  

   PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf

   IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
   UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
   DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

   PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
   CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
   PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf

[LibraryClasses.common.SEC]
   PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
   UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf

[PcdsFixedAtBuild.common]

[Components.common]
   Fuloong6003Pkg/Sec/SecCore.inf
   Fuloong6003Pkg/Sec/Exception/Exception.inf

