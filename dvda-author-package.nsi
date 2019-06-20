;___________________________________________________________________________________________

;NSIS installer script for dvda-author Copyright Fabrice Nicol 2008 <fabnicol@users.sourceforge.net>
;see COPYING for lincense. This script is part of the dvda-author package

;_____________________________________________________________________________________________


; Compile from within this directory 
; Modern interface settings

!include "MUI2.nsh"
!include "x64.nsh"

!define version  "19.06"
!define prodname "dvda-author-package"
!define mainexe  "dvda-author"
!define guiexe   "dvda-author-gui"
!define setup    "${prodname}-${version}.win32.installer.exe"
!define srcdir   "."
!define website  "http://dvd-audio.sourceforge.net"

!define images   "${srcdir}\images\nsis"

!define binary   "${guiexe}.exe"
!define icon     "${guiexe}.ico"
!define regkey   "Software\${prodname}-${version}"
!define uninstkey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}-${version}"
!define startmenu   "$SMPROGRAMS\${prodname}-${version}"
!define uninstaller "uninstall.exe"
!define notefile    "${srcdir}\README"

; put MUI_ICON before all pages

!define MUI_ICON "${srcdir}\${guiexe}.ico"

; welcome pages here for some odd buggy reason (version 2.39)

!define MUI_WELCOMEFINISHPAGE
!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH
!define MUI_WELCOMEFINISHPAGE_BITMAP  "${srcdir}\images\nsis\welcome.bmp"
!define MUI_WELCOMEPAGE_TEXT  $(wizard1)
!define MUI_WELCOMEPAGE_TITLE $(wizard2)

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${srcdir}\images\nsis\headerLeft.bmp" ; optional
!define MUI_ABORTWARNING


!insertmacro MUI_PAGE_WELCOME  ; must appear BEFORE MUI_LANGUAGE for some odd reason.
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French" ; must appear BEFORE language strings


; Language strings

 LangString  wizard1 ${LANG_ENGLISH} "This wizard will guide you through the installation of ${prodname} version ${version}. Click next to continue."
 LangString  wizard1 ${LANG_FRENCH}  "Installation de ${prodname} version ${version}. Appuyer sur suivant pour continuer."
 LangString  wizard2 ${LANG_ENGLISH} "${prodname} installation"
 LangString  wizard2 ${LANG_FRENCH}  "Installation de ${prodname}"
 LangString title1 ${LANG_ENGLISH}  "Installation completed"
 LangString title1 ${LANG_FRENCH}   "Installation terminée"
 LangString title1 ${LANG_ENGLISH} "Installation completed"
 LangString title1 ${LANG_FRENCH}  "Installation terminée"
 LangString text1 ${LANG_ENGLISH}  "${prodname} ${version} sucessfully installed to $INSTDIR"
 LangString text1 ${LANG_FRENCH}  "${prodname} ${version} a été installé dans $INSTDIR"
 LangString text2 ${LANG_ENGLISH} "Show README file"
 LangString text2 ${LANG_FRENCH}  "Afficher le fichier README"
 LangString link1 ${LANG_ENGLISH} "Browse DVD audio tools webpage"
 LangString link1 ${LANG_FRENCH} "Aller au site internet de DVD audio Tools"
 LangString uninstall ${LANG_ENGLISH}    "${prodname} uninstall"
 LangString uninstall ${LANG_FRENCH}     "désinstallation de ${prodname} "
 LangString completed ${LANG_ENGLISH}    "Installation completed"
 LangString completed ${LANG_FRENCH}    "Installation terminée"

 
 LangString Sec2Name ${LANG_ENGLISH} "${guiexe}"
 LangString Sec3Name ${LANG_ENGLISH} "${mainexe}"
 LangString Sec4Name ${LANG_ENGLISH} "source code"
 
 LangString Message  ${LANG_ENGLISH} "Click on Yes to install ${prodname}"

 
 LangString Sec2Name ${LANG_FRENCH} "interface graphique"
 LangString Sec3Name ${LANG_FRENCH} "dvda-author"
 LangString Sec4Name ${LANG_FRENCH} "code source"
 LangString Message  ${LANG_FRENCH} "Appuyer sur Oui pour installer ${prodname}"

 
 LangString DESC_sec2 ${LANG_ENGLISH} "Install GUI"
 LangString DESC_sec3 ${LANG_ENGLISH} "Install binary"
 LangString DESC_sec4 ${LANG_ENGLISH} "Install source code"


 LangString DESC_sec2 ${LANG_ENGLISH} "Installer l'interface"
 LangString DESC_sec3 ${LANG_ENGLISH} "Installer l'exécutable principal"
 LangString DESC_sec4 ${LANG_FRENCH} "Installer le code source"



 LicenseLangString myLicenseData ${LANG_ENGLISH} "${srcdir}\COPYING"
 LicenseLangString myLicenseData ${LANG_FRENCH} "${srcdir}\COPYING"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_ENGLISH} "${prodname} English version"
 LangString Name ${LANG_FRENCH}  "${prodname} version française"
 Name $(Name)

