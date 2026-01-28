//---------------------------------------------------------------------------

#include "WCout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#define DEBUG_MODE 1

TWCout WCout;

//enum MODE { OFF, ON, ACTIVE, ENABLED, INACTIVE, DISABLED};         //TWCout State


TWCout& TWCout::operator<<(const Txt& Param) { return AddText(Param); }
TWCout& TWCout::operator<<(const double& Param) { return AddDouble(Param); }
TWCout& TWCout::operator<<(const int& Param) { return AddInt(Param);}
TWCout& TWCout::operator<<(const char Param) { return AddChar(Param); }
TWCout& TWCout::operator<<(SPACES SPS) { return AddTrailingSpaces(SPS); }
TWCout& TWCout::operator<<(FFloat FF) { return setFloatingPointFormat(FF); }   //Format Floats
TWCout& TWCout::operator<<(DEBUGMode DBItem) {   return setDBMode(DBItem); }
TWCout& TWCout::operator<<(wcSTATUS appState) {   return setAppState(appState); }
TWCout& TWCout::operator<<(AUTOSPACEMode AUtoSP) { return setAUTOSpace(AUtoSP);   }
TWCout& TWCout::operator<<(INTWidth FI)  { return setINTWidth(FI);  }
TWCout& TWCout::operator<<(const char* CP) { return AddText(UnicodeString(CP)); }




TWCout& TWCout::operator<<(TShow SHOW)
{
if (isWCOUTDisabled) return *this;
  ShowMessage(get());
  ClearText();
  return *this;

}

void TWCout::Enable() {
  isWCOUTDisabled=false;
}


bool TWCout::isDisabled()
{
  return isWCOUTDisabled;
}



//enum MsgType { MSGInfo, MSGError, MSGConfirm, MSGWarning };
TWCout& TWCout::operator<<(TShowOperator SHOWStyle) {
SHOWType T = SHOWStyle.T;
int IconType;
		 if (T==Info) { MsgInfo(get()); }
	else if (T==Error) { MsgError(get()); }
	else if (T==Warning) { MsgWarning(get()); }
 //	else if (T==Confirm) { MsgConfirmation(get()); }
	else if (T==Hand) { MsgHand(get()); }
	else if (T==Stop) { MsgStop(get());  }
 //	else if (T==Question) { MsgInquiry(get()); }
	else ShowMessage(get());

	ClearText();
  return *this;
}

//Standard MessageBox with true/false return type
//Buttons can be Yes/NO     OR   OK/Cancel
bool TWCout::DLGConfirm(const UnicodeString &msg,
					   const UnicodeString &caption, ConfirmButtons Cb)
{
if (isDisabled()) return false;
int MB_BtnType;                     // btnYES_NO,  btnOK_CANCEL

if (Cb==btnYES_NO) MB_BtnType=MB_YESNO;
else               MB_BtnType=MB_OKCANCEL;




	int r = ::MessageBoxW(
		Application->Handle,
		msg.c_str(),
		caption.c_str(),
		MB_BtnType | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_APPLMODAL
	);
	return (r == IDYES || r==IDOK);
}


// CUSTOM MesssageBox with user buttons and Icon
//Call windows Dialog box with your own options.
//OR_Buttons = MB_OK  MB_OKCANCEL MB_YESNO  MB_YESNOCANCEL MB_ABORTRETRYIGNORE
//OR_Icons = MB_OK, MB_ICONWARNING, MB_ICONERROR, MB_ICONERROR
//returns    IDOK or IDCANCEL or IDYES or IDNO


int TWCout::MessageBox(const Txt& theMessage, const Txt& Caption, int BIT_Flags) {
if (isWCOUTDisabled) {  return -1; }


	int returnVal = ::MessageBoxW(
		Application->Handle,
		theMessage.c_str(),
		Caption.c_str(),
		BIT_Flags
	);
	return returnVal;

}



Txt TWCout::operator()() {
  return get();
}

Txt TWCout::get() {
  return ACCst;
}

const Txt ENDofLINE=L"\r\n";
const Txt TABMark = L"\t";
void TWCout::AddMoreText(const Txt& MoreText)
{
	  if (isWCOUTDisabled==true) {
		return;
	  }


	  if (MoreText.IsEmpty()) {
		return;
	  }

	  ACCst+=MoreText;
	  if (isAutoSPACEInsert) {
		  if (MoreText[MoreText.Length()] != ' ') {
			if (MoreText == ENDofLINE || MoreText==TABMark) {  //Don't add space beginning of new line
			  return;
			}
			ACCst+=L" ";  	//Add a space if does not end in space
		  }
	  }
}

TWCout& TWCout::setAUTOSpace(AUTOSPACEMode AS) {
	 if (AS.Mode==ON) {
	   isAutoSPACEInsert=true;
	 }
	 if (AS.Mode==OFF) {
	   isAutoSPACEInsert=false;
	 }
	 return *this;
}




TWCout& TWCout::AddText(const UnicodeString& theText) {
if (isWCOUTDisabled) return *this;

   Txt Temp=theText;

		if (isStringWidthOn) {
		  Temp.sprintf(L"%-*s", StringWidth, Temp.c_str());
		  AddMoreText(Temp);

		} else {
		  AddMoreText(Temp);

		}

		 if (isTrailingSpace) {
		   AddMoreText(TxtSpaces);

		}


		return *this;
	}





TWCout& TWCout::AddInt(int IR) {
Txt iText;
	if (INTEGERWidth == 0) {
	  iText=UnicodeString(IR);
	} else {
	  iText.printf(L"%*d",INTEGERWidth, IR);
	}
  AddMoreText(iText);
  return *this;
}

//enum STATE { OFF, ON, DISABLE, ENABLE };



TWCout& TWCout::setAppState(wcSTATUS theStatus)
{
enum MODE Mode = theStatus.SM;


  //if (Mode==OFF || Mode==DISABLED || Mode==INACTIVE) {
   if (Mode==OFF) {
	isWCOUTDisabled=true;
 // } else if ((Mode==ON || Mode==ENABLED || Mode==ACTIVE) ) {
  } else if (Mode==ON) {
	isWCOUTDisabled=false;
   //	isDisabled=false;
  } else if (Mode==RESET) {
	ResetAll();
    return *this;
  }

  /****
  if (isWCOUTDisabled) {
	ShowMessage(L" WCout is Disabld ");
  } else {
	ShowMessage(L" WCout is ENAbled ");
  }
****/
  return *this;
}

void TWCout::ResetAll()
{
 isFORMATFloatOn=false;
 isDEBUGModeOn=false;
 isWCOUTDisabled=false;      //WCout <<
 isAutoSPACEInsert=false;
 isTrailingSpace=false;
 isStringWidthOn=false;
}



//FF 7.3   .3  for 3 decimals could be 2.999999 (int)part=2;

TWCout& TWCout::AddDouble(double dblVAL) {

Txt Temp;
   if (isFORMATFloatOn) {
	 Temp.printf(L"%*.*f", FloatWidth, FloatPrecis, dblVAL);
   } else {
	 Temp=(Txt)dblVAL;
	}

	AddMoreText(Temp);
	return *this;
}

TWCout& TWCout::operator<<(STWIDTH STRWIDTHOperator) {
StringWidth = STRWIDTHOperator.StringWidth;

#ifdef DEBUG_MODE
  ShowMessage(L" String Width set at "+(Txt)StringWidth);
#endif
  isStringWidthOn=StringWidth>0;
  return *this;
}

 //TWCout& operator<<(DEBUGMode DBItem);  //W-10,  W-30


//enum DEBUGenum { DB=100, db, ACTIVE, INACTIVE };
TWCout& TWCout::setDBMode(DEBUGMode DBItem) {
int DEBUGenum = DBItem.Item;
//ShowMessage(L"Add db item "+(Txt)DEBUGenum);
bool isAPP=DEBUGenum>=100;
  if (isAPP) {
		if (DEBUGenum==ON) {
		  isDEBUGModeOn=true;
		} else if (DEBUGenum==OFF) {
		  isDEBUGModeOn=false;
		}
  } else {



  }
  return *this;
}




TWCout& TWCout::AddTrailingSpaces(SPACES SPACESOperator) {
	 TrailingSpaces = SPACESOperator.NumberOfSpaces;

	 if (TrailingSpaces>0) {
	   isTrailingSpace=true;
	   TxtSpaces=L"";
	   for(int i=0; i< TrailingSpaces; i++) {
		 TxtSpaces+=L" ";
       }
	 } else {
	   isTrailingSpace=false;
	 }
#ifdef DEBUG_MODE
	 ShowMessage(L" Traiing Spaces specified "+(Txt)TrailingSpaces);
#endif
	 return *this;
 }

