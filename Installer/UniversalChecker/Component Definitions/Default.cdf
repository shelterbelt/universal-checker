[Integrated Utility Files]
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=Installign the integrated scan utilty...
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component includes the integrated scan utilty program files.
DISPLAYTEXT=Integrated Utilties
IMAGE=
DEFSELECTION=Yes
filegroup0=Integrated Utiltiy Files
requiredby0=Program Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Components]
component0=Help Files
component1=Integrated Utility Files
component2=Program Files
component3=External Utilty Files
component4=Shared DLLs

[Help Files]
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=Installing help files...
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component include the help files for UniversalChecker.
DISPLAYTEXT=Help Files
IMAGE=
DEFSELECTION=Yes
filegroup0=Help Files
requiredby0=Program Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=ALWAYSOVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[TopComponents]
component0=Program Files
component1=Help Files
component2=External Utilty Files
component3=Integrated Utility Files
component4=Shared DLLs

[Program Files]
required0=Help Files
SELECTED=Yes
FILENEED=CRITICAL
required1=External Utilty Files
HTTPLOCATION=
STATUS=Installing the UniversalChecker application...
required2=Integrated Utility Files
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
required3=Shared DLLs
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component includes the UniveralChecker application program files.
DISPLAYTEXT=Program Files
IMAGE=
DEFSELECTION=Yes
filegroup0=Control Files
COMMENT=
INCLUDEINBUILD=Yes
filegroup1=Data Files
INSTALLATION=ALWAYSOVERWRITE
filegroup2=Integrated Utiltiy Files
filegroup3=Program Executable Files
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Shared DLLs]
SELECTED=Yes
FILENEED=CRITICAL
HTTPLOCATION=
STATUS=Installing Microsoft foundation clases...
UNINSTALLABLE=Yes
TARGET=<WINSYSDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component contains the Microsoft Foundation Classes package which contains system elements required by UniversalChecker.
DISPLAYTEXT=Microsoft Foundation Classes
IMAGE=
DEFSELECTION=Yes
filegroup0=Shared DLLs
requiredby0=Program Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=NEVEROVERWRITE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=Windows Operating System\Windows System Folder

[External Utilty Files]
SELECTED=Yes
FILENEED=STANDARD
HTTPLOCATION=
STATUS=Installing scan utilities...
UNINSTALLABLE=Yes
TARGET=<TARGETDIR>
FTPLOCATION=
VISIBLE=Yes
DESCRIPTION=This component includes non-integrated scan utility program files.
DISPLAYTEXT=Scan Utilities
IMAGE=
DEFSELECTION=Yes
filegroup0=External Utility Files
requiredby0=Program Files
COMMENT=
INCLUDEINBUILD=Yes
INSTALLATION=NEWERVERSION\NEWERDATE
COMPRESSIFSEPARATE=No
MISC=
ENCRYPT=No
DISK=ANYDISK
TARGETDIRCDROM=
PASSWORD=
TARGETHIDDEN=General Application Destination

[Info]
Type=CompDef
Version=1.00.000
Name=

