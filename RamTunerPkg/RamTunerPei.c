#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/GlobalVariable.h>

/**
  RamTuner PEIM entry point.

  This PEIM locates the ReadOnlyVariable2 PPI and attempts to read a
  UEFI variable named "RamTunerProfile". It emits DEBUG output to the
  serial console and leaves a placeholder for future Intel FSP-M UPD
  override logic for Kaby Lake / B250.

  @param  FileHandle   The file handle of this PEIM.
  @param  PeiServices  The PEI services table.

  @retval EFI_SUCCESS            The entry point completed successfully.
  @retval EFI_NOT_FOUND          The required PPI was not found.
  @retval other                  An error from the variable read.
**/
EFI_STATUS
EFIAPI
RamTunerPeiEntry (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *ReadOnlyVarPpi;
  UINTN DataSize;
  VOID *Buffer;
  CHAR16 VariableName[] = L"RamTunerProfile";

  DEBUG ((DEBUG_INFO, "RamTunerPei: Entry point initialized\n"));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&ReadOnlyVarPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RamTunerPei: Failed to locate ReadOnlyVariable2 PPI: %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "RamTunerPei: Located ReadOnlyVariable2 PPI\n"));
  DEBUG ((DEBUG_INFO, "RamTunerPei: Attempting to read variable %S\n", VariableName));

  DataSize = 0;
  Buffer = NULL;
  Status = ReadOnlyVarPpi->GetVariable (
             ReadOnlyVarPpi,
             VariableName,
             &gEfiGlobalVariableGuid,
             NULL,
             &DataSize,
             Buffer
             );

  if (Status == EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_INFO, "RamTunerPei: Variable %S exists and requires %u bytes\n", VariableName, DataSize));
  } else if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "RamTunerPei: Variable %S not found\n", VariableName));
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RamTunerPei: Unexpected error reading %S: %r\n", VariableName, Status));
  } else {
    DEBUG ((DEBUG_INFO, "RamTunerPei: Variable %S read successfully with %u bytes\n", VariableName, DataSize));
  }

  DEBUG ((DEBUG_INFO, "RamTunerPei: Placeholder for future FSP-M UPD override logic\n"));

  /**
    Future work:
    - Identify the FSP-M UPD structure used by the target B250 platform.
    - Locate the UPD image in CAR or heap before FSP-M execution.
    - Patch timing values such as tCL, tRCD, tRP, and frequency.
    - Validate the resulting configuration and preserve OEM defaults if the profile is invalid.
  **/

  return EFI_SUCCESS;
}
