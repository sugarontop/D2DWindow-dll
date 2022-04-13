#pragma once






//
// 描画後の指定部分をID2D1Bitmapへ変換
//
bool RenderTargetToBitmp(ID2D1RenderTarget* rt, D2D1_RECT_U rc, ID2D1Bitmap** out);


//
// StreamからBitmapを再現
//
//bool CreateBitampFromStream(ID2D1RenderTarget* pRenderTarget, IStream* sm, ID2D1Bitmap** nbmp);


bool IStreamToBitmap(ID2D1RenderTarget* rt, IStream* sm, ID2D1Bitmap** bmp);

//
// 内部でIWICBitmapに変換後、pngなどのファイルに変換。
//
bool SaveBitmapToFile(LPCWSTR fnm, ID2D1Bitmap* srcBitmap);

//
// 指定画面サイズで、メモリ上にID2D1RenderTargetを作成
//
bool CreateMemoryRenderTarget( UINT width, UINT height, ID2D1RenderTarget** out);

//
// ID2D1BitmapからIWICBitmapへ変換
//
bool BitmpaToIWICBitmap(ID2D1Bitmap* srcBitmap, IWICBitmap** out);


//
// BitmapからStreamへ変換
//
bool BitmapToIStream(ID2D1Bitmap* pBitmap, IStream* out);


// ID2D1DeviceContextなので、使いにくい
bool BitmapSaveToFile(ID2D1DeviceContext* cxt, LPCWSTR fnm, ID2D1Image* bmp2);
