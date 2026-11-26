set(Launcher_CommonName "NarutoLauncher")
set(Launcher_DisplayName "NarutoLauncher")
set(Launcher_AppID "org.narutolauncher.NarutoLauncher")
# URL schemes must be lowercase, while the binary name is not
string(TOLOWER "${Launcher_APP_BINARY_NAME}" Launcher_URLScheme)
set(Launcher_Domain "prismlauncher.org")
set(Launcher_Git "https://github.com/PrismLauncher/PrismLauncher")

set(Launcher_Name "${Launcher_CommonName}" PARENT_SCOPE)
set(Launcher_DisplayName "${Launcher_DisplayName}" PARENT_SCOPE)
set(Launcher_ENVName "NARUTOLAUNCHER" PARENT_SCOPE)
set(Launcher_URLScheme "${Launcher_URLScheme}" PARENT_SCOPE)
set(Launcher_Domain "${Launcher_Domain}" PARENT_SCOPE)
set(Launcher_Git "${Launcher_Git}" PARENT_SCOPE)

set(Launcher_IconFileName "${Launcher_AppID}.png")
set(Launcher_Copyright "© 2022-2026 Prism Launcher Contributors\\n© 2021-2022 PolyMC Contributors\\n© 2012-2021 MultiMC Contributors")
set(Launcher_Copyright "${Launcher_Copyright}" PARENT_SCOPE)
set(Launcher_UserAgent "${Launcher_CommonName}/${Launcher_VERSION_NAME}" PARENT_SCOPE)
set(Launcher_ConfigFile "${Launcher_APP_BINARY_NAME}.cfg" PARENT_SCOPE)
set(Launcher_AppID "${Launcher_AppID}" PARENT_SCOPE)
set(Launcher_IconFileName "${Launcher_IconFileName}" PARENT_SCOPE)

set(Launcher_Branding_ICO "program_info/${Launcher_APP_BINARY_NAME}.ico")
set(Launcher_Branding_ICO "${Launcher_Branding_ICO}" PARENT_SCOPE)
set(Launcher_Branding_WindowsRC "program_info/${Launcher_APP_BINARY_NAME}.rc" PARENT_SCOPE)
set(Launcher_Branding_LogoQRC "program_info/${Launcher_APP_BINARY_NAME}.qrc" PARENT_SCOPE)
set(Launcher_Authors "MultiMC & Prism Launcher Contributors")

set(Launcher_Portable_File "program_info/portable.txt" PARENT_SCOPE)

configure_file(${Launcher_APP_BINARY_NAME}.rc.in ${Launcher_APP_BINARY_NAME}.rc @ONLY)
configure_file(${Launcher_APP_BINARY_NAME}.qrc.in ${Launcher_APP_BINARY_NAME}.qrc @ONLY)
configure_file(${Launcher_APP_BINARY_NAME}.manifest.in ${Launcher_APP_BINARY_NAME}.manifest @ONLY)
configure_file(${Launcher_APP_BINARY_NAME}.ico ${Launcher_APP_BINARY_NAME}.ico COPYONLY)
configure_file(${Launcher_IconFileName} ${Launcher_IconFileName} COPYONLY)

if(MSVC)
    set(Launcher_MSVC_Redist_NSIS_Section [=[
!ifdef haveNScurl
Section "Visual Studio Runtime"
    Var /GLOBAL vc_redist_exe
    ${If} ${IsNativeARM64}
        StrCpy $vc_redist_exe "vc_redist.arm64.exe"
    ${Else}
        StrCpy $vc_redist_exe "vc_redist.x64.exe"
    ${EndIf}
    DetailPrint 'Downloading Microsoft Visual C++ Redistributable...'
    NScurl::http GET "https://aka.ms/vs/17/release/$vc_redist_exe" "$INSTDIR\vc_redist\$vc_redist_exe" /INSIST /CANCEL /Zone.Identifier /END
    Pop $0
    ${If} $0 == "OK"
        DetailPrint "Download successful"
        ExecWait "$INSTDIR\vc_redist\$vc_redist_exe /install /passive /norestart"
    ${Else}
        DetailPrint "Download failed with error $0"
    ${EndIf}
SectionEnd
!endif
]=])
endif()

configure_file(win_install.nsi.in win_install.nsi @ONLY)
