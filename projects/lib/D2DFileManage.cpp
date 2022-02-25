#include "pch.h"
#include "D2DFileManage.h"
#include "FilePackTool.h"



using namespace V6;
using namespace FILEPACK;

#define  APP (D2DApp::GetInstance())
#define ROWHEIGHT 20.0f

typedef std::function<void (LPCWSTR dir, WIN32_FIND_DATA* findData)> FindFunction;
void ListDirectoryContents( LPCWSTR dirName, LPCWSTR fileMask, FindFunction& func );
static void _Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp);

std::function<int(BOne*)> BOne::click_;
std::function<void(std::wstring)> D2DFileManage::OnClick_;

#define BFLG(a,bit)	((a&bit)==bit)
#define TEMP_VALUE 100

static bool bone_sort(std::shared_ptr<BOne>& a, std::shared_ptr<BOne>& b)
{
	bool ba = (dynamic_cast<BOnes*>(a.get()) != nullptr);
	bool bb = (dynamic_cast<BOnes*>(b.get()) != nullptr);


	if (ba && bb)
		return *a < *b;
	else if (ba && !bb)
		return true;
	else if (!ba && bb)
		return false;

	return *a < *b;

};


struct FullPath
{
	FullPath(){};


	void Parse(std::wstring _fullpath)
	{
		fullpath = _fullpath;
		int len = (int)_fullpath.length();

		_ASSERT(len >= 3);
		
		WCHAR d = fullpath[0];
		if (('a' <= d && d <= 'z' ) || ('A' <= d && d <= 'Z' ))
			if ( fullpath[1] == L':' )
				drive = fullpath.substr(0,3);
		
		updir = drive;

		for(int i=len-1; i > 0; i-- )
		{
			if (fullpath[i] == L'\\')
			{
				if ( fnm.length() == 0)
				{
					fnm = fullpath.substr((i+1),len-(i+1));				
					dir = fullpath.substr(0,i+1);
				}
				else
				{
					updir = fullpath.substr(0,i+1);
					break;
				}								
			}
		}
	}

	void Parse(std::wstring path, std::wstring fnm)
	{
		_ASSERT(path.length() != 0);

		if ( path[path.length()-1] != L'\\' )
			path += L'\\';

		auto fullpath = path + fnm;
		Parse(fullpath);
	}

	//ex C:\windows\system32\my.txt
	std::wstring drive;		// "C:\"
	std::wstring fullpath;	// "C:\windows\system32\my.txt"
	std::wstring fnm;		// "my.txt"
	std::wstring dir;		// "C:\windows\system32\"
	std::wstring updir;		// "C:\windows\"
};



static std::wstring XD(const std::wstring& dir)
{
	if (dir.length() == 0 )
		return dir;

	if ( dir[dir.length()-1] == L'\\' )
		return dir;

	return dir+L'\\';
}
static std::wstring XDN(const std::wstring& dir)
{
	int len = (int)dir.length();
	UINT s = 0;
	for(int i = len-1; i > 0; i-- )
	{	
		if (dir[i] == L'\\' )
		{
			s = i+1;
			break;
		}
	}

	if ( len == s )
		return dir;

	return dir.substr(s, len-s);
}

static std::wstring XDUP(const std::wstring& dir)
{
	int len = (int)dir.length();
	UINT s = 0;
	for(int i = len-2; i > 0; i-- )
	{	
		if (dir[i] == L'\\' )
		{
			s = i+1;
			break;
		}
	}

	return dir.substr(0, s);
}


