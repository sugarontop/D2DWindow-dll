#include "pch.h"

//#import "msxml6.dll"
#include "msxml6.tlh"
#pragma comment( lib, "msxml6")

#include "xml.h"


using namespace MSXML2;


BS gDcoFileName;

IXMLDOMDocument2Ptr LoadFile(LPCWSTR fnm)
{
	IXMLDOMDocument2Ptr pXMLDoc;
	HRESULT hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60), nullptr, CLSCTX_INPROC_SERVER);

	auto vbl = pXMLDoc->load(_variant_t(fnm));

	if ( vbl == VARIANT_FALSE )
		return nullptr;

	return pXMLDoc;
}

bool LoadDocument(LPCWSTR fnm, std::vector<DocFunc>& ar)
{
	gDcoFileName = fnm;
	
	IXMLDOMDocument2Ptr pXMLDoc = LoadFile(fnm);

	auto root = pXMLDoc->GetdocumentElement();
	auto ls = root->selectNodes(BS(L"t"));

	for(long i=0; i < ls->Getlength(); i++)
	{
		auto nd = ls->Getitem(i);

		DocFunc d;


		d.title = nd->selectSingleNode(BS(L"title"))->Gettext();
		d.param = nd->selectSingleNode(BS(L"params"))->Gettext();
		d.note = nd->selectSingleNode(BS(L"note"))->Gettext();


		ar.push_back(d);
	}

	return true;
}
bool SaveDocument(int idx, DocFunc doc)
{
	IXMLDOMDocument2Ptr pXMLDoc = LoadFile(gDcoFileName);


	auto root = pXMLDoc->GetdocumentElement();
	auto ls = root->selectNodes(BS(L"t"));
	
	auto nd = ls->Getitem(idx);

	nd->selectSingleNode(BS(L"params"))->Puttext(doc.param);
	nd->selectSingleNode(BS(L"note"))->Puttext(doc.note);
	
	pXMLDoc->save(variant_t(gDcoFileName));

	return true;
}