#include "pch.h"
#include "CJsValueRef.h"
#include "javasc.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"
#include "D2DWindowControl.h"
#include "TestObject.h"

#pragma comment (lib, "chakrart") // win10, win11 OS support version

using namespace V6;

//https://github.com/chakra-core/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Reference

JsRuntimeHandle runtime;
JsContextRef gcontext;


extern UIHandleWin hwin;

struct UIHandleRap
{
	UIHandleRap(){}
	UIHandleRap(UIHandle hh):h(hh){}

	UIHandle h;

	static void CALLBACK Del(LPVOID p)
	{ 
		delete (UIHandleRap*)p; 
	}
};

struct ActiveObject
{
	UIHandle h;

};

static ActiveObject select_obj;
std::map<int, JsValueRef> objBank;

std::map<std::wstring, UIHandle> UIHandleMap;

#define IfFailRet
#define IfFailError( func,msg)

JsErrorCode CreateHostContext(JsRuntimeHandle runtime, int argc, wchar_t *argv [], int argumentsStart, JsContextRef *context);
JsErrorCode DefineHostCallback(JsValueRef globalObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState);
JsValueRef CALLBACK createFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsValueRef CALLBACK selectFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsValueRef CALLBACK setFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsValueRef CALLBACK getFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsValueRef CALLBACK moveFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsValueRef CALLBACK destroyFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
bool JavascriptAppInit()
{
	JsContextRef context;
	int argc = 1;
	WCHAR* argv[1];
	
	
	JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);
	CreateHostContext(runtime, argc,argv,1,&context);

	
	JsSetCurrentContext(context);
	
	gcontext = context;


	return true;

}
void JavascriptAppExit()
{
	if ( runtime )
	{
		
		
		JsSetCurrentContext(gcontext);
	
		JsCollectGarbage(runtime);

		JsDisposeRuntime(runtime);
		runtime = nullptr;
	}
}

void JsRegistUIHandle(std::wstring nm, UIHandle h)
{
	UIHandleMap[nm]=h;
}
UIHandle JsRegistGet(std::wstring nm)
{
	return UIHandleMap[nm];
}

void JsRun(LPCWSTR script )
{

	static unsigned currentSourceContext = 0;

	JsSetCurrentContext(gcontext); 

	JsValueRef result;
	JsErrorCode er = JsRunScript(script, currentSourceContext++, L"", &result);

	if ( JsNoError != er )
	{
		JsValueRef excp;
		JsGetAndClearException(&excp);

		//JsValueType ty;
		//JsGetValueType(excp,&ty);
	}
}
void JsCall(LPCWSTR funcnm, JsValueRef* pval, int pvalcnt)
{
	JsSetCurrentContext(gcontext); 

	JsPropertyIdRef funcid;
	auto er = JsGetPropertyIdFromName(funcnm, &funcid); 
	JsValueRef func;

	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject); 

	IfFailRet(JsGetProperty(globalObject, funcid, &func));
	
	//auto *text = L"OnInit";
	//JsValueRef val;
	//JsPointerToString(text, sizeof(text), &val);

	JsValueRef undefined;
	JsGetUndefinedValue(&undefined);

	JsValueRef args[] = { undefined  };

	JsValueRef* parg = new JsValueRef[pvalcnt+1];
	parg[0] = undefined;
	for(int i = 1; i < pvalcnt+1; i++ )
	{
		parg[i] = pval[i-1];
	}
	JsValueRef result;
	er = JsCallFunction(func, parg, pvalcnt+1, &result);

	delete [] parg;
}



JsErrorCode CreateHostContext(JsRuntimeHandle runtime, int argc, wchar_t *argv [], int argumentsStart, JsContextRef *context)
{
	
	JsCreateContext(runtime, context);
	
	JsSetCurrentContext(*context);

	
	JsValueRef hostObject;
	JsCreateObject(&hostObject);

	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject); // from context

	
	JsPropertyIdRef hostPropertyId;
	JsGetPropertyIdFromName(L"dd", &hostPropertyId); // –¼‘O‚Ì“o˜^

	JsSetProperty(globalObject, hostPropertyId, hostObject, true); // (globalObject, hostPropertyId,)-> hostObject ‚Ì‚R‚Â‚ð•R‚Ã‚¯


	IfFailRet(DefineHostCallback(hostObject, L"select", selectFunc, nullptr)); // <-- with hostObject
	IfFailRet(DefineHostCallback(hostObject, L"create", createFunc, nullptr));

//	IfFailRet(DefineHostCallback(hostObject, L"echo", echo, nullptr)); // global‚Å‚Ì
//	IfFailRet(DefineHostCallback(hostObject, L"api", apiFunc, nullptr));

//	IfFailRet(DefineHostCallback(globalObject, L"set", setFunc, nullptr));


	JsValueRef arguments;
	JsCreateArray(argc - argumentsStart, &arguments);

	for (int index = argumentsStart; index < argc; index++)
	{
		JsValueRef argument;
		IfFailRet(JsPointerToString(argv[index], wcslen(argv[index]), &argument));// Create the argument value.
		
		JsValueRef indexValue;
		IfFailRet(JsIntToNumber(index - argumentsStart, &indexValue));// Create the index.

		IfFailRet(JsSetIndexedProperty(arguments, indexValue, argument));// Set the value.
	}

	JsPropertyIdRef argumentsPropertyId;
	JsGetPropertyIdFromName(L"arguments", &argumentsPropertyId);

	JsSetProperty(hostObject, argumentsPropertyId, arguments, true);

	JsSetCurrentContext(gcontext);

	return JsNoError;
}



JsErrorCode DefineHostCallback(JsValueRef myObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	IfFailRet(JsGetPropertyIdFromName(callbackName, &propertyId));

	JsValueRef function;
	IfFailRet(JsCreateFunction(callback, callbackState, &function));

	IfFailRet(JsSetProperty(myObject, propertyId, function, true));


#ifdef _DEBUG

	JsPropertyIdRef funcid;
	auto er = JsGetPropertyIdFromName(callbackName, &funcid); 
	JsValueRef func;
    er = JsGetProperty(myObject, funcid, &func);
	JsValueType ty1;
	er =  JsGetValueType(func, &ty1);
	_ASSERT(ty1 == JsFunction);

#endif

	return JsNoError;
}


JsErrorCode ImplementsObjectCallback(JsValueRef myObject)
{
	IfFailRet(DefineHostCallback(myObject, L"set", setFunc, nullptr));
	IfFailRet(DefineHostCallback(myObject, L"get", getFunc, nullptr));
	IfFailRet(DefineHostCallback(myObject, L"move", moveFunc, nullptr));
	IfFailRet(DefineHostCallback(myObject, L"destroy", destroyFunc, nullptr));

	return JsNoError;
}





JsValueRef CALLBACK setFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	if ( 2 <= argumentCount )
	{
		LPVOID p;
		JsValueRef obj = arguments[0];
		IfFailRet(JsGetExternalData( obj, &p));
		UIHandleRap* k = (UIHandleRap*)p;

		CJsValueRef v(arguments[1]);
		auto arg = v.ToString();

		D2DSendMessage(k->h, WM_D2D_COMMAND_SET, (LPARAM)k->h.p, (LPARAM)arg.c_str());
	}
	return 0;
}
JsValueRef CALLBACK getFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	if ( 2 <= argumentCount )
	{
		LPVOID p;
		JsValueRef obj = arguments[0];
		IfFailRet(JsGetExternalData( obj, &p));
		UIHandleRap* k = (UIHandleRap*)p;

		CJsValueRef v(arguments[1]);
		auto arg = v.ToString();

		D2DSendMessage(k->h, WM_D2D_COMMAND_GET, (LPARAM)k->h.p, (LPARAM)arg.c_str());
	}
	return 0;
}



JsValueRef CALLBACK moveFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	if ( 3 <= argumentCount )
	{
		LPVOID p;
		JsValueRef obj = arguments[0];
		IfFailRet(JsGetExternalData( obj, &p));
		UIHandleRap* k = (UIHandleRap*)p;

		CJsValueRef v(arguments[1]);
		auto x = v.ToDouble();
		CJsValueRef v1(arguments[2]);
		auto y = v1.ToDouble();

		D2DControl* tt = (D2DControl*)(k->h.p);

		if ( tt )
		{
			FRectF& frc = tt->GetRectSmooth();
			FRectF dst(frc);
			dst.SetPoint(FPointF(x,y));

			D2DSmoothRect(1,100,hwin, &frc, dst);
		}		
	}
	return 0;
}

JsValueRef CALLBACK destroyFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	LPVOID p;
	JsValueRef obj = arguments[0];
	IfFailRet(JsGetExternalData( obj, &p));
	UIHandleRap* k = (UIHandleRap*)p;
	D2DControl* tt = (D2DControl*)(k->h.p);
	if ( tt )
	{
		tt->Destroy();
		k->h.p = nullptr;
		k->h.typ = TYP_NULL;
	}

	return 0;
}

JsValueRef CALLBACK selectFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	std::wstring select_item;
	
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		CJsValueRef v(arguments[index]);
		select_item = v.ToString();
		break;
	}

	//auto uh = D2DGetControlFromName(hwin, select_item.c_str());

	auto uh = UIHandleMap[select_item];


	if ( uh.p )
	{
		select_obj.h = uh;
	
		//// objì¬
		UIHandleRap* rap = new UIHandleRap(uh);

		JsValueRef ret;
		auto er = JsCreateExternalObject( rap, UIHandleRap::Del, &ret);

		// obj‚Éset function‚ðŽÀ‘•
		ImplementsObjectCallback( ret);
		return ret;
	}	

	return 0;

	
}


std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split );

bool CreateD2DObject(std::wstring& cmdstr, UIHandle* ret)
{
	auto ar = SplitW(cmdstr.c_str(),L"&");

	std::wstring type;
	bool bl = false;

	for(auto& it : ar)
	{
		auto ar2 = SplitW(it.c_str(), L"=");
		if (ar2[0]==L"type")
		{
			type = ar2[1];
			bl = true;
			break;
		}
	}

	if ( !bl ) return false;


	struct crprm
	{
		crprm():x(0),y(0),cx(200),cy(26),id(10000),nm(L"noname"){}
		float x,y;
		float cx,cy;
		UINT id;
		std::wstring nm;
	};
	
	crprm prm;

	for(auto& it : ar)
	{
		auto ar2 = SplitW(it.c_str(), L"=");
		auto key = ar2[0];
		auto val = ar2[1].c_str();
		if ( key == L"x")
			prm.x = (float)_wtof(val);
		else if ( key == L"y")
			prm.y = (float)_wtof(val);
		else if ( key == L"cx")
			prm.cx =(float)_wtof(val); 
		else if ( key == L"cy")
			prm.cy = (float)_wtof(val);
		else if ( key == L"id")
			prm.id = _wtoi(val);
		else if ( key == L"nm")
			prm.nm = ar2[1];
	}

	FRectF rc(prm.x,prm.y,FSizeF(prm.cx,prm.cy));

	if (type == L"combobox")
		*ret = D2DCreateDropdownListbox(select_obj.h, rc, STAT_DEFAULT,prm.nm.c_str(), prm.id);
	else if (type == L"textbox")
		*ret = D2DCreateTextbox(select_obj.h, rc, false, STAT_DEFAULT,prm.nm.c_str(), prm.id);
	else if (type == L"button")
		*ret = D2DCreateButton(select_obj.h, rc, STAT_DEFAULT,prm.nm.c_str(), prm.id);
	else if (type == L"listbox")
		*ret = D2DCreateListbox(select_obj.h, rc, STAT_DEFAULT,prm.nm.c_str(), prm.id);
	else if (type == L"grid")
		*ret = D2DCreateGridView(select_obj.h, rc, STAT_DEFAULT,prm.nm.c_str(), prm.id);
	else if (type == L"testobj1")
		*ret = CreateTestObj(select_obj.h, rc, prm.nm.c_str(), prm.id);
	else
		bl = false;

	if (bl)
	{
		JsRegistUIHandle(prm.nm, *ret);
	}

	return bl;

}



JsValueRef CALLBACK createFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	std::wstring select_item;
	
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		CJsValueRef v(arguments[index]);
		select_item = v.ToString();
		break;
	}

	if ( D2DIsControls(select_obj.h))
	{
		UIHandle uh;
		if ( CreateD2DObject(select_item, &uh))
		{
			
			//select_obj.h = uh;
	
			// objì¬
			UIHandleRap* rap = new UIHandleRap(uh);

			JsValueRef ret;
			auto er = JsCreateExternalObject( rap, UIHandleRap::Del, &ret);

			JsValueType ty;
			auto ty1= JsGetValueType(ret, &ty);
			_ASSERT(ty == JsObject);

			auto id = D2DGetId(uh);
			objBank[id] = ret;
	
			// obj‚Éset function‚ðŽÀ‘•

			ImplementsObjectCallback(ret);

			//IfFailRet(DefineHostCallback(ret, L"set", setFunc, nullptr));
			//IfFailRet(DefineHostCallback(ret, L"get", getFunc, nullptr));

			return ret;
		}
	}

	return 0;
}
std::wstring UrlDecode(const std::wstring& s)
{
	std::wstringstream sm;
	LPCWSTR p = s.c_str();
	while(*p != 0 )
	{
		if ( *p == '%' )
		{
			WCHAR cb[2]={*(p+1),0};
			WCHAR bb[2]={*(p+2),0};

			int ks=0;
			if ('0' <= bb[0] && bb[0] <='9' )
				ks = bb[0]-'0';
			if ('A' <= bb[0] && bb[0] <='F' )
				ks = 10+bb[0]-'A';
			else if ('a' <= bb[0] && bb[0] <='f' )
				ks = 10+bb[0]-'a';

			sm << (WCHAR)(_wtoi(cb)*16+ks);
			p+=3;
		}
		else
			sm << *(p++);
	}
	return sm.str();
}

static std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split )
{
    std::vector<std::wstring> ar;
    int splen = lstrlenW(split);
    int len = lstrlenW(str);
    _ASSERT( 0 < splen && splen <= 2  );

    int si = 0;
    for( int i = 0; i <= len; i++ )
    {
        if ( str[i] == split[0] || (i == len && 0 < len) )
        {
            if (splen == 1 || (splen == 2 && (i == len || str[i+1] == split[1] )) )
            {
                std::wstring s( &str[si], i-si );
                ar.push_back( std::wstring(s.c_str()));
                si = i + splen;
            }
        }       
    }
    return ar;
}