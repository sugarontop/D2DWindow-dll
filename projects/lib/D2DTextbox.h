#pragma once

#include "TSF/TextContainer.h"
#include "TSF/TextEditor.h"
#include "D2DApp.h"
#include "D2DWindowControl.h"


struct AppBase;


#define CARET_W	2

namespace V6
{
	class ScrollbarParent
	{
		public :
			ScrollbarParent(){}
			
			virtual float sc_barTotalHeight(bool bHbar)=0;
			virtual float sc_dataHeight(bool bHbar)=0;
	};

	class Scrollbar
	{
		public :
			Scrollbar();

			void Create(ScrollbarParent* parent, FRectF rc );
			void OnDraw(D2DContext& cxt);
			virtual LRESULT WndProc(AppBase& hWnd, UINT msg, WPARAM wp, LPARAM lp) ;
			void SetTotalHeight( float h );
			float Scroll();
			 float SetScroll(float offset);
			float Width() const { return rc_.Width(); }
			float Height() const { return rc_.Height(); }
			void Clear();
		private :
			FRectF rc_;
			ScrollbarParent* parent_;
			float total_height_;
			float offbar_y_;
			float scbarThumbHeight_;
			float scbai_;
			D2DMat mat_;
			bool bvscrollbar_;

		public :
			int pos_;
	};


	class D2DTextbox: 
		public IBridgeTSFInterface, 
		public ScrollbarParent,
		public D2DControl
	{
	public :
		D2DTextbox();
		virtual ~D2DTextbox();

		
		void SetText(LPCWSTR str, int len, int insert_pos=0 );
		void GetText( std::wstringstream* out,  bool crlf = true );
		bool GetSelectText(std::wstringstream* out, bool crlf = true);
		void Clear();
		void SetReadonly(bool bReadonly);
		bool SetFont(LPCWSTR fontnm, float height);

		virtual bool OnChangeFocus(bool bActive, D2DCaptureObject* pnew) ;
		bool IsMultiline() const;
		int CurrentPos() const;

		void SetBackColor(ColorF clr){ back_=clr; }
		void SetForeColor(ColorF clr){ fore_=clr; }
		void SetBorderColor(ColorF clr){ border_=clr; }
		virtual int GetTypeid() const override { return TYP_TEXTBOX; }
	public :
		void CreateControl(D2DWindow* parent, D2DControls* pacontrol, TYP typ, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
		virtual void Draw(D2DContext& cxt) override;
		virtual FRectF GetRect() const override { return rctext_; }
		virtual void SetRect(const FRectF& rc)  override { rctext_ = rc; }
		virtual std::wstring GetTreeTyp(USHORT* typ) override;
	public :
		static void* CreateInputControl(D2DWindow* parent);
		static void DestroyInputControl(){};
		void StatActive(bool bActive);


		static std::wstring Ascii2W( LPCSTR s );
		static std::string W2Ascii( LPCWSTR s );

	protected :
		void ActiveSw(bool bActive);
		static std::wstring ConvertInputText(LPCWSTR text, int typ);
		
		void AutoScroll();
		FRectF GetVsrollbarRect() const;
		TSF::CTextEditorCtrl* ctrl() const;

		void SetMultilineText(LPCWSTR str, int len, int insert_pos );
		void SetSinglelineText(LPCWSTR str, int len, int insert_pos );

	public:
		// D2DCaptureObject interface
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	public :
		// IBridgeTSFInterface
		virtual FRectF GetClientRect()  const override;
		virtual IDWriteTextFormat* GetFormat()  const override { return fmt_; }
		virtual TYP GetType()  const override { return typ_; }
	protected :
		// ScrollbarParent interface
		virtual float sc_barTotalHeight(bool bHbar) override;
		virtual float sc_dataHeight(bool bHbar) override;

	protected :		
		FRectF rctext_;
		IBridgeTSFInterface::TYP typ_;		
		ComPTR<IDWriteTextFormat> fmt_;
		ComPTR<IDWriteTextLayout> text_layout_;		
		TSF::CTextContainer ct_;
		D2DMat mat_, mat_sc_;
		Scrollbar vscrollbar_;
		DWRITE_TEXT_METRICS tm_;
		ColorF back_;
		ColorF fore_;
		ColorF border_;
		

	};
};