void D2DFileManage::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);

	ID2D1Bitmap* x[] = {bmp_[0],bmp_[1]};

	root_->Draw(cxt, mat, x );
	
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

				if (typ_ == TYP::SOLO)
					root_->OnDblClick(pm.pt);
				else if (typ_ == TYP::TREE)
					root_->OnClick(pm.pt);
				

				UINT cnt = root_->ChildCount();
				root_->height_ = ROWHEIGHT * cnt;
				rc_.SetHeight(root_->height_);
				rc_.SetWidth(600);

				parent_control_->SendMesage(WM_D2D_SET_SIZE,1,0);
				r = 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.IsCapture(this))
			{
				r = 1;			
				b.bRedraw = true;
			}
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
		case WM_RBUTTONDOWN:
		{
			// 1’iã‚Ö
			MouseParam& pm = *(MouseParam*)lParam;			

			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				auto dir = XDUP(root_->fullname_);
				
				if ( dir != L"" )
				{
					rc_.SetHeight(800);
					RootChange(dir);
				}
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
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				if ( 1 == lParam)				
					ChangeTyp(D2DFileManage::TYP::TREE);
				else
					ChangeTyp(D2DFileManage::TYP::SOLO);
				r = 1;
			}
		}
		break;
	}

	return r;
}
void D2DFileManage::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	rc_ = rc;

	root_ = std::make_shared<BOnes>();
	typ_ = TYP::SOLO;

	make_root(L"C:\\");
	root_->SetText(L"c:\\");
	root_->bOpen_ = true;


	ComPTR<IStream> sm;
	if ( 1 == parent_window_->SendMessage(WM_D2D_GET_RESOURCE_BINARY, 0, (LPARAM)&sm))
	{
		std::vector<Item> ar;
		ListContents(sm, ar);

		for(auto& it : ar )
		{
			if ( it.fnm == L"folder.png" )
			{
				ComPTR<IStream> sm1;
				if (DeSerialize( sm, it, &sm1 ))
					_Stream2Bitmap(sm1,*(parent->GetContext()), &(bmp_[bmp::folder]));
			}
			else if ( it.fnm == L"file.png" )
			{
				ComPTR<IStream> sm1;
				if (DeSerialize( sm, it, &sm1 ))
					_Stream2Bitmap(sm1,*(parent->GetContext()), &(bmp_[bmp::file]));
			}

		}

	}


	BOne::click_ = [this](BOne* pb)->int
	{		
		if ( !dynamic_cast<BOnes*>(pb) && OnClick_ )
		{
			auto text = pb->info_.cFileName;

			auto fnm = XD(pb->dir_) + text;

			OnClick_(fnm);
		}

		return 0;
	};

}
std::wstring D2DFileManage::GetTreeTyp(USHORT* typ)
{
	*typ = 2;
	return L"D2DFileManage";
}

void D2DFileManage::make_root( LPCWSTR root_dir )
{
	FindFunction fn = [this](LPCWSTR dir, WIN32_FIND_DATA* fd)
	{
		auto att = fd->dwFileAttributes;
		
		if ( !(att&FILE_ATTRIBUTE_HIDDEN) )
		{
			if ( att&FILE_ATTRIBUTE_DIRECTORY)
			{
				if ( wcscmp(fd->cFileName,L".") && wcscmp(fd->cFileName,L"..") ) 
				{
					auto bones = std::make_shared<BOnes>(fd->cFileName, dir);	
					root_->ar_.push_back(bones);
					bones->info_ = *fd;
				}
			}
			else if ( att&FILE_ATTRIBUTE_ARCHIVE)
			{
				auto bone = std::make_shared<BOne>(fd->cFileName,dir);
				root_->ar_.push_back(bone);
				bone->info_ = *fd;
			}
		}
	};

	ListDirectoryContents(root_dir, L"*.*", fn);

	std::sort(root_->ar_.begin(),root_->ar_.end(), bone_sort );
}
int D2DFileManage::RootChange(std::wstring dir)
{
	std::wstring nm = XDN(dir);

	root_->fullname_ = dir;

	root_->clear();

	root_->SetText(nm.c_str());

	make_root( dir.c_str() );

	return 0;
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

LPCWSTR FILETIME_YYYYMMDD( const FILETIME& ftm)
{
	static WCHAR cb[256];

	SYSTEMTIME st;
	FileTimeToSystemTime( &ftm, &st );
	swprintf_s(cb,256,L"%d/%02d/%02d", st.wYear, st.wMonth, st.wDay );
	return cb;
}

void BOne::Draw(D2DContext& cxt,D2DMatrix& mat,ID2D1Bitmap** img)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	(*cxt)->DrawBitmap(img[1]);

	if ( textlayout_ == nullptr )
	{				
		WCHAR cb[256];
		auto text = info_.cFileName;
		swprintf_s(cb,256,L"%s   %s %d", text, FILETIME_YYYYMMDD(info_.ftCreationTime), info_.nFileSizeLow );		

		cxt.CreateTextLayout(cb, FSizeF(1000,ROWHEIGHT), &textlayout_);
	}
	
	(*cxt)->DrawTextLayout(FPointF(15,0),textlayout_, cxt.black_);

	mat.Offset(0,ROWHEIGHT);
}
void BOnes::Draw(D2DContext& cxt,D2DMatrix& mat, ID2D1Bitmap** img)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	(*cxt)->DrawBitmap(img[0]);

	
	if ( textlayout_ == nullptr )
	{		
		WCHAR cb[256];
		auto text = info_.cFileName;
		if ( fullname_.length() > 0 && fullname_!=text)
			swprintf_s(cb,256,L"%s   (%s)", text, fullname_.c_str());		
		else
			lstrcpy(cb, text);

		cxt.CreateTextLayout(cb, FSizeF(1000,ROWHEIGHT), &textlayout_);
	}
	
	(*cxt)->DrawTextLayout(FPointF(25,0),textlayout_, cxt.black_);
	
	mat.Offset(0,ROWHEIGHT);

	if ( bOpen_ )
	{		
		auto x = mat._31;
		mat.Offset(30,0);
		for(auto& it : ar_)
		{
			it->Draw(cxt,mat,img);			
		}
		mat._31=x;
	}
}

