#pragma once

#include "D2DContext.h"

namespace V6
{
 class TreeListItem;
 class TreeList;

 typedef std::shared_ptr<TreeListItem> spTreeListItem;

  class TreeListItem
 {
	friend class TreeList;
	public :
		TreeListItem();
		TreeListItem(const std::wstring& s);
		~TreeListItem();
		void Draw(D2DContext& cxt);
		void Clear();
		void Add(bool expand, spTreeListItem child );
		void ExpandShow(bool b=true){ isexpand_=b; }

	protected :
		std::wstring target_;				
		int buf_index_;
		bool isexpand_;
		FSizeF sz_;
		D2DMat mat_;
 };

 class TreeListItemBuf
 {
	public :
		TreeListItemBuf();
		static int NewBuffer();
		static std::vector<spTreeListItem>& GetBuffer( int idx );
		static void ReleaaseBuffer( int idx );
	protected :
		static std::unordered_map<int, std::vector<spTreeListItem>> sbuf_;
 };

 class TreeList
 {
	public :
		TreeList();
		~TreeList();

		void Add( spTreeListItem item );
		void Clear();

		void Draw(D2DContext& cxt);

		void Create( FRectF rc );
		bool LButtonDown(FPointF& pt);
		bool MouseMove(FPointF& pt);
		bool LButtonUp(FPointF& pt);


		void test();
		void test2();

		void UpdateScrollbar();
		float sc_barThumbHeight();
		float sc_barTotalHeight();
		float sc_dataHeight();
	protected :
		int buf_index_;
		D2DMat mat_;
		FRectF rc_;
		float offbar_y_;
		float scdata_;
		float scbarThumbHeight_;
		float scbai_;
		
 };


 };


