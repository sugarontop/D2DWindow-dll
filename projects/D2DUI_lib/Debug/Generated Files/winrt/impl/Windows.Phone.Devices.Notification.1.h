// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210930.14

#pragma once
#ifndef WINRT_Windows_Phone_Devices_Notification_1_H
#define WINRT_Windows_Phone_Devices_Notification_1_H
#include "winrt/impl/Windows.Phone.Devices.Notification.0.h"
WINRT_EXPORT namespace winrt::Windows::Phone::Devices::Notification
{
    struct __declspec(empty_bases) IVibrationDevice :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IVibrationDevice>
    {
        IVibrationDevice(std::nullptr_t = nullptr) noexcept {}
        IVibrationDevice(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct __declspec(empty_bases) IVibrationDeviceStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IVibrationDeviceStatics>
    {
        IVibrationDeviceStatics(std::nullptr_t = nullptr) noexcept {}
        IVibrationDeviceStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
