#pragma once

typedef std::wstring wstring;

namespace V6 
{
class D2DCaptureObject;

class D2DApp
{
	public :
		D2DApp();
		static void SetD2DAppForDLL(D2DApp* p);
		static D2DApp& GetInstance();
	public :
		void SetCapture(D2DCaptureObject* target);
		D2DCaptureObject* ReleaseCapture();
		bool IsCapture(D2DCaptureObject* target);
		D2DCaptureObject* GetCapture();
		D2DCaptureObject* GetCapture2();
		int IsCaptureEx(D2DCaptureObject* target);
		int Count() { return (int)capture_.size(); }

		//D2DCaptureObject* Pop2();
		//void Push2(D2DCaptureObject* target);
		
	protected :
		std::stack<D2DCaptureObject*> capture_;
		D2DCaptureObject* modal_dialog_;

		//std::stack<D2DCaptureObject*> modal_dialog_;
		static D2DApp* globalapp_;
		
		
};


};