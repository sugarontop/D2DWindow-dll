#pragma once






//
// �`���̎w�蕔����ID2D1Bitmap�֕ϊ�
//
bool RenderTargetToBitmp(ID2D1RenderTarget* rt, D2D1_RECT_U rc, ID2D1Bitmap** out);

bool CreateBitampFromStream(ID2D1RenderTarget* pRenderTarget, IStream* sm, ID2D1Bitmap** nbmp);

//
// ������IWICBitmap�ɕϊ���Apng�Ȃǂ̃t�@�C���ɕϊ��B
//
bool SaveBitmapToFile(LPCWSTR fnm, ID2D1Bitmap* srcBitmap);

//
// �w��T�C�Y�̃��������ID2D1RenderTarget���쐬
//
bool CreateMemoryRenderTarget( UINT width, UINT height, ID2D1RenderTarget** out);

//
// ID2D1Bitmap����IWICBitmap�֕ϊ�
//
bool BitmpaToIWICBitmap(ID2D1Bitmap* srcBitmap, IWICBitmap** out);


// ID2D1DeviceContext�Ȃ̂ŁA�g���ɂ���
bool BitmapSaveToFile(ID2D1DeviceContext* cxt, LPCWSTR fnm, ID2D1Image* bmp2);
