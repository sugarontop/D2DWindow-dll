#pragma once

struct UIHandle;

bool JavascriptAppInit();
void JavascriptAppExit();
void JsRun(LPCWSTR script);

void JsCall(LPCWSTR funcnm, JsValueRef* pval, int pvalcnt);

void JsRegistUIHandle(std::wstring nm, UIHandle h);
UIHandle JsRegistGet(std::wstring nm);

std::wstring UrlDecode(const std::wstring& s);