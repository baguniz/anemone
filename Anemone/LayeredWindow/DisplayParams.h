/**
* This file is part of Anemone.
*
* Anemone is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* The Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Anemone is distributed in the hope that it will be useful,
*
* But WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Anemone.
*
*   If not, see <http://www.gnu.org/licenses/>.
*
**/

#pragma once

#include "SkImageInfo.h"
#include "SkSurfaceProps.h"
#include "GrContextOptions.h"
#include "SkSurface.h"

struct DisplayParams {
	DisplayParams()
		: fColorType(kN32_SkColorType)
		, fColorSpace(nullptr)
		, fMSAASampleCount(1)
		, fSurfaceProps(SkSurfaceProps::kLegacyFontHost_InitType)
	{}

	SkColorType fColorType;
	sk_sp<SkColorSpace> fColorSpace;
	int fMSAASampleCount;
	GrContextOptions fGrContextOptions;
	SkSurfaceProps fSurfaceProps;
};
