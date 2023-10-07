// ---------------------------------------------------------------------------

#ifndef KURSWORKH
#define KURSWORKH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <winuser.h>
#include "Windows.h"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include <string>

// ---------------------------------------------------------------------------
class TForm1 : public TForm {
__published: // IDE-managed Components
	TStringGrid *StringGrid1;
	TMainMenu *MainMenu1;
	TMenuItem *MenuFail;
	TMenuItem *MenuRedact;
	TMenuItem *Results;
	TMenuItem *Create;
	TMenuItem *Open;
	TMenuItem *Increase_Hours;
	TMenuItem *Calculate_Extremes;
	TMenuItem *Save_As;
	TMenuItem *Calculate_Sredn_Intensivn;
	TOpenDialog *OpenDialogFile;
	TSaveDialog *SaveDialogFile;
	TMemo *Memo;
	TStatusBar *SbInfo;
	TMenuItem *Delete_Hour;
	TMenuItem *Insert_Hour;
	TMenuItem *Add_Transport;
	TMenuItem *Delete_Transport;
	TMenuItem *Insert_Transport;
	TFontDialog *FontDialog1;
	TMenuItem *Exit;
	TButton *Shrift;
	TMenuItem *Save;
	TCheckBox *Redactirovanie;
	TCheckBox *cbEditAutoWidth;
	TCheckBox *cbRowRangeSelect;
	TMenuItem *Hours;
	TImageList *ImageList1;
	TButton *Button_Increase_Hours;
	TButton *ButtonDelete_Hour;
	TButton *ButtonAdd_Transport;
	TButton *ButtonDelete_Transport;
	TButton *ButtonInsert_Hour;
	TStringGrid *StringGrid2;
	TStringGrid *StringGrid3;
	TButton *ButtonInsert_Transport;
	TButton *bExportToTxt;

	void __fastcall CreateClick(TObject *Sender);
	void __fastcall OpenClick(TObject *Sender);
	void __fastcall Save_AsClick(TObject *Sender);
	void __fastcall Increase_HoursClick(TObject *Sender);
	void __fastcall Calculate_Sredn_IntensivnClick(TObject *Sender);
	void __fastcall Calculate_ExtremesClick(TObject *Sender);
	void __fastcall bExportToTxtClick(TObject *Sender);
	void __fastcall Delete_HourClick(TObject *Sender);
	void __fastcall Insert_HourClick(TObject *Sender);
	void __fastcall Add_TransportClick(TObject *Sender);
	void __fastcall Delete_TransportClick(TObject *Sender);
	void __fastcall Insert_TransportClick(TObject *Sender);
	void __fastcall StringGrid1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall StringGrid1Click(TObject *Sender);
	void __fastcall osdLoadSaveShow(TObject *Sender);
	void __fastcall ExitClick(TObject *Sender);
	void __fastcall StringGrid1GetEditText(TObject *Sender, int ACol, int ARow,
		UnicodeString &Value);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	// void __fastcall FontDialog1Apply(TObject *Sender, HWND Wnd);
	void __fastcall ShriftClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
		bool &CanSelect);
	void __fastcall StringGrid1SetEditText(TObject *Sender, int ACol, int ARow,
		const UnicodeString Value);
	void __fastcall RedactirovanieClick(TObject *Sender);
	void __fastcall cbEditAutoWidthClick(TObject *Sender);
	void __fastcall cbRowRangeSelectClick(TObject *Sender);
	void __fastcall bDelRowRangeClick(TObject *Sender);
	void __fastcall bDelColRangeClick(TObject *Sender);
	void __fastcall StringGrid1DblClick(TObject *Sender);
	void __fastcall StringGrid1ColumnMoved(TObject *Sender, int FromIndex,
		int ToIndex);
	void __fastcall StringGrid1RowMoved(TObject *Sender, int FromIndex,
		int ToIndex);
	void __fastcall sgResultsDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall StringGrid1MouseMove(TObject *Sender, TShiftState Shift,
		int X, int Y);
	void __fastcall Button_Increase_HoursClick(TObject *Sender);
	void __fastcall ButtonDelete_HourClick(TObject *Sender);
	void __fastcall ButtonAdd_TransportClick(TObject *Sender);
	void __fastcall ButtonDelete_TransportClick(TObject *Sender);
	void __fastcall ButtonInsert_HourClick(TObject *Sender);
	void __fastcall StringGrid2DrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall StringGrid3DrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall ButtonInsert_TransportClick(TObject *Sender);

private: // User declarations
	bool __fastcall TForm1::SaveData(bool ShowDialog); // метод
	bool __fastcall TForm1::CheckChangesAndSave();
	bool __fastcall TForm1::CheckGridCell(int col, int row);
	void __fastcall DlgFontChange(TObject* Sender);
	void __fastcall sgFontChanged(TObject* Sender);
	typedef int TCmpFunc(const double& s1, const double& s2);
	void __fastcall SortGridByCol(TStringGrid *grid, int colIdx, bool ascending,
		TCmpFunc cmpFunc);

	int FSortedColNbr;
	UnicodeString FCurCellText;

	bool flag_mistake = false;

	bool AskClearGrid;

	int col = 0, row = 0;

	DynamicArray<int>arrmax;
	DynamicArray<int>arrmin;
	DynamicArray<int>RedColorCountGrid;

public: // User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall TForm1::updateColor();
};

// ---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
// ---------------------------------------------------------------------------
#endif
