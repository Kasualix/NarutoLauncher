
// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2022 r58Playz <r58playz@gmail.com>
 *  Copyright (C) 2024 timoreo <contact@timoreo.fr>
 *  Copyright (C) 2024 Trial97 <alexandru.tripon97@gmail.com>
 *  Copyright (C) 2025 TheKodeToad <TheKodeToad@proton.me>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include <QString>

#include "HardwareInfo.h"

namespace SysInfo {
QString currentSystem()
{
    return "windows";
}

QString useQTForArch()
{
    return QSysInfo::currentCpuArchitecture();
}

int defaultMaxJvmMem()
{
    // If totalRAM < 6GB, use (totalRAM / 1.5), else 4GB
    if (const uint64_t totalRAM = HardwareInfo::totalRamMiB(); totalRAM < (4096 * 1.5))
        return totalRAM / 1.5;
    else
        return 4096;
}

QString getSupportedJavaArchitecture()
{
    auto arch = useQTForArch();
    if (arch == "x86_64")
        return "windows-x64";
    if (arch == "i386")
        return "windows-x86";
    // Unknown, maybe arm, appending arch
    return "windows-" + arch;
}
}  // namespace SysInfo
