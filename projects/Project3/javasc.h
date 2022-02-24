#pragma once

bool JavascriptAppInit();
void JavascriptAppExit();
void JsRun(LPCWSTR script);


std::wstring UrlDecode(const std::wstring& s);