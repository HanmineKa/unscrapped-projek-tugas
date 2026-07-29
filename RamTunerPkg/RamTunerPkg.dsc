[Defines]
  PLATFORM_NAME                   = RamTuner
  PLATFORM_GUID                   = 4D4D2B18-2D66-42E9-8D7D-5A7F6E8C9B0A
  PLATFORM_VERSION                = 0.1
  DSC_SPECIFICATION               = 0x0001001B
  OUTPUT_DIRECTORY                = Build/RamTuner
  SUPPORTED_ARCHITECTURES         = X64
  BUILD_TARGETS                   = DEBUG|RELEASE
  SKUID_IDENTIFIER                = DEFAULT

[Components]
  RamTunerPkg/RamTunerPei.inf
