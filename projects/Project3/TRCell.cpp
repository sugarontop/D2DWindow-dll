#include "pch.h"
#include "WindowsProjectD2D.h"
#include "D2DContext.h"

////////////////////////////////////
#include "AppBase.h"
#include "D2D1UI_1.h"
#include "D2DMessage.h"

using namespace V6;
#include "trcell.h"

TRCell::TRCell()
{
	typ_ = TYP::STRING;
	max_len_ = -1;
	edit_ = false;
	rc_ = FRectF(FPointF(), FSizeF(200,26));
	val_ = L"empty";
}

/// ////////////////////////////////////////////
TRCellRow::TRCellRow()
{
	pcell_ = nullptr;
	celcnt_ = 0;
	txbox_idx_ = 0;
}
TRCellRow::TRCellRow( FRectF* rc, int celcnt )
{
	pcell_ = new TRCell[celcnt];
	
	for(int i=0; i < celcnt; i++ )
		pcell_[i].rc_ = rc[i];
	
	celcnt_ = celcnt;
	txbox_idx_ = 0;
}
TRCellRow::TRCellRow( FRectF* rc, int celcnt, int )
{
	pcell_ = new TRCell[celcnt];

	for(int i=0; i < celcnt; i++ )
	{
		pcell_[i].rc_ = rc[i];
		pcell_[i].edit_ = true;
	}

	celcnt_ = celcnt;
	txbox_idx_ = 0;

}
TRCellRow::~TRCellRow()
{
	Clear();
}
void TRCellRow::Draw(D2DContext& cxt)
{	
	ComPTR<ID2D1SolidColorBrush> br;
	(*cxt)->CreateSolidColorBrush(D2RGB(0,255,255), &br);

	for(int i= 0; i < celcnt_; i++ )
	{
		auto p = pcell_[i];
		

		(*cxt)->DrawRectangle(p.rc_, br );

		if ( p.edit_ == false )
			(*cxt)->FillRectangle(p.rc_, cxt.black_ );

		(*cxt)->DrawText(p.val_.c_str(), (int)p.val_.length(), cxt.textformat_,  p.rc_, cxt.white_); 
	}
}
void TRCellRow::Clear()
{
	delete [] pcell_;
	pcell_ = nullptr;
	celcnt_ = 0;
	txbox_idx_ = 0;
}

/// ///////////////////////////////////////////////
TRCellTable::TRCellTable()
{

}
void TRCellTable::Create( TRCell* p, int cellcnt, int rowcnt )
{
	for(int i = 0; i <rowcnt; i++ )
	{
		auto row = std::make_shared<TRCellRow>();
		tbl_.push_back(row);
	}
}
void TRCellTable::Clear()
{
	tbl_.clear();
}
void TRCellTable::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	float height = 26;

	mat.PushTransform();
	for(auto& it : tbl_ )
	{
		it->Draw(cxt);
		mat.Offset( 0, height );
	}
	mat.PopTransform();
}