; MUI macros   


!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_BUTTON  "OK"
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_TEXT $(text2)
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_${prodname}"
!define MUI_FINISHPAGE_LINK $(link1)
!define MUI_FINISHPAGE_LINK_LOCATION "http://dvd-audio.sourceforge.net"



!insertmacro MUI_PAGE_LICENSE "${srcdir}\COPYING"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
;!insertmacro MUI_PAGE_FINISH
;!insertmacro MUI_UNPAGE_CONFIRM
;!insertmacro MUI_UNPAGE_INSTFILES
UninstallText  $(uninstall)
CompletedText  $(completed)

;XPStyle on
ShowInstDetails show
ShowUninstDetails show
RequestExecutionLevel user

Caption "${prodname}"

OutFile "${setup}"

SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal

InstallDir "C:\Users\Public\${prodname}-${version}"
InstallDirRegKey HKLM "${regkey}" ""


AutoCloseWindow false
ShowInstDetails show


;--------------------------------
; Functions and subs
;--------------------------------


Function Launch_${prodname}
  Exec '"notepad" "$INSTDIR\README" '
FunctionEnd

Function .onInit
     !define MUI_LANGDLL_WINDOWTITLE "Installation language/Langue d'installation"
     !define MUI_LANGDLL_INFO "Select language/Sélectionner la langue"
     !define MUI_LANGDLL_ALWAYSSHOW

  SetOutPath $TEMP
  File /oname=spltmp.bmp "${images}\spltmp.bmp"

  advsplash::show 1300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\spltmp.bmp

;Calling format
;       advsplash::show Delay FadeIn FadeOut KeyColor FileName

  !insertmacro MUI_LANGDLL_DISPLAY


;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Modern UI Test" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

FunctionEnd




Section
MessageBox MB_YESNO|MB_ICONINFORMATION $(Message)  IDNO Fin IDYES End
Fin: Abort
End:

CreateDirectory "${startmenu}"

SectionEnd




Section  ; allways done
   SetOutPath $INSTDIR
  !ifdef icon
    CreateShortCut "${startmenu}\${prodname}.lnk" "$INSTDIR\${binary}" "" "$INSTDIR\${icon}"
  !else
    CreateShortCut "${startmenu}\${prodname}.lnk" "$INSTDIR\${binary}"
  !endif

  WriteRegStr HKCR "${prodname}\Shell\open\command\" "" '"$INSTDIR\${binary} "%1"'

  !ifdef icon
    WriteRegStr HKCR "${prodname}\DefaultIcon" "" "$INSTDIR\${icon}"
  !endif

  CreateShortCut "${startmenu}\${uninstaller}.lnk" "$INSTDIR\${uninstaller}"

  !ifdef website
  WriteINIStr "${startmenu}\${prodname} website.url" "InternetShortcut" "URL" ${website}
  !endif
  WriteRegStr HKLM "${regkey}" "Install_Dir" "$INSTDIR"
  ; write uninstall strings
  WriteRegStr HKLM "${uninstkey}" "DisplayName" "${prodname} (uninstall only)"
  WriteRegStr HKLM "${uninstkey}" "UninstallString" '"$INSTDIR\${uninstaller}"'
  SetOutPath $INSTDIR

  WriteUninstaller "${uninstaller}"

  SetOutPath $INSTDIR ; for working directory
  File "${srcdir}\README"
  File "${srcdir}\COPYING"
  File "${srcdir}\ChangeLog" 
  File "${srcdir}\GUI.shtml" 
  File /r  "${srcdir}\images"  
  File /r  "${srcdir}\bin"  
  File /r  "${srcdir}\data"  
 
SectionEnd


Section  $(Sec2Name) sec2 ; binary

  SetOutPath $INSTDIR ; for working directory
  File  "${srcdir}\${guiexe}.exe"  "${srcdir}\lplex.exe"  "${srcdir}\*.dll"  "${srcdir}\*.conf"  "${srcdir}\*.ico"  

SectionEnd

Section  $(Sec3Name) sec3 ; binary

  SetOutPath $INSTDIR ; for working directory
  File  "${srcdir}\${mainexe}.exe"   "${srcdir}\*.conf"    

SectionEnd

Section $(Sec4Name) sec4 ; source code

  SetOutPath $INSTDIR ; for working directory
  
  File  "${srcdir}\*.pro*" "${srcdir}\Makefile.Release"  "${srcdir}\*.nsi"  "${srcdir}\src\*.qrc" 

SectionEnd


; to be placed after definition of sec1, ..., sec4 hence here.
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${sec2} $(DESC_sec2)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec3} $(DESC_sec3)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec4} $(DESC_sec4)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"

  DeleteRegKey HKLM "${uninstkey}"
  DeleteRegKey HKLM "${regkey}"

  Delete "${startmenu}\*.*"
  Delete "${startmenu}"

  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR"


SectionEnd

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

BrandingText "${prodname}.${version}"

; eof
