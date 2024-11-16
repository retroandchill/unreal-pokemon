// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDCustomVersion.h"
#include "Serialization/CustomVersion.h"

const FGuid FPaperZDCustomVersion::GUID(0x11310AED, 0x2E554D61, 0xAF679AA3, 0xC5A1082A);

// Register the custom version with core
FCustomVersionRegistration GRegisterPaperZDCustomVersion(FPaperZDCustomVersion::GUID, FPaperZDCustomVersion::LatestVersion, TEXT("Paper_ZD_Version"));