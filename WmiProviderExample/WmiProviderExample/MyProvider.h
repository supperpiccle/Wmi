#pragma once
#include <wbemprov.h>
#pragma comment(lib, "wbemuuid.lib")

class MyProvider : public IWbemProviderInit, public IWbemPropertyProvider, public IWbemServices
{
protected:
        ULONG m_cRef;         //Object reference count
        IWbemServices* m_pNamespace;

        SCODE GetByPath(BSTR ObjectPath,
            IWbemClassObject FAR* FAR* ppObj,
            IWbemContext* pCtx);
public:
        //Non-delegating object IUnknown

        STDMETHODIMP QueryInterface(REFIID, PVOID*);
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP_(ULONG) Release(void);

    virtual HRESULT STDMETHODCALLTYPE Initialize(
        /* [string][unique][in] */ __RPC__in_opt_string LPWSTR wszUser,
        /* [in] */ LONG lFlags,
        /* [string][in] */ __RPC__in_string LPWSTR wszNamespace,
        /* [string][unique][in] */ __RPC__in_opt_string LPWSTR wszLocale,
        /* [in] */ __RPC__in_opt IWbemServices* pNamespace,
        /* [in] */ __RPC__in_opt IWbemContext* pCtx,
        /* [in] */ __RPC__in_opt IWbemProviderInitSink* pInitSink) override;

    virtual HRESULT STDMETHODCALLTYPE GetProperty(
        /* [in] */ long lFlags,
        /* [in] */ __RPC__in const BSTR strLocale,
        /* [in] */ __RPC__in const BSTR strClassMapping,
        /* [in] */ __RPC__in const BSTR strInstMapping,
        /* [in] */ __RPC__in const BSTR strPropMapping,
        /* [out] */ __RPC__out VARIANT* pvValue) override;
    
    virtual HRESULT STDMETHODCALLTYPE PutProperty(
        /* [in] */ long lFlags,
        /* [in] */ __RPC__in const BSTR strLocale,
        /* [in] */ __RPC__in const BSTR strClassMapping,
        /* [in] */ __RPC__in const BSTR strInstMapping,
        /* [in] */ __RPC__in const BSTR strPropMapping,
        /* [in] */ __RPC__in const VARIANT* pvValue) override;




     HRESULT STDMETHODCALLTYPE OpenNamespace( 
         /* [in] */ const BSTR Namespace,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemServices __RPC_FAR *__RPC_FAR *ppWorkingNamespace,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppResult) 
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE CancelAsyncCall( 
         /* [in] */ IWbemObjectSink __RPC_FAR *pSink)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE QueryObjectSink( 
         /* [in] */ long lFlags,
         /* [out] */ IWbemObjectSink __RPC_FAR *__RPC_FAR *ppResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE GetObject( 
         /* [in] */ const BSTR ObjectPath,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemClassObject __RPC_FAR *__RPC_FAR *ppObject,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppCallResult)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE GetObjectAsync( 
         /* [in] */ const BSTR ObjectPath,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler);
     
     HRESULT STDMETHODCALLTYPE PutClass( 
         /* [in] */ IWbemClassObject __RPC_FAR *pObject,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppCallResult) 
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE PutClassAsync( 
         /* [in] */ IWbemClassObject __RPC_FAR *pObject,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE DeleteClass( 
         /* [in] */ const BSTR Class,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppCallResult)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE DeleteClassAsync( 
         /* [in] */ const BSTR Class,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE CreateClassEnum( 
         /* [in] */ const BSTR Superclass,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [out] */ 
         IEnumWbemClassObject __RPC_FAR *__RPC_FAR *ppEnum)
         {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE CreateClassEnumAsync( 
         /* [in] */ const BSTR Superclass,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE PutInstance( 
         /* [in] */ IWbemClassObject __RPC_FAR *pInst,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppCallResult)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE PutInstanceAsync( 
         /* [in] */ IWbemClassObject __RPC_FAR *pInst,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE DeleteInstance( 
         /* [in] */ const BSTR ObjectPath,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [unique][in][out] */ IWbemCallResult __RPC_FAR *__RPC_FAR *ppCallResult)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE DeleteInstanceAsync( 
         /* [in] */ const BSTR ObjectPath,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE CreateInstanceEnum( 
         /* [in] */ const BSTR Class,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [out] */ IEnumWbemClassObject __RPC_FAR *__RPC_FAR *ppEnum)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE CreateInstanceEnumAsync( 
         /* [in] */ const BSTR Class,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler);
     
     HRESULT STDMETHODCALLTYPE ExecQuery( 
         /* [in] */ const BSTR QueryLanguage,
         /* [in] */ const BSTR Query,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [out] */ IEnumWbemClassObject __RPC_FAR *__RPC_FAR *ppEnum)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE ExecQueryAsync( 
         /* [in] */ const BSTR QueryLanguage,
         /* [in] */ const BSTR Query,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE ExecNotificationQuery( 
         /* [in] */ const BSTR QueryLanguage,
         /* [in] */ const BSTR Query,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [out] */ IEnumWbemClassObject __RPC_FAR *__RPC_FAR *ppEnum)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE ExecNotificationQueryAsync( 
         /* [in] */ const BSTR QueryLanguage,
         /* [in] */ const BSTR Query,
         /* [in] */ long lFlags,
         /* [in] */ IWbemContext __RPC_FAR *pCtx,
         /* [in] */ IWbemObjectSink __RPC_FAR *pResponseHandler)
     {return WBEM_E_NOT_SUPPORTED;};
     
     HRESULT STDMETHODCALLTYPE ExecMethod( const BSTR,
                                           const BSTR,
                                           long,
                                           IWbemContext*,
                                           IWbemClassObject*,
                                           IWbemClassObject**,
                                           IWbemCallResult**)
     {return WBEM_E_NOT_SUPPORTED;}

     HRESULT STDMETHODCALLTYPE ExecMethodAsync( const BSTR,
                                                const BSTR,
                                                long, 
                                                IWbemContext*,
                                                IWbemClassObject*,
                                                IWbemObjectSink*)
     {return WBEM_E_NOT_SUPPORTED;}

     private:
};

SCODE CreateInst( IWbemServices * pNamespace,
                  LPWSTR pKey, long lVal,
                  IWbemClassObject ** pNewInst,
                  WCHAR * pwcClassName,
                  IWbemContext  *pCtx); 
