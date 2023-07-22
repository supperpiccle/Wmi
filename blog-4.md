## Implementing a WMI Provider
We have all the prereq knowledge to build a WMI provider.  We have a basic understanding of how to write WMI classes in MOF, and how to write a COM class.  Now we can write a WMI provider.  In the next post, I'll go over how to attach a debugger to your provider because it's not really obvious how to do that.

In this post, we will write an extremely simple WMI provider.  Also this code is pretty garbage tbh.  I mostly just fixed the code that MS gave.

## Mof File
For our WMI provider, we will use the following MOF file
```
Instance of __Win32Provider as $P   // $P is an alias
{
  // Name that describes your provider
  Name        = "InstProvSamp" ;   
  ClsId   = "{b0757c01-0675-4011-9c96-ab1461c357f0}" ;
  HostingModel = "LocalSystemHost:InstProvSamp";
} ;  

instance of __InstanceProviderRegistration
{
  Provider = $P;                // Alias to the __Win32Provider
  SupportsPut = FALSE;          // Does not support the Put method
  SupportsGet = TRUE;           // Supports the Get method
  SupportsDelete = FALSE;       // Does not support the Delete method
  SupportsEnumeration = TRUE;   // Supports enumeration.
};

instance of __PropertyProviderRegistration
{
  Provider = $P;
  SupportsGet = TRUE;
  SupportsPut = FALSE;
};

[dynamic, provider("InstProvSamp")] // uses the InstProvSamp Provider
                                    // information is dynamically
                                    // supported by the provider
class InstProvSamp
{
  [key]
  // MyKey uniquely identifies this class
  String MyKey="HELLO";   

  // InstProvSamp dynamically updates MyValue
  [PropertyContext("Name")] 
  uint32 MyValue;
};
```

There's a lot I didn't talk about in this MOF file, so I'll break it down.
```
Instance of __Win32Provider as $P   // $P is an alias
{
  // Name that describes your provider
  Name        = "InstProvSamp" ;   
  ClsId   = "{b0757c01-0675-4011-9c96-ab1461c357f0}" ;
  HostingModel = "LocalSystemHost:InstProvSamp";
} ;  
```
`__Win32Provider` is a CIM class that Microsoft exports.  If you intend for your WMI class to be a provider you must define an instance of this class.  "Name" here can be whatever you want, but `ClsId` must be the clsid of your COM class.  Please use a different value than me! :).  "HostingModel" here determines where your COM provider runs.  This option means "Run the com server in a wmiprvse.exe instance that is running as LocalSystem".  The reason I chose this option is because 
* no other wmi providers use this option because of security concerns which means when we break into a debugger we won't mess with any other wmi providers
* LocalSystem will prevent a ton of "access denied" errors you may get. This is just for playing around so we don't have to concern ourselves with too much security.  Don't realease a wmi provider under this hosting model unless you absolutely have to.

The `instance of __InstanceProviderRegistration` informs wmi that our WMI provider "provides" instances:
```
instance of __InstanceProviderRegistration
{
  Provider = $P;                // Alias to the __Win32Provider
  SupportsPut = FALSE;          // Does not support the Put method
  SupportsGet = TRUE;           // Supports the Get method
  SupportsDelete = FALSE;       // Does not support the Delete method
  SupportsEnumeration = TRUE;   // Supports enumeration.
};
```
This tells WMI that your provider allows querying for instances (SupportsGet + SupportsEnumeration).  When a request comes to get instances of `InstProvSamp`, our COM server will be invoked to create/retrive those instances.


The `__PropertyProviderRegistration` tells WMI that our provider also provides the property values for each instance that it creates
```
instance of __PropertyProviderRegistration
{
  Provider = $P;
  SupportsGet = TRUE;
  SupportsPut = FALSE;
};
```
This just says that wmi can query the "MyValue" and "MyKey" properties, but you cannot set it.  The COM provider is completely responsible for the value.


Quick sidebar: During my experimentation with WMI, I wondered if it would be possible to have a `__PropretyProviderRegistration` but _not_ a `__InstanceproviderRegistration`.  If you know you'll only have 1 instance, I imagined it would be just as easy to declare the instance in the mof file.  This would simplify the code quite a bit because then we'd only have to write to code to provide property values. However, it seems that have to declare a `__InstanceProviderRegistration` to have any other kind of provider type (property or method).

Finally, we can define the class itself.
```
[dynamic, provider("InstProvSamp")] // uses the InstProvSamp Provider
                                    // information is dynamically
                                    // supported by the provider
class InstProvSamp
{
  [key]
  // MyKey uniquely identifies this class
  String MyKey="HELLO";   

  // InstProvSamp dynamically updates MyValue
  [PropertyContext("Name")] 
  uint32 MyValue;
};
```

