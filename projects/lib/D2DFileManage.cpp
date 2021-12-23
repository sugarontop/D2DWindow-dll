#include "pch.h"
#include "D2DWindow.h" 
#include "D2DFileManage.h"
#include "D2D1UI_1.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())
#define ROWHEIGHT 26

typedef std::function<void (LPCWSTR dir, WIN32_FIND_DATA* findData)> FindFunction;
void ListDirectoryContents( LPCWSTR dirName, LPCWSTR fileMask, FindFunction& func );

void D2DFileManage::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);

	root_.Draw(cxt, mat);

	mat.PopTransform();
}
LRESULT D2DFileManage::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;










	return r;
}
void D2DFileManage::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	rc_ = rc;

	make_root();
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
				auto bones = std::make_shared<BOnes>(fd->cFileName);	
				root_.ar_.push_back(bones);
			}
			else if ( att&FILE_ATTRIBUTE_ARCHIVE)
			{
				auto bone = std::make_shared<BOne>(fd->cFileName);
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
			func( fullName, &findData );
		}
		else 
			func( fullName, &findData );

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
	
	cxt.DText(FPointF(), text_,D2RGB(0,0,0));
	mat.Offset(0,ROWHEIGHT);
}
void BOnes::Draw(D2DContext& cxt,D2DMatrix& mat)
{
	mat_ = mat.Copy();
	cxt.DText(FPointF(), L"->", D2RGB(0,0,0));
	cxt.DText(FPointF(50,0), text_, D2RGB(0,0,0));
	mat.Offset(0,ROWHEIGHT);

	if ( bOpen_ )
	{
		auto x = mat._31;		
		mat.Offset(30,0);
		for(auto& it : ar_)
		{
			it->Draw(cxt,mat);
		}

		mat._31 = x;
	}

}