// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210930.14

#pragma once
#ifndef WINRT_Windows_Devices_Portable_2_H
#define WINRT_Windows_Devices_Portable_2_H
#include "winrt/impl/Windows.Devices.Portable.1.h"
WINRT_EXPORT namespace winrt::Windows::Devices::Portable
{
    struct ServiceDevice
    {
        ServiceDevice() = delete;
        static auto GetDeviceSelector(winrt::Windows::Devices::Portable::ServiceDeviceType const& serviceType);
        static auto GetDeviceSelectorFromServiceId(winrt::guid const& serviceId);
    };
    struct StorageDevice
    {
        StorageDevice() = delete;
        static auto FromId(param::hstring const& deviceId);
        static auto GetDeviceSelector();
    };
}
#endif