The top piece `[dynamic, provider("InstProvSamp")]` is required if you are using a provider.  From the msdn page about property provider registration:
> Be sure to tag the class with both the Dynamic and Provider qualifiers. The Dynamic qualifier signals that WMI should use a dynamic provider to retrieve the class instances that contain the supported properties. The Provider qualifier specifies the name of the provider that WMI should use.

Sidebar #2: This verbage makes me think it's not possible to have a property provider and _not_ an instance provider.  No big deal imo.

## C++ code.
I won't list the C++ code in it's entirety but will comment on some of the interesting pieces.

Provider.h
```
#include <wbemprov.h>
#pragma comment(lib, "wbemuuid.lib")

class MyProvider : public IWbemProviderInit, public IWbemPropertyProvider, public IWbemServices

```
Gotta include wbemprov.h that declares a most of the WMI interfaces and you'll need to link against "wbemuuid.lib" to satisfy the linker for the dll's we will link with at runtime.  You can see from `MyProvider` here that we inherit from a number of differnt interfaces:
* IWbemProviderInit - Required for all providers.  Used to inform WMI of successful initialization during startup.
* IWbemPropertyProvider - Inherited because we have said that our provider provides property values (say that 3 times fast!)
* IWbemServices - This provides the instances and classes.  In our mof file we only declared our class as an instance provider, so the class creation functions won't be called.  you can see in our class we stub most of the functions from this interface.


The default Initialize method that the docs give:
```
HRESULT STDMETHODCALLTYPE MyProvider::Initialize(
	/* [string][unique][in] */ __RPC__in_opt_string LPWSTR wszUser,
	/* [in] */ LONG lFlags,
	/* [string][in] */ __RPC__in_string LPWSTR wszNamespace,
	/* [string][unique][in] */ __RPC__in_opt_string LPWSTR wszLocale,
	/* [in] */ __RPC__in_opt IWbemServices* pNamespace,
	/* [in] */ __RPC__in_opt IWbemContext* pCtx,
	/* [in] */ __RPC__in_opt IWbemProviderInitSink* pInitSink)
{
    if(! pNamespace)
    {
        pInitSink->SetStatus(WBEM_E_FAILED , 0);
    }
    else
    {
        m_pNamespace = pNamespace;
        m_pNamespace->AddRef();
        pInitSink->SetStatus(WBEM_S_INITIALIZED, 0);
    }

    
    return WBEM_S_NO_ERROR;
}
```
I don't know the circumstances that cause `pNamespace` to be `nullptr`, but looks like it can be.  I find it really weird here that you must call `pInitSink->SetStatus` _and_ return the appropriate status code, but this is Microsoft.


The functions below cover enumeration and "get".  The difference is that one returns a list of instances for this class, and the other looks for a specific instance by name.  The implementation is pretty self explanatory.
```
SCODE MyProvider::CreateInstanceEnumAsync( const BSTR RefStr,
                                         long lFlags,
                                         IWbemContext *pCtx,
                                         IWbemObjectSink FAR* pHandler)
...
...
...

SCODE MyProvider::GetObjectAsync(const BSTR      ObjectPath,
                               long            lFlags,
                               IWbemContext    *pCtx,
                               IWbemObjectSink FAR* pHandler)

```

And finally, you've got the code necessary to to load our provider.
```
PS C:\Users\cplus\source\repos\WmiProviderExample\WmiProviderExample> mofcomp.exe .\MyProvider.mof
Microsoft (R) MOF Compiler Version 10.0.22621.1
Copyright (c) Microsoft Corp. 1997-2006. All rights reserved.
Parsing MOF file: .\MyProvider.mof
MOF file has been successfully parsed
Storing data in the repository...
WARNING: File .\MyProvider.mof does not contain #PRAGMA AUTORECOVER.
If the WMI repository is rebuilt in the future, the contents of this MOF file will not be included in the new WMI repository.
To include this MOF file when the WMI Repository is automatically reconstructed, place the #PRAGMA AUTORECOVER statement on the first line of the MOF file.
Done!
PS C:\Users\cplus\source\repos\WmiProviderExample\WmiProviderExample> regsvr32.exe ..\x64\Debug\WmiProviderExample.dll
PS C:\Users\cplus\source\repos\WmiProviderExample\WmiProviderExample> Get-CimInstance -Namespace root/cimv2 InstProvSamp


MyKey   MyValue PSComputerName
-----   ------- --------------
MyValue       1
```