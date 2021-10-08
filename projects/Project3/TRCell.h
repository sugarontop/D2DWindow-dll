#pragma once
struct TRCell
{
public :
	TRCell();

	enum class TYP { STRING, MONEY, DATE, NUMBER };

	TYP typ_;
	short max_len_;
	bool edit_;
	FRectF rc_;
	std::wstring val_;

};

class TRCellRow
{
public :
	TRCellRow();
	TRCellRow( FRectF* rc, int celcnt );
	TRCellRow( FRectF* rc, int celcnt, int );
	~TRCellRow();
	void Draw(D2DContext& cxt);
	void Clear();
private :
	TRCell* pcell_;
	int celcnt_;
	FRectF rc_;
	int txbox_idx_;
};

class TRCellTable
{
public :
	TRCellTable();
	

	void Create( TRCell* p, int cellcnt, int rowcnt );
	void Draw(D2DContext& cxt);
	void Clear();

private :
	std::vector< std::shared_ptr<TRCellRow>> tbl_;
};