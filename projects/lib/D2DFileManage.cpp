#include "pch.h"
#include "D2DWindow.h" 
#include "D2DFileManage.h"
#include "D2D1UI_1.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define ROWHEIGHT 26

typedef std::function<void (LPCWSTR dir, WIN32_FIND_DATA* findData)> FindFunction;
void ListDirectoryContents( LPCWSTR dirName, LPCWSTR fileMask, FindFunction& func );


#define BFLG(a,bit)	((a&bit)==bit)

void D2DFileManage::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);

	//cxt.DFillRect(rc_.ZeroRect(), ColorF::LightGray);

	root_.Draw(cxt, mat);
	
	mat.PopTransform();
}
LRESULT D2DFileManage::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch(message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;			

			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				APP.SetCapture(this);
				root_.OnClick(pm.pt);

				FSizeF sz = rc_.Size();
				parent_control_->WndProc(b,WM_D2D_SET_SIZE_SIZE,1,(LPARAM)&sz);

				r = 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			//if ( APP.IsCapture(this))
			//	r = 1;			
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				r = 1;

				

			}
		}
		break;
		case WM_D2D_SET_SIZE_SIZE:
		{
			FSizeF& sz = *(FSizeF*)lParam;
			//rc_.SetSize(sz);

			
			return 0;

		}
		break;
	}

	return r;
}
void D2DFileManage::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	rc_ = rc;

	make_root();
	root_.SetText(L"c:\\");
	root_.bOpen_ = true;
}
std::wstring D2DFileManage::GetTreeTyp(USHORT* typ)
{
	*typ = 2;
	return L"D2DFileManage";
}

void D2DFileManage::make_root()
{
	FindFunction fn = [this](LPCWSTR dir, WIN32_FIND_DATA* fd)
	{
		auto att = fd->dwFileAttributes;
		
		if ( !(att&FILE_ATTRIBUTE_HIDDEN) )
		{
			if ( att&FILE_ATTRIBUTE_DIRECTORY)
			{
				auto bones = std::make_shared<BOnes>(fd->cFileName, dir);	
				root_.ar_.push_back(bones);
			}
			else if ( att&FILE_ATTRIBUTE_ARCHIVE)
			{
				auto bone = std::make_shared<BOne>(fd->cFileName,dir);
				root_.ar_.push_back(bone);
			}
		}
	};

	ListDirectoryContents(L"c:\\", L"*.*", fn);
}


void ListDirectoryContents( LPCWSTR dirName, LPCWSTR fileMask, FindFunction& func )
{
	TCHAR* fileName;
	TCHAR curDir[MAX_PATH];
	TCHAR fullName[MAX_PATH];
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	if ( !GetCurrentDirectory( 256, curDir ))
		return;

	if ( wcscmp( dirName, L"." ) && wcscmp( dirName, L".." ))
	{
		if ( !SetCurrentDirectory( dirName ))
			return;
	}
	else
		return;

	if (!GetFullPathName( fileMask,MAX_PATH,fullName,&fileName ))
		return;

	fileHandle = FindFirstFile (fileMask, &findData );
	while ( fileHandle != INVALID_HANDLE_VALUE )
	{
		if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			//ListDirectoryContents( findData.cFileName, fileMask, func );
			func( dirName, &findData );
		}
		else 
			func( dirName, &findData );

		if ( !FindNextFile( fileHandle,&findData ))
			break;
	}

	FindClose( fileHandle );
	SetCurrentDirectory( curDir );
}

//////////////////////////////////////////////////////////////////

void BOne::Draw(D2DContext& cxt,D2DMatrix& mat)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	cxt.DText(FPointF(), text_,D2RGB(0,0,0));
	mat.Offset(0,ROWHEIGHT);
}
void BOnes::Draw(D2DContext& cxt,D2DMatrix& mat)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	cxt.DText(FPointF(), L"->", D2RGB(0,0,0));
	cxt.DText(FPointF(25,0), text_, D2RGB(0,0,0));
	mat.Offset(0,ROWHEIGHT);

	if ( bOpen_ )
	{		
		auto x = mat._31;
		mat.Offset(30,0);
		for(auto& it : ar_)
		{
			it->Draw(cxt,mat);			
		}
		mat._31=x;
	}
}

bool BOne::OnClick(const FPointF& ptdev)
{	
	auto pt = mat_.DPtoLP(ptdev);
		
	if ( 0 <= pt.y && pt.y <= ROWHEIGHT && pt.x < 100 )
		return true;

	return false;
}

static std::wstring XD(const std::wstring& dir)
{
	if (dir[dir.length()-1] == L'\\' )
		return dir;

	return dir+L'\\';
}


bool BOnes::OnClick(const FPointF& ptdev)
{
	bool bl = BOne::OnClick(ptdev);

	if (!bl && bOpen_)
	{
		for(auto& it : ar_)
		{
			if (it->OnClick(ptdev))
			{
				bl = true;
				break;
			}
		}
	}	
	else if (bl)
	{
		bOpen_ = !bOpen_;

		if ( bOpen_ && ar_.empty() )
		{			
			std::wstring dir = XD(dir_);
			dir = dir + text_;

			FindFunction fn = [this](LPCWSTR dir, WIN32_FIND_DATA* fd)
			{
				auto att = fd->dwFileAttributes;
		
				if ( !BFLG(att,FILE_ATTRIBUTE_HIDDEN))
				{
					if ( BFLG(att,FILE_ATTRIBUTE_DIRECTORY))
					{
						if ( wcscmp( fd->cFileName, L"." ) && wcscmp( fd->cFileName, L".." ))
						{
							auto bones = std::make_shared<BOnes>(fd->cFileName, dir);	
							ar_.push_back(bones);
						}
					}
					else if ( BFLG(att,FILE_ATTRIBUTE_ARCHIVE))
					{
						auto bone = std::make_shared<BOne>(fd->cFileName,dir);
						ar_.push_back(bone);
					}
				}
			};

			ListDirectoryContents(dir.c_str(), L"*.*", fn );

		}
	}

	return bl;
}