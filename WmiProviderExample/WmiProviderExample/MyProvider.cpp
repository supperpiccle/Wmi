
#define _WIN32_WINNT 0x0500

#include <Windows.h>
#include <initguid.h>
#include "MyProvider.h"


#include <objbase.h>
#include <process.h>
#include <strsafe.h>
#include <sddl.h>

#include "Utils.h"

typedef struct {
   const WCHAR * pwcKey;
   long lValue;
   } InstDef;

InstDef MyDefs[] = {{L"MyValue", 1}};

int glNumInst = 1;

STDMETHODIMP MyProvider::QueryInterface(REFIID riid, PVOID* ppv)
{
    *ppv=NULL;

    // Because you have dual inheritance,
    // it is necessary to cast the return type

    if(riid== IID_IWbemServices)
       *ppv=(IWbemServices*)this;

    if(riid== IID_IWbemPropertyProvider)
       *ppv=(IWbemPropertyProvider*)this;

    if(riid == IID_IUnknown || riid== IID_IWbemProviderInit)
       *ppv=(IWbemProviderInit*)this;
    

    if (NULL!=*ppv) 
    {
        AddRef();
        return NOERROR;
    }
    else
        return E_NOINTERFACE;
  
}


STDMETHODIMP_(ULONG) MyProvider::AddRef(void)
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) MyProvider::Release(void)
{
    ULONG nNewCount = InterlockedDecrement((long *)&m_cRef);
    if (0L == nNewCount)
        delete this;
    
    return nNewCount;
}

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

