#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {

	class D2DMDIChild;

	class D2DMDIFrame : public D2DControls
	{
		friend class D2DMDIChild;
		public:
			D2DMDIFrame();
			virtual ~D2DMDIFrame();

			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual std::wstring GetTreeTyp(USHORT* typ) override;

			std::shared_ptr<D2DMDIChild> Add(const FRectF& rc, DWORD stat, LPCWSTR name, int id);

			void SetTopMDIChild(short idx);

			D2DMDIChild* GetChild(short idx) const;
		
		protected:
			std::map<short, FRectF> prvRect_;
			short largest_idx_, active_idx_;
			UINT_PTR top_;
	};


	class D2DMDIChild : public D2DControls
	{
		friend class D2DMDIFrame;
		public:
			D2DMDIChild();
			D2DMDIChild(short idx);
			virtual ~D2DMDIChild();

			
			virtual void Draw(D2DContext& cxt) override;
			virtual int GetTypeid() const override { return TYP_CONTROLS; }
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual std::wstring GetTreeTyp(USHORT* typ) override;


			enum TitleMouse{ MOVE, DESTROY,LARGE,SMALL};
			enum MTYP { MDI_MOVABLE,MDI_FULLSIZE };


			TitleMouse GetTMD() const { return tmd_; }
			void SetTMD(TitleMouse t){ tmd_ = t; }

			short GetIdx() const { return idx_; }
		protected :
			void SetMTYP(MTYP p){ mtyp_ = p; }
		protected :
			short idx_;
			TitleMouse tmd_;
			MTYP mtyp_;


	};



};