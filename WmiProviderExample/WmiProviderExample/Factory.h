#pragma once
#include <Windows.h>

// This class is the class factory for CInstPro objects.

class CProvFactory : public IClassFactory
{
protected:
    ULONG m_cRef;

public:
    CProvFactory(void);
    ~CProvFactory(void);

    //IUnknown members
    STDMETHODIMP QueryInterface(REFIID, PVOID*);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //IClassFactory members
    STDMETHODIMP CreateInstance(LPUNKNOWN, REFIID, PVOID*);
    STDMETHODIMP LockServer(BOOL);
};