void TWCout::SendToClipboard()
{
if (isWCOUTDisabled) return;

    // Assuming get() returns Embarcadero UnicodeString (UTF-16)
    UnicodeString s = get();
    const int lenChars = s.Length();        // number of UTF-16 code units (not counting null)

    if (lenChars <= 0) return;

    if (!OpenClipboard(nullptr))
        return;

    // Clear current clipboard contents
    EmptyClipboard();

    // Allocate global memory for UTF-16 text including null terminator
    const SIZE_T bytes = (static_cast<SIZE_T>(lenChars) + 1) * sizeof(wchar_t);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (!hMem)
    {
        CloseClipboard();
        return;
    }

    wchar_t* dst = static_cast<wchar_t*>(GlobalLock(hMem));
    if (!dst)
    {
        GlobalFree(hMem);
        CloseClipboard();
        return;
    }

    // Copy UnicodeString data to clipboard buffer
    // UnicodeString::c_str() returns a null-terminated UTF-16 string.
    lstrcpyW(dst, s.c_str());

    GlobalUnlock(hMem);

    // Clipboard now owns hMem if SetClipboardData succeeds
    if (!SetClipboardData(CF_UNICODETEXT, hMem))
    {
        GlobalFree(hMem); // only free if SetClipboardData failed
    }

	CloseClipboard();

}

void TWCout::ClearText() {
  ACCst=L"";

}


void TWCout::operator>>(Txt& theText)
{
  theText = get();
}

void TWCout::operator>>(CLIP b)
{
  SendToClipboard();
}


TWCout& TWCout::operator<<(ACTION w)
{
  if (isWCOUTDisabled) return *this;

//enum WHAT { EL, el, NL, TAB, SHOW, CLIPBOARD, CLEAR };
  switch (w) {     //NEW Line
	case EL:
	case NL:
	case L:
	  AddMoreText(ENDofLINE);
	  break;
  case TAB:        //Tab
	  AddMoreText(TABMark);
	  break;
  case SHOWMSG:
	  ShowMessage(get());   //SHows but does NOT Clear Buffer
	  ClearText();
	  break;
  case CLEAR:
	   ClearText();           //Clear Text, New Text
	  break;
   default:
	  break;

  }

  return *this;
}

TWCout& TWCout::AddChar(char theChar) {
  AddMoreText(theChar);
  return *this;
}

#include <System.StrUtils.hpp>    // for ReplaceStr (sometimes preferred)
#include <algorithm>             // for std::max (and std::clamp if available)

//#define DB_PARSE_FLOAT_FORMAT
void TWCout::FFParseFloatSpec(double dblSpec)
{
    if (dblSpec < 0.0)
    {
        MsgError(L"Negative float format invalid: FF-" + UnicodeString(dblSpec));
        isFORMATFloatOn = false;
        return;
    }

    // ------------------------------------------------------------------------
    // 1. Convert to string with high but finite precision
    //    %.9f is usually enough — most people don't write FF-12.3456789
    // ------------------------------------------------------------------------
	UnicodeString s = UnicodeString::FormatFloat(L"%.9f", dblSpec);

    // Remove scientific notation if it appears (very rare for small widths)
	//s = s.UpperCase().Replace(L"E+", L"").Replace(L"E-", L"-").Replace(L"E", L"");

  s = ReplaceStr(s, L"E+", L"");
  s = ReplaceStr(s, L"E-", L"-");
  s = ReplaceStr(s, L"E", L"");





    // Trim trailing zeros after decimal point
    int dotPos = s.Pos(L'.');
    if (dotPos > 0)
    {
        while (s.Length() > dotPos && s[s.Length()] == '0')
			s.SetLength(s.Length() - 1);

        // If only '.' remains at the end → remove it
        if (s.Length() == dotPos)
            s.SetLength(s.Length() - 1);
    }

    // ------------------------------------------------------------------------
    // 2. Split into integer + fractional part
    // ------------------------------------------------------------------------
    dotPos = s.Pos(L'.');

    int width = 0;
    int prec  = 0;

    if (dotPos == 0)
    {
        // No decimal part given → width only, default precision
        width = s.ToIntDef(0);
        prec  = 2;                // ← your choice: 0, 1, 2, ...
    }
    else
    {
        UnicodeString intPart  = s.SubString(1, dotPos - 1);
        UnicodeString fracPart = s.SubString(dotPos + 1, 999);

        width = intPart.ToIntDef(0);
        prec  = fracPart.ToIntDef(0);

        // If user wrote FF-12.10 we usually want prec=2, not prec=10
        // So take only the significant digits (stop at first non-zero or limit length)
        if (fracPart.Length() > 1)
        {
            // Option A: take only first 1-2 digits (most common intent)
            prec = fracPart.SubString(1, 2).ToIntDef(0);   // handles 10 → 1, but 10 → 10 ok

            // Option B: trim trailing zeros in fracPart too (stricter)
            // int lastNonZero = fracPart.Length();
            // while (lastNonZero > 0 && fracPart[lastNonZero] == '0') --lastNonZero;
            // prec = fracPart.SubString(1, lastNonZero).ToIntDef(0);
        }
    }

    // ------------------------------------------------------------------------
    // 3. Apply sane limits
    // ------------------------------------------------------------------------
	FloatWidth     = std::max(0, width);
    FloatPrecis    = std::clamp(prec, 0, 8);   // 8 or 9 is usually plenty
    isFORMATFloatOn = (FloatWidth > 0 || FloatPrecis > 0);

#ifdef DB_PARSE_FLOAT_FORMAT
    UnicodeString debugMsg = L"FF input: " + UnicodeString(dblSpec) +
                             L"  → string: \"" + s + L"\"" +
                             L"  → width=" + UnicodeString(FloatWidth) +
                             L" prec=" + UnicodeString(FloatPrecis);
    ShowMessage(debugMsg);
#endif
}

TWCout& TWCout::setFloatingPointFormat(FFloat FFOperator) {  //Format Float


  if (isWCOUTDisabled) return *this;
  double FloatSpec = FFOperator.FFloat;
#ifdef DB_PARSE_FLOAT_FORMAT
  ShowMessage(L"setFloatingPointFormat  Float Spec = "+(Txt)FloatSpec);
#endif
  FFParseFloatSpec(FloatSpec);
  return *this;

}


//To maintain consistent FF-OFF,  FF-ON  syntax with other options
//FF-7.5   expects floating point Param OFF enum allready defined
// Can't reuse the numeric overload (FF-7.2) because OFF/ON are enum MODE values.
TWCout& TWCout::operator<<(FFloatSwitch FF)
{
  MODE M = FF.Mode;
  if (M==OFF || M==RESET) { isFORMATFloatOn = false; }
  else if (M==ON) {
	if (FloatWidth <= 0)  FloatWidth = 0;   // or 7
	if (FloatPrecis < 0 || FloatPrecis > 9) FloatPrecis = 2;
	isFORMATFloatOn = true;

  }
  return *this;

}

//(INTWidth FI)  { return setINTWidth(FI);  }
TWCout& TWCout::setINTWidth(INTWidth FI) {

  if (FI.Width>10) { INTEGERWidth = 10; }
  else INTEGERWidth = FI.Width;
  //ShowMessage(L"Calling set int Width  "+(Txt)INTEGERWidth);

 // AddMoreText(Temp);
  return *this;
}


void TWCout::DLGMsg(int MBCombo, const UnicodeString &msg,
					const UnicodeString &caption)
{
if (isDisabled()) return;

	::MessageBoxW(
		Application->Handle,
		msg.c_str(),
		caption.c_str(),
		MBCombo
	);
}




void TWCout::MsgError(const UnicodeString &msg,
					 const UnicodeString &caption)
{
   DLGMsg(MB_OK | MB_ICONERROR | MB_APPLMODAL, msg, caption);
}

void TWCout::MsgInfo(const UnicodeString &msg,
					const UnicodeString &caption)
{
	DLGMsg(MB_OK | MB_ICONINFORMATION | MB_APPLMODAL,  msg, caption);

}


void TWCout::MsgWarning(const UnicodeString &msg,
					 const UnicodeString &caption)
{
   DLGMsg(MB_OK | MB_ICONWARNING | MB_APPLMODAL,  msg, caption);
}

void TWCout::MsgStop(const UnicodeString &msg,
					 const UnicodeString &caption)
{
   DLGMsg(MB_OK | MB_ICONSTOP | MB_APPLMODAL,  msg, caption);
}
// Confirmation (Yes/No with question icon)
//   bool MsgConfirm(const Txt& ConfirmationMessage);
void TWCout::MsgHand(const UnicodeString &msg,
					 const UnicodeString &caption)
{
   DLGMsg(MB_ICONHAND | MB_APPLMODAL,  msg, caption);
}
