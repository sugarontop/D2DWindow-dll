// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.210930.14

#pragma once
#ifndef WINRT_Windows_System_Diagnostics_0_H
#define WINRT_Windows_System_Diagnostics_0_H
WINRT_EXPORT namespace winrt::Windows::Data::Json
{
    struct JsonObject;
}
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct HResult;
    template <typename TResult, typename TProgress> struct __declspec(empty_bases) IAsyncOperationWithProgress;
}
WINRT_EXPORT namespace winrt::Windows::Foundation::Collections
{
    template <typename T> struct __declspec(empty_bases) IVectorView;
    template <typename T> struct __declspec(empty_bases) IVector;
    struct ValueSet;
}
WINRT_EXPORT namespace winrt::Windows::System
{
    struct AppDiagnosticInfo;
    struct User;
}
WINRT_EXPORT namespace winrt::Windows::System::Diagnostics
{
    enum class DiagnosticActionState : int32_t
    {
        Initializing = 0,
        Downloading = 1,
        VerifyingTrust = 2,
        Detecting = 3,
        Resolving = 4,
        VerifyingResolution = 5,
    };
    struct IDiagnosticActionResult;
    struct IDiagnosticInvoker;
    struct IDiagnosticInvoker2;
    struct IDiagnosticInvokerStatics;
    struct IProcessCpuUsage;
    struct IProcessCpuUsageReport;
    struct IProcessDiagnosticInfo;
    struct IProcessDiagnosticInfo2;
    struct IProcessDiagnosticInfoStatics;
    struct IProcessDiagnosticInfoStatics2;
    struct IProcessDiskUsage;
    struct IProcessDiskUsageReport;
    struct IProcessMemoryUsage;
    struct IProcessMemoryUsageReport;
    struct ISystemCpuUsage;
    struct ISystemCpuUsageReport;
    struct ISystemDiagnosticInfo;
    struct ISystemDiagnosticInfoStatics;
    struct ISystemMemoryUsage;
    struct ISystemMemoryUsageReport;
    struct DiagnosticActionResult;
    struct DiagnosticInvoker;
    struct ProcessCpuUsage;
    struct ProcessCpuUsageReport;
    struct ProcessDiagnosticInfo;
    struct ProcessDiskUsage;
    struct ProcessDiskUsageReport;
    struct ProcessMemoryUsage;
    struct ProcessMemoryUsageReport;
    struct SystemCpuUsage;
    struct SystemCpuUsageReport;
    struct SystemDiagnosticInfo;
    struct SystemMemoryUsage;
    struct SystemMemoryUsageReport;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::System::Diagnostics::IDiagnosticActionResult>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IDiagnosticInvoker>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IDiagnosticInvoker2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IDiagnosticInvokerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessCpuUsage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessCpuUsageReport>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics2>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiskUsage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessDiskUsageReport>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessMemoryUsage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemCpuUsage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemCpuUsageReport>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfoStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemMemoryUsage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::DiagnosticActionResult>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::DiagnosticInvoker>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessCpuUsage>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessCpuUsageReport>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessDiskUsage>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessDiskUsageReport>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessMemoryUsage>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::SystemCpuUsage>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::SystemCpuUsageReport>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::SystemDiagnosticInfo>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::SystemMemoryUsage>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::SystemMemoryUsageReport>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Diagnostics::DiagnosticActionState>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::DiagnosticActionResult> = L"Windows.System.Diagnostics.DiagnosticActionResult";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::DiagnosticInvoker> = L"Windows.System.Diagnostics.DiagnosticInvoker";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessCpuUsage> = L"Windows.System.Diagnostics.ProcessCpuUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessCpuUsageReport> = L"Windows.System.Diagnostics.ProcessCpuUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo> = L"Windows.System.Diagnostics.ProcessDiagnosticInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessDiskUsage> = L"Windows.System.Diagnostics.ProcessDiskUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessDiskUsageReport> = L"Windows.System.Diagnostics.ProcessDiskUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessMemoryUsage> = L"Windows.System.Diagnostics.ProcessMemoryUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport> = L"Windows.System.Diagnostics.ProcessMemoryUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::SystemCpuUsage> = L"Windows.System.Diagnostics.SystemCpuUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::SystemCpuUsageReport> = L"Windows.System.Diagnostics.SystemCpuUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::SystemDiagnosticInfo> = L"Windows.System.Diagnostics.SystemDiagnosticInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::SystemMemoryUsage> = L"Windows.System.Diagnostics.SystemMemoryUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::SystemMemoryUsageReport> = L"Windows.System.Diagnostics.SystemMemoryUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::DiagnosticActionState> = L"Windows.System.Diagnostics.DiagnosticActionState";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IDiagnosticActionResult> = L"Windows.System.Diagnostics.IDiagnosticActionResult";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IDiagnosticInvoker> = L"Windows.System.Diagnostics.IDiagnosticInvoker";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IDiagnosticInvoker2> = L"Windows.System.Diagnostics.IDiagnosticInvoker2";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IDiagnosticInvokerStatics> = L"Windows.System.Diagnostics.IDiagnosticInvokerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessCpuUsage> = L"Windows.System.Diagnostics.IProcessCpuUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessCpuUsageReport> = L"Windows.System.Diagnostics.IProcessCpuUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo> = L"Windows.System.Diagnostics.IProcessDiagnosticInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo2> = L"Windows.System.Diagnostics.IProcessDiagnosticInfo2";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics> = L"Windows.System.Diagnostics.IProcessDiagnosticInfoStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics2> = L"Windows.System.Diagnostics.IProcessDiagnosticInfoStatics2";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiskUsage> = L"Windows.System.Diagnostics.IProcessDiskUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessDiskUsageReport> = L"Windows.System.Diagnostics.IProcessDiskUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessMemoryUsage> = L"Windows.System.Diagnostics.IProcessMemoryUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport> = L"Windows.System.Diagnostics.IProcessMemoryUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemCpuUsage> = L"Windows.System.Diagnostics.ISystemCpuUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemCpuUsageReport> = L"Windows.System.Diagnostics.ISystemCpuUsageReport";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo> = L"Windows.System.Diagnostics.ISystemDiagnosticInfo";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfoStatics> = L"Windows.System.Diagnostics.ISystemDiagnosticInfoStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemMemoryUsage> = L"Windows.System.Diagnostics.ISystemMemoryUsage";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport> = L"Windows.System.Diagnostics.ISystemMemoryUsageReport";
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IDiagnosticActionResult>{ 0xC265A296,0xE73B,0x4097,{ 0xB2,0x8F,0x34,0x42,0xF0,0x3D,0xD8,0x31 } }; // C265A296-E73B-4097-B28F-3442F03DD831
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IDiagnosticInvoker>{ 0x187B270A,0x02E3,0x4F86,{ 0x84,0xFC,0xFD,0xD8,0x92,0xB5,0x94,0x0F } }; // 187B270A-02E3-4F86-84FC-FDD892B5940F
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IDiagnosticInvoker2>{ 0xE3BF945C,0x155A,0x4B52,{ 0xA8,0xEC,0x07,0x0C,0x44,0xF9,0x50,0x00 } }; // E3BF945C-155A-4B52-A8EC-070C44F95000
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IDiagnosticInvokerStatics>{ 0x5CFAD8DE,0xF15C,0x4554,{ 0xA8,0x13,0xC1,0x13,0xC3,0x88,0x1B,0x09 } }; // 5CFAD8DE-F15C-4554-A813-C113C3881B09
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessCpuUsage>{ 0x0BBB2472,0xC8BF,0x423A,{ 0xA8,0x10,0xB5,0x59,0xAE,0x43,0x54,0xE2 } }; // 0BBB2472-C8BF-423A-A810-B559AE4354E2
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessCpuUsageReport>{ 0x8A6D9CAC,0x3987,0x4E2F,{ 0xA1,0x19,0x6B,0x5F,0xA2,0x14,0xF1,0xB4 } }; // 8A6D9CAC-3987-4E2F-A119-6B5FA214F1B4
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo>{ 0xE830B04B,0x300E,0x4EE6,{ 0xA0,0xAB,0x5B,0x5F,0x52,0x31,0xB4,0x34 } }; // E830B04B-300E-4EE6-A0AB-5B5F5231B434
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo2>{ 0x9558CB1A,0x3D0B,0x49EC,{ 0xAB,0x70,0x4F,0x7A,0x11,0x28,0x05,0xDE } }; // 9558CB1A-3D0B-49EC-AB70-4F7A112805DE
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics>{ 0x2F41B260,0xB49F,0x428C,{ 0xAA,0x0E,0x84,0x74,0x4F,0x49,0xCA,0x95 } }; // 2F41B260-B49F-428C-AA0E-84744F49CA95
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics2>{ 0x4A869897,0x9899,0x4A44,{ 0xA2,0x9B,0x09,0x16,0x63,0xBE,0x09,0xB6 } }; // 4A869897-9899-4A44-A29B-091663BE09B6
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiskUsage>{ 0x5AD78BFD,0x7E51,0x4E53,{ 0xBF,0xAA,0x5A,0x6E,0xE1,0xAA,0xBB,0xF8 } }; // 5AD78BFD-7E51-4E53-BFAA-5A6EE1AABBF8
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessDiskUsageReport>{ 0x401627FD,0x535D,0x4C1F,{ 0x81,0xB8,0xDA,0x54,0xE1,0xBE,0x63,0x5E } }; // 401627FD-535D-4C1F-81B8-DA54E1BE635E
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessMemoryUsage>{ 0xF50B229B,0x827C,0x42B7,{ 0xB0,0x7C,0x0E,0x32,0x62,0x7E,0x6B,0x3E } }; // F50B229B-827C-42B7-B07C-0E32627E6B3E
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport>{ 0xC2C77CBA,0x1951,0x4685,{ 0x85,0x32,0x7E,0x74,0x9E,0xCF,0x8E,0xEB } }; // C2C77CBA-1951-4685-8532-7E749ECF8EEB
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemCpuUsage>{ 0x6037B3AC,0x02D6,0x4234,{ 0x83,0x62,0x7F,0xE3,0xAD,0xC8,0x1F,0x5F } }; // 6037B3AC-02D6-4234-8362-7FE3ADC81F5F
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemCpuUsageReport>{ 0x2C26D0B2,0x9483,0x4F62,{ 0xAB,0x57,0x82,0xB2,0x9D,0x97,0x19,0xB8 } }; // 2C26D0B2-9483-4F62-AB57-82B29D9719B8
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo>{ 0xA290FE05,0xDFF3,0x407F,{ 0x9A,0x1B,0x0B,0x2B,0x31,0x7C,0xA8,0x00 } }; // A290FE05-DFF3-407F-9A1B-0B2B317CA800
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfoStatics>{ 0xD404AC21,0xFC7D,0x45F0,{ 0x9A,0x3F,0x39,0x20,0x3A,0xED,0x9F,0x7E } }; // D404AC21-FC7D-45F0-9A3F-39203AED9F7E
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemMemoryUsage>{ 0x17FFC595,0x1702,0x49CF,{ 0xAA,0x27,0x2F,0x0A,0x32,0x59,0x14,0x04 } }; // 17FFC595-1702-49CF-AA27-2F0A32591404
    template <> inline constexpr guid guid_v<winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport>{ 0x38663C87,0x2A9F,0x403A,{ 0xBD,0x19,0x2C,0xF3,0xE8,0x16,0x95,0x00 } }; // 38663C87-2A9F-403A-BD19-2CF3E8169500
    template <> struct default_interface<winrt::Windows::System::Diagnostics::DiagnosticActionResult>{ using type = winrt::Windows::System::Diagnostics::IDiagnosticActionResult; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::DiagnosticInvoker>{ using type = winrt::Windows::System::Diagnostics::IDiagnosticInvoker; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessCpuUsage>{ using type = winrt::Windows::System::Diagnostics::IProcessCpuUsage; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessCpuUsageReport>{ using type = winrt::Windows::System::Diagnostics::IProcessCpuUsageReport; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo>{ using type = winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessDiskUsage>{ using type = winrt::Windows::System::Diagnostics::IProcessDiskUsage; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessDiskUsageReport>{ using type = winrt::Windows::System::Diagnostics::IProcessDiskUsageReport; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessMemoryUsage>{ using type = winrt::Windows::System::Diagnostics::IProcessMemoryUsage; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport>{ using type = winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::SystemCpuUsage>{ using type = winrt::Windows::System::Diagnostics::ISystemCpuUsage; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::SystemCpuUsageReport>{ using type = winrt::Windows::System::Diagnostics::ISystemCpuUsageReport; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::SystemDiagnosticInfo>{ using type = winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::SystemMemoryUsage>{ using type = winrt::Windows::System::Diagnostics::ISystemMemoryUsage; };
    template <> struct default_interface<winrt::Windows::System::Diagnostics::SystemMemoryUsageReport>{ using type = winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport; };
    template <> struct abi<winrt::Windows::System::Diagnostics::IDiagnosticActionResult>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_ExtendedError(winrt::hresult*) noexcept = 0;
            virtual int32_t __stdcall get_Results(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IDiagnosticInvoker>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall RunDiagnosticActionAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IDiagnosticInvoker2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall RunDiagnosticActionFromStringAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IDiagnosticInvokerStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetDefault(void**) noexcept = 0;
            virtual int32_t __stdcall GetForUser(void*, void**) noexcept = 0;
            virtual int32_t __stdcall get_IsSupported(bool*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessCpuUsage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetReport(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessCpuUsageReport>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_KernelTime(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_UserTime(int64_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_ProcessId(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_ExecutableFileName(void**) noexcept = 0;
            virtual int32_t __stdcall get_Parent(void**) noexcept = 0;
            virtual int32_t __stdcall get_ProcessStartTime(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_DiskUsage(void**) noexcept = 0;
            virtual int32_t __stdcall get_MemoryUsage(void**) noexcept = 0;
            virtual int32_t __stdcall get_CpuUsage(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetAppDiagnosticInfos(void**) noexcept = 0;
            virtual int32_t __stdcall get_IsPackaged(bool*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetForProcesses(void**) noexcept = 0;
            virtual int32_t __stdcall GetForCurrentProcess(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics2>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall TryGetForProcessId(uint32_t, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiskUsage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetReport(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessDiskUsageReport>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_ReadOperationCount(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_WriteOperationCount(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_OtherOperationCount(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_BytesReadCount(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_BytesWrittenCount(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_OtherBytesCount(int64_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessMemoryUsage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetReport(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_NonPagedPoolSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PageFaultCount(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_PageFileSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PagedPoolSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PeakNonPagedPoolSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PeakPageFileSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PeakPagedPoolSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PeakVirtualMemorySizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PeakWorkingSetSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_PrivatePageCount(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_VirtualMemorySizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_WorkingSetSizeInBytes(uint64_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemCpuUsage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetReport(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemCpuUsageReport>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_KernelTime(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_UserTime(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_IdleTime(int64_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_MemoryUsage(void**) noexcept = 0;
            virtual int32_t __stdcall get_CpuUsage(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfoStatics>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetForCurrentSystem(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemMemoryUsage>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall GetReport(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport>
    {
        struct __declspec(novtable) type : inspectable_abi
        {
            virtual int32_t __stdcall get_TotalPhysicalSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_AvailableSizeInBytes(uint64_t*) noexcept = 0;
            virtual int32_t __stdcall get_CommittedSizeInBytes(uint64_t*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IDiagnosticActionResult
    {
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::hresult) ExtendedError() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::Collections::ValueSet) Results() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IDiagnosticActionResult>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IDiagnosticActionResult<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IDiagnosticInvoker
    {
        WINRT_IMPL_AUTO(winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::System::Diagnostics::DiagnosticActionResult, winrt::Windows::System::Diagnostics::DiagnosticActionState>) RunDiagnosticActionAsync(winrt::Windows::Data::Json::JsonObject const& context) const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IDiagnosticInvoker>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IDiagnosticInvoker<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IDiagnosticInvoker2
    {
        WINRT_IMPL_AUTO(winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::System::Diagnostics::DiagnosticActionResult, winrt::Windows::System::Diagnostics::DiagnosticActionState>) RunDiagnosticActionFromStringAsync(param::hstring const& context) const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IDiagnosticInvoker2>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IDiagnosticInvoker2<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IDiagnosticInvokerStatics
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::DiagnosticInvoker) GetDefault() const;
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::DiagnosticInvoker) GetForUser(winrt::Windows::System::User const& user) const;
        [[nodiscard]] WINRT_IMPL_AUTO(bool) IsSupported() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IDiagnosticInvokerStatics>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IDiagnosticInvokerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessCpuUsage
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessCpuUsageReport) GetReport() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessCpuUsage>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessCpuUsage<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessCpuUsageReport
    {
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::TimeSpan) KernelTime() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::TimeSpan) UserTime() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessCpuUsageReport>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessCpuUsageReport<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiagnosticInfo
    {
        [[nodiscard]] WINRT_IMPL_AUTO(uint32_t) ProcessId() const;
        [[nodiscard]] WINRT_IMPL_AUTO(hstring) ExecutableFileName() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo) Parent() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::DateTime) ProcessStartTime() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessDiskUsage) DiskUsage() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessMemoryUsage) MemoryUsage() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessCpuUsage) CpuUsage() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiagnosticInfo<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiagnosticInfo2
    {
        WINRT_IMPL_AUTO(winrt::Windows::Foundation::Collections::IVector<winrt::Windows::System::AppDiagnosticInfo>) GetAppDiagnosticInfos() const;
        [[nodiscard]] WINRT_IMPL_AUTO(bool) IsPackaged() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfo2>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiagnosticInfo2<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiagnosticInfoStatics
    {
        WINRT_IMPL_AUTO(winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo>) GetForProcesses() const;
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo) GetForCurrentProcess() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiagnosticInfoStatics<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiagnosticInfoStatics2
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo) TryGetForProcessId(uint32_t processId) const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiagnosticInfoStatics2>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiagnosticInfoStatics2<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiskUsage
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessDiskUsageReport) GetReport() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiskUsage>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiskUsage<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessDiskUsageReport
    {
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) ReadOperationCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) WriteOperationCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) OtherOperationCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) BytesReadCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) BytesWrittenCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(int64_t) OtherBytesCount() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessDiskUsageReport>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessDiskUsageReport<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessMemoryUsage
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport) GetReport() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessMemoryUsage>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessMemoryUsage<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_IProcessMemoryUsageReport
    {
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) NonPagedPoolSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint32_t) PageFaultCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PageFileSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PagedPoolSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PeakNonPagedPoolSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PeakPageFileSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PeakPagedPoolSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PeakVirtualMemorySizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PeakWorkingSetSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) PrivatePageCount() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) VirtualMemorySizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) WorkingSetSizeInBytes() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::IProcessMemoryUsageReport>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_IProcessMemoryUsageReport<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemCpuUsage
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::SystemCpuUsageReport) GetReport() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemCpuUsage>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemCpuUsage<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemCpuUsageReport
    {
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::TimeSpan) KernelTime() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::TimeSpan) UserTime() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::Foundation::TimeSpan) IdleTime() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemCpuUsageReport>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemCpuUsageReport<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemDiagnosticInfo
    {
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::SystemMemoryUsage) MemoryUsage() const;
        [[nodiscard]] WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::SystemCpuUsage) CpuUsage() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfo>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemDiagnosticInfo<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemDiagnosticInfoStatics
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::SystemDiagnosticInfo) GetForCurrentSystem() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemDiagnosticInfoStatics>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemDiagnosticInfoStatics<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemMemoryUsage
    {
        WINRT_IMPL_AUTO(winrt::Windows::System::Diagnostics::SystemMemoryUsageReport) GetReport() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemMemoryUsage>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemMemoryUsage<D>;
    };
    template <typename D>
    struct consume_Windows_System_Diagnostics_ISystemMemoryUsageReport
    {
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) TotalPhysicalSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) AvailableSizeInBytes() const;
        [[nodiscard]] WINRT_IMPL_AUTO(uint64_t) CommittedSizeInBytes() const;
    };
    template <> struct consume<winrt::Windows::System::Diagnostics::ISystemMemoryUsageReport>
    {
        template <typename D> using type = consume_Windows_System_Diagnostics_ISystemMemoryUsageReport<D>;
    };
}
#endif
