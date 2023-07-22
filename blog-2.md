# Modeling WMI Classes

Querying WMI is cool and all, but it is possible to extend WMI's capabilities by adding our own WMI classes and instances

## MOF file
In Wmi, data is structured in MOF files. The formating of MOF files are pretty self explanatory if you've delt with C++ classes much.  An example MOF file is below.
```
class User
{
  [key]
  // MyKey uniquely identifies this class
  String Name;   

  uint32 LogonCount;
};
```

This mof file defines a User class that is indexed by Name and has the "LogonCount" parameter.  In MOF, it's possible to define instances in the same MOF file.  So something like this
```
class User
{
  [key]
  // MyKey uniquely identifies this class
  String Name;   

  uint32 LogonCount;
};

instance of User
{
  Name="Todd";
  LogonCount=10;
};
```

We can install this mof file with a utility called "mofcomp.exe" that's included as part of the visual studio installation.  Save the snippet above as "Example.mof" and run in an elevated terminal
```
PS C:\Users\cplus\source\repos\wmi_blog\2> mofcomp.exe .\Example.mof
Microsoft (R) MOF Compiler Version 10.0.22621.1
Copyright (c) Microsoft Corp. 1997-2006. All rights reserved.
Parsing MOF file: .\Example.mof
MOF file has been successfully parsed
Storing data in the repository...
WARNING: File .\Example.mof does not contain #PRAGMA AUTORECOVER.
If the WMI repository is rebuilt in the future, the contents of this MOF file will not be included in the new WMI repository.
To include this MOF file when the WMI Repository is automatically reconstructed, place the #PRAGMA AUTORECOVER statement on the first line of the MOF file.
Done
```

There are some warnings about not having #PRAGMA AUTORECOVER, but since we are just testing, I don't care if we lose this mof file on repo rebuild.

Now we have a "User".  To find it we can use
```
PS C:\Users\cplus\source\repos\wmi_blog\2> get-cimclass -ClassName User -Namespace root/default


   NameSpace: ROOT/default

CimClassName                        CimClassMethods      CimClassProperties
------------                        ---------------      ------------------
User                                {}                   {LogonCount, Name}
```

Why did I specify "root/default" as the namespace to search?!  I told you wmi puts most things in "root/cimv2" (the default namespace for any wmi tool you use).  The reason is because when you don't specify an explicit namespace in the mof file, "root/default" is chosen for you. For the rest of this series, I will continue to use "root/default" so that you don't have to search through the hundreds of other entires in "root/cimv2".  If you want to stick your class in the "root/cimv2" namespace, you can simply add the below snippet to make all your classes in the root/cimv2 namespace.
```
#pragma namespace ("root/cimv2")
```

The command above found the class definition for "User", but how do we see the instance "Todd" that we also created in the mof file?  There is a pretty easy way to do this in powershell.
```
PS C:\Users\cplus\source\repos\wmi_blog\2> Get-CimInstance -ClassName User -Namespace root/default

LogonCount Name PSComputerName
---------- ---- --------------
        10 Todd
```

With this we have defined class and created some static instances.  The problem with static instances like this is we can't modify either the Name or LogonCount.  This method of instances might be useful for storing data that won't change during the lifetime of a boot but most interesting things involve data that is updated.  I mean we were able to launch processes via wmi in the previous post!

There are much more to MOF files, but I feel the documentation does a better job of explaining it than I could, and some of the more interesting features don't make a ton of sense until we create a provider.

To accomplish more complicated tasks via wmi, we will be required to create a "provider".  This is essentially a COM server.  Wmi requests/queries will be dispatched to the provider where it will provide data, update an instance, or execute methods!  The next post will go over what is required to build a COM server.  If you're like me, you've built apps that used COM objects, but never built a COM server!  I had to really play around with the example Microsoft provided for WMI because it does not work!!