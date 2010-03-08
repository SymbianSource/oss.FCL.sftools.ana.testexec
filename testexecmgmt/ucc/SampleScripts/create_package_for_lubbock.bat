@rem
@rem Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
@@rem
@@rem

@echo off

echo ----------------------------------------------------------------------------------------
echo .
echo Creating a package with all the required files to run Sample4 on Typhoon/Lubbock
echo .
echo ----------------------------------------------------------------------------------------

mkdir cf_lubbock

xcopy sti.0004.ini			.\cf_lubbock
xcopy sts.0004.script			.\cf_lubbock
xcopy sti.0006.ini			.\cf_lubbock
xcopy sts.0006.script			.\cf_lubbock


copy \epoc32\release\arm4\urel\scheduletest.exe			.\cf_lubbock
copy \epoc32\release\arm4\urel\CINIDATA.DLL			.\cf_lubbock
copy \epoc32\release\arm4\urel\INTEGRATIONTESTUTILS.DLL		.\cf_lubbock
copy \epoc32\release\arm4\urel\NetworkEmulatorControl.dll	.\cf_lubbock
copy \epoc32\release\arm4\urel\NEControlST.dll			.\cf_lubbock
copy \epoc32\release\arm4\urel\pingeng.dll			.\cf_lubbock
copy \epoc32\release\arm4\urel\InetUtilsST.dll			.\cf_lubbock
copy \epoc32\release\arm4\urel\TS_SockBench.DLL			.\cf_lubbock

copy \epoc32\release\arm4\urel\ced.exe				.\cf_lubbock
copy \epoc32\release\arm4\urel\ceddump.exe			.\cf_lubbock

copy ..\ConfigurationFiles\cdb.lubbock.cfg 			.\cf_lubbock
