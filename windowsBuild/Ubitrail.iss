[Setup]
DefaultDirName={pf}\Ubitrail
OutputDir=.\
MinVersion=5.0
AppName=Ubitrail
AppVerName=1.2
LanguageDetectionMethod=none
AppID={{7807E304-090C-4F99-8371-1993E17C7568}
AppCopyright=Quentin Geissmann
ChangesAssociations=true
SetupIconFile=ubitrail.ico
PrivilegesRequired=poweruser
ShowLanguageDialog=no
DefaultGroupName=Ubitrail
UsePreviousGroup=true
LicenseFile=COPYING.txt

[Files]
Source: bin\*; DestDir: {app}\bin
Source: lib\gtk-2.0\2.10.0\engines\*; DestDir: {app}\lib\gtk-2.0\2.10.0\engines
Source: etc\pango\*; DestDir: {app}\etc\pango
Source: etc\gtk-2.0\*; DestDir: {app}\etc\gtk-2.0
Source: share\themes\MS-Windows\gtk-2.0\*; DestDir: {app}\share\themes\MS-Windows\gtk-2.0
Source: Ubitrail.ico; DestDir: {app}
[Icons]
Name: {group}\Ubitrail; Filename: {app}\bin\Ubitrail.exe; IconFilename: {app}\ubitrail.ico
Name: {group}\Uninstall Ubitrail; Filename: {app}\unins000.exe
Name: {group}\Website; Filename: http://ubitrail.sourceforge.net/
