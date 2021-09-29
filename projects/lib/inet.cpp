#include "pch.h"
#include <msxml6.h>
#pragma comment( lib, "msxml6")

#include "inet.h"

class CBstr
{
	public :
		CBstr():bs(nullptr){}
		CBstr(LPCWSTR s){ bs = ::SysAllocString(s); }
		CBstr(int a)
		{
			WCHAR val[64];		
			wsprintf(val, L"%d", a);
			bs = ::SysAllocString(val);
		}
		~CBstr(){ clear(); }

		CBstr& operator =(LPCWSTR s)
		{
			clear();
			bs = ::SysAllocString(s);
			return *this;
		}
		void clear()
		{
			if ( bs )	::SysFreeString(bs); 
			bs = nullptr;
		}

		BSTR* operator &() { return &bs; }
		operator BSTR() const { return bs; }
		operator int() const { return _wtoi(bs); }
	private :
		BSTR bs;
};

DWORD WINAPI InetAsync( LPVOID p )
{
	InternetInfo* cn = (InternetInfo*)p;
	_ASSERT(cn);

	auto hr = CoInitialize(0);
	if ( hr!=S_OK )
	{
		cn->errormsg = ::SysAllocString(L"CoInitialize fail");
		throw (long)1;
	}

	cn->throwerror = 0;
	cn->result = -1;
	cn->content_len = 0;
	cn->pstream = nullptr;


	CLSID clsid;
	IServerXMLHTTPRequest* req = nullptr;

	try {
		hr = CLSIDFromProgID( L"Msxml2.ServerXMLHTTP.6.0", &clsid );
		if ( hr!=S_OK )
		{
			cn->errormsg = ::SysAllocString(L"not installed ServerXMLHTTP.6.0");
			throw (long)2;
		}

		hr = CoCreateInstance( clsid,NULL,CLSCTX_INPROC_SERVER,IID_IServerXMLHTTPRequest,(LPVOID*)&req );
		if ( hr!=S_OK )
		{
			cn->errormsg = ::SysAllocString(L"fail creaet IID_IServerXMLHTTPRequest");
			throw (long)3;
		}

		VARIANT v[4];
		for(int i=0; i < 4; i++) VariantInit(&v[i]);
		
		v[0].vt = VT_BOOL;
		v[0].boolVal = VARIANT_FALSE;

		
		req->setTimeouts(0,60*1000,30*1000,30*1000); // default value

		CBstr method = (cn->bGet ? L"GET" : L"POST" );

		if ( S_OK != req->open(method, cn->url, v[0],v[1],v[2]))
			throw (long)4;

		// through  all server cert error 

		VARIANT kv; 	::VariantInit(&kv);
		kv.vt=VT_I4;
		kv.intVal=SXH_SERVER_CERT_IGNORE_ALL_SERVER_ERRORS ;
		req->setOption(SERVERXMLHTTP_OPTION::SXH_OPTION_IGNORE_SERVER_SSL_CERT_ERROR_FLAGS, kv);

		if ( cn->bGet )
		{
			// GET 
		}
		else 
		{
			// POST
			CBstr x1(L"application/x-www-form-urlencoded");
			CBstr x2(SysStringLen(cn->postdata));

			req->setRequestHeader( CBstr(L"Content-Type"), x1);
			req->setRequestHeader( CBstr(L"Content-Length"), x2);
			v[3].bstrVal =  cn->postdata; v[3].vt = VT_BSTR;
		}

		if ( S_OK !=req->send(v[3]) )
			throw (long)5;

		long st = 0;
		int k = 0;
		while(1)
		{
			req->get_readyState(&st) ;
			if ( st == READYSTATE_COMPLETE ) // 4
			{
				req->get_status(&cn->result);
				break;
			}
		}
	}
	catch( long err )
	{
		cn->throwerror = err;
	}

	if ( cn->throwerror == 0 && cn->result == 200 )
	{
		CBstr clen;
		auto hr = req->getResponseHeader(CBstr(L"Content-Length"), &clen);		
		if ( hr == S_OK )
			cn->content_len = (int)clen;

		hr = req->getResponseHeader(CBstr(L"Content-Type"), &cn->content_type);

		VARIANT vsm;
		::VariantInit(&vsm);
		cn->throwerror = 1000;

		if ( S_OK == req->get_responseStream( &vsm ))
		{						
			hr = V_UNKNOWN(&vsm)->QueryInterface( IID_IStream, (void**)&cn->pstream );
			if ( hr == S_OK )
				cn->throwerror = 0;

			::VariantClear(&vsm);
		}
	}

	if ( req )
		req->Release();

	CoUninitialize();

	return 0;
}

