#pragma once

#define TESTTEST 50

class CToolboxControls
{

	public :
		
		CToolboxControls(){}

		struct WhiteBoard
		{
			DelegateDrawFunc drawFunc;
			DelegateProcFunc procFunc;
			int typ;
			D2DMat mat;
		};

		struct CaptureObj
		{
			FRectF rc;
			UIHandle me;
			int active_idx;
			D2DMat mat;
			WhiteBoard wboard;
			UIHandleWin hwin;
			byte show_stat; // 0:unvisible, 1:visible, 2: on moving show, 3:on moving hide

			WhiteBoard wbbox;
			FRectF rcwbbox;
			UIHandle wbboxhandle;
			FRectF acordion[TESTTEST];
			byte acordion_idx;
			DWORD time_s;
		};


		CaptureObj obj_;

		enum TYP { LEFT_SIDE, RIGHT_SIDE };

		void CreateControl(UIHandleWin hwin, UIHandle hcs, TYP typ, LPCWSTR nm );
		bool SwShow();

};
