;overwrite ucc.ini file with new locations
FileOpen $R3 "$INSTDIR\UCC\bin\ucc.ini" w
FileWrite $R3 "[SyncService]$\r$\n"
FileWrite $R3 "ini = $INSTDIR\UCC\SyncService\Interface\Host\SyncServiceInterface.ini$\r$\n"
FileWrite $R3 "host = 127.0.0.1$\r$\n$\r$\n"
FileWrite $R3 "[TestDriverService]$\r$\n"
FileWrite $R3 "ini = $INSTDIR\UCC\TestDriverService\Interface\Host\TestDriverServiceInterface.ini$\r$\n"
FileWrite $R3 "host = 127.0.0.1$\r$\n$\r$\n"
FileClose $R3 ; and close the file

FileOpen $R3 "$INSTDIR\UCC\bin\SyncService.ini" w
FileWrite $R3 "[SyncService]$\r$\n"
FileWrite $R3 "Connection = tcp$\r$\n"
FileWrite $R3 "SerialPort = $\r$\n"
FileWrite $R3 "IPAddress = 192.168.0.3:3000$\r$\n"
FileWrite $R3 "Timeout = 60$\r$\n"
FileWrite $R3 "Poll = 5$\r$\n"
FileWrite $R3 "STAT = $INSTDIR\stat\bin\stat.dll$\r$\n"
FileClose $R3 ; and close the file

FileOpen $R3 "$INSTDIR\UCC\bin\TestDriverService.ini" w
FileWrite $R3 "[TestDriverService]$\r$\n"
FileWrite $R3 "Exe = $INSTDIR\testdriver\testdriver.cmd$\r$\n"
FileWrite $R3 "Port = 3002$\r$\n"
FileWrite $R3 "Poll = 5$\r$\n"
FileClose $R3 ; and close the file

nsExec::ExecToLog "$INSTDIR\UCC\oncrpc\oncrpc.exe"