bool BOne::OnClick(const FPointF& ptdev)
{	
	auto pt = mat_.DPtoLP(ptdev);
		
	if ( 0 <= pt.y && pt.y <= ROWHEIGHT && pt.x < TEMP_VALUE )
	{
		if ( click_ )
			click_(this);

		return true;
	}

	return false;
}

std::wstring str_toupper(std::wstring s) 
{
	std::transform(s.begin(), s.end(), s.begin(), [](WCHAR c){ return std::toupper(c); });
    return s;
}

bool BOne::operator < (BOne& a )
{
	auto atext = a.info_.cFileName;
	auto text = info_.cFileName;

	std::wstring aa = str_toupper(atext);
	std::wstring bb = str_toupper(text);

	return bb < aa;
}
bool BOne::OnDblClick(const FPointF& ptdev)
{
	return false;
}
bool BOnes::OnDblClick(const FPointF& ptdev)
{
	bool bl = false;
	auto pt = mat_.DPtoLP(ptdev);		
	if ( 0 <= pt.y && pt.y <= ROWHEIGHT && pt.x < TEMP_VALUE )
		bl = true;

	if (!bl)
	{
		for(auto& it : ar_)
		{
			if (it->OnDblClick(ptdev))
			{
				bl = true;
				break;
			}
		}
	}	
	else if (bl)
	{
		auto parent = dynamic_cast<D2DFileManage*>(APP.GetCapture());
		_ASSERT(parent);

		if (dir_ != L"")
		{
			std::wstring dir = XD(dir_);
			dir = dir + info_.cFileName; //text_;

			parent->RootChange(dir);
		}
	}
	return bl;
}

void BOnes::clear()
{
	ar_.clear();
	textlayout_.Release();

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
			dir = dir + info_.cFileName; //text_;

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

							bones->info_ = *fd;
						}
					}
					else if ( BFLG(att,FILE_ATTRIBUTE_ARCHIVE))
					{
						auto bone = std::make_shared<BOne>(fd->cFileName,dir);
						ar_.push_back(bone);
						bone->info_ = *fd;
					}
				}
			};

			ListDirectoryContents(dir.c_str(), L"*.*", fn );			

			std::sort(ar_.begin(),ar_.end(), bone_sort );

		}
		else if ( !bOpen_ )
		{
			ar_.clear();
		}
	}

	return bl;
}
UINT BOnes::ChildCount()
{
	if (bOpen_)
	{
		UINT cnt=1;
		for(auto& it : ar_)
			cnt += it->ChildCount();
		
		return cnt;
	}
	return 1;
}
bool BOnes::operator < (BOnes& a )
{
	auto atext = a.info_.cFileName;
	auto text = info_.cFileName;

	std::wstring aa = str_toupper(atext);
	std::wstring bb = str_toupper(text);

	return bb < aa;
}

static void _Stream2Bitmap( IStream* sm, ID2D1RenderTarget* target, ID2D1Bitmap** bmp)
{
	ComPTR<IWICImagingFactory> d2dWICFactory;
	ComPTR<IWICBitmapDecoder> d2dDecoder;
	ComPTR<IWICFormatConverter> d2dConverter;
	ComPTR<IWICBitmapFrameDecode> d2dBmpSrc;

	auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));
	
	_ASSERT(hr == S_OK);
	hr = d2dWICFactory->CreateDecoderFromStream(sm, 0, WICDecodeMetadataCacheOnLoad, &d2dDecoder);
	_ASSERT(hr == S_OK);
	hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);
	_ASSERT(hr == S_OK);
	hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);
	_ASSERT(hr == S_OK);
	hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	_ASSERT(hr == S_OK);
	hr = target->CreateBitmapFromWicBitmap(d2dConverter, NULL, bmp);

	_ASSERT(hr == S_OK);
}