HRESULT STDMETHODCALLTYPE MyProvider::GetProperty(
    /* [in] */ long lFlags,
    /* [in] */ __RPC__in const BSTR strLocale,
    /* [in] */ __RPC__in const BSTR strClassMapping,
    /* [in] */ __RPC__in const BSTR strInstMapping,
    /* [in] */ __RPC__in const BSTR strPropMapping,
    /* [out] */ __RPC__out VARIANT* pvValue)
{
    pvValue->vt = VT_UI4;
    pvValue->uintVal = 101;
    return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE MyProvider::PutProperty(
    /* [in] */ long lFlags,
    /* [in] */ __RPC__in const BSTR strLocale,
    /* [in] */ __RPC__in const BSTR strClassMapping,
    /* [in] */ __RPC__in const BSTR strInstMapping,
    /* [in] */ __RPC__in const BSTR strPropMapping,
    /* [in] */ __RPC__in const VARIANT* pvValue)
{
    return WBEM_S_FALSE;
}

SCODE MyProvider::CreateInstanceEnumAsync( const BSTR RefStr,
                                         long lFlags,
                                         IWbemContext *pCtx,
                                         IWbemObjectSink FAR* pHandler)
{
    //
    //Impersonate the client
    HRESULT hr = CoImpersonateClient () ;

    if ( FAILED ( hr ) )
    {
        pHandler->SetStatus ( 0 , hr , NULL , NULL ) ;
        return hr ;
    }

    // Check to see if call is at lower than 
    // RPC_C_IMP_LEVEL_IMPERSONATE level. If that is the case,
    // the provider will not be able to impersonate
    // the client to access the protected resources. 

    DWORD t_CurrentImpersonationLevel = 
        GetCurrentImpersonationLevel () ;
    if ( t_CurrentImpersonationLevel < RPC_C_IMP_LEVEL_IMPERSONATE )
    {
        // Revert before you perform any operations 
        CoRevertToSelf () ;

        hr = WBEM_E_ACCESS_DENIED;
        pHandler->SetStatus ( 0 , hr , NULL , NULL ) ;
        return hr ;
    }

    SCODE sc;
    IWbemClassObject FAR* pNewInst;
  
    // Do a check of arguments and ensure
    // you have a pointer to Namespace

    if(pHandler == NULL || m_pNamespace == NULL)
        return WBEM_E_INVALID_PARAMETER;

    sc = CreateInst(m_pNamespace, (LPWSTR)(MyDefs[0].pwcKey), MyDefs[0].lValue, &pNewInst, RefStr, pCtx);
 
    if (sc != S_OK)
    {

    }
    else
    {
        // Send the object to the caller

        pHandler->Indicate(1,&pNewInst);
        pNewInst->Release();
    }


    // Set status

    pHandler->SetStatus(0,sc,NULL, NULL);

    return sc;
}

SCODE MyProvider::GetByPath(BSTR ObjectPath,
                          IWbemClassObject FAR* FAR* ppObj,
                          IWbemContext  *pCtx)
{
    SCODE sc = S_OK;
    
    int iCnt;

    // Do a simple path parse.  The path looks like
    // InstProvSamp.MyKey="a"
    // Create a test string with just the part between quotes.

    WCHAR wcTest[MAX_PATH+1];
    memset(wcTest, NULL, sizeof(wcTest));
    StringCbCopyW(wcTest, sizeof(wcTest), ObjectPath);

    WCHAR * pwcTest, * pwcCompare = NULL;
    int iNumQuotes = 0;
    for(pwcTest = wcTest; *pwcTest; pwcTest++)
        if(*pwcTest == L'\"')
        {
            iNumQuotes++;
            if(iNumQuotes == 1)
            {
                pwcCompare = pwcTest+1;
            }
            else if(iNumQuotes == 2)
            {
                *pwcTest = NULL;
                break;
            }
        }
        else if(*pwcTest == L'.')
            *pwcTest = NULL;    // isolate the class name.
    if(iNumQuotes != 2)
        return WBEM_E_FAILED;

    // check the instance list for a match.

    for(iCnt = 0; iCnt < glNumInst; iCnt++)
    {
        if(!_wcsicmp(MyDefs[iCnt].pwcKey, pwcCompare))
        {
            sc = CreateInst(m_pNamespace,(LPWSTR)MyDefs[iCnt].pwcKey, MyDefs[iCnt].lValue, ppObj, wcTest, pCtx);
            return sc;
        }
    }

    return WBEM_E_NOT_FOUND;
}

SCODE MyProvider::GetObjectAsync(const BSTR      ObjectPath,
                               long            lFlags,
                               IWbemContext    *pCtx,
                               IWbemObjectSink FAR* pHandler)
{
    //Impersonate the client
    HRESULT hr = CoImpersonateClient () ;

    if ( FAILED ( hr ) )
    {
        pHandler->SetStatus ( 0 , hr , NULL , NULL ) ;
        return hr ;
    } 

    // Check to see if call is at the 
    // RPC_C_IMP_LEVEL_IDENTIFY level. If that is the case,
    // the provider will not be able to impersonate
    // the client to access the protected resources.
    if (GetCurrentImpersonationLevel () == RPC_C_IMP_LEVEL_IDENTIFY)
    {
        hr = WBEM_E_ACCESS_DENIED;
        pHandler->SetStatus ( 0 , hr , NULL , NULL ) ;
        return hr ;
    }

    SCODE sc;
    IWbemClassObject FAR* pObj;
    BOOL bOK = FALSE;

    // Do a check of arguments and ensure
    // you have a pointer to Namespace

    if(ObjectPath == NULL || 
        pHandler == NULL || 
        m_pNamespace == NULL)
        return WBEM_E_INVALID_PARAMETER;

    // do the get, pass the object on to the notify
    
    sc = GetByPath(ObjectPath,&pObj, pCtx);
    if(sc == S_OK) 
    {
        pHandler->Indicate(1,&pObj);
        pObj->Release();
        bOK = TRUE;
    }

    sc = (bOK) ? S_OK : WBEM_E_NOT_FOUND;

    // Set Status

    pHandler->SetStatus(0,sc, NULL, NULL);

    return sc;
}
