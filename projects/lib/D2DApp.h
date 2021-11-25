#pragma once

typedef std::wstring wstring;

namespace V6 
{
class D2DCaptureObject;


#define CAPTURE_TEST

#ifdef CAPTURE_TEST

class D2DApp
{
	public :
		D2DApp();
		static void SetD2DAppForDLL(D2DApp* p);
	public :
		void SetCapture(D2DCaptureObject* target);
		D2DCaptureObject* ReleaseCapture();
		bool IsCapture(D2DCaptureObject* target);
		D2DCaptureObject* GetCapture();
		int IsCaptureEx(D2DCaptureObject* target);

		static D2DApp& GetInstance();

	protected :
		std::stack<D2DCaptureObject*> capture_;
		static D2DApp* globalapp_;
		
};

#else

class D2DApp
{
	protected :
		D2DApp();
	public :
		virtual void SetCapture(D2DCaptureObject* target);
		virtual void ReleaseCapture();
		bool IsCapture(D2DCaptureObject* target);
		D2DCaptureObject* GetCapture();
		int IsCaptureEx(D2DCaptureObject* target);

		static D2DApp& GetInstance();

	protected :
		std::vector<D2DCaptureObject*> capture_;

		
};

#endif

};