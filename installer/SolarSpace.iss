#define MyAppName "Solar Space"
#define MyAppVersion "1.2.0"
#define MyAppPublisher "Victor Semionov"
#define MyAppPublisherShort "vsemionov"
#define MyPublisherURL "http://www.vsemionov.org/"
#define MyAppURL "http://www.vsemionov.org/solar-space/"
#define MyAppExeName "Solar Space.exe"
#define MyAppScrName "SolSpace.scr"

[Setup]
AppId={{DEA29385-E983-4A44-81AB-F9CA69961459}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
LicenseFile=..\LICENSE.txt
InfoAfterFile=..\AUTHORS.txt
OutputDir=..\build
OutputBaseFilename={#MyAppName} {#MyAppVersion}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "setsaver"; Description: "&Set as the current screen saver"; GroupDescription: "Miscellaneous:"

[Files]
Source: "..\Release\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\{#MyAppExeName}"; DestDir: "{win}"; DestName: "{#MyAppScrName}"; Flags: ignoreversion
Source: "..\freetype6.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "..\zlib1.dll"; DestDir: "{sys}"; Flags: sharedfile
Source: "..\Solar Space.d2"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Solar System.d2"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\README.rst"; DestDir: "{app}"; DestName: "README.txt"; Flags: ignoreversion isreadme
Source: "..\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\AUTHORS.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\CHANGES.txt"; DestDir: "{app}"; Flags: ignoreversion

[Registry]
Root: HKLM; Subkey: "Software\{#MyAppPublisherShort}\{#MyAppName}"; ValueType: string; ValueName: "Data Directory"; ValueData: "{app}"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Control Panel\Desktop"; ValueType: string; ValueName: "SCRNSAVE.EXE"; ValueData: "{code:GetShortName|{win}\{#MyAppScrName}}"; Tasks: setsaver

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "-s"
Name: "{group}\Options"; Filename: "{app}\{#MyAppExeName}"; Parameters: "-c 0"; IconIndex: 1
Name: "{group}\Readme"; Filename: "{app}\README.txt"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Parameters: "-s"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, "&", "&&")}}"; Parameters: "-s"; Flags: nowait postinstall skipifsilent

