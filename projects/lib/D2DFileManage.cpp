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

static std::wstring XD(const std::wstring& dir)
{
	if (dir[dir.length()-1] == L'\\' )
		return dir;

	return dir+L'\\';
}


void D2DFileManage::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);

	ID2D1Bitmap* x[] = {bmp_[0],bmp_[1]};

	root_.Draw(cxt, mat, x );
	
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

				UINT cnt = root_.ChildCount();
				root_.height_ = ROWHEIGHT * cnt;
				rc_.SetHeight(root_.height_);
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
			auto fnm = XD(pb->dir_) + pb->text_;

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


BOne::~BOne()
{
	::SysFreeString(text_);
}

void BOne::Draw(D2DContext& cxt,D2DMatrix& mat,ID2D1Bitmap** img)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	(*cxt)->DrawBitmap(img[1]);
	cxt.DText(FPointF(15,0), text_,D2RGB(0,0,0));
	mat.Offset(0,ROWHEIGHT);
}
void BOnes::Draw(D2DContext& cxt,D2DMatrix& mat, ID2D1Bitmap** img)
{
	mat_ = mat.Copy();
	(*cxt)->SetTransform(mat_);

	(*cxt)->DrawBitmap(img[0]);

	cxt.DText(FPointF(25,0), text_, D2RGB(0,0,0));
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
		
	if ( 0 <= pt.y && pt.y <= ROWHEIGHT && pt.x < 100 )
	{
		if ( click_ )
			click_(this);

		return true;
	}

	return false;
}

bool BOne::operator < (BOne& a )
{
	if (dynamic_cast<BOnes*>( &a ))
	{
		return false;
	}

	return text_ < a.text_;
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


			auto sort = [](std::shared_ptr<BOne>& a, std::shared_ptr<BOne>& b)->bool
			{
				
				return a < b;

			};

			std::sort(ar_.begin(),ar_.end(), sort );

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
bool BOnes::operator < (BOne& a )
{
	if (dynamic_cast<BOnes*>( &a ))
	{
		return text_ < a.text_;
	}

	return false;
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