#include "pch.h"
#include "TextEditor.h"
#include "TextStoreACP.h"

using namespace TSF;

#ifndef _WINDOWS
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Core;
#endif


CTextEditor::CTextEditor() 
{
    pTextStore_ = NULL;    
	ct_ = NULL;
	pCompositionRenderInfo_ = NULL;
    nCompositionRenderInfo_ = 0;
	pDocumentMgr_ = NULL;
	pInputContext_ = NULL;
    search_x_ = 0;
    weak_tmgr_ = NULL;
}

CTextEditor::~CTextEditor() 
{
    UninitTSF();
	
}

//----------------------------------------------------------------
//
// Application Initialize
//
//----------------------------------------------------------------

BOOL CTextEditor::InitTSF(HWND hWnd, ITfThreadMgr2* tmgr, TfClientId clientid)
{
	BOOL ret = FALSE;

    pTextStore_ = new CTextStore(this);

    weak_tmgr_ = tmgr;

	ITfDocumentMgr* pDocumentMgrPrev = NULL;
    ComPTR<IUnknown> q;

    if (!pTextStore_) 
		goto Exit;
    
	if (FAILED(tmgr->CreateDocumentMgr(&pDocumentMgr_)))
		goto Exit;


   
    pTextStore_->QueryInterface( IID_IUnknown, (void**)&q);

    if (FAILED(pDocumentMgr_->CreateContext(clientid, 0, q, &pInputContext_, &ecTextStore_)))
		goto Exit;

    if (FAILED(pDocumentMgr_->Push(pInputContext_)))
		goto Exit;

#if ( _WIN32_WINNT_WIN8 <= _WIN32_WINNT )
    tmgr->SetFocus(pDocumentMgr_);
#else
	if (FAILED( g_pThreadMgr->AssociateFocus(hWnd, pDocumentMgr_, &pDocumentMgrPrev)))
		goto Exit;
#endif
	
    pTextStore_->InitSink(tmgr, clientid);
    


	hWnd_ = hWnd;

	if ( pDocumentMgrPrev )
		pDocumentMgrPrev->Release();

	pTextEditSink_ = new CTextEditSink(this);
    if (!pTextEditSink_)
		goto Exit;

    pTextEditSink_->_Advise(pInputContext_);

	ret = TRUE;

Exit :
	if ( pDocumentMgrPrev )
		pDocumentMgrPrev->Release();
    
    return ret;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextEditor::UninitTSF( )
{
    if (pTextEditSink_)
    {
        pTextEditSink_->_Unadvise();
        pTextEditSink_->Release();
        pTextEditSink_ = NULL;
    }
	
	if (pDocumentMgr_)
    {
        pDocumentMgr_->Pop(TF_POPF_ALL);
		
		pDocumentMgr_->Release();
		pDocumentMgr_ = NULL;
	}

	if (pInputContext_)
	{
		pInputContext_->Release();
		pInputContext_ = NULL;
	}

    if (pTextStore_)
    {
        pTextStore_->CloseSink(weak_tmgr_);


        while( pTextStore_->Release() );
        pTextStore_ = NULL;
    }

    return TRUE;
}
//----------------------------------------------------------------
//
// move Caret 
//
//----------------------------------------------------------------

void CTextEditor::MoveSelection(int nSelStart, int nSelEnd, bool bTrail)
{
	if ( nSelEnd < nSelStart )
		std::swap( nSelStart, nSelEnd );
	
	if ( ct_ )
	{
		int nTextLength = (int)ct_->GetTextLength();
		if (nSelStart >= nTextLength)
			nSelStart = nTextLength;

		if (nSelEnd >= nTextLength)
			nSelEnd = nTextLength;

		// set caret position
		ct_->SetSelStart(nSelStart); 
		ct_->SetSelEnd(nSelEnd);

		ct_->bSelTrail_ = bTrail;

    

		pTextStore_->OnSelectionChange();
	}
}


//----------------------------------------------------------------
//
//	caretが動く時
//
//----------------------------------------------------------------

void CTextEditor::MoveSelectionNext()
{
    int nTextLength = (int)ct_->GetTextLength();

	int zCaretPos = (ct_->bSelTrail_ ? ct_->SelEnd() : ct_->SelStart() );
	zCaretPos = min(nTextLength, zCaretPos+1); // 1:次の文字

    ct_->SetSelStart(zCaretPos);
    ct_->SetSelEnd(zCaretPos);
    pTextStore_->OnSelectionChange();
}

//----------------------------------------------------------------
//
//　caretが動く時
//
//----------------------------------------------------------------

void CTextEditor::MoveSelectionPrev()
{
	int zCaretPos = (ct_->bSelTrail_ ? ct_->SelEnd() : ct_->SelStart() );
	zCaretPos = max(0, zCaretPos-1);


    ct_->SetSelStart(zCaretPos);
    ct_->SetSelEnd(zCaretPos);

    pTextStore_->OnSelectionChange();
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
BOOL CTextEditor::MoveSelectionAtPoint(FPointF ptlog)
{
    BOOL bRet = FALSE;
    int nSel = (int)layout_.CharPosFromPoint(ptlog);
    if (nSel != -1)
    {
        MoveSelection(nSel, nSel, true);
        bRet = TRUE;
    }
    return bRet;
}

//----------------------------------------------------------------
//
// VK_DOWN,VK_UP
//
//----------------------------------------------------------------
BOOL CTextEditor::MoveSelectionUpDown(BOOL bUp, bool bShiftKey )
{
    int caret = CurrentCaretPos();

    UINT nSel = bUp ? ct_->SelEnd() : ct_->SelStart() ;


    if ( caret ==  layout_.CharPosMap_.size())
        caret--; // 文末

    if ( bUp && caret > 0)
    {
        auto c = layout_.CharPosMap_[caret];

        FRectF rctarget = c.rc;        
        rctarget.Offset( 0, -c.rc.Height()); // up

        FPointF pttarget( rctarget.CenterPt());

        pttarget.x = max( search_x_, pttarget.x );
        search_x_ =  pttarget.x;


        int target=caret;

        for(int i = caret; i >=0; i-- )
        {
            c = layout_.CharPosMap_[i];

            if ( c.rc.PtInRect(pttarget) )
            {
                target = i;
                break;
            }            
            else if (  c.rc.top < pttarget.y && c.rc.left < pttarget.x )
            {
                target = i;
                break;
            }

        }
      
        if ( bShiftKey )
            MoveSelection(nSel, target,!bUp );
        else 
            MoveSelection(target, target,!bUp );
        return TRUE;

    }
    else if ( !bUp && -1 < caret && caret <  (int)layout_.CharPosMap_.size())
    {
        auto c = layout_.CharPosMap_[caret];

        FRectF rctarget = c.rc;        
        rctarget.Offset( 0, c.rc.Height()); // down

        FPointF pttarget( rctarget.CenterPt());
        pttarget.x = max( search_x_, pttarget.x );
        search_x_ =  pttarget.x;

        UINT target=(UINT)layout_.CharPosMap_.size()-1;

        for(UINT i = caret; i  < layout_.CharPosMap_.size(); i++ )
        {
            c = layout_.CharPosMap_[i];

            if ( c.rc.PtInRect(pttarget) )
            {
                target = i;
                break;
            }            
            else if ( c.rc.top > pttarget.y && i > 0)
            {
                target = i-1;
                break;
            }
        }

        if ( bShiftKey )
            MoveSelection(nSel, target,!bUp );
        else 
            MoveSelection(target, target,!bUp );
        return TRUE;
    }
    else 
        return FALSE;

}


//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextEditor::MoveSelectionToLineFirstEnd(BOOL bFirst, bool bShiftKey)
{
    BOOL bRet = FALSE;
    UINT nSel,nSel2;

    if (bFirst)
    {
		// when pushed VK_HOME
		ct_->nStartCharPos_ = 0;
		nSel2 = ct_->SelEnd();
        nSel = layout_.FineFirstEndCharPosInLine(ct_->SelStart(), TRUE);
    }
    else
    {
        // when pushed VK_END
		ct_->nStartCharPos_ = 0;
		nSel2 = ct_->SelStart();
		nSel = layout_.FineFirstEndCharPosInLine(ct_->SelEnd(), FALSE);
    }

    if (nSel != (UINT)-1)
    {
		if ( bShiftKey )
		{
			MoveSelection(nSel, nSel2,true);
		}
		else
		{
			MoveSelection(nSel, nSel);
		}

        bRet = TRUE;
    }

	
    return bRet;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditor::InvalidateRect()
{
#ifdef _WINDOWS
    ::InvalidateRect(hWnd_, NULL, FALSE);
#endif
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
BOOL CTextEditor::InsertAtSelection(LPCWSTR psz)
{
    //layout_.bRecalc_ = true;
    LONG lOldSelEnd = ct_->SelEnd();
    if (!ct_->RemoveText(ct_->SelStart(), ct_->SelEnd() - ct_->SelStart()))
        return FALSE;

	UINT nrCnt;
    if (!ct_->InsertText(ct_->SelStart(), psz, (UINT)wcslen(psz), nrCnt))
        return FALSE;

	
    ct_->SetSelStart( ct_->SelStart()+ (int)wcslen(psz));
    ct_->SetSelEnd( ct_->SelStart());



    
	LONG acs = ct_->SelStart();
	LONG ecs = ct_->SelEnd();
	pTextStore_->OnTextChange(acs, lOldSelEnd, ecs);



    pTextStore_->OnSelectionChange();
    return TRUE;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextEditor::DeleteAtSelection(BOOL fBack)
{
    //layout_.bRecalc_ = true;

    if (!fBack && (ct_->SelEnd() < (int)ct_->GetTextLength()))
    {
        if (!ct_->RemoveText(ct_->SelEnd(), 1))
            return FALSE;

		
		LONG ecs = ct_->SelEnd();

        pTextStore_->OnTextChange(ecs, ecs + 1, ecs);
    }
	 
    if (fBack && (ct_->SelStart() > 0))
    {
        if (!ct_->RemoveText(ct_->SelStart() - 1, 1))
            return FALSE;

        ct_->SetSelStart( ct_->SelStart() -1);
        ct_->SetSelEnd( ct_->SelStart());




		LONG acs = ct_->SelStart();
        pTextStore_->OnTextChange(acs, acs + 1, acs );
        pTextStore_->OnSelectionChange();
    }

    return TRUE;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextEditor::DeleteSelection()
{
    //layout_.bRecalc_ = true;
    ULONG nSelOldEnd = ct_->SelEnd();
    ct_->RemoveText(ct_->SelStart(), ct_->SelEnd() - ct_->SelStart());

    ct_->SetSelEnd( ct_->SelStart());


	LONG acs = ct_->SelStart();

    pTextStore_->OnTextChange(acs, nSelOldEnd, acs);
    pTextStore_->OnSelectionChange();

    return TRUE;
}
 
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
int CTextEditor::CurrentCaretPos()
{
    if ( ct_ )
	    return (ct_->bSelTrail_ ? ct_->SelEnd() : ct_->SelStart() );
    return 0;
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditor::Render(D2DContext& cxt, DWRITE_TEXT_METRICS*ptm, ID2D1SolidColorBrush* br)
{	
	int zCaretPos = CurrentCaretPos();

    (*cxt)->GetTransform(&mat_);

    if ( layout_.bRecalc_ )
	{       
		if ( bri_ && bri_->GetType() == IBridgeTSFInterface::PASSWORD)
		{
			int len = min(50, ct_->GetTextLength());
			LPCWSTR s = L"**************************************************";
			layout_.CreateLayout(cxt, s, len, ct_->view_size_, ct_->bSingleLine_, zCaretPos, ct_->nStartCharPos_, cxt.tsf_text_format_);
		}
		else if ( ct_->bSingleLine_ )
		{
			LPCWSTR s = ct_->GetTextBuffer();
			UINT len = ct_->GetTextLength();
			
			layout_.CreateLayout(cxt, s, len, ct_->view_size_, ct_->bSingleLine_, zCaretPos, ct_->nStartCharPos_, cxt.tsf_text_format_);

		}
		else 
			layout_.CreateLayout(cxt, ct_->GetTextBuffer(), ct_->GetTextLength(), ct_->view_size_, ct_->bSingleLine_, zCaretPos, ct_->nStartCharPos_, cxt.tsf_text_format_);


		layout_.bRecalc_ = false;
	}

	int selstart = (int)ct_->SelStart() - ct_->nStartCharPos_;
	int selend = (int)ct_->SelEnd() - ct_->nStartCharPos_;

	layout_.Render(cxt,br, ct_->rc_, ct_->GetTextBuffer(), (int)ct_->GetTextLength(), selstart, selend,ct_->bSelTrail_,pCompositionRenderInfo_, nCompositionRenderInfo_);

	

    *ptm = layout_.GetTextMetrics();
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditor::CalcRender(D2DContext& cxt )
{
	int x = 0;
	
    if (bri_->GetType() == IBridgeTSFInterface::PASSWORD)
    {
        int len = min(50, ct_->GetTextLength());
        LPCWSTR s = L"**************************************************";
        layout_.CreateLayout(cxt, s, len, ct_->view_size_, ct_->bSingleLine_, 0, ct_->nStartCharPos_, cxt.tsf_text_format_);
    }
    else
	    layout_.CreateLayout(cxt, ct_->GetTextBuffer(), ct_->GetTextLength(), ct_->view_size_, ct_->bSingleLine_,0, x, cxt.tsf_text_format_);	

	layout_.bRecalc_ = false;
}


RECT CTextEditor::CandidateRect(RECT rclog) const
{    
     if (pmat_) 
	 {
		FRectF xrc = pmat_->LPtoDP(rclog);
		RECT ret = this->ClientToScreen( xrc.GetRECT());

		//TRACE( L"CTextEditor::CandidateRec %f %f\n", xrc.top, xrc.bottom);
		return ret;
     }

	 //TRACE( L"???? CTextEditor::CandidateRec %d %d\n", rclog.top, rclog.bottom);
     return rclog;
}



BOOL CTextEditor::IsImeOn() const
{
	ComPTR<ITfCompartmentMgr>  pCompartmentMgr;
	BOOL isIMEOn = FALSE;

	if (0 == weak_tmgr_->QueryInterface(IID_ITfCompartmentMgr, (LPVOID*)&pCompartmentMgr))
	{
		ComPTR<ITfCompartment> cp1;

		if (S_OK == pCompartmentMgr->GetCompartment(GUID_COMPARTMENT_KEYBOARD_OPENCLOSE, &cp1))
		{
			VARIANT v;
			cp1->GetValue(&v);

			isIMEOn = !(v.lVal == 0);

		}
	}
	return isIMEOn;
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------



void CTextEditor::SetFocus(D2DMat* pmat)
{
    if (pDocumentMgr_)
    {
        if ( pmat )
			pmat_ = pmat;

        weak_tmgr_->SetFocus(pDocumentMgr_);
    }
}


//----------------------------------------------------------------
//
// 文字エリアの実設定
//
//----------------------------------------------------------------
void CTextEditor::Reset( IBridgeTSFInterface* bi )
{ 	
	bri_ = bi;
	
	if ( bi )
	{
        FRectF rc = bi->GetClientRect();

		ct_->rc_ = rc;		
        ct_->view_size_.cx = 99999;
		ct_->view_size_.cy =(LONG)(rc.bottom - rc.top);

        if ( bi->GetType() == IBridgeTSFInterface::MULTILINE )
            ct_->view_size_.cx = (LONG)(rc.right - rc.left);
	}
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

void CTextEditor::ClearCompositionRenderInfo()
{
    if (pCompositionRenderInfo_)
    {
        LocalFree(pCompositionRenderInfo_);
        pCompositionRenderInfo_ = NULL;
        nCompositionRenderInfo_ = 0;
    }
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditor::OnComposition( int msg, int len )
{
    switch( msg ) 
    {
        case 1:
        {
            ti_.decoration_typ = 1;

            ti_.decoration_start_pos = GetSelectionStart();
            ti_.decoration_end_pos = GetSelectionEnd();
        }
        break;
        case 2:
        {            
            ti_.decoration_start_pos = max(0, GetSelectionEnd() - len);
            ti_.decoration_end_pos =GetSelectionEnd();
        }
        break;
        case 3:
        {
            ti_.decoration_typ = 0;
            ti_.decoration_end_pos = 0;
            ti_.decoration_start_pos = 0;
        }
        break;
    }
}

void CTextEditor::OnChangeIME(bool bOn)
{
	#define WM_D2D_ONIME_ONOFF (WM_APP+20)

	SendMessage(GetWnd(),WM_D2D_ONIME_ONOFF, (bOn?1:0), 0);

}

RECT CTextEditor::ClientToScreen(RECT rc) const
{    

#ifdef _WINDOWS
    ::ClientToScreen(GetWnd(), (POINT *)&rc.left);
    ::ClientToScreen(GetWnd(), (POINT *)&rc.right);
    return rc;
#else

    //DIP value = (physical pixel x 96) / DPI
    //physical pixel value = (DIP x DPI) / 96

    auto bnd = CoreWindow::GetForCurrentThread().Bounds(); // DIP

    auto view = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();


    rc.left += (LONG)bnd.X;
    rc.top += (LONG)bnd.Y;
    rc.right += (LONG)(bnd.X);
    rc.bottom += (LONG)(bnd.Y);

    auto f = [view](LONG a)->LONG
    {
        return (LONG)(a * view.LogicalDpi() / 96.0f);
    };

    rc.left = f(rc.left);
    rc.top = f(rc.top);
    rc.right = f(rc.right);
    rc.bottom = f(rc.bottom);

    return rc; // physical pixel
#endif

}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

BOOL CTextEditor::AddCompositionRenderInfo(int nStart, int nEnd, TF_DISPLAYATTRIBUTE *pda)
{
    if (pCompositionRenderInfo_)
    {
        void *pvNew = LocalReAlloc(pCompositionRenderInfo_, 
                                   (nCompositionRenderInfo_ + 1) * sizeof(COMPOSITIONRENDERINFO),
                                   LMEM_MOVEABLE | LMEM_ZEROINIT);
        if (!pvNew)
            return FALSE;

        pCompositionRenderInfo_ = (COMPOSITIONRENDERINFO *)pvNew;
    }
    else
    {
        pCompositionRenderInfo_ = (COMPOSITIONRENDERINFO *)LocalAlloc(LPTR,
                                   (nCompositionRenderInfo_ + 1) * sizeof(COMPOSITIONRENDERINFO));
        if (!pCompositionRenderInfo_)
            return FALSE;
    }
    pCompositionRenderInfo_[nCompositionRenderInfo_].nStart = nStart;
    pCompositionRenderInfo_[nCompositionRenderInfo_].nEnd = nEnd;
    pCompositionRenderInfo_[nCompositionRenderInfo_].da = *pda;
    nCompositionRenderInfo_++;

    return TRUE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef _WINDOWS

void CTextEditorCtrl::SetContainer( CTextContainer* ct, IBridgeTSFInterface* brt )
{
	CTextEditor::SetContainer(ct);

	Reset(brt);

    if ( ct )
    {
        ct->SetSelStart(0);
        ct->SetSelEnd(0);
    }

}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

HWND CTextEditorCtrl::Create(HWND hwndParent, ITfThreadMgr2* tmgr, TfClientId clientid)
{
    hWnd_ = hwndParent;
	
	InitTSF(hWnd_, tmgr, clientid);

	pTextEditSink_->OnChanged_ = std::bind(&CTextEditorCtrl::OnEditChanged, this );

	static CTextContainer dumy;
	SetContainer(  &dumy , NULL );

    return hWnd_;
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditorCtrl::OnEditChanged()
{
	// from CTextEditSink::OnEndEdit

    layout_.bRecalc_ = true;
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------
void CTextEditorCtrl::Clear()
{
    layout_.bRecalc_ = true;
    search_x_ = 0;
    ti_ = {};
}
//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

LRESULT CTextEditorCtrl::WndProc(TSFApp* d, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;

    if ( ct_ )
    {
        switch (message)
        {
            /*case WM_PAINT:
                   OnPaint(d->cxt_);
		    break;*/
            /*case WM_SETFOCUS:
                focusは別系統で
			    this->OnSetFocus(wParam, lParam);
		    break;*/
            case WM_KEYDOWN:
                if ( this->OnKeyDown(wParam, lParam) )
				    ret = 1;
		    break;
            case WM_LBUTTONDOWN:
            {
                search_x_ = 0;
                MouseParam* pm = (MouseParam*)lParam;
                D2DMatrix m(pm->mat);
                FPointF pt = m.DPtoLP(pm->pt);

                this->OnLButtonDown( pt.x, pt.y );
                ret = 1;
            }
		    break;
            case WM_LBUTTONUP:
            {
                MouseParam* pm = (MouseParam*)lParam;

                D2DMatrix m(pm->mat);
                FPointF pt = m.DPtoLP(pm->pt);
                this->OnLButtonUp(pt.x, pt.y);
                ret = 1;
            }
		    break;
            case WM_MOUSEMOVE:           
            {
                MouseParam* pm = (MouseParam*)lParam;

                D2DMatrix m(pm->mat);
                FPointF pt = m.DPtoLP(pm->pt);
                bool bl = wParam & MK_LBUTTON;
                this->OnMouseMove(pt.x, pt.y, bl);
                ret = 1;
            }
		    break;
            case WM_CHAR:
                // wParam is a character of the result string. 
			    bool bControlKey = (GetKeyState(VK_CONTROL) & 0x80) != 0;			  
			    if ( bControlKey )
				    return 0;

                WCHAR wch = (WCHAR)wParam;
			    // ansi charcter input.
                bool bMultiline = (bri_->GetType() == IBridgeTSFInterface::MULTILINE);
                if ( bMultiline )
                {
                    if ( wch >= L' ' ||  (wch == L'\r'&& !ct_->bSingleLine_ ) || wch == L'\t' ) 
                    {				
                        if ( wch < 256 )
                        {
                            if ( wch == L'\r' )
                                wch = L'\n';

                            const WCHAR wc[2] = { wch, 0 }; 
                            this->InsertAtSelection(wc);
                            ret = 1;
                        }				   
                    }
                }
                else
                {
			        if ( wch >= L' ' ||  wch == L'\t' ) 
                    {				
				        if ( wch < 256 )
				        {
					        const WCHAR wc[2] = { wch, 0 }; 
		                    this->InsertAtSelection(wc);
                            ret = 1;
				        }				   
                    }
                }
                layout_.bRecalc_ = true;
		    break;
	    }
    }
    return ret;
}




//----------------------------------------------------------------
// 
//
//
//----------------------------------------------------------------

BOOL CTextEditorCtrl::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
    BOOL ret = true; 

	bool pushShift   = (GetKeyState(VK_SHIFT)& 0x80) != 0;

	int nSelStart;
    int nSelEnd;
    switch (0xff & wParam)
    {
        case VK_LEFT:
             if (pushShift)
             {                 				 
				 nSelStart = GetSelectionStart();
                 nSelEnd = GetSelectionEnd();
                 if (nSelStart > 0)
                 {					
					if ( nSelStart == nSelEnd )
						ct_->bSelTrail_ = false;

					if ( ct_->bSelTrail_ )
						MoveSelection(nSelStart, nSelEnd-1, true );
					else
						MoveSelection(nSelStart - 1, nSelEnd, false);											
                 }
             }
             else
             {
                 MoveSelectionPrev();
             }
             search_x_ = 0;
		break;

        case VK_RIGHT:
             if (pushShift)
             {
                 nSelStart = GetSelectionStart();
                 nSelEnd = GetSelectionEnd();

				 if ( nSelStart == nSelEnd )
						ct_->bSelTrail_ = true;

				if ( ct_->bSelTrail_ )
					MoveSelection(nSelStart, nSelEnd + 1,true);
				else
					MoveSelection(nSelStart+1, nSelEnd,false);
             }
             else
             {
                 MoveSelectionNext();
             }
             search_x_ = 0;
		break;

        case VK_UP:
             ret = MoveSelectionUpDown(TRUE, pushShift);
		break;

        case VK_DOWN:
             ret = MoveSelectionUpDown(FALSE, pushShift);
		break;

        case VK_HOME:
             ret = MoveSelectionToLineFirstEnd(TRUE, pushShift);
		break;

        case VK_END:
             ret = MoveSelectionToLineFirstEnd(FALSE, pushShift);
		break;

        case VK_DELETE:
             nSelStart = GetSelectionStart();
             nSelEnd = GetSelectionEnd();
             if (nSelStart == nSelEnd)
             {
                 DeleteAtSelection(FALSE);
             }
             else
             {
                 DeleteSelection();
             }
             
		break;

        case VK_BACK:
             nSelStart = GetSelectionStart();
             nSelEnd = GetSelectionEnd();
             if (nSelStart == nSelEnd)
             {
                 DeleteAtSelection(TRUE);
             }
             else
             {
                 DeleteSelection();
             }
             
		break;
		case VK_ESCAPE:
			nSelEnd = GetSelectionEnd();
			MoveSelection(nSelEnd, nSelEnd);
		break;
    }



	return ret;
}



//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

void CTextEditorCtrl::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
    //SetFocus();
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

void CTextEditorCtrl::OnLButtonDown(float x, float y)
{
    FPointF pt;
    SelDragStart_ = -1;
    pt.x = x;
    pt.y = y; 

    if (MoveSelectionAtPoint(pt))
    {
        InvalidateRect();
        SelDragStart_ = GetSelectionStart();
    }
	else
	{
		int end = ct_->SelEnd();
		MoveSelection( end, end, true);

	}
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

void CTextEditorCtrl::OnLButtonUp(float x, float y)
{
    int nSelStart = GetSelectionStart();
    int nSelEnd = GetSelectionEnd();
    FPointF pt;    
    pt.x = x;
    pt.y = y;

    if (MoveSelectionAtPoint(pt))
    {
        int nNewSelStart = GetSelectionStart();
        int nNewSelEnd = GetSelectionEnd();

		auto bl = true;
			if ( nNewSelStart < SelDragStart_)
				bl = false;

        MoveSelection(min(nSelStart, nNewSelStart), max(nSelEnd, nNewSelEnd),bl); 
        InvalidateRect();

        SelDragStart_ = GetSelectionStart();
    }
}

//----------------------------------------------------------------
//
//
//
//----------------------------------------------------------------

void CTextEditorCtrl::OnMouseMove(float x, float y, bool bLbutton)
{   
    if( bLbutton)
    {
        FPointF pt;
        pt.x = x;
        pt.y = y;

        if (MoveSelectionAtPoint(pt))
        {
            int nNewSelStart = GetSelectionStart();
            int nNewSelEnd = GetSelectionEnd();

			auto bl = true;
			if ( nNewSelStart < SelDragStart_)
				bl = false;

            MoveSelection(min(SelDragStart_, nNewSelStart), max(SelDragStart_, nNewSelEnd), bl); 
            InvalidateRect();
        }
    }
}

#endif

//////////////////////////////////////////////////////////////////////////////
//
// CTextEditSink
//
//////////////////////////////////////////////////////////////////////////////
#include "DisplayAttribute.h"
#pragma region IUnknown
//+---------------------------------------------------------------------------
//
// IUnknown
//
//----------------------------------------------------------------------------

STDAPI CTextEditSink::QueryInterface(REFIID riid, void **ppvObj)
{
    *ppvObj = NULL;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ITfTextEditSink))
    {
        *ppvObj = (ITfTextEditSink *)this;
    }

    if (*ppvObj)
    {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDAPI_(ULONG) CTextEditSink::AddRef()
{
    return ++_cRef;
}

STDAPI_(ULONG) CTextEditSink::Release()
{
    long cr;

    cr = --_cRef;

    if (cr == 0)
    {
        delete this;
    }

    return cr;
}
#pragma endregion
//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------
#define TES_INVALID_COOKIE  ((DWORD)(-1))

CTextEditSink::CTextEditSink(CTextEditor *pEditor)
{
    _cRef = 1;
    _dwEditCookie = TES_INVALID_COOKIE;
    _pEditor = pEditor;
}

//+---------------------------------------------------------------------------
//
// EndEdit
//
//----------------------------------------------------------------------------

STDAPI CTextEditSink::OnEndEdit(ITfContext *pic, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord)
{
#ifdef _WINDOWS
    CDispAttrProps *pDispAttrProps = GetDispAttrProps();
    if (pDispAttrProps)
    {
        IEnumTfRanges *pEnum;
        if (SUCCEEDED(pEditRecord->GetTextAndPropertyUpdates(TF_GTP_INCL_TEXT,
                                                             pDispAttrProps->GetPropTablePointer(),
                                                             pDispAttrProps->Count(),
                                                             &pEnum)) && pEnum)
        {
            ITfRange *pRange;
            if (pEnum->Next(1, &pRange, NULL) == S_OK)
            {
                // We check if there is a range to be changed.
                pRange->Release();

                _pEditor->ClearCompositionRenderInfo();

                // We read the display attribute for entire range.
                // It could be optimized by filtering the only delta with ITfEditRecord interface. 
                ITfRange *pRangeEntire = NULL;
                ITfRange *pRangeEnd = NULL;
                if (SUCCEEDED(pic->GetStart(ecReadOnly, &pRangeEntire)) &&
                    SUCCEEDED(pic->GetEnd(ecReadOnly, &pRangeEnd)) &&
                    SUCCEEDED(pRangeEntire->ShiftEndToRange(ecReadOnly, pRangeEnd, TF_ANCHOR_END)))
                {
                    IEnumTfRanges *pEnumRanges;
                    ITfReadOnlyProperty *pProp = NULL;

                    GetDisplayAttributeTrackPropertyRange(ecReadOnly, pic, pRangeEntire, &pProp, pDispAttrProps);

                    if (SUCCEEDED(pProp->EnumRanges(ecReadOnly, &pEnumRanges, pRangeEntire)))
                    {
                        while (pEnumRanges->Next(1, &pRange, NULL) == S_OK)
                        {
                            TF_DISPLAYATTRIBUTE da;
                            TfGuidAtom guid;
                            if (GetDisplayAttributeData(ecReadOnly, pProp, pRange, &da, &guid) == S_OK)
                            {
                                ITfRangeACP *pRangeACP;
                                if (pRange->QueryInterface(IID_ITfRangeACP, (void **)&pRangeACP) == S_OK)
                                {
                                    LONG nStart;
                                    LONG nEnd;
                                    pRangeACP->GetExtent(&nStart, &nEnd);
                                    
									
									_pEditor->AddCompositionRenderInfo(nStart, nStart + nEnd, &da);


                                    pRangeACP->Release();
                                }
                            }
                        }
                    }
                }

                if (pRangeEntire)
                    pRangeEntire->Release();
                if (pRangeEnd)
                    pRangeEnd->Release();
 
            }
            pEnum->Release();
        }

        delete pDispAttrProps;
    }
#endif
	if ( OnChanged_ )
		OnChanged_();

	

    return S_OK;
}

#pragma region Advise_Unadvice
//+---------------------------------------------------------------------------
//
// CTextEditSink::Advise
//
//----------------------------------------------------------------------------

HRESULT CTextEditSink::_Advise(ITfContext *pic)
{
    HRESULT hr;
    ITfSource *source = NULL;

    _pic = NULL;
    hr = E_FAIL;

    if (FAILED(pic->QueryInterface(IID_ITfSource, (void **)&source)))
        goto Exit;

    if (FAILED(source->AdviseSink(IID_ITfTextEditSink, (ITfTextEditSink *)this, &_dwEditCookie)))
        goto Exit;

    _pic = pic;
    _pic->AddRef();

    hr = S_OK;

Exit:
    if (source)
        source->Release();
    return hr;
}

//+---------------------------------------------------------------------------
//
// CTextEditSink::Unadvise
//
//----------------------------------------------------------------------------

HRESULT CTextEditSink::_Unadvise()
{
    HRESULT hr;
    ITfSource *source = NULL;

    hr = E_FAIL;

    if (_pic == NULL)
        goto Exit;

    if (FAILED(_pic->QueryInterface(IID_ITfSource, (void **)&source)))
        goto Exit;

    if (FAILED(source->UnadviseSink(_dwEditCookie)))
        goto Exit;

    hr = S_OK;

Exit:
    if (source)
        source->Release();

    if (_pic)
    {
        _pic->Release();
        _pic = NULL;
    }

    return hr;
}
#pragma endregion