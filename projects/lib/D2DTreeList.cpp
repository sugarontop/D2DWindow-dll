#include "pch.h"
#include "D2DTreeList.h"

using namespace V6;

#define ITEM_HEIGHT 35

std::unordered_map<int, std::vector<spTreeListItem>> TreeListItemBuf::sbuf_;

TreeListItemBuf::TreeListItemBuf()
{ 
	
}
int TreeListItemBuf::NewBuffer()
{ 	
	int idx = 1 + (int)sbuf_.size();

	std::vector<spTreeListItem> ar;
	sbuf_[idx] = ar;
	return idx;
}
std::vector<spTreeListItem>& TreeListItemBuf::GetBuffer( int idx )
{ 
	auto it = sbuf_.find(idx);
	_ASSERT( it != sbuf_.end() ) ;

	return it->second;
}
void TreeListItemBuf::ReleaaseBuffer( int idx )
{ 
	auto it = sbuf_.find(idx);
	if ( it == sbuf_.end() ) return;
	it->second.clear();
	sbuf_.erase(it);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

TreeListItem::TreeListItem()
{
	isexpand_ = false;
	buf_index_ = 0;
}
TreeListItem::TreeListItem(const std::wstring& s)
{
	target_ = s;
	buf_index_ = 0;
	isexpand_ = false;
}
void TreeListItem::Add( bool expand, spTreeListItem child )
{
	if ( buf_index_ == 0 )
		buf_index_ = TreeListItemBuf::NewBuffer();
	std::vector<spTreeListItem>& ar = TreeListItemBuf::GetBuffer(buf_index_); 

	ar.push_back(child);

	child->sz_ = FSizeF(1000,ITEM_HEIGHT);
	isexpand_ = expand;
}
void TreeListItem::Draw(D2DContext& cxt)
{
	bool bl =  ( isexpand_ && buf_index_ != 0  );
	FRectF rc(0,0,1000,30);
	D2DMatrix mat(*cxt);
	mat_ =mat;

	auto s = target_;
	if (  !isexpand_ && buf_index_ != 0 )
	{
		s = L"+" + target_;
	}

	(*cxt)->DrawText( s.c_str(), s.length(), cxt.textformat_, rc, cxt.black_ );	

	if ( bl )
	{
		std::vector<spTreeListItem>& ar = TreeListItemBuf::GetBuffer(buf_index_); 
		
		mat.Offset( 50, ITEM_HEIGHT );
		for(auto& it : ar )
		{
			it.get()->Draw(cxt);
			mat.Offset( 0, ITEM_HEIGHT );
		}
		mat.Offset( -50, -ITEM_HEIGHT );		
	}
}
TreeListItem::~TreeListItem()
{
	Clear();
}
void TreeListItem::Clear()
{
	if ( buf_index_ != 0 )
		TreeListItemBuf::ReleaaseBuffer(buf_index_);
	buf_index_ = 0;
}

//////////////////////////////////////////////////////////////
TreeList::TreeList()
{ 
	buf_index_ = TreeListItemBuf::NewBuffer();
	offbar_y_ = 0;
}
TreeList::~TreeList()
{
	Clear();
}
void TreeList::Clear()
{
	if ( buf_index_ != 0 )
		TreeListItemBuf::ReleaaseBuffer(buf_index_);
	buf_index_ = 0;
}
void TreeList::Add( spTreeListItem item )
{ 
	std::vector<spTreeListItem>& ar = TreeListItemBuf::GetBuffer(buf_index_); 
	ar.push_back(item);

	item->sz_ = FSizeF(1000,ITEM_HEIGHT);
}


void TreeList::Draw(D2DContext& cxt)
{ 
	D2DMatrix mat(*cxt);
	mat_ = mat;


	mat.PushTransform();

	cxt.DFillRect( rc_, D2RGB(220,220,220));

	mat.Offset(rc_.left, rc_.top);	

	// scrollbarÇ™îÚÇ—èoÇ∑èÍçáÇÕÅAoffbar_y_ÇïœçXÇ∑ÇÈ
	{
		mat.PushTransform();
		if ( sc_dataHeight())
		{
			mat.Offset(0, offbar_y_);	
			float overflow = max(0.0f, sc_dataHeight()-sc_barTotalHeight());
			scbarThumbHeight_ = sc_barTotalHeight()-overflow;
			FRectF scbar( rc_.Size().width-20, 0, rc_.Size().width,  scbarThumbHeight_);
//scbar = mat.LPtoDP(scbar);
			if ( rc_.bottom < scbar.bottom )
			{
				offbar_y_ -= (scbar.bottom-rc_.bottom);
			}
			else if ( rc_.top > scbar.top )
			{
				offbar_y_ = 0;
			}
		}
		mat.PopTransform();
	}

	mat.PushTransform();

	mat.Offset(0, -offbar_y_ * scbai_);	

	FPointF scroll_pt1(0,0);
	FPointF scroll_pt2(0,0);
	scroll_pt1 = mat.LPtoDP(scroll_pt1);			

	{
		mat.PushTransform();
			std::vector<spTreeListItem>& ar = TreeListItemBuf::GetBuffer(buf_index_); 

			for(auto& it : ar)
			{		
				it.get()->Draw(cxt);
				mat.Offset(0, ITEM_HEIGHT);
			}			
			scroll_pt2 = mat.LPtoDP(scroll_pt2);			
		mat.PopTransform();
	}
	scdata_ = scroll_pt2.y - scroll_pt1.y;

	/////////////////////
	mat.PopTransform();



	mat.PushTransform();
		mat.Offset(0.0f, offbar_y_);	
		float overflow = max(0, sc_dataHeight()-sc_barTotalHeight());
		scbarThumbHeight_ = sc_barTotalHeight()-overflow;
		const float min_thum_height=8.0f;
		scbai_ = 1.0f; 
		if ( scbarThumbHeight_ < min_thum_height  )
		{
			scbarThumbHeight_ = min_thum_height;
			scbai_ = (sc_dataHeight() - sc_barTotalHeight()) / (sc_barTotalHeight()-scbarThumbHeight_); //());			
		}

		FRectF scbar( rc_.Size().width-20, 0, rc_.Size().width,  scbarThumbHeight_);
		cxt.DFillRect(scbar, D2GRAY);

	mat.PopTransform();
	mat.PopTransform();

	cxt.DFillRect(FRectF(-5,-5,5,5), D2GRAY);

}
float TreeList::sc_barThumbHeight()
{	
	return scbarThumbHeight_;
}
float TreeList::sc_barTotalHeight()
{	
	return rc_.Height();
}
float TreeList::sc_dataHeight()
{
	return scdata_;
}

void TreeList::UpdateScrollbar()
{


}



static FPointF ptold;
bool TreeList::MouseMove(FPointF& pt)
{
	offbar_y_ = max( 0, offbar_y_+ (pt.y - ptold.y));	

	offbar_y_ = min (sc_barTotalHeight()-sc_barThumbHeight() , offbar_y_);

	ptold = pt;

	return false;
}



void TreeList::Create( FRectF rc )
{
	rc_ = rc;
	scbarThumbHeight_ = sc_barTotalHeight();
	scbai_ = 1.0f;
}


bool TreeList::LButtonDown(FPointF& pt)
{
	auto rc1 = mat_.LPtoDP(rc_);


	if (rc1.PtInRect(pt) && rc1.right-30 < pt.x )
	{		
		ptold = pt;
		return true;
	}
	else if (rc1.PtInRect(pt) && pt.x < rc1.left+100 )
	{		
		std::vector<spTreeListItem>& ar = TreeListItemBuf::GetBuffer(buf_index_); 

		for(auto& it : ar)
		{		
			FRectF rc(FPointF(0,0), it.get()->sz_);

			D2DMat mat = it.get()->mat_;

			auto pt2  = mat.DPtoLP(pt);

			if ( rc.PtInRect(pt2) )
			{
				if (it->buf_index_ != 0 )
				{
					bool bl = it->isexpand_;
					it->isexpand_ = !bl;
				}
			}
		}

		UpdateScrollbar();
	}

	return false;
}


bool TreeList::LButtonUp(FPointF& pt)
{

	return false;
}





std::shared_ptr<TreeListItem> gg;
bool gggg = false;

void TreeList::test()
{
	Add( std::make_shared<TreeListItem>(L"top level1 1"));
	Add(  std::make_shared<TreeListItem>(L"top level1 2"));

	auto a1 =  std::make_shared<TreeListItem>(L"top level1 3");
	a1->Add( true,  std::make_shared<TreeListItem>(L"child level2 1"));
	a1->Add( true, std::make_shared<TreeListItem>(L"child level2 2"));

	a1->ExpandShow(gggg);
	gg = a1;

	Add( std::shared_ptr<TreeListItem>( a1 ));
	Add(  std::make_shared<TreeListItem>(L"top level1 4"));

	for( int i =0; i <3; i++ ){
	auto a2 =  std::make_shared<TreeListItem>(L"top level1 5");
	a2->Add( true,  std::make_shared<TreeListItem>(L"child level2 1"));
	a2->Add( true, std::make_shared<TreeListItem>(L"child level2 2"));
	a2->Add( true,  std::make_shared<TreeListItem>(L"child level2 3"));
	a2->Add( true, std::make_shared<TreeListItem>(L"child level2 4"));

	Add( std::shared_ptr<TreeListItem>( a2 ));
	a2->ExpandShow(gggg);
	a1 = a2;
	}

	a1->ExpandShow(true);

	Add(  std::make_shared<TreeListItem>(L"top level1 6"));

	

}
void TreeList::test2()
{
	gggg = !gggg;
	gg->ExpandShow(gggg);
}