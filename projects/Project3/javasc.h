#pragma once

bool JavascriptAppInit();
void JavascriptAppExit();
void JsRun(LPCWSTR script);

void JsCall(LPCWSTR funcnm, JsValueRef* pval, int pvalcnt);

std::wstring UrlDecode(const std::wstring& s);