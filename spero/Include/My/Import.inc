
 library Advapi32,'Advapi32.dll',\
         Gdi32,'Gdi32.dll',\
         Ole32,'Ole32.dll',\
         Comctl32,'Comctl32.dll',\
         Comdlg32,'Comdlg32.dll',\
         Dxva2,'Dxva2.dll',\
         Msimg32,'Msimg32.dll',\
         Kernel32,'Kernel32.dll',\
         Ntdll,'Ntdll.dll',\
         Shell32,'Shell32.dll',\
         User32,'User32.dll'

 import Ntdll,\
        NtSetDebugFilterState,'NtSetDebugFilterState',3,\
        NtSetInformationProcess,'NtSetInformationProcess',4,\
        NtSetInformationThread,'NtSetInformationThread',4,\
        NtGetContextThread,'NtGetContextThread',2,\
        NtQueryObject,'NtQueryObject',5,\
        NtQuerySystemInformation,'NtQuerySystemInformation',4,\
        NtQueryVirtualMemory,'NtQueryVirtualMemory',6


 import Msimg32,\
        TransparentBlt,'TransparentBlt',11

 import Dxva2,\
        DestroyPhysicalMonitor,'DestroyPhysicalMonitor',1,\
        DestroyPhysicalMonitors,'DestroyPhysicalMonitors',2,\
        GetPhysicalMonitorsFromHMONITOR,'GetPhysicalMonitorsFromHMONITOR',3,\
        GetNumberOfPhysicalMonitorsFromHMONITOR,'GetNumberOfPhysicalMonitorsFromHMONITOR',2,\
        GetCapabilitiesStringLength,'GetCapabilitiesStringLength',2,\
        GetVCPFeatureAndVCPFeatureReply,'GetVCPFeatureAndVCPFeatureReply',5,\
        CapabilitiesRequestAndCapabilitiesReply,'CapabilitiesRequestAndCapabilitiesReply',3,\
        GetMonitorTechnologyType,'GetMonitorTechnologyType',2,\
        SetVCPFeature,'SetVCPFeature',3

 import Advapi32,\
        RegDeleteKeyValueW,'RegDeleteKeyValueW',3,\
        RegDeleteKeyExW,'RegDeleteKeyExW',4,\
        RegOpenKeyExW,'RegOpenKeyExW',5,\
        RegCreateKeyExW,'RegCreateKeyExW',9,\
        RegSetValueExW,'RegSetValueExW',6

 import Gdi32,\
        BeginPath,'BeginPath',1,\
        BitBlt,'BitBlt',9,\
        CreateCompatibleDC,'CreateCompatibleDC',1,\
        CreateRectRgn,'CreateRectRgn',4,\
        CreateFontIndirectW,'CreateFontIndirectW',1,\
        DeleteDC,'DeleteDC',1,\
        DeleteObject,'DeleteObject',1,\
        EndPath,'EndPath',1,\
        ExtTextOutW,'ExtTextOutW',8,\
        GetTextExtentPoint32W,'GetTextExtentPoint32W',4,\
        GetRegionData,'GetRegionData',3,\
        GetObjectW,'GetObjectW',3,\
        GetStockObject,'GetStockObject',1,\
        GetTextMetricsW,'GetTextMetricsW',2,\
        GetTextFaceW,'GetTextFaceW',3,\
        FillPath,'FillPath',1,\
        PatBlt,'PatBlt',6,\
        Polyline,'Polyline',3,\
        PolyTextOutW,'PolyTextOutW',3,\
        PtInRegion,'PtInRegion',3,\
        Rectangle,'Rectangle',5,\
        RectInRegion,'RectInRegion',2,\
        RectVisible,'RectVisible',2,\
        RoundRect,'RoundRect',7,\
        SetROP2,'SetROP2',2,\
        SetDCPenColor,'SetDCPenColor',2,\
        SetDCBrushColor,'SetDCBrushColor',2,\
        SetBkMode,'SetBkMode',2,\
        SetBkColor,'SetBkColor',2,\
        SelectObject,'SelectObject',2,\
        SetTextColor,'SetTextColor',2


 import Ole32,\
        CoInitializeEx,'CoInitializeEx',2,\
        CoTaskMemFree,'CoTaskMemFree',1

 import Comctl32,\
        CreateStatusWindowW,'CreateStatusWindowW',4,\
        ImageList_Add,'ImageList_Add',3,\
        ImageList_Destroy,'ImageList_Destroy',1,\
        ImageList_Create,'ImageList_Create',5,\
        ImageList_GetImageCount,'ImageList_GetImageCount',1,\
        ImageList_SetOverlayImage,'ImageList_SetOverlayImage',3,\
        ImageList_ReplaceIcon,'ImageList_ReplaceIcon',3,\
        InitCommonControlsEx,'InitCommonControlsEx',1


 import Comdlg32,\
        CommDlgExtendedError,'CommDlgExtendedError',0,\
        GetOpenFileNameW,'GetOpenFileNameW',1

 import Kernel32,\
        AcquireSRWLockExclusive,'AcquireSRWLockExclusive',1,\
        AcquireSRWLockShared ,'AcquireSRWLockShared',1,\
        AddVectoredContinueHandler,'AddVectoredContinueHandler',2,\
        AddVectoredExceptionHandler,'AddVectoredExceptionHandler',2,\
        DebugActiveProcess,'DebugActiveProcess',1,\
        DeleteFileW,'DeleteFileW',1,\
        DeviceIoControl,'DeviceIoControl',8,\
        IsDebuggerPresent,'IsDebuggerPresent',0,\
        InitializeSRWLock,'InitializeSRWLock',1,\
        LoadLibraryW,'LoadLibraryW',1,\
        LoadLibraryExW,'LoadLibraryExW',3,\
        VerifyVersionInfoW,'VerifyVersionInfoW',3,\
        VirtualAllocEx,'VirtualAllocEx',5,\
        FindClose,'FindClose',1,\
        FindFirstFileExW,'FindFirstFileExW',6,\
        FindNextFileW,'FindNextFileW',2,\
        FormatMessageW,'FormatMessageW',7,\
        FreeLibrary,'FreeLibrary',1,\
        CloseHandle,'CloseHandle',1,\
        CreateDirectoryW,'CreateDirectoryW',2,\
        CreateEventExW,'CreateEventExW',4,\
        CreateFileW,'CreateFileW',7,\
        CreateThread,'CreateThread',6,\
        CreateProcessW,'CreateProcessW',10,\
        OpenProcess,'OpenProcess',3,\
        ExitThread,'ExitThread',1,\
        ExitProcess,'ExitProcess',1,\
        GetTickCount,'GetTickCount',0,\
        GetProcAddress,'GetProcAddress',2,\
        GetProcessHeap,'GetProcessHeap',0,\
        GetCurrentProcessId,'GetCurrentProcessId',0,\
        GetCurrentProcess,'GetCurrentProcess',0,\
        GetModuleHandleW,'GetModuleHandleW',1,\
        GetModuleHandleExW,'GetModuleHandleExW',3,\
        GetModuleFileNameW,'GetModuleFileNameW',3,\
        GetCommandLineW,'GetCommandLineW',0,\
        GetFileSizeEx,'GetFileSizeEx',2,\
        GetLastError,'GetLastError',0,\
        GetStartupInfoW,'GetStartupInfoW',1,\
        HeapAlloc,'HeapAlloc',3,\
        HeapFree,'HeapFree',3,\
        HeapCreate,'HeapCreate',3,\
        ResumeThread,'ResumeThread',1,\
        Sleep,'Sleep',1,\
        SetUnhandledExceptionFilter,'SetUnhandledExceptionFilter',1,\
        SetErrorMode,'SetErrorMode',1,\
        SetPriorityClass,'SetPriorityClass',2,\
        SetFileAttributesW,'SetFileAttributesW',2,\
        SetEndOfFile,'SetEndOfFile',1,\
        SetEvent,'SetEvent',1,\
        SetFilePointer,'SetFilePointer',4,\
        SleepEx,'SleepEx',2,\
        TerminateProcess,'TerminateProcess',2,\
        QueryDosDeviceW,'QueryDosDeviceW',3,\
        QueryPerformanceCounter,'QueryPerformanceCounter',1,\
        QueryPerformanceFrequency,'QueryPerformanceFrequency',1,\
        QueryFullProcessImageNameW,'QueryFullProcessImageNameW',4,\
        ReleaseSRWLockShared,'ReleaseSRWLockShared',1,\
        ReleaseSRWLockExclusive,'ReleaseSRWLockExclusive',1,\
        ReadFile,'ReadFile',5,\
        VirtualFree,'VirtualFree',3,\
        VirtualAlloc,'VirtualAlloc',4,\
        WaitForDebugEvent,'WaitForDebugEvent',2,\
        WaitForSingleObject,'WaitForSingleObject',2,\
        WaitForSingleObjectEx,'WaitForSingleObjectEx',3,\
        WriteFile,'WriteFile',5

 import User32,\
        AppendMenuW,'AppendMenuW',4,\
        BeginPaint,'BeginPaint',2,\
        CreatePopupMenu,'CreatePopupMenu',0,\
        CreateDialogIndirectParamW,'CreateDialogIndirectParamW',5,\
        CreateWindowExW,'CreateWindowExW',12,\
        CallNextHookEx,'CallNextHookEx',4,\
        ClientToScreen,'ClientToScreen',2,\
        CheckDlgButton,'CheckDlgButton',3,\
        DestroyMenu,'DestroyMenu',1,\
        DestroyIcon,'DestroyIcon',1,\
        DispatchMessageW,'DispatchMessageW',1,\
        DialogBoxIndirectParamW,'DialogBoxIndirectParamW',5,\
        DeleteMenu,'DeleteMenu',3,\
        DestroyWindow,'DestroyWindow',1,\
        DefWindowProcW,'DefWindowProcW',4,\
        DrawFrameControl,'DrawFrameControl',4,\
        DrawEdge,'DrawEdge',4,\
        DrawFocusRect,'DrawFocusRect',2,\
        EnumWindows,'EnumWindows',2,\
        EnumDisplayMonitors,'EnumDisplayMonitors',4,\
        EndDialog,'EndDialog',2,\
        EndPaint,'EndPaint',2,\
        keybd_event,'keybd_event',4,\
        KillTimer,'KillTimer',2,\
        InvertRect,'InvertRect',2,\
        IntersectRect,'IntersectRect',3,\
        IsDlgButtonChecked,'IsDlgButtonChecked',2,\
        IsZoomed,'IsZoomed',1,\
        IsWindowVisible,'IsWindowVisible',1,\
        FrameRect,'FrameRect',3,\
        FillRect,'FillRect',3,\
        FindWindowExW,'FindWindowExW',4,\
        LoadIconW,'LoadIconW',2,\
        LoadImageW,'LoadImageW',6,\
        LoadMenuIndirectW,'LoadMenuIndirectW',1,\
        GetMenuItemInfoW,'GetMenuItemInfoW',4,\
        GetMenuItemID,'GetMenuItemID',2,\
        GetDesktopWindow,'GetDesktopWindow',0,\
        GetMenuContextHelpId,'GetMenuContextHelpId',1,\
        GetWindowModuleFileNameW,'GetWindowModuleFileNameW',3,\
        GetMessageExtraInfo,'GetMessageExtraInfo',0,\
        GetCapture,'GetCapture',0,\
        GetKeyNameTextW,'GetKeyNameTextW',3,\
        GetCursorPos,'GetCursorPos',1,\
        GetSysColor,'GetSysColor',1,\
        GetSubMenu,'GetSubMenu',2,\
        GetDlgItem,'GetDlgItem',2,\
        GetDC,'GetDC',1,\
        GetWindowThreadProcessId,'GetWindowThreadProcessId',2,\
        GetForegroundWindow,'GetForegroundWindow',0,\
        GetFocus,'GetFocus',0,\
        GetTopWindow,'GetTopWindow',1,\
        GetClassLongW,'GetClassLongW',2,\
        GetWindowLongW,'GetWindowLongW',2,\
        GetWindow,'GetWindow',2,\
        GetWindowRect,'GetWindowRect',2,\
        GetShellWindow,'GetShellWindow',0,\
        GetUpdateRgn,'GetUpdateRgn',3,\
        GetUpdateRect,'GetUpdateRect',3,\
        GetClientRect,'GetClientRect',2,\
        GetMessageW,'GetMessageW',4,\
        GetWindowPlacement,'GetWindowPlacement',2,\
        PeekMessageW,'PeekMessageW',5,\
        PhysicalToLogicalPoint,'PhysicalToLogicalPoint',2,\
        PostMessageW,'PostMessageW',4,\
        PostThreadMessageW,'PostThreadMessageW',4,\
        PostQuitMessage,'PostQuitMessage',1,\
        mouse_event,'mouse_event',5,\
        MapVirtualKeyExW,'MapVirtualKeyExW',3,\
        MessageBoxExW,'MessageBoxExW',5,\
        MonitorFromWindow,'MonitorFromWindow',2,\
        SetClassLongW,'SetClassLongW',3,\
        SetTimer,'SetTimer',4,\
        SendInput,'SendInput',3,\
        SendDlgItemMessageW,'SendDlgItemMessageW',5,\
        SendMessageW,'SendMessageW',4,\
        SetForegroundWindow,'SetForegroundWindow',1,\
        SetMenuItemInfoW,'SetMenuItemInfoW',4,\
        SetMessageExtraInfo,'SetMessageExtraInfo',1,\
        SetWindowsHookExW,'SetWindowsHookExW',4,\
        SetWindowPos,'SetWindowPos',7,\
        SetWindowLongW,'SetWindowLongW',3,\
        SetWindowPlacement,'SetWindowPlacement',2,\
        ShowWindow,'ShowWindow',2,\
        ScreenToClient,'ScreenToClient',2,\
        ValidateRgn,'ValidateRgn',2,\
        UnionRect,'UnionRect',3,\
        UnhookWindowsHookEx,'UnhookWindowsHookEx',1,\
        UpdateWindow,'UpdateWindow',1,\
        TrackPopupMenuEx,'TrackPopupMenuEx',6,\
        ReleaseDC,'ReleaseDC',2,\
        RegisterWindowMessageW,'RegisterWindowMessageW',1,\
        RegisterClassExW,'RegisterClassExW',1,\
        MapVirtualKeyW,'MapVirtualKeyW',2,\
        WindowFromPoint,'WindowFromPoint',2,\;sizeof.POINT/4
        WindowFromPhysicalPoint,'WindowFromPhysicalPoint',2;sizeof.POINT/4

 import Shell32,\
        ShellExecuteExW,'ShellExecuteExW',1,\
        Shell_NotifyIconW,'Shell_NotifyIconW',2,\
        SHGetKnownFolderPath,'SHGetKnownFolderPath',4