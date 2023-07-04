# WMI

In my career, I've always heard of people using WMI, so I wanted to see what all the hoopla was about.  Up until now, all I had ever seen was the occansional wmic command.  What I didn't realize was how rich the information is and that you can run wmi commands to "do" things like start processes.


<css>
pre code {
    overflow-x: scroll;
    overflow-y: hidden;
    display: block;
    line-height: 1.6em;
font-size: 11px;
}
</css>

## Basic Wmi Commands
The best way I know to introduce WMI is to just show some commands and their output:

`PS C:\Program Files (x86)\Windows Kits\10\Debuggers\x64> WMIC.exe CPU
AddressWidth  Architecture  AssetTag                Availability  Caption                                Characteristics  ConfigManagerErrorCode  ConfigManagerUserConfig  CpuStatus  CreationClassName  CurrentClockSpeed  CurrentVoltage  DataWidth  Description                            DeviceID  ErrorCleared  ErrorDescription  ExtClock  Family  InstallDate  L2CacheSize  L2CacheSpeed  L3CacheSize  L3CacheSpeed  LastErrorCode  Level  LoadPercentage  Manufacturer  MaxClockSpeed  Name                                            NumberOfCores  NumberOfEnabledCore  NumberOfLogicalProcessors  OtherFamilyDescription  PartNumber              PNPDeviceID  PowerManagementCapabilities  PowerManagementSupported  ProcessorId       ProcessorType  Revision  Role  SecondLevelAddressTranslationExtensions  SerialNumber            SocketDesignation  Status  StatusInfo  Stepping  SystemCreationClassName  SystemName       ThreadCount  UniqueId  UpgradeMethod  Version  VirtualizationFirmwareEnabled  VMMonitorModeExtensions  VoltageCaps
64            9             To Be Filled By O.E.M.  3             Intel64 Family 6 Model 140 Stepping 1  252                                                               1          Win32_Processor    2995               8
64         Intel64 Family 6 Model 140 Stepping 1  CPU0                                      100       198                  5120                       12288        0                            6      3               GenuineIntel  2995           11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz  4              4                    8                                                  To Be Filled By O.E.M.                                            FALSE                     BFEBFBFF000806C1  3                        CPU   TRUE                                     To Be Filled By O.E.M.  U3E1               OK      3                     Win32_ComputerSystem     DESKTOP-JPETSOM  8                      1                       TRUE                           TRUE`