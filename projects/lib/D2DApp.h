#pragma once

typedef std::wstring wstring;

namespace V6 
{
class D2DCaptureObject;
class D2DApp
{
	protected :
		D2DApp();
	public :
		virtual void SetCapture(D2DCaptureObject* target);
		virtual void ReleaseCapture();
		bool IsCapture(D2DCaptureObject* target);
		D2DCaptureObject* GetCapture();

		static D2DApp& GetInstance();

	protected :
		std::vector<D2DCaptureObject*> capture_;
};

};