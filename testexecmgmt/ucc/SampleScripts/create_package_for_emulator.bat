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

echo -------------------------------------------------------------------------------
echo .
echo Copying all configuration and script files to required locations (wins/winscw).
echo .
echo -------------------------------------------------------------------------------

mkdir ..\..\..\wins\c\NE
mkdir ..\..\..\winscw\c\NE

copy ..\ConfigurationFiles\dcomm.ini ..\..\..\data\

copy ..\ConfigurationFiles\cdb.emulator*.cfg ..\..\..\wins\c\NE
copy .\sti.0003.ini ..\..\..\wins\c\NE
copy .\sti.0005.ini ..\..\..\wins\c\NE
copy .\sti.0007.ini ..\..\..\wins\c\NE
copy .\sts.0003.script ..\..\..\wins\c\NE
copy .\sts.0005.script ..\..\..\wins\c\NE
copy .\sts.0007.script ..\..\..\wins\c\NE

copy ..\ConfigurationFiles\cdb.emulator*.cfg ..\..\..\winscw\c\NE
copy .\sti.0003.ini ..\..\..\winscw\c\NE
copy .\sti.0005.ini ..\..\..\winscw\c\NE
copy .\sti.0007.ini ..\..\..\winscw\c\NE
copy .\sts.0003.script ..\..\..\winscw\c\NE
copy .\sts.0005.script ..\..\..\winscw\c\NE
copy .\sts.0007.script ..\..\..\winscw\c\NE
