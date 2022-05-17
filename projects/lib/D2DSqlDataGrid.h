#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
#include "D2DScrollbar.h"
namespace V6
{
	class D2DSqlDataGrid : public D2DControls
	{
		public :
			D2DSqlDataGrid(){};
			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt);
			void Clear();

			void AddTitle(const std::wstring& title);
			void AddRow(const std::wstring& row);

			void SetData(LPCWSTR str);
			virtual int GetTypeid() const { return TYP_SQLDATAGRID; }
		protected :
			void DrawTitle(D2DContext& cxt);
			void DrawRowData(D2DContext& cxt);
			void SetViewMaxSize(FSizeF sz);
			bool IsSelectedCell(int no, int col);

		protected :
			D2DColor fore_,back_;

			int vrow_top_;

			struct ColumTitle
			{
				std::wstring title;
				float width;
			};

			struct RowData
			{
				UINT no;
				std::wstring row;
			};

			std::vector<ColumTitle> colnms_;
			std::vector<RowData> rows_;

			
			std::shared_ptr<D2DScrollbar> scv_;
			float vscroll_x_;

			struct RowCol
			{
				int r,c;
			};

			RowCol select_row_col_;


	};

};


