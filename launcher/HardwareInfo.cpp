// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2026 Octol1ttle <l1ttleofficial@outlook.com>
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
 */

#include "HardwareInfo.h"

#include <QDebug>
#include <QStringList>

#include <QSettings>

#include <dxgi1_6.h>
#include <windows.h>

#include <winrt/base.h>
using winrt::com_ptr;

QString HardwareInfo::cpuInfo()
{
    const QSettings registry(R"(HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\CentralProcessor\0)", QSettings::NativeFormat);
    return registry.value("ProcessorNameString").toString();
}

uint64_t HardwareInfo::totalRamMiB()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof status;

    if (GlobalMemoryStatusEx(&status) == TRUE) {
        // transforming bytes -> mib
        return status.ullTotalPhys / 1024 / 1024;
    }

    qWarning() << "Could not get total RAM: GlobalMemoryStatusEx";
    return 0;
}

uint64_t HardwareInfo::availableRamMiB()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof status;

    if (GlobalMemoryStatusEx(&status) == TRUE) {
        // transforming bytes -> mib
        return status.ullAvailPhys / 1024 / 1024;
    }

    qWarning() << "Could not get available RAM: GlobalMemoryStatusEx";
    return 0;
}

QStringList HardwareInfo::gpuInfo()
{
    com_ptr<IDXGIFactory6> factory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        qWarning() << "Could not create DXGI factory:" << Qt::hex << hr;
        return { "GPU discovery failed: could not create DXGI factory" };
    }

    UINT i = 0;
    com_ptr<IDXGIAdapter> adapter;
    QStringList out;
    while (factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC desc;
        hr = adapter->GetDesc(&desc);
        if (SUCCEEDED(hr)) {
            out << "GPU: " + QString::fromWCharArray(desc.Description);  // NOLINT(*-pro-bounds-array-to-pointer-decay, *-no-array-decay)
        } else {
            qWarning() << "Could not get DXGI adapter description:" << Qt::hex << hr;
        }

        ++i;
    }

    return out;
}
