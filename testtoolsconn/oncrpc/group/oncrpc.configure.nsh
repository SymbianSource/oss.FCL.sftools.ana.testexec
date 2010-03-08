push $0
push $1

nsSCM::QueryStatus portmap 
Pop $0 ; return error/success
Pop $1 ; return service status
  
  ;!define SERVICE_STOPPED                0x00000001
  ;!define SERVICE_START_PENDING          0x00000002
  ;!define SERVICE_STOP_PENDING           0x00000003
  ;!define SERVICE_RUNNING                0x00000004
  ;!define SERVICE_CONTINUE_PENDING       0x00000005
  ;!define SERVICE_PAUSE_PENDING          0x00000006
  ;!define SERVICE_PAUSED                 0x00000007
  
${If} $0 == "error"  
  nsExec::ExecToLog "$WINDIR\system32\portmap.exe /registerservice"
${EndIf}

pop $1
pop $0