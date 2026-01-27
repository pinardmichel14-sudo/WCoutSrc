//---------------------------------------------------------------------------
#pragma hdrstop


//Space pad  RPAD-3, RPAD-10,  rpad-2
enum WIDTH { W, w };         	//String Width
enum FLOATFormat { FF, ff };   	//Format Floats  FF-7.2
enum INTFormat { FI, fi };      //Format Ints   FI-6,  Fi-10
enum DEBUGenum { DB=100, db };  //DEBUG Mode
enum TShow { SHOW };
enum ConfirmButtons { btnYES_NO,  btnOK_CANCEL };
enum CLIP { CLIPBOARD };

//const double NONE = { -1.0 };

enum STATE { STS, STATUS };         //TWCout State OFF nothing shown, used
enum MODE { OFF, ON, RESET };
struct wcSTATUS { STATE ST; MODE SM; };
wcSTATUS operator-(STATE State, MODE Mode) {
  return { State, Mode };
}
//WCout  << STATUS-OFF;  WCout << STATUS-ON;  WCout << STATUS-RESET...
//WCout << ST-OFF;  WCout << ST-ON;  WCout << ST-RESET...

struct FFloat { FLOATFormat FF; double FFloat; };
FFloat operator-(FLOATFormat FF, double FFloat) {
  return { FF, FFloat };                  // contains Width and Precis
} //  FF-7.2 Width 7, Decimals 2   FF-Width.Decimals
//WCout << FF-8.1  Width 8 decimals 1
///WCout << FF-8.5  Width 8, decimals 5

//2nd param to FFloat is floating point (OFF can NOT be send so
//use this instead to call behind the scenes
struct FFloatSwitch { FLOATFormat FF; MODE Mode; };
FFloatSwitch operator-(FLOATFormat FF, MODE Mode ) {
  return { FF, Mode };                  // contains Width and Precis
} //FF-OFF    or   FF-ON
//Float FF is passed 7.3 allow FF-ON,  FF-OFF like other types

struct INTWidth { INTFormat FI; int Width; };
INTWidth operator-(INTFormat FI, int Width ) {
  return { FI, Width };                  // contains Width and Precis
} //FI-3    or   fi-5

struct STWIDTH { WIDTH WT; int StringWidth; };
STWIDTH operator-(WIDTH STRW, int StringWidth) {
  return { STRW, StringWidth };
} ///WCout <<  W-30    /WCout <<  W-20    /WCout <<  W-32


enum SPACE { RPAD, rpad }; //AUTO = AFTER each << insert a space
struct SPACES { SPACE SP; int NumberOfSpaces; };
SPACES operator-(SPACE SP, int NUMSpaces) {
  return { SP, NUMSpaces };
}


struct DEBUGMode { DEBUGenum db; int Item; };
DEBUGMode operator-(DEBUGenum db, int ItemNO) {
  return { db, ItemNO  };
  }
enum ACTION { L, EL, NL, TAB, SHOWMSG, CLEAR };
//   NEW Line   WCout << EL, WCout << el, NL  insert NEW LINE
//   TAB        WCout << TAB          Insert TAb
//   SHOW       WCout << SHOW;        displays in ShowMessage AND Clears text
//   SHOWMSG    WCout << SHOWMSG      displays in ShowMessage BUT does NOT Clear
//   CLIPBOARD  WCout << CLIPBOARD    Paste to clipboard





enum TAUTOSPACE { AUTOSPACE };
struct AUTOSPACEMode { TAUTOSPACE ST; MODE Mode; };
AUTOSPACEMode operator-(TAUTOSPACE ST, MODE Mode) {
  return { ST, Mode };
}
// WCout << AUTOSPACE-ON,    WCout << AUTOSPACE-OFF

//Call windows Dialog box with your own options.
//OR_Buttons = MB_OK  MB_OKCANCEL MB_YESNO  MB_YESNOCANCEL MB_ABORTRETRYIGNORE
//OR_Icons = MB_OK, MB_ICONWARNING, MB_ICONERROR, MB_ICONERROR
struct DLGWin { int BIT_Flags; const UnicodeString& UserMsg;  };
DLGWin operator-(int BIT_Flags, const UnicodeString& UserMsg ) {
  return { BIT_Flags, UserMsg };
}

//COMMON windows Message boxes
//enum MsgType { MSGInfo, MSGError, MSGConfirm, MSGWarning, MSGStop, MSGHand };
enum SHOWType { Info, Error, Confirm, Warning, Stop, Hand, Question };
// WCout << L"Slippery road ahead " << SHOW-Warning;

struct TShowOperator { TShow SHOW; SHOWType T; };
TShowOperator operator-(TShow SHOW, SHOWType T ) {
  return { SHOW, T };    //T = Icon  Warning, Error, Info, Question
}



using Txt = UnicodeString;
class TWCout {
public:
//Operator >>  //sends all text to Unicode String or theClipboard
   void operator>>(Txt& theText);
   void operator>>(CLIP CLIPBOARD);

//Operator <<  adds string types, number types
   TWCout& operator<<(const Txt& Param);
   TWCout& operator<<(const char* CP);
   TWCout& operator<<(const double& Param);
   TWCout& operator<<(const int& Param);
   TWCout& operator<<(const char Param);

//Operator <<  adds formatting of strings, numeric types, modes of operation
   TWCout& operator<<(SPACES SPS);        //Trailing Spaces, << RPAD-4, RPAD-10
   TWCout& operator<<(FFloat FF);         //Format Float: FF-7.2,   FF-10.3
   TWCout& operator<<(FFloatSwitch FF);   //Switch Float: FF-OFF,   FF-ON
   TWCout& operator<<(INTWidth FI);       //Format Int: FI-5,   fi-10
   TWCout& operator<<(STWIDTH W); 	      //String Width W-10,  W-30
   TWCout& operator<<(DEBUGMode DBItem);  //
  // TWCout& operator<<(bool& B);
   TWCout& operator<<(ACTION a); // WCout << EL, el, TAB, SHOW, CLIPBOARD, CLEAR;
							   //          ---End Line-- SHOW - Clipb -  EmptyText

   TWCout& operator<<(AUTOSPACEMode s);  //Add space if words touching
										 //<< AUTOSPACE-ON / -OFF

   TWCout& operator<<(wcSTATUS s);  //<< STATUS-ON / -OFF
								   //OFF NOTHING sent };

//Operator << Displays message Dialogs or with chosen Icon
   TWCout& operator<<(TShow SHOW);
   TWCout& operator<<(TShowOperator SHOWType);
   //enum MsgType { MSGInfo, MSGError, MSGConfirm, MSGWarning };
   //Must send JUST BEFORE SHOW i.e. WCout << MSGInfo << SHOW;

   Txt operator()(); //retrieves ALL the text accumulated
   Txt get();  		//retrieves ALL the text accumulated
   bool getOPState() { return isWCOUTDisabled; }

////CUSTOMIZABLE MessageBox: User can specify own Buttons and Icons
//OR_Buttons = MB_OK  MB_OKCANCEL MB_YESNO  MB_YESNOCANCEL MB_ABORTRETRYIGNORE
//OR_Icons = MB_OK, MB_ICONWARNING, MB_ICONERROR, MB_ICONERROR


   int MessageBox(const Txt& Message, const Txt& Caption, int OR_Flags);
   //SAME order as MessageBoxW(Handle, Message, Caption, OR_Flags(Icon, Buttons))




   void SendToClipboard();
   //true user answered YES, false user answered NO
   bool isDisabled();
   void Enable();

bool DLGConfirm(const UnicodeString &msg,
					   const UnicodeString &caption = L"Confirm", ConfirmButtons CB=btnOK_CANCEL);
   //Simple Confirm:  Yes\No buttons   OR  Ok\Cancel buttons
   //Yes button returns true  No button returns false;
   //btnYesNO=false    OK / Cancel button displayed


protected:



bool isFORMATFloatOn=false;
bool isDEBUGModeOn=false;
bool isWCOUTDisabled=false;      //WCout <<
bool isAutoSPACEInsert=false;
bool isTrailingSpace=false;
bool isStringWidthOn=false;

   TWCout& AddInt(int IR);
   TWCout& AddDouble(double IR);
   TWCout& AddChar(char theChar);
   TWCout& AddText(const UnicodeString& theText);
   TWCout& AddTrailingSpaces(SPACES SP);

   TWCout& setFloatingPointFormat(FFloat CF);
   TWCout& setDBMode(DEBUGMode DBm);
   TWCout& setAppState(wcSTATUS st);
   TWCout& setAUTOSpace(AUTOSPACEMode As);    //Add space if words touching
   TWCout& setINTWidth(INTWidth Iw);

   int FloatWidth=5;
   int FloatPrecis=2;
   int INTEGERWidth=0;
   int TrailingSpaces=0;
   int StringWidth;

   void FFParseFloatSpec(double FF);
   virtual void AddMoreText(const Txt& MoreText);  //Inherit to use with USER enums
   void ClearText();
   void ResetAll();

   UnicodeString ACCst;
   Txt TxtSpaces { L"" };

   void DLGMsg(int MBCombo, const UnicodeString &msg,  const UnicodeString &caption = L"Information");

//All called by SHOW-Error,  SHOW-Warning,  SHOW-Info, SHOW-Question dislaying
//the matching Icon
	void MsgError(const UnicodeString &msg, const UnicodeString &caption = L"Error");
	void MsgInfo(const UnicodeString &msg,  const UnicodeString &caption = L"Information");
	void MsgWarning(const UnicodeString &msg, const UnicodeString &caption = L"Warning");
	void MsgStop(const UnicodeString &msg, const UnicodeString &caption = L"Stop");
	void MsgHand(const UnicodeString &msg,  const UnicodeString &caption = L"Alert");













};

extern TWCout WCout;





//---------------------------------------------------------------------------

