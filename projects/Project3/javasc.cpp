#include "pch.h"
#include "CJsValueRef.h"
#include "javasc.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

#pragma comment (lib, "chakrart") // win10, win11 OS support version

using namespace V6;

JsRuntimeHandle runtime;
JsContextRef gcontext;

#define IfFailRet
#define IfFailError( func,msg)

JsErrorCode CreateHostContext(JsRuntimeHandle runtime, int argc, wchar_t *argv [], int argumentsStart, JsContextRef *context);
JsValueRef CALLBACK selectFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState);
JsErrorCode DefineHostCallback(JsValueRef globalObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState);

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
		JsSetCurrentContext(JS_INVALID_REFERENCE);
	

		JsDisposeRuntime(runtime);
		runtime = nullptr;
	}
}


static unsigned currentSourceContext = 0;

void JsRun(LPCWSTR script )
{

	JsSetCurrentContext(gcontext); 

	JsValueRef result;
	JsErrorCode errorCode = JsRunScript(script, currentSourceContext++, L"", &result);

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
	JsGetPropertyIdFromName(L"host", &hostPropertyId); // –¼‘O‚Ì“o˜^

	JsSetProperty(globalObject, hostPropertyId, hostObject, true); // globalObject, hostPropertyId, hostObject ‚Ì‚R‚Â‚ð•R‚Ã‚¯


//	IfFailRet(DefineHostCallback(hostObject, L"echo", echo, nullptr)); // hostObject.echo
//	IfFailRet(DefineHostCallback(hostObject, L"api", apiFunc, nullptr));


	IfFailRet(DefineHostCallback(globalObject, L"select", selectFunc, nullptr));
//	IfFailRet(DefineHostCallback(globalObject, L"set", setFunc, nullptr));


	JsValueRef arguments;
	JsCreateArray(argc - argumentsStart, &arguments);

	for (int index = argumentsStart; index < argc; index++)
	{
		//
		// Create the argument value.
		//

		JsValueRef argument;
		IfFailRet(JsPointerToString(argv[index], wcslen(argv[index]), &argument));

		//
		// Create the index.
		//

		JsValueRef indexValue;
		IfFailRet(JsIntToNumber(index - argumentsStart, &indexValue));

		//
		// Set the value.
		//

		IfFailRet(JsSetIndexedProperty(arguments, indexValue, argument));
	}

	JsPropertyIdRef argumentsPropertyId;
	JsGetPropertyIdFromName(L"arguments", &argumentsPropertyId);

	JsSetProperty(hostObject, argumentsPropertyId, arguments, true);


	JsSetCurrentContext(JS_INVALID_REFERENCE);

	return JsNoError;
}

JsErrorCode DefineHostCallback(JsValueRef globalObject, const wchar_t *callbackName, JsNativeFunction callback, void *callbackState)
{
	JsPropertyIdRef propertyId;
	IfFailRet(JsGetPropertyIdFromName(callbackName, &propertyId));

	JsValueRef function;
	IfFailRet(JsCreateFunction(callback, callbackState, &function));

	IfFailRet(JsSetProperty(globalObject, propertyId, function, true));

	return JsNoError;
}



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


JsValueRef CALLBACK selectFunc(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, _In_opt_ void *callbackState)
{
	std::wstring select_item;
	
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		CJsValueRef v(arguments[index]);
		select_item = v.ToString();
		break;
	}

	auto uh = D2DGetControlFromName(hwin, select_item.c_str());

	select_obj.h = uh;
	
	// objì¬
	UIHandleRap* rap = new UIHandleRap(uh);

	JsValueRef ret;
	auto er = JsCreateExternalObject( rap, UIHandleRap::Del, &ret);
	
	// obj‚Éset function‚ðŽÀ‘•
	IfFailRet(DefineHostCallback(ret, L"set", setFunc, nullptr));


	return ret;
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