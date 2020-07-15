; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Linpop"
#define MyAppVersion "1.0.3"
#define MyAppPublisher "Neusoft"
#define MyAppURL "http://www.neusoft.com/"
#define MyAppExeName "Linpop.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{A32666B5-6F11-4251-A306-B26F851F463A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
; Remove the following line to run in administrative install mode (install for all users.)
PrivilegesRequired=lowest
OutputBaseFilename=LinpopSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "F:\Lioker\Desktop\Linpop-pro\Linpop.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\opengl32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\Lioker\Desktop\Linpop-pro\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "F:\Lioker\Desktop\Linpop-pro\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "F:\Lioker\Desktop\Linpop-pro\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "F:\Lioker\Desktop\Linpop-pro\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "F:\Lioker\Desktop\Linpop-pro\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

