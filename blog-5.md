## Debugging
Debugging WMI providers isn't quite as easy or intuitive as I would hope it was.  For one thing, any crash that may happen in the provider simply logs to the windows event log because it is running under the wmiprsrv.exe service which is in session 0.  There is no desktop in session 0, so you can't interact with it directly on the desktop.  Instead, you must do some magic to have the application immediately run under a debugger that exposes a port that you can connect to.  I followed https://imperfect.work/2022/06/16/debug-session-0-apps/ which I'll summarize here:

* Add a key to HKLM\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options named  WmiPrvSE.exe
* Under that key add a value named "Debuger" and set to `C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\ntsd.exe -server npipe:pipe=dbg%d -noio -g -G` Be sure to correct for your path to ntsd.exe.

What does this do exactly?  I think the resource above summarizes best
> This means whenever Windows wishes to launch “notepad.exe” it will run above command and add “notepad.exe” at the end, effectively start a debug session. The parameters are:
> * Start a debugger server with a named pipe with the specified name, and the NTSD process ID is the suffix so it is possible to debug multiple instances.
> * No input or output.
> * Ignore the initial breakpoint when the process is started.
> * Ignore the final breakpoint at process termination.

Once you run your provider, you'll have two instances of wmiprsvc running
```
PS C:\Users\cplus\source\repos\WmiProviderExample\WmiProviderExample> tasklist | findstr /i ntsd
ntsd.exe                      9608 Services                   0     20,888 K
ntsd.exe                     10128 Services                   0     20,836 K
```