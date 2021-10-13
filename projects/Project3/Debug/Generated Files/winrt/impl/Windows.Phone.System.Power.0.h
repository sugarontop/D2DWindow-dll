// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210930.14

#pragma once
#ifndef WINRT_Windows_Phone_System_Power_0_H
#define WINRT_Windows_Phone_System_Power_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    template <typename T> struct __declspec(empty_bases) EventHandler;
    struct EventRegistrationToken;
}
WINRT_EXPORT namespace winrt::Windows::Phone::System::Power
{
    enum class PowerSavingMode : int32_t
    {
        Off = 0,
        On = 1,
    };
    struct IPowerManagerStatics;
    struct IPowerManagerStatics2;
    struct PowerManager;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Phone::System::Power::IPowerManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Phone::System::Power::IPowerManagerStatics2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Phone::System::Power::PowerManager>{ using type = class_category; };
    template <> struct category<winrt::Windows::Phone::System::Power::PowerSavingMode>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Phone::System::Power::PowerManager> = L"Windows.Phone.System.Power.PowerManager";
    template <> inline constexpr auto& name_v<winrt::Windows::Phone::System::Power::PowerSavingMode> = L"Windows.Phone.System.Power.PowerSavingMode";
    template <> inline constexpr auto& name_v<winrt::Windows::Phone::System::Power::IPowerManagerStatics> = L"Windows.Phone.System.Power.IPowerManagerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Phone::System::Power::IPowerManagerStatics2> = L"Windows.Phone.System.Power.IPowerManagerStatics2";
    template <> inline constexpr guid guid_v<winrt::Windows::Phone::System::Power::IPowerManagerStatics>{ 0x25DE8FD0,0x1C5B,0x11E1,{ 0xBD,0xDB,0x08,0x00,0x20,0x0C,0x9A,0x66 } }; // 25DE8FD0-1C5B-11E1-BDDB-0800200C9A66
    template <> inline constexpr guid guid_v<winrt::Windows::Phone::System::Power::IPowerManagerStatics2>{ 0x596236CF,0x1918,0x4551,{ 0xA4,0x66,0xC5,0x1A,0xAE,0x37,0x3B,0xF8 } }; // 596236CF-1918-4551-A466-C51AAE373BF8
    template <> struct abi<winrt::Windows::Phone::System::Power::IPowerManagerStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PowerSavingMode(int32_t*) noexcept = 0;
            virtual int32_t __stdcall add_PowerSavingModeChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_PowerSavingModeChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Phone::System::Power::IPowerManagerStatics2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_PowerSavingModeEnabled(bool*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Phone_System_Power_IPowerManagerStatics
    {
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Phone::System::Power::PowerSavingMode) PowerSavingMode() const;
        WINRT_IMPL_AUTO(winrt::event_token) PowerSavingModeChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& changeHandler) const;
        using PowerSavingModeChanged_revoker = impl::event_revoker<winrt::Windows::Phone::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::Phone::System::Power::IPowerManagerStatics>::remove_PowerSavingModeChanged>;
        [[nodiscard]] PowerSavingModeChanged_revoker PowerSavingModeChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& changeHandler) const;
        WINRT_IMPL_AUTO(void) PowerSavingModeChanged(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Windows::Phone::System::Power::IPowerManagerStatics>
    {
        template <typename D> using type = consume_Windows_Phone_System_Power_IPowerManagerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Phone_System_Power_IPowerManagerStatics2
    {
        [[nodiscard]] WINRT_IMPL_AUTO(bool) PowerSavingModeEnabled() const;
    };
    template <> struct consume<winrt::Windows::Phone::System::Power::IPowerManagerStatics2>
    {
        template <typename D> using type = consume_Windows_Phone_System_Power_IPowerManagerStatics2<D>;
    };
}
#endif