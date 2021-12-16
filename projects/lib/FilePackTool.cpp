
#include "pch.h"
#include "filepacktool.h"

namespace FILEPACK {

void some_file_to_files(std::wstring dst, std::wstring fnms, std::vector<Item>& ar)
{
	WIN32_FIND_DATA t={};
		
	auto inpt = FindFirstFile(fnms.c_str(), &t);
	while( inpt != INVALID_HANDLE_VALUE )
	{
		Item s;
		s.fnm = t.cFileName;
		s.size = t.nFileSizeLow;
		ar.push_back(s);
		
		if (!FindNextFile(inpt, &t))
			break;
	}
	FindClose( inpt );
}


bool CreateSerializeBin( std::vector<Item>& file_info_ar,  IStream** psm )
{	
	// alloc Destination memory
	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, psm);
	if ( hr != 0)
		return false;
	IStream* sm = *psm;
	ULONG scblen;

	// 1 write FileHead
	FileHead fh;
	fh.magic_number = MAGIC_NUMBER;
	fh.version = 1;
	fh.HeadCount = (WORD)file_info_ar.size();

	if ( 0 != sm->Write(&fh,sizeof(fh),&scblen))
	{
		sm->Release();
		return false;
	}

	// alloc temporary memory
	bool bl = true;
	DWORD pos = 0;
	IStream* sm1;
	hr = ::CreateStreamOnHGlobal(NULL,TRUE, &sm1);
	if ( hr != 0)
		return false;

	// copy file to temp memory
	std::map<std::wstring, DWORD> fnm_length;
	for(auto& it : file_info_ar )
	{
		auto h = ::CreateFile(it.fnm.c_str(),GENERIC_READ, 0,NULL,OPEN_EXISTING,NULL,NULL); 

		if ( h == INVALID_HANDLE_VALUE )
		{
			bl = false;
			break;
		}
		auto cblen = it.size;

		byte* cb = new byte[cblen];
		bl =  ::ReadFile(h, cb, cblen,nullptr,nullptr);

		if ( bl )
		{
			fnm_length[it.fnm] = pos;
			sm1->Write(cb, cblen, &scblen);

			pos += scblen;
		}
		
		::CloseHandle(h);
		delete [] cb;

		if ( !bl )
			break;
	}
	DWORD total=pos;

	// 2 write Head
	for(auto& it : file_info_ar)
	{
		Head h = {};
		h.enable = true;
		lstrcpy(h.fnm,it.fnm.c_str());
		h.len = it.size;
		h.pos = fnm_length[it.fnm];
		
		sm->Write(&h,sizeof(h),&scblen);
	}


	// 3 write Contents
	ULARGE_INTEGER u1,u2;
	ULARGE_INTEGER cb={}; 
	LARGE_INTEGER pos1={}; 
	cb.QuadPart = total;

	sm1->Seek(pos1,STREAM_SEEK_SET, &u1);
	
	bl = (S_OK == sm1->CopyTo(sm,cb,&u1,&u2));

	sm1->Release();



	return bl;

}
bool Convert2File(LPCWSTR fnm,DWORD magic_number, IStream* src)
{
	// create IStream file

	LARGE_INTEGER pos={}; 
	ULARGE_INTEGER d;

	if ( S_OK != src->Seek(pos,STREAM_SEEK_SET, &d))
		return false;

	auto h = ::CreateFile(fnm,GENERIC_WRITE, 0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	if ( h == INVALID_HANDLE_VALUE )
		return false;

	if ( magic_number != 0 )
		::WriteFile(h, &magic_number, sizeof(DWORD), nullptr,nullptr);

	byte cb[256];
	ULONG len;
	while( S_OK == src->Read(cb, 256, &len) && len != 0 )
	{
		::WriteFile(h, cb, len, nullptr,nullptr);
	}

	::CloseHandle(h);

	return true;
}
bool Convert2Stream(LPCWSTR fnm,DWORD magic_number, IStream** pdst)
{
	// create IStream file

	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, pdst);
	if ( hr != 0)
		return false;

	bool bl = false;
	ULONG len;
	LARGE_INTEGER pos={}; 
	
	auto h = ::CreateFile(fnm,GENERIC_READ, 0,NULL,OPEN_EXISTING,0,NULL); 
	if ( h == INVALID_HANDLE_VALUE )
		return false;
	
	if ( magic_number != 0 )
	{
		::ReadFile(h, &magic_number, sizeof(DWORD), &len, nullptr);	
		if (magic_number != MAGIC_NUMBER )
			goto end;
	}


	byte cb[256];
	
	while( ::ReadFile(h, cb, 256, &len, nullptr) && len != 0 )
	{
		ULONG slen;
		(*pdst)->Write(cb, len, &slen);		
		
		_ASSERT(len==slen);
	}

	bl = true;

end:
	::CloseHandle(h);

	return bl;
}
bool ListContents( IStream* src, std::vector<Item>& ar )
{
	LARGE_INTEGER pos={}; 
	ULARGE_INTEGER d;

	if ( S_OK != src->Seek(pos,STREAM_SEEK_SET, &d))
		return false;

	FileHead h={};
	ULONG sh;
	if (S_OK == src->Read(&h,sizeof(h),&sh))
	{
		if ( h.magic_number != MAGIC_NUMBER || h.version != 1 )
			return false;
	}
	
	for(UINT i= 0; i < h.HeadCount; i++)
	{
		Head head={};

		src->Read(&head,sizeof(head),&sh);

		Item s;
		s.fnm = head.fnm;
		s.size = head.len;
		s.pos = head.pos;
		ar.push_back(s);
	}

	return true;
}

bool DeSerialize( IStream* src, Item s, IStream** psm )
{
	LARGE_INTEGER pos={}; 
	ULARGE_INTEGER ulen,d;

	if ( S_OK != src->Seek(pos,STREAM_SEEK_SET, &d))
		return false;

	FileHead fh;
	ULONG clen;
	src->Read(&fh,sizeof(fh),&clen);

	auto off = fh.HeadCount * sizeof(Head) + s.pos;


	pos.QuadPart = off;
	src->Seek(pos,STREAM_SEEK_CUR,&d);


	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, psm);
	if ( hr != 0 )
		return false;

	ULARGE_INTEGER d1,d2;
	ulen.QuadPart = s.size;
	src->CopyTo(*psm, ulen, &d1,&d2);



	return true;

}

#include <compressapi.h>
#pragma comment (lib,"Cabinet.lib")

#define ALGORITHM COMPRESS_ALGORITHM_XPRESS_HUFF 
#define MYASSERT(bl)	if ((bl)==FALSE) goto ERR;

bool Compress(IStream* src, IStream** pdst )
{
	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, pdst);
	if ( hr != 0)
		return false;

	LARGE_INTEGER pos={}; 
	ULARGE_INTEGER ulen,d,cur;
	//src->Seek(pos, STREAM_SEEK_END, &ulen);
	//src->Seek(pos, STREAM_SEEK_SET, &d);

	src->Seek(pos, STREAM_SEEK_SET, &cur);
	src->Seek(pos, STREAM_SEEK_END, &ulen);
	ulen.QuadPart = ulen.QuadPart - cur.QuadPart;
	pos.QuadPart = (LONGLONG)cur.QuadPart;
	src->Seek(pos, STREAM_SEEK_SET, &d);


	COMPRESSOR_HANDLE Compressor    = NULL;
	byte* InputBuffer;
	UINT InputFileSize;
	SIZE_T CompressedDataSize, CompressedBufferSize;
	byte* CompressedBuffer = NULL;


	ULONG dd;
	InputFileSize = ulen.LowPart;
	InputBuffer = new byte[InputFileSize];
	src->Read(InputBuffer,InputFileSize,&dd);

	auto Success = CreateCompressor(
        ALGORITHM, //  Compression Algorithm
        NULL,                           //  Optional allocation routine
        &Compressor);   
	MYASSERT(Success)

	Success = Compress(
        Compressor,                  //  Compressor Handle
        InputBuffer,                 //  Input buffer, Uncompressed data
        InputFileSize,               //  Uncompressed data size
        NULL,                        //  Compressed Buffer
        0,                           //  Compressed Buffer size
        &CompressedBufferSize);      //  Compressed Data size

	MYASSERT( Success == FALSE && CompressedBufferSize > 0 )

	CompressedBuffer = new byte[CompressedBufferSize];


	Success = Compress(
        Compressor,             //  Compressor Handle
        InputBuffer,            //  Input buffer, Uncompressed data
        InputFileSize,          //  Uncompressed data size
        CompressedBuffer,       //  Compressed Buffer
        CompressedBufferSize,   //  Compressed Buffer size
        &CompressedDataSize);   //  Compressed Data size
	MYASSERT(Success)
	
	(*pdst)->Write(CompressedBuffer, (ULONG)CompressedDataSize,&dd);
	
	/////////////////////////////////////////////////////////////////////

	delete [] CompressedBuffer;
	delete [] InputBuffer;

    return true;

ERR:
	
	delete [] CompressedBuffer;
	delete [] InputBuffer;

    return false;

}



bool DeCompress(IStream* src, IStream** pdst )
{
	HRESULT hr = ::CreateStreamOnHGlobal(NULL,TRUE, pdst);
	if ( hr != 0)
		return false;
	
	LARGE_INTEGER pos={}; 
	ULARGE_INTEGER ulen,d,cur;

	src->Seek(pos, STREAM_SEEK_SET, &cur);
	src->Seek(pos, STREAM_SEEK_END, &ulen);
	ulen.QuadPart = ulen.QuadPart - cur.QuadPart;
	pos.QuadPart = (LONGLONG)cur.QuadPart;
	src->Seek(pos, STREAM_SEEK_SET, &d);

	COMPRESSOR_HANDLE Decompressor    = NULL;
	byte* InputBuffer;
	UINT InputFileSize;
	SIZE_T DecompressedBufferSize, DeCompressedDataSize;
	byte* DeCompressedBuffer = NULL;


	ULONG dd;
	InputFileSize = ulen.LowPart;
	InputBuffer = new byte[InputFileSize];
	src->Read(InputBuffer,InputFileSize,&dd);

	BOOL Success = CreateDecompressor(
        ALGORITHM, //  Compression Algorithm
        NULL,                           //  Optional allocation routine
        &Decompressor);                 //  Handle

	MYASSERT(Success)


	Success = Decompress(
        Decompressor,                //  Compressor Handle
        InputBuffer,            //  Compressed data
        InputFileSize,               //  Compressed data size
        NULL,                        //  Buffer set to NULL
        0,                           //  Buffer size set to 0
        &DecompressedBufferSize);    //  Decompressed Data size

	
	MYASSERT( Success == FALSE && DecompressedBufferSize != 0 && DecompressedBufferSize < 100*1E6)

	DeCompressedBuffer = new byte[DecompressedBufferSize];

	Success = Decompress(
        Decompressor,             //  Compressor Handle
        InputBuffer,            //  Input buffer, Uncompressed data
        InputFileSize,          //  Uncompressed data size
        DeCompressedBuffer,       //  Compressed Buffer
        DecompressedBufferSize,   //  Compressed Buffer size
        &DeCompressedDataSize);   //  Compressed Data size
	
	MYASSERT(Success)

	
	(*pdst)->Write(DeCompressedBuffer, (ULONG)DeCompressedDataSize,&dd);

	/////////////////////////////////////////////////////////////////////
	delete [] DeCompressedBuffer;
	delete [] InputBuffer;

	return true;


ERR:
	delete [] DeCompressedBuffer;
	delete [] InputBuffer;

	return false;
}

}