
DetailPrint "Checking if wintap is installed..."

!ifndef ClassGUID
!define ClassGUID "{4d36e972-e325-11ce-bfc1-08002be10318}"
!endif
  
push $0
push $1
push $2
push $4

${GetParameters} $1

${If} $1 =~ "/NOWINTAP"
  DetailPrint "Wintap installation overridden - skipping installation..."
  GoTo end
${EndIf}

;checking if wintap is installed
#IntOp $0 $0 & 0
ClearErrors
/*next:
  IntFmt $1 "%04d" $0
  ReadRegStr $2 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "NetCfgInstanceId"
  ReadRegStr $4 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "DriverDesc"
  StrCmp "TAP-Win32 Adapter V8" $4 found
  IntOp $0 $0 + 1
*/

StrCpy $0 0
StrCpy $7 0 
/*loop:
  
  StrCmp $1 "" notfound
  ReadRegStr $2 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "NetCfgInstanceId"
  ReadRegStr $4 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "DriverDesc"
  StrCmp "TAP-Win32 Adapter V8" $4 found
  StrCpy $6 $1
  IntOp $6 $6 + 1 
  IntOp $0 $0 + 1
  GoTo loop
done:*/

EnumRegKey $1 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}" $0
IntOp $6 $1 + 1 
${While} $1 != ""
  ReadRegStr $2 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "NetCfgInstanceId"
  ReadRegStr $4 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$1" "DriverDesc"
  StrCmp "TAP-Win32 Adapter V8" $4 found
  IntOp $0 $0 + 1
  EnumRegKey $1 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}" $0
  IntFmt $7 "%04d" $6
  ${If} "$7" == "$1" 
      IntOp $6 $6 + 1 
  ${EndIf}
${EndWhile}
GoTo notfound
;an error means that we trie to browse through not existing keys 
;- search was finished, wintap is not there
#IfErrors notfound next

;if found skipping installation
found:
  DetailPrint "Wintap found - skipping installation..."
  GoTo end
  
;if not found we install it
notfound:
  DetailPrint "Wintap not found - starting installation. Please wait."

  ;a variable to contain the next entry that will be created by the driver installer
  #IntOp $0 $6
  IntFmt $0 "%04d" $6
  
  ;launch the tapinstall (devcon) or do something to install the driver
  SetOutPath "$INSTDIR\driver" ;working directory must be tap installer directory
  nsExec::ExecToLog "$INSTDIR\driver\tapinstaller"
  
  ;rename network connection to TAP-Win32
  ReadRegStr $3 HKLM "SYSTEM\CurrentControlSet\Control\Class\${ClassGUID}\$0" "NetCfgInstanceId"
  WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Network\${ClassGUID}\$3\Connection" "Name" "TAP-Win32"

  ;message to wait, as the following step takes a stupid amount of time
  ;and it seems that the PC is doing nothing
  DetailPrint "Configuring TAP-Win32 Adapter..."
  DetailPrint "The process will take a while, please wait..."

  ;set TAP-Win32 ip's using netsh
  nsExec::ExecToLog "netsh interface ip set address TAP-Win32 static 192.168.0.1 255.255.255.0"
  
  DetailPrint "Wintap installation completed..."
  
end:
  pop $4
  pop $2
  pop $1
  pop $0
  DetailPrint "========================================="
