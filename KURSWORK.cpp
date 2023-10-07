// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "funk.h"
#include "KURSWORK.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1; /* $00F2D3B7 */
const TColor MIN_COLOR {
    0x0084C5FF
};
const TColor MAX_COLOR {
    0x0091FF91
};
const String S_EDITOR = L"Интенсивности движения"; // Редактор было написано
const String S_NONAME = L"<Безымянный>";

// ---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
    SbInfo->Panels->operator[](1)->Text = L"";
    // возможно надо через событие OnCreate
    Form1->Memo /* ->Lines */ ->Clear();
    StringGrid1->Cells[0][0] = "Вид тр-та\\Часы";
    StringGrid1->Cells[0][1] = "Nавт/час";
    StringGrid1->Cells[0][2] = "Nпр/час";
    StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
		"Средняя интенсивность";
    Memo->Lines->Add(UnicodeString(L"          - цвет максимума"));
    Memo->Lines->Add(UnicodeString(L""));
    Memo->Lines->Add(UnicodeString(L"          - цвет минимума"));
    Memo->Lines->Add(UnicodeString(L""));
    Memo->Lines->Add
        (UnicodeString
        (L"Nавт/час - интенсивность движения для автобусов на автомагистрали.")
        );
    Memo->Lines->Add(UnicodeString(L""));
    Memo->Lines->Add
        (UnicodeString
        (L"Nпр/час - интенсивность движения для прочего транспорта на автомагистрали.")
        );
    // StringGrid1->Options= StringGrid1->Cols[StringGrid1->ColCount-1]->Options<<goEditing;
    StringGrid1->Font->OnChange = sgFontChanged;
    StringGrid1-> ColWidths[0]=124;
    StringGrid1-> ColWidths[StringGrid1->ColCount-1]=184;
}
// ---------------------------------------------------------------------------
const int MIN_COL_WIDTH = 12;

int CalcGridColWidth(TStringGrid *grid, int colIdx) {
    if (!grid || colIdx < 0 || colIdx >= grid->ColCount)
        return 0;
    grid->Canvas->Font->Assign(grid->Font);
    int maxWidth = 0;
    for (int i = 0; i < grid->RowCount; i++) {
        /* int textWidth;
         if(colIdx==grid->ColCount-1)
         int textWidth=40+grid->Canvas->TextWidth(grid->Cells[colIdx][i]);
         else */
        int textWidth = 12 + grid->Canvas->TextWidth(grid->Cells[colIdx][i]);
        // прибавил 12 пикселей,т.к надпись "средняя интенсивность не влезала"
        if (textWidth > maxWidth)
            maxWidth = textWidth;
    }
    return maxWidth + MIN_COL_WIDTH;
}

int AdjustGridColWidths(TStringGrid *grid) {
    if (!grid)
        return 0;
    int totalWidth = 0;
    for (int j = 0; j < grid->ColCount; j++) {
        int w = CalcGridColWidth(grid, j);
        grid->ColWidths[j] = w;
        totalWidth += w;
    }
    if (!(grid->Options * (TGridOptions() << goFixedVertLine << goVertLine))
        .Empty())
        totalWidth += grid->ColCount * grid->GridLineWidth;
    return totalWidth;
}

void __fastcall TForm1::sgFontChanged(TObject* Sender) {
    StringGrid1->Canvas->Font->Assign(StringGrid1->Font);
    StringGrid1->DefaultRowHeight = StringGrid1->Canvas->TextHeight(L"hy") + 4;
    if (cbEditAutoWidth->Checked)
        AdjustGridColWidths(StringGrid1);
}

int cmpFunc2(const double& s1, const double& s2) {
    return s1 - s2;
}
// typedef int TCmpFunc(const double /*UnicodeString*/& s1,const double /*UnicodeString*/& s2);

void __fastcall TForm1::SortGridByCol(TStringGrid *grid, int colIdx,
    bool ascending, TCmpFunc cmpFunc)
{ // Добавил TForm1::,чтобы функция стала воспринимать StringGrid1

    TStrings *col = grid->Cols[colIdx];
    TStrings *tmpRow = new TStringList();
    try {
        for (int i = grid->FixedRows; i < grid->RowCount - 1; i++)
            for (int j = i + 1; j < grid->RowCount; j++)
                if (cmpFunc(StrToInt(StringGrid1->Cells[colIdx][i]),
                    StrToInt(StringGrid1->Cells[colIdx][j])) > 0 && ascending ||
                    cmpFunc(StrToInt(StringGrid1->Cells[colIdx][i]),
                    StrToInt(StringGrid1->Cells[colIdx][j])) < 0 && !ascending)
                {
                    tmpRow->Assign(grid->Rows[i]);
                    grid->Rows[i]->Assign(grid->Rows[j]);
                    grid->Rows[j]->Assign(tmpRow);
                }

    }
    __finally {
        delete tmpRow;
    }

    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

}

void __fastcall TForm1::CreateClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    if (!CheckChangesAndSave())
        return;
    for (int i = StringGrid1->ColCount - 1; i > 0; i--) {
        StringGrid1->Cols[i]->Clear();
    }
    for (int i = StringGrid1->RowCount - 1; i > 0; i--) {
        StringGrid1->Rows[i]->Clear();
    }
    StringGrid1->RowCount = 3;
    StringGrid1->ColCount = 4;
    StringGrid1->Cells[0][0] = "Вид тр-та\\Часы";
    StringGrid1->Cells[0][1] = "Nавт/час";
    StringGrid1->Cells[0][2] = "Nпр/час";
    StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
        "Средняя интенсивность";
    SaveDialogFile->FileName = L"";
    Save->Enabled = false;
    Caption = S_EDITOR + L" - " + S_NONAME;

    if (cbEditAutoWidth->Checked)
        AdjustGridColWidths(StringGrid1);

    FSortedColNbr = 0;

    Increase_Hours->Enabled = true;
    Delete_Hour->Enabled = true;
    Insert_Hour->Enabled = true;
    Add_Transport->Enabled = true;
    Delete_Transport->Enabled = true;
    Insert_Transport->Enabled = true;
    ButtonInsert_Transport->Enabled = true;
    Button_Increase_Hours->Enabled = true;
    ButtonDelete_Hour->Enabled = true;
    ButtonInsert_Hour->Enabled = true;
    ButtonAdd_Transport->Enabled = true;
    ButtonDelete_Transport->Enabled = true;

    StringGrid1->Col = 1;
    // без этого курсор ячейки не переходит в новую нужную позицию
    StringGrid1->SetFocus();
    RedColorCountGrid.Length = 0;
    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::OpenClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    if (!CheckChangesAndSave())
        // false вернётся при нажатии ОТМЕНА в запросе сохранения изменений,
            return; // либо пользователь нажал ДА,но сохранение не выполнилось из-за каких-либо ошибок

    StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
        "Средняя интенсивность";
    // true вернётся,если нет несохранённых изменений
    // либо при нажатии НЕТ в запросе сохранения изменений
    SbInfo->Panels->operator[](1)->Text = L"";
    // либо пользователь нажал ДА и сохранение выполнилось успешно
    if (OpenDialogFile->Execute()) {
        AnsiString OFile = OpenDialogFile->FileName;

        DynamicArray<DynamicArray<int> >masavtobus;
        DynamicArray<String>TimeAndVT;
        int kol_ch = 0;
        int first_ch = 0;
        int step = 0;
        int kol_TimeAndVT = 0;
        int Summa = 0;
        int CurrentSumma = 0;
        /* if(!OpenFile(AnsiString(OFile).c_str(),masavtobus,kol_ch,first_ch,step,kol_TimeAndVT,TimeAndVT)){
         SbInfo -> Panels -> operator [](1) -> Text =L"Ошибка чтения файла \""+OFile+L"\"!";
         return;
         } */
        try {
            // OpenFile(AnsiString(OFile).c_str(),masavtobus,kol_ch,first_ch,step,kol_TimeAndVT,TimeAndVT,Summa,CurrentSumma);
            if (!OpenFile(AnsiString(OFile).c_str(), masavtobus, kol_ch,
                first_ch, step, kol_TimeAndVT, TimeAndVT, Summa, CurrentSumma))
            {
                Application->MessageBox
                    ((String(L"Не удалось считать данные из файла \"") +
                    OpenDialogFile->FileName + L"\"!").w_str(),
                    Application->Title.w_str(), MB_OK | MB_ICONERROR);
                return;
            }

        }
        catch (Exception &e)
        { // переменная,из которой берём дополнительную информацию об ошибке
            Application->MessageBox
                (Format(L"Ошибка открытия файла \"%s\":\"%s\"",
                // выделения памяти
                ARRAYOFCONST((OpenDialogFile->FileName, e.Message))).w_str(),
                Application->Title.w_str(), MB_OK | MB_ICONERROR);
            // иконка,сигнализирующая об ошибке

            return;
        }

        if (masavtobus.Length) {
            StringGrid1->ColCount = (masavtobus[0].Length) + 2;
            StringGrid1->RowCount = kol_TimeAndVT;
        }

        for (int i = 0; i < kol_ch; i++) {
            StringGrid1->Cells[i + 1][0] = first_ch + (i * step);
        }

        for (int j = 0; j < TimeAndVT.Length; j++) {
            StringGrid1->Cells[0][j] = TimeAndVT[j];
        }

        for (int j = 0; j < masavtobus.Length; j++) {
            for (int i = 0; i < (StringGrid1->ColCount) - 2; i++)
                /* for(int i = 0; i < masavtobus[j].Length; i++) */ {
                StringGrid1->Cells[i + 1][j + 1] = (masavtobus[j][i]);
                CurrentSumma += masavtobus[j][i];
            }
        }
        if (CurrentSumma != Summa) {
            Application->MessageBox
                ((String(L"Файл \"") + OpenDialogFile->FileName +
                L"\" содержит некорректные данные!").w_str(),
                Application->Title.w_str(), MB_OK | MB_ICONERROR);
            CreateClick(NULL);
            return;
        }

        SbInfo->Panels->operator[](1)->Text =
            L"Данные успешно прочитаны из файла \"" + OFile + L" \"!";
        Results->Enabled = True;

        StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
            "Средняя интенсивность";
        SaveDialogFile->FileName = OpenDialogFile->FileName;

        /*if (cbEditAutoWidth->Checked)
            AdjustGridColWidths(StringGrid1);*/
        if (cbEditAutoWidth->Checked)
            AdjustGridColWidths(StringGrid1);

        else{
            for (int ACol = 0; ACol < StringGrid1->ColCount; ACol++)
                StringGrid1->ColWidths[ACol] = StringGrid1->DefaultColWidth;
            StringGrid1-> ColWidths[0]=124;
            StringGrid1-> ColWidths[StringGrid1->ColCount-1]=184;
        }

        Save->Enabled = false;
        Caption = S_EDITOR + L" - " + (SaveDialogFile->FileName);
        FSortedColNbr = 0;

        Increase_Hours->Enabled = true;
        Delete_Hour->Enabled = true;
        Insert_Hour->Enabled = true;
        Add_Transport->Enabled = true;
        Delete_Transport->Enabled = true;
        Insert_Transport->Enabled = true;
        ButtonInsert_Transport->Enabled = true;
        Button_Increase_Hours->Enabled = true;
        ButtonDelete_Hour->Enabled = true;
        ButtonInsert_Hour->Enabled = true;
        ButtonAdd_Transport->Enabled = true;
        ButtonDelete_Transport->Enabled = true;

        RedColorCountGrid.Length = 0; // чтобы min и max  убрать краску
        arrmax.Length = 0;
        arrmin.Length = 0;
        StringGrid1->Repaint();
    }
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Save_AsClick(TObject *Sender) {
    for (int j = 0; j < StringGrid1->RowCount /* -1 */ ; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                SbInfo->Panels->operator[](1)->Text = L"Заполните все ячейки!";
                ShowMessage(L"Заполните все нефиксированные ячейки!");
                updateColor();
                return;
            }
        }
    }
    Memo->Clear();
    SbInfo->Panels->operator[](1)->Text = L"";
    SaveData(true); // потому что диалог сохранения должен отобразиться всегда

    if (SaveData) {
        SbInfo->Panels->operator[](1)->Text =
            L"Данные успешно записаны в файл \"" + (SaveDialogFile->FileName)
            + L"\"!";
        ShowMessage(L"Данные успешно записаны в файл \"" +
            (SaveDialogFile->FileName) + L"\"!");
        Results->Enabled = True;
        Save->Enabled = false;
        Caption = S_EDITOR + L" - " + (SaveDialogFile->FileName);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Increase_HoursClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    StringGrid1->ColCount++;
    // чтобы код не повторялся,увеличиваю кол-во столбцов сразу, из-за этого индексы столбцов уменьшил в дальнейшем
    StringGrid1->Cols[StringGrid1->ColCount - 1] =
        StringGrid1->Cols[StringGrid1->ColCount - 2];
    StringGrid1->Col = StringGrid1->ColCount - 2;
    // без этого курсор ячейки не переходит в новую нужную позицию
    StringGrid1->Cols[StringGrid1->Col]->Clear();
    StringGrid1->SetFocus();
    if (StringGrid1->ColCount > 2) {
        Delete_Hour->Enabled = true;
        ButtonDelete_Hour->Enabled = true;
    }

    int n;
    if (!TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 3][0], n) ||
        !TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 4][0], n) ||
        StringGrid1->ColCount < 4)
        return;

    int step = StringGrid1->Cells[StringGrid1->ColCount - 3][0] -
        StringGrid1->Cells[StringGrid1->ColCount - 4][0];
    StringGrid1->Cells[StringGrid1->ColCount - 2][0] =
        StrToInt(StringGrid1->Cells[StringGrid1->ColCount - 3][0]) + step;

    if (StrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0]) + step >= 24)
    {
        Increase_Hours->Enabled = false;
        Button_Increase_Hours->Enabled = false;
    }

    if (StrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0]) + step <=
        23 && Insert_Hour->Enabled == false) {
        Insert_Hour->Enabled = true;
        ButtonInsert_Hour->Enabled = true;
    }

    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }

}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TForm1::Calculate_Sredn_IntensivnClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    for (int j = 1; j < StringGrid1->RowCount /* -1 */ ; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                SbInfo->Panels->operator[](1)->Text = L"Заполните все ячейки!";
                ShowMessage(L"Заполните все нефиксированные ячейки!");
                updateColor();
                return;
            }
        }
    }

    SbInfo->Panels->operator[](1)->Text = L"";
    DynamicArray<DynamicArray<int> >masavtobus;
    int kol_elem =
        (((StringGrid1->ColCount) - 2) * (StringGrid1->RowCount - 1));
    float sravt = 0;

    for (int j = 1; j < (StringGrid1->RowCount); j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            sravt += StrToInt(StringGrid1->Cells[i][j]);
        }

        StringGrid1->Cells[StringGrid1->ColCount - 1][j] =
            (sravt / ((StringGrid1->ColCount) - 2));
        StringGrid1->Cells[StringGrid1->ColCount - 1][j] =
            Format(L"%.3f",
            ARRAYOFCONST(((long double)(sravt / ((StringGrid1->ColCount)
            - 2)))));
        sravt = 0;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Calculate_ExtremesClick(TObject *Sender) {
    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();
    SbInfo->Panels->operator[](1)->Text = L"";
    for (int j = 1; j < StringGrid1->RowCount /* -1 */ ; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                SbInfo->Panels->operator[](1)->Text = L"Заполните все ячейки!";
                ShowMessage(L"Заполните все нефиксированные ячейки!");
                updateColor();
                return;
            }
        }
    }

    SbInfo->Panels->operator[](1)->Text = L"";

    /* try{
     arrmax.Length=((StringGrid1->ColCount)-2)*2;
     arrmin.Length=((StringGrid1->ColCount)-2)*2;
     }catch(...){
     arrmax.Length=0;
     arrmin.Length=0;
     } */
    // int step = StrToInt(StringGrid1->Cells[2][0])- StrToInt(StringGrid1->Cells[1][0]);
    int kol_elem =
        (((StringGrid1->ColCount) - 2) * ((StringGrid1->RowCount) - 1));

    for (int s = 1; s < (StringGrid1->RowCount); s++) {
        for (int ch = 0 /* StrToInt(StringGrid1->Cells[1][0]) */ ;
        ch < StringGrid1->ColCount -
            2 /* StrToInt(StringGrid1->Cells[(StringGrid1->ColCount)-2][0]) */ ;
        ch++ /* =step */) {

            for (int i = 1; i <= ((StringGrid1->ColCount) - 2); i++) {

                if (i == ((StringGrid1->ColCount) - 2)) {
                    ch++ /* =step */ ;
                    continue;
                }

                else if (StrToInt(StringGrid1->Cells[i + 1][s]) >=
                    StrToInt(StringGrid1->Cells[i][s])) {

                    while (StrToInt(StringGrid1->Cells[i + 1][s]) >=
                        StrToInt(StringGrid1->Cells[i][s])) {
                        i++;
                        ch++ /* =step */ ;

                        if (i == ((StringGrid1->ColCount) - 2)) {
                            break;
                        }
                    }

                    if (i == ((StringGrid1->ColCount) - 2)) {
                        continue;
                    }

                    // Memo->Lines->Add(UnicodeString(L"Максимум: ")+StrToInt(StringGrid1->Cells[i][s]));
                    /* flag_max=true;
                     col=i;
                     row=s;
                     StringGrid1->Repaint(); */
                    arrmax.Length++;
                    arrmax[arrmax.High] = i;
                    arrmax.Length++;
                    arrmax[arrmax.High] = s;

                    /* arrmax[p1]=i;
                     p1++;
                     arrmax[p1]=s;
                     p1++; */
                    i--;
                    ch++ /* =step */ ;
                }

                else if (StrToInt(StringGrid1->Cells[i + 1][s]) <=
                    StrToInt(StringGrid1->Cells[i][s])) {
                    while (StrToInt(StringGrid1->Cells[i + 1][s]) <=
                        StrToInt(StringGrid1->Cells[i][s])) {
                        i++;
                        ch++ /* =step */ ;
                        if (i == ((StringGrid1->ColCount) - 2)) {
                            break;
                        }
                    }
                    if (i == ((StringGrid1->ColCount) - 2)) {
                        continue;
                    }

                    // Memo->Lines->Add(UnicodeString(L"Минимум: ")+StrToInt(StringGrid1->Cells[i][s]));
                    /* flag_min=true;
                     col=StringGrid1->Col;
                     row=StringGrid1->Row;
                     StringGrid1->Repaint(); */

                    /* StringGrid1->Canvas->Brush->Color=clRed;//минимумы
                     StringGrid1->Canvas->FillRect(Rect); //Заливаем ячейку
                     StringGrid1->Canvas->TextOut(Rect->Left,Rect->Top,StringGrid1->Cells[ACol][ARow]; */

                    arrmin.Length++;
                    arrmin[arrmin.High] = i;
                    arrmin.Length++;
                    arrmin[arrmin.High] = s;
                    /* arrmin[p2]=i;
                     p2++;
                     arrmin[p2]=s;
                     p2++; */

                    i--;
                    ch++ /* =step */ ;
                }
            }

        }

    }
    StringGrid1->Repaint();
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::bExportToTxtClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    String backupFileName = SaveDialogFile->FileName;
    SaveDialogFile->DefaultExt = "txt";
    SaveDialogFile->Filter = "Данные об интенсивностях (*.txt)|*.txt|Все файлы (*.*)|*.*";
    SaveDialogFile->FileName =
        ExtractFileName(ChangeFileExt(SaveDialogFile->FileName, L""));
    if (!SaveDialogFile->Execute())
        return;

    ofstream file;
    file.open(AnsiString(SaveDialogFile->FileName).c_str());
    if (!file.is_open()) {
        Application->MessageBox
            ((String(L"Не удалось открыть файл \"") + SaveDialogFile->FileName +
            L"\"!").w_str(), Application->Title.w_str(), MB_OK | MB_ICONERROR);
        return;
    }

    DynamicArray<int>MaxTextSize;
    try {
        MaxTextSize.Length = StringGrid1->ColCount;
    }
    catch (...) {
        Application->MessageBox((String(L"Ошибка выделения оперативной памяти"))
            .w_str(), Application->Title.w_str(), MB_OK | MB_ICONERROR);
        return;
    }

    for (int i = 0; i < StringGrid1->ColCount; i++) {
        MaxTextSize[i] = 0;
        for (int j = 0; j < StringGrid1->RowCount; j++)
            if (MaxTextSize[i] < StringGrid1->Cells[i][j].Length())
                MaxTextSize[i] = StringGrid1->Cells[i][j].Length() + 6;
    }

    for (int j = 0; j < StringGrid1->RowCount; j++) {
        for (int i = 0; i < StringGrid1->ColCount; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                file << std::setw(MaxTextSize[i]) << "";
            }
            else
                file << std::setw(MaxTextSize[i]) << AnsiString
                    (StringGrid1->Cells[i][j]).c_str();
        }
        file << "\n";
    }
    if (arrmax.Length != 0) {
        file << "\n";
        file << std::setw(20) << " Максимумы:";
        for (int i = 0; i < arrmax.Length; i += 2)
            file << std::setw(8) << AnsiString
                (StringGrid1->Cells[arrmax[i]][arrmax[i + 1]]).c_str();
        // file << "\n";
    }

    if (arrmin.Length != 0) {
        file << "\n";
        file << std::setw(20) << "  Минимумы:";
        for (int i = 0; i < arrmin.Length; i += 2)
            file << std::setw(8) << AnsiString
                (StringGrid1->Cells[arrmin[i]][arrmin[i + 1]]).c_str();
        // file << "\n";
    }

    file.close();
    SaveDialogFile->FileName = backupFileName;
    ShowMessage(L"Результаты успешно сохранены в файл");
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Delete_HourClick(TObject *Sender) {
    if ((StringGrid1->Selection.Right - StringGrid1->Selection.Left + 1) != 1) {
        bDelColRangeClick(NULL);
        return;
    }

    SbInfo->Panels->operator[](1)->Text = L"";
    switch (Application->MessageBox(UnicodeString(L"Удалить час \"" + StringGrid1->Cells[StringGrid1->Col][0] + L"\"?").w_str(),
        Application->Title.w_str(),
        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
        // MB_ICONQUESTION(В окне сообщения появляется иконка знака вопроса).
    { // MB_DEFBUTTON1(Первая кнопка - основная (кнопка используемая по умолчанию). MB_DEFBUTTON1 - значение по умолчанию, если MB_DEFBUTTON2, MB_DEFBUTTON3, или MB_DEFBUTTON4 не определены.)
    case ID_YES:
        /* if(p1!=0 || p2!=0){
         for(int i=0;i<p1;i++){
         if(StringGrid1->Col==arrmax[i] && StringGrid1->Row==arrmax[i+1]){
         p1=0;            //чтобы min и max  убрать краску
         arrmax.Length=0;
         p2=0;
         arrmin.Length=0;
         StringGrid1->Repaint();
         }
         }
         for(int i=0;i<p2;i++){
         if(StringGrid1->Col==arrmin[i] && StringGrid1->Row==arrmin[i+1]){
         p1=0;            //чтобы min и max  убрать краску
         arrmax.Length=0;
         p2=0;
         arrmin.Length=0;
         StringGrid1->Repaint();
         }
         }
         } */
        if (arrmax.Length != 0 || arrmin.Length != 0) {
            // p1=0;            //чтобы min и max  убрать краску
            arrmax.Length = 0;
            // p2=0;
            arrmin.Length = 0;
            StringGrid1->Repaint();
        }

        for (int i = StringGrid1->Col; i < StringGrid1->ColCount - 1; i++)
            StringGrid1->Cols[i] = StringGrid1->Cols[i + 1];

        if (StringGrid1->Col != 1)
            StringGrid1->Col--;
        // без этого курсор ячейки не переходит в новую нужную позицию
        StringGrid1->Cols[StringGrid1->ColCount - 1]->Clear();
        StringGrid1->SetFocus();
        StringGrid1->ColCount--;
        if (StringGrid1->ColCount == 3) {
            Delete_Hour->Enabled = false;
            ButtonDelete_Hour->Enabled = false;
        }
        int n;
        if (TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 3][0],
            n) && TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0],
            n) && Increase_Hours->Enabled == false) {
            int step = StringGrid1->Cells[StringGrid1->ColCount - 2][0] -
                StringGrid1->Cells[StringGrid1->ColCount - 3][0];
            if (StrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0]) +
                step < 24) {
                Increase_Hours->Enabled = true;
                Button_Increase_Hours->Enabled = true;
            }
        }

        if (TryStrToInt(StringGrid1->Cells[1][0], n) && TryStrToInt
            (StringGrid1->Cells[2][0], n)) {
            int step = StringGrid1->Cells[2][0] - StringGrid1->Cells[1][0];
            if (StrToInt(StringGrid1->Cells[1][0]) - step >=
                0 && Insert_Hour->Enabled == false) {
                Insert_Hour->Enabled = true;
                ButtonInsert_Hour->Enabled = true;
            }
        }

        if (Save->Enabled == false) {
            Save->Enabled = true;
            Caption = Caption + L" *";
        }
        if (cbEditAutoWidth->Checked)
            AdjustGridColWidths(StringGrid1);

        if (StringGrid1->Cells[StringGrid1->ColCount - 1][1] != L"") {
            StringGrid1->Cols[StringGrid1->ColCount - 1]->Clear();
            StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
                "Средняя интенсивность";
        }

    case ID_NO:
        return;

    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Insert_HourClick(TObject *Sender) {
    int n;
    if (TryStrToInt(StringGrid1->Cells[1][0], n) && TryStrToInt
        (StringGrid1->Cells[2][0], n))
    { // второе условие,т.к если дан только один час,то не могу посчитать шаг,также захватывается столбец "Средние интенсивности"
        int step = StrToInt(StringGrid1->Cells[2][0]) -
            StrToInt(StringGrid1->Cells[1][0]);
        if (StringGrid1->Cells[1][0] - step < 0) {
            Insert_Hour->Enabled = false;
            ButtonInsert_Hour->Enabled = false;
            return;
        }
    }

    // p1=0;//чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

    SbInfo->Panels->operator[](1)->Text = L"";
    StringGrid1->ColCount++;
    // чтобы код не повторялся,увеличиваю кол-во столбцов сразу, из-за этого индексы столбцов увеличил в дальнейшем
    for (int i = StringGrid1->ColCount - 1; i > 1; i--)
        StringGrid1->Cols[i] = StringGrid1->Cols[i - 1];
    StringGrid1->Col = 1;
    // без этого курсор ячейки не переходит в новую нужную позицию
    StringGrid1->Cols[StringGrid1->Col]->Clear();
    StringGrid1->SetFocus();

    if (StringGrid1->ColCount > 2) {
        Delete_Hour->Enabled = true;
        ButtonDelete_Hour->Enabled = true;
    }

    // int n;
    if (!TryStrToInt(StringGrid1->Cells[2][0], n) || !TryStrToInt
        (StringGrid1->Cells[3][0], n) || StringGrid1->ColCount <= 4)
        // второе условие,т.к если дан только один час,то не могу посчитать шаг,также захватывается столбец "Средние интенсивности"
            return;
    int step = StrToInt(StringGrid1->Cells[3][0]) -
        StrToInt(StringGrid1->Cells[2][0]);
    StringGrid1->Cells[1][0] = StrToInt(StringGrid1->Cells[2][0]) - step;
    if (StringGrid1->Cells[1][0] - step < 0) {
        Insert_Hour->Enabled = false;
        ButtonInsert_Hour->Enabled = false;
    }

    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }

    if (StringGrid1->Cells[StringGrid1->ColCount - 1][1] != L"") {
        StringGrid1->Cols[StringGrid1->ColCount - 1]->Clear();
        StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
            "Средняя интенсивность";
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Add_TransportClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    StringGrid1->RowCount++;
    StringGrid1->Row = StringGrid1->RowCount - 1;
    // без этого курсор ячейки не переходит в новую нужную позицию
    StringGrid1->Rows[StringGrid1->Row]->Clear();
    StringGrid1->SetFocus();
    if (StringGrid1->RowCount >= 2) {
        Delete_Transport->Enabled = true;
        ButtonDelete_Transport->Enabled = true;
    }
    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Delete_TransportClick(TObject *Sender) {
    if ((StringGrid1->Selection.Top - StringGrid1->Selection.Bottom + 1) != 1) {
        bDelRowRangeClick(NULL);
        return;
    }
    SbInfo->Panels->operator[](1)->Text = L"";
    switch (Application->MessageBox(UnicodeString(L"Удалить вид транспорта\"" + StringGrid1->Cells[0][StringGrid1->Row] + L"\"?")
        .w_str(), Application->Title.w_str(),
        MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
        // MB_ICONQUESTION(В окне сообщения появляется иконка знака вопроса).
    { // MB_DEFBUTTON1(Первая кнопка - основная (кнопка используемая по умолчанию). MB_DEFBUTTON1 - значение по умолчанию, если MB_DEFBUTTON2, MB_DEFBUTTON3, или MB_DEFBUTTON4 не определены.)
    case ID_YES:
        for (int i = StringGrid1->Row; i < StringGrid1->RowCount - 1; i++)
            StringGrid1->Rows[i] = StringGrid1->Rows[i + 1];
        // StringGrid1->Rows[StringGrid1->RowCount-1]->Clear();
        StringGrid1->RowCount--;
        if (StringGrid1->RowCount == 1) {
            Delete_Transport->Enabled = false;
            ButtonDelete_Transport->Enabled = false;
        }
        Save->Enabled = true;
        if (cbEditAutoWidth->Checked)
            AdjustGridColWidths(StringGrid1);
        if (Save->Enabled == false) {
            Save->Enabled = true;
            Caption = Caption + L" *";
        }

    case ID_NO:
        return;
    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Insert_TransportClick(TObject *Sender) {
    // p1=0;//чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

    SbInfo->Panels->operator[](1)->Text = L"";
    StringGrid1->RowCount++;
    for (int i = StringGrid1->RowCount - 1; i > StringGrid1->Row; i--)
        StringGrid1->Rows[i] = StringGrid1->Rows[i - 1];
    StringGrid1->Rows[StringGrid1->Row]->Clear();
    StringGrid1->SetFocus();
    if (StringGrid1->RowCount == 2)
        Delete_Transport->Enabled = true;

    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1KeyPress(TObject *Sender,
    System::WideChar &Key) {

    SbInfo->Panels->operator[](1)->Text = L"";
    if (!StringGrid1->EditorMode) // если таблица не в режиме редактирования
            return;

    if (StringGrid1->Col == 0 /* && StringGrid1->Row!=0 */)
        return;

    if (Key == VK_ESCAPE && StringGrid1->EditorMode)
    { // ESC и находится ли таблица в режиме редактирования?
        StringGrid1->Cells[StringGrid1->Col][StringGrid1->Row] = FCurCellText;
        return;
    }

    if (Key == VK_RETURN /* Enter */ && StringGrid1->EditorMode) {
        return;
    }

    /* if(RedColorCountGrid.Length!=0){
     RedColorCountGrid.Length=0;
     StringGrid1->Repaint();

     } */
    if (RedColorCountGrid.Length != 0) {
        for (int i = 0; i < RedColorCountGrid.Length; i += 2) {
            if (StringGrid1->Col == RedColorCountGrid[i]
                && StringGrid1->Row == RedColorCountGrid[i + 1]) {
                RedColorCountGrid[i] = -1;
                RedColorCountGrid[i + 1] = -1;
            }

        }
    }

    if (arrmax.Length != 0 || arrmin.Length != 0) {
        for (int i = 0; i < arrmax.Length; i++) {
            /* if(StringGrid1->Col==arrmax[i] && StringGrid1->Row==arrmax[i+1]) */
            {
                // p1=0;            //чтобы min и max  убрать краску
                arrmax.Length = 0;
                // p2=0;
                arrmin.Length = 0;
                StringGrid1->Repaint();
            }
        }
        for (int i = 0; i < arrmin.Length; i++) {
            /* if(StringGrid1->Col==arrmin[i] && StringGrid1->Row==arrmin[i+1]) */
            {
                // p1=0;            //чтобы min и max  убрать краску
                arrmax.Length = 0;
                // p2=0;
                arrmin.Length = 0;
                StringGrid1->Repaint();
            }
        }
    }

    if (!((Key >= L'0' && Key <= L'9')
        /* || Key == FormatSettings.DecimalSeparator */)
        && Key != VK_BACK /* Backspace */ && Key != 1 /* Ctrl+A */ && Key !=
        3 /* Ctrl+C */ && Key != 22 /* Ctrl+V */ && Key != 24 /* Ctrl+X */ &&
        Key != 26 /* Ctrl+Z */ && Key != VK_RETURN /* Enter */) {
        SbInfo->Panels->operator[](1)->Text =
            L"Ошибка! Введен недопустимый сивол '" + UnicodeString(Key) + L"'";
        Key = 0;
        return;
    }
    else
        // flag=false;
        // col=StringGrid1->Col;
        // row=StringGrid1->Row;
        // StringGrid1->Repaint();
            SbInfo->Panels->operator[](1)->Text = L"";
    // MenuSave->Enabled=true;
    // ButtonSave->Enabled=true;

    /* if(StringGrid1->Col == (StringGrid1->ColCount-2) && StringGrid1->Row == StringGrid1->RowCount-1){//чтобы "Результаты" разблокировались
     if(StringGrid1->Cells[StringGrid1->ColCount-2][StringGrid1->RowCount-1]==L""){                //при вводе с клавиатуры
     for(int j=1;j<StringGrid1->RowCount-1;j++){
     for(int i=1;i<StringGrid1->ColCount-2;i++){//мне не надо смотреть на пустоту ячейку в последнем столбе,ведь я пишу туда
     if(StringGrid1->Cells[i][j]==L""){
     return;
     }
     }
     }
     Results->Enabled=true;
     }
     } */

    /* if(StringGrid1->Col == (StringGrid1->ColCount-2) && StringGrid1->Row == StringGrid1->RowCount-1){
     Results->Enabled=true;
     } */

    if (StringGrid1->Cells[StringGrid1->ColCount - 1][1] != L"") {
        StringGrid1->Cols[StringGrid1->ColCount - 1]->Clear();
        StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
            "Средняя интенсивность";
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TForm1::SaveData(bool ShowDialog) {

    String svdFileName = SaveDialogFile->FileName;
    SbInfo->Panels->operator[](1)->Text = L"";
    if (ShowDialog) { // если известно имя текущего файла,то ShowDialog=false
        if (!SaveDialogFile->Execute())
            return false;
    }
    DynamicArray<DynamicArray<int> >masavtobus;
    DynamicArray<String>TimeAndVT;

    /* for(int j=0;j<StringGrid1->RowCount;j++){
     for(int i=1;i<StringGrid1->ColCount-1;i++){
     if(StringGrid1->Cells[i][j]==L""){
     SbInfo -> Panels -> operator [](1) -> Text =L"Заполните все ячейки!";
     ShowMessage(L"Заполните все нефиксированные ячейки!");
     updateColor();
     return false;
     }
     }
     } */

    try {
        masavtobus.Length = ((StringGrid1->RowCount) - 1);
        for (int i = 0; i < masavtobus.Length; i++) {
            masavtobus[i].Length = (StringGrid1->ColCount) - 2;
        }

        TimeAndVT.Length = StringGrid1->RowCount;

    }
    catch (Exception &e)
    { // переменная,из которой берём дополнительную информацию об ошибке
        Application->MessageBox(Format(L"Ошибка сохранения файла \"%s\":\"%s\"",
            // выделения памяти
            ARRAYOFCONST((SaveDialogFile->FileName, e.Message))).w_str(),
            Application->Title.w_str(), MB_OK | MB_ICONERROR);
        // иконка,сигнализирующая об ошибке

        SaveDialogFile->FileName = svdFileName;
        /* masavtobus.Length =0;
         for(int i = 0; i < masavtobus.Length; i++){
         masavtobus[i].Length=0;
         } */
        return false;
    }

    /* for(int i=1;i<=2;i++){
     String s=StringGrid1->Cells[i][0];
     s= StringReplace(s,'▼',' ',TReplaceFlags()<<rfReplaceAll << rfIgnoreCase);
     StringGrid1->Cells[i][0]=s;
     } */
    for (int i = 1; i <= 2; i++) {
        int n;
        if (!TryStrToInt(StringGrid1->Cells[i][0], n)) {
            String s = StringGrid1->Cells[i][0];
            s.Delete(s.Length(), 3);
            int b;
            TryStrToInt(s, b);
            // только с помощью Try удалось присвоить значение типа int ячейке Stringgrid!
            StringGrid1->Cells[i][0] = b;
            // return false;это я смотрел удаляет ли в таблице.Удаляет, но просто StrToInt,без Try, не присваивает как число
        }
    }

    int Summa = 0;
    int first_ch = StrToInt(StringGrid1->Cells[1][0]);
    Summa += first_ch;
    int step = StrToInt(StringGrid1->Cells[2][0]) -
        StrToInt(StringGrid1->Cells[1][0]); // короче попробовать
    Summa += step;
    int kol_ch =
        (step + (StrToInt(StringGrid1->Cells[(StringGrid1->ColCount) - 2][0]) -
        StrToInt(StringGrid1->Cells[1][0]))) / step;
    Summa += kol_ch;

    int kol_TimeAndVT = StringGrid1->RowCount;
    for (int i = 0; i < TimeAndVT.Length; i++) {
        TimeAndVT[i] = StringGrid1->Cells[0][i];
    }

    for (int j = 0; j < masavtobus.Length; j++) {
        for (int i = 0; i < masavtobus[j].Length; i++) {
            masavtobus[j][i] = StrToInt(StringGrid1->Cells[i + 1][j + 1]);
            Summa += masavtobus[j][i];
        }
    }

    if (!SaveFile(AnsiString(SaveDialogFile->FileName).c_str(), masavtobus,
        first_ch, step, kol_ch, kol_TimeAndVT, TimeAndVT, Summa)) {
        return false;
    }
    // ShowMessage(L"Данные успешно записаны в файл!");
    /* SbInfo -> Panels -> operator [](1) -> Text =L"Данные успешно записаны в файл \""+(SaveDialogFile->FileName)+L"\"!";
     ShowMessage(L"Данные успешно записаны в файл \""+(SaveDialogFile->FileName)+L"\"!");
     Results -> Enabled = True;
     Save ->Enabled=false;
     Caption=S_EDITOR+L" - "+(SaveDialogFile->FileName); */
    return true;
}

bool __fastcall TForm1::CheckChangesAndSave() {
    if (Save->Enabled) {
        String s;

        if (!SaveDialogFile->FileName.IsEmpty())
            s = SaveDialogFile->FileName;
        else {
            s = S_NONAME;
        }
        switch (Application->MessageBox
            ((UnicodeString(L"Сохранить изменения в \"") + s + L"\"?").w_str(),
            Application->Title.w_str(), MB_YESNOCANCEL | MB_ICONQUESTION)) {
        case ID_YES:
            /* if(!SaveData(SaveDialogFile->FileName.IsEmpty()))
             return false;
             break; */
            return SaveData(SaveDialogFile->FileName.IsEmpty());
        case ID_CANCEL: // кейс NO не пишем,т.к не найдя нужного кейса,
            return false; // выйдем из switch и всё равно вернем true
        }

    }
    return true;
}

void __fastcall TForm1::StringGrid1Click(TObject *Sender) {
    /* if(!Save ->Enabled){
     Caption=Caption+L" *";
     Save ->Enabled=true;
     } */
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TForm1::osdLoadSaveShow(TObject *Sender) {
    //
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ExitClick(TObject *Sender) {
    SbInfo->Panels->operator[](1)->Text = L"";
    switch (Application->MessageBox
        (UnicodeString(L"Вы действительно хотите выйти? ").w_str(),
        Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))
    {
    case ID_YES:
        // CheckChangesAndSave();
        // SaveData(SaveDialogFile->FileName.IsEmpty());
        Close();
    case ID_NO:
        return;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1GetEditText(TObject *Sender, int ACol,
    int ARow, UnicodeString &Value)
    // чтобы запомнить старое содержимое ячейки для ESC
{
    FCurCellText = Value;
}
// ---------------------------------------------------------------------------

/* void __fastcall TForm1::CheckBoxClick(TObject *Sender)
 {
 #define cb ((TCheckBox*) Sender)
 cb ->Font -> Style = cb -> Checked? cb ->Font -> Style << fsBold:cb ->Font -> Style >> fsBold;
 } */
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose) {
    CanClose = CheckChangesAndSave();
    /* if(!CheckChangesAndSave())
     CanClose=false; */

}
// ---------------------------------------------------------------------------

/* void __fastcall TForm1::FontDialog1Apply(TObject *Sender, HWND Wnd)
 {
 //StringGrid1->Tag=1;
 //StringGrid1->Font=FontDialog1->Font;

 //((TFont*)FontDialog1->Tag)->Assign(FontDialog1->Font);
 } */
// ---------------------------------------------------------------------------
void __fastcall selectFont(TFont* font, const TFontDialogOptions &options =
    TFontDialogOptions() << fdEffects)
{ // fdEffects-"видоизменение" (цвет,подчёркивание,зачёркивание)

    class THelper {
    public:
        TFont* srcFontRef;
        bool srcFontAppliedTo, dlgFontChanged;

        void __fastcall ApplyBtnClick(TObject* Sender, HWND Wnd) {
            if (dlgFontChanged) {
                srcFontRef->Assign(((TFontDialog*)Sender)->Font);
                srcFontAppliedTo = true;
                dlgFontChanged = false;
            }
        }

        void __fastcall DlgFontChange(TObject* Sender) {
            dlgFontChanged = true;
        }

        THelper(TFont* _font) {
            srcFontRef = _font;
            srcFontAppliedTo = false;
            dlgFontChanged = false;
        }
    } hlpr(font);

    TFontDialog *fontDlg = new TFontDialog(NULL);
    try {
        fontDlg->Options = options;
        fontDlg->OnApply = hlpr.ApplyBtnClick;
        fontDlg->Font = font;
        fontDlg->Font->OnChange = hlpr.DlgFontChange;
        TFont *svdFont = new TFont();
        try {
            svdFont->Assign(font);
            if (!fontDlg->Execute()) {
                if (hlpr.srcFontAppliedTo)
                    font->Assign(svdFont);
                return;
            }
            if (hlpr.dlgFontChanged)
                font->Assign(fontDlg->Font);
        }
        __finally {
            delete svdFont;
        }
    }
    __finally {
        delete fontDlg;
    }
}

void __fastcall TForm1::ShriftClick(TObject *Sender) {
    /* FontDialog1->Font=StringGrid1->Font;
     if(!FontDialog1->Execute())
     return;
     StringGrid1->Font=FontDialog1->Font; */
    selectFont(StringGrid1->Font);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::SaveClick(TObject *Sender) {
    for (int j = 0; j < StringGrid1->RowCount /* -1 */ ; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                SbInfo->Panels->operator[](1)->Text = L"Заполните все ячейки!";
                ShowMessage(L"Заполните все нефиксированные ячейки!");
                updateColor();
                return;
            }
        }
    }
    SbInfo->Panels->operator[](1)->Text = L"";
    SaveData(SaveDialogFile->FileName.IsEmpty());
    // IsEmpty возвращает значение True, если переменная не инициализована, или для нее явно указано значение Empty; в противном случае оно возвращает значение False.

    if (SaveData) {
        SbInfo->Panels->operator[](1)->Text =
            L"Данные успешно записаны в файл \"" + (SaveDialogFile->FileName)
            + L"\"!";
        ShowMessage(L"Данные успешно записаны в файл \"" +
            (SaveDialogFile->FileName) + L"\"!");
        Results->Enabled = True;
        Save->Enabled = false;
        Caption = S_EDITOR + L" - " + (SaveDialogFile->FileName);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1SelectCell(TObject *Sender, int ACol,
    int ARow, bool &CanSelect) {
    CanSelect = ACol != StringGrid1->ColCount - 1;
    // запрет редактирования последнего столба со средней интенсивностью
    SbInfo->Panels->operator[](1)->Text = L"";

    if (!Save->Enabled && ((TStringGrid*)Sender)
        ->EditorMode && // ((TStringGrid*)Sender) то же самое,что и StringGrid1
        FCurCellText != ((TStringGrid*)Sender)->Cells[((TStringGrid*)Sender)
        ->Col][((TStringGrid*)Sender)->Row]) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }
    if (((TStringGrid*)Sender)->EditorMode && !CheckGridCell
        (((TStringGrid*)Sender)->Col, ((TStringGrid*)Sender)->Row)) {
        SbInfo->Panels->operator[](1)->Text =
            Format(UnicodeString
            (L"Значение \"%s\", находящееся в ячейке [%d,%d],") +
            L" не является целым положительным числом.",
            ARRAYOFCONST((StringGrid1->Cells[StringGrid1->Col]
            [StringGrid1->Row], StringGrid1->Col + 1, StringGrid1->Row + 1)));
        Application->MessageBox(SbInfo->Panels->operator[](1)->Text.w_str(),
            (Application->Title + L" (OnSelectCell)").w_str(),
            MB_OK | MB_ICONERROR);

        StringGrid1->Tag = true;
        flag_mistake = true;
        col = StringGrid1->Col;
        row = StringGrid1->Row;
        StringGrid1->Repaint();
    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1SetEditText(TObject *Sender, int ACol,
    int ARow, const UnicodeString Value) {
    /* SbInfo -> Panels -> operator [](1) -> Text =L"";
     if(((TStringGrid*)Sender)->Tag){
     if(!((TStringGrid*)Sender)->EditorMode)
     if(!CheckGridCell(ACol,ARow)){
     SbInfo -> Panels -> operator [](1) -> Text=Format(UnicodeString(L"Значение \"%s\", находящееся в ячейке [%d,%d],")
     + L" не является английиским словом.",
     ARRAYOFCONST((Value,ACol,ARow+1)));
     Application->MessageBox(SbInfo -> Panels -> operator [](1)->Text.w_str(),
     (Application->Title+L" (OnSetEditText)").w_str(),MB_OK|MB_ICONERROR);
     }
     } */

    if (!CheckGridCell(ACol, ARow)) {
        SbInfo->Panels->operator[](1)->Text =
            Format(UnicodeString
            (L"Значение \"%s\", находящееся в ячейке [%d,%d],") +
            L" не является целым положительным числом.",
            ARRAYOFCONST((Value, ACol + 1, ARow + 1)));
        if (!((TStringGrid*)Sender)->EditorMode) {
            if (!((TStringGrid*)Sender)->Tag)
                Application->MessageBox
                    (SbInfo->Panels->operator[](1)->Text.w_str(),
                (Application->Title + L" (OnSetEditText)").w_str(),
                MB_OK | MB_ICONERROR);
        }

        else
            StringGrid1->Tag = false;
    }

    if (cbEditAutoWidth->Checked)
        StringGrid1->ColWidths[ACol] = CalcGridColWidth(StringGrid1, ACol);

    if (!Save->Enabled && !StringGrid1->EditorMode && FCurCellText != Value)
        Save->Enabled = true;

    // if(Results->Enabled==false){
    for (int j = 1; j < StringGrid1->RowCount; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                Results->Enabled = false;
                return;
            }
        }
    }
    Results->Enabled = true;
    // }
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::RedactirovanieClick(TObject *Sender) {
#define cb ((TCheckBox*) Sender)
    cb->Font->Style = cb->Checked ? cb->Font->Style << fsBold :
        cb->Font->Style >> fsBold; // чтобы шрифт стал толще

    if (Redactirovanie->Checked) {
        StringGrid1->Options = StringGrid1->Options << goAlwaysShowEditor;
        StringGrid1->FixedCols = 0;
        StringGrid1->FixedRows = 0;
    }
    else {
        StringGrid1->Options = StringGrid1->Options >> goAlwaysShowEditor;
        StringGrid1->FixedCols = 1;
        StringGrid1->FixedRows = 1;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cbEditAutoWidthClick(TObject *Sender) {
#define cb ((TCheckBox*) Sender)
    cb->Font->Style = cb->Checked ? cb->Font->Style << fsBold :
        cb->Font->Style >> fsBold; // чтобы шрифт стал толще

    if (cbEditAutoWidth->Checked)
        AdjustGridColWidths(StringGrid1);

    else {
        for (int ACol = 0; ACol < StringGrid1->ColCount; ACol++)
            StringGrid1->ColWidths[ACol] = StringGrid1->DefaultColWidth;
        StringGrid1-> ColWidths[0]=124;
        StringGrid1-> ColWidths[StringGrid1->ColCount-1]=184;
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TForm1::CheckGridCell(int col, int row) {
    /* if(col!=0 && row!=0){
     if(StringGrid1->Cells[col][row]!=L""){
     int n;
     if(!TryStrToInt(StringGrid1->Cells[col][row],n))
     return false;
     }
     }
     return true; */

    if (col != 0) {
        if (StringGrid1->Cells[col][row] != L"") {
            int n;
            if (!TryStrToInt(StringGrid1->Cells[col][row], n))
                return false;
        }
    }
    return true;
}

void __fastcall TForm1::updateColor() {
    RedColorCountGrid.Length = 0;
    for (int j = 1; j < StringGrid1->RowCount; j++) {
        for (int i = 1, tmp; i < StringGrid1->ColCount; i++) {
            if (!TryStrToInt(StringGrid1->Cells[i][j], tmp)) {
                SbInfo->Panels->operator[](1)->Text =
                    String("Ошибка, в ячейке [") + i + L"][" + j +
                    L"] находится недопустимое значение";
                RedColorCountGrid.Length++;
                RedColorCountGrid[RedColorCountGrid.High] = i;
                RedColorCountGrid.Length++;
                RedColorCountGrid[RedColorCountGrid.High] = j;
            }
        }
    }
    StringGrid1->Repaint();
    /* if (RedColorCountGrid.Length == 0)
     {
     SbInfo -> Panels -> operator [](1) -> Text = String("");
     AskClearGrid = false;
     } */
}

void __fastcall TForm1::cbRowRangeSelectClick(TObject *Sender) {
#define cb ((TCheckBox*) Sender)
    cb->Font->Style = cb->Checked ? cb->Font->Style << fsBold :
        cb->Font->Style >> fsBold; // чтобы шрифт стал толще

    if (cbRowRangeSelect->Checked)
        StringGrid1->Options =
            StringGrid1->Options >> goEditing /* <<goRowSelect */ ;
    else
        StringGrid1->Options =
            StringGrid1->Options << goEditing /* >>goRowSelect */ ;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::bDelRowRangeClick(TObject *Sender) {
    /* if(StringGrid1->RowCount==1)
     return; */

    if (Application->MessageBox
        ((String(L"Вы действительно хотите удалить выделенные строки  с \"") +
        StringGrid1->Cells[0][StringGrid1->Selection.Top] + L"\" по \"" +
        StringGrid1->Cells[0][StringGrid1->Selection.Bottom] + L"\"?").w_str(),
        Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)
        != ID_YES)
        return;

    /* int l = StringGrid1->Selection.Left, r = StringGrid1->Selection.Right;
     if (StringGrid1->ColCount - (StringGrid1->Selection.Right - StringGrid1->Selection.Left + 1) == 1)
     {
     StringGrid1->Cols[1]->Clear();
     l++;
     } */

    int n = StringGrid1->Selection.Bottom - StringGrid1->Selection.Top + 1;
    if (StringGrid1->RowCount - n >= 2) {

        // if(StringGrid1->Selection.Bottom <StringGrid1->RowCount-1){
        for (int i = StringGrid1->Selection.Bottom + 1;
        i < StringGrid1->RowCount; i++)
            StringGrid1->Rows[i - n] = StringGrid1->Rows[i];

        StringGrid1->RowCount = StringGrid1->RowCount - n;
        /* }
         else{
         StringGrid1->RowCount=2;
         StringGrid1->Options=StringGrid1->Options>>goEditing;
         bDelRowRange->Enabled=false;
         Delete_Transport->Enabled=false;
         } */
    }
    else {
        StringGrid1->RowCount = 2;
        StringGrid1->Rows[1]->Clear();
        StringGrid1->Options = StringGrid1->Options >> goEditing;
        Delete_Transport->Enabled = false;
        ButtonDelete_Transport->Enabled = false;
    }

    if (cbEditAutoWidth->Checked)
        AdjustGridColWidths(StringGrid1);

    /* if(StringGrid1->RowCount==1)
     return;

     if(Application->MessageBox((UnicodeString(L"Вы действительно хотите удалить выделенные строки (") +
     StringGrid1->Selection.Top + L'-' +
     StringGrid1->Selection.Bottom + L")?").w_str(),
     Application->Title.w_str(),MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) !=ID_YES)
     return;

     int n = StringGrid1->Selection.Bottom - StringGrid1->Selection.Top+1;
     if(StringGrid1->RowCount-n>=2)

     if(StringGrid1->Selection.Bottom <StringGrid1->RowCount-1){
     int t=StringGrid1->Selection.Top,
     b=StringGrid1->Selection.Bottom;
     for(int i=t;i<=b;i++)
     StringGrid1->DeleteRow(t);
     if(t<StringGrid1->RowCount)
     StringGrid1->RowCount=t;
     }
     else
     StringGrid1->RowCount-=n;

     else{
     StringGrid1->RowCount=2;
     StringGrid1->Options=StringGrid1->Options>>goEditing;
     bDelRowRange->Enabled=false;
     Delete_Transport->Enabled=false;
     } */
    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }
}

void __fastcall TForm1::bDelColRangeClick(TObject *Sender) {

   int n;
   if(StringGrid1->Selection.Right==StringGrid1->ColCount-1){
    if (Application->MessageBox
        ((String
        (L"Вы действительно хотите удалить выделенные столбцы с часами с \"") +
        StringGrid1->Cells[StringGrid1->Selection.Left][0] + L"\" до \"" +
        StringGrid1->Cells[StringGrid1->ColCount-2][0] + L"\"?").w_str(),
        Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)
        != ID_YES)
        return;

      n = StringGrid1->Selection.Right - StringGrid1->Selection.Left;
   }
   else {
    if(Application->MessageBox
        ((String
        (L"Вы действительно хотите удалить выделенные столбцы с часами с \"") +
        StringGrid1->Cells[StringGrid1->Selection.Left][0] + L"\" до \"" +
        StringGrid1->Cells[StringGrid1->Selection.Right][0] + L"\"?").w_str(),
        Application->Title.w_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)
        != ID_YES)
        return;

       n = StringGrid1->Selection.Right - StringGrid1->Selection.Left + 1;
    }

    if (arrmax.Length != 0 || arrmin.Length != 0) {
        // p1=0;//чтобы min и max  убрать краску
        arrmax.Length = 0;
        // p2=0;
        arrmin.Length = 0;
        StringGrid1->Repaint();
    }
    /*int n = StringGrid1->Selection.Right - StringGrid1->Selection.Left + 1;*/
    if (StringGrid1->ColCount - n >= 3) {
        for (int i = StringGrid1->Selection.Right + 1;
        i < StringGrid1->ColCount; i++)
            StringGrid1->Cols[i - n] = StringGrid1->Cols[i];

        StringGrid1->ColCount = StringGrid1->ColCount - n;
    }
    else {

        StringGrid1->ColCount = 3;
        StringGrid1->Cols[1]->Clear();
        StringGrid1->Options = StringGrid1->Options >> goEditing;
        ButtonDelete_Hour->Enabled = false;
        Delete_Hour->Enabled = false;
    }

    if (TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 3][0],
        n) && TryStrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0],
        n) && Increase_Hours->Enabled == false) {
        int step = StringGrid1->Cells[StringGrid1->ColCount - 2][0] -
            StringGrid1->Cells[StringGrid1->ColCount - 3][0];
        if (StrToInt(StringGrid1->Cells[StringGrid1->ColCount - 2][0]) +
            step < 24) {
            Increase_Hours->Enabled = true;
            Button_Increase_Hours->Enabled = true;
        }
    }

    // (StringGrid1->Cells[StringGrid1->ColCount - 1][1] != L"") {
        StringGrid1->Cols[StringGrid1->ColCount - 1]->Clear();
        StringGrid1->Cells[(StringGrid1->ColCount) - 1][0] =
            "Средняя интенсивность";
    //}

    if (cbEditAutoWidth->Checked)
        AdjustGridColWidths(StringGrid1);

    if (Save->Enabled == false) {
        Save->Enabled = true;
        Caption = Caption + L" *";
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1DblClick(TObject *Sender) {
    if (KeyboardStateToShiftState() != TShiftState() << ssCtrl << ssLeft)
        return;

    for (int j = 1; j < StringGrid1->RowCount /* -1 */ ; j++) {
        for (int i = 1; i < StringGrid1->ColCount - 1; i++) {
            if (StringGrid1->Cells[i][j] == L"") {
                SbInfo->Panels->operator[](1)->Text = L"Заполните все ячейки!";
                ShowMessage(L"Заполните все нефиксированные ячейки!");
                updateColor();
                return;
            }
        }
    }

    if (Redactirovanie->Checked) {
        /* SbInfo -> Panels -> operator [](1) -> Text=UnicodeString(L"Пожалуйста,выключите режим редактирования для корректной работы функции сортировки!");
         Application->MessageBox(SbInfo -> Panels -> operator [](1)->Text.w_str(),
         (Application->Title+L" (SortGridByCol)").w_str(),MB_OK|MB_ICONEXCLAMATION);
         return; */
        StringGrid1->Options = StringGrid1->Options >> goAlwaysShowEditor;
        StringGrid1->FixedCols = 1;
        StringGrid1->FixedRows = 1;
        Redactirovanie->Checked = false;
        // убираю галочку "Режим редактирования"
    }
    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

    TPoint p;
    GetCursorPos(&p); // p=Mouse->CursorPos; это экранные координаты
    p = StringGrid1->ScreenToClient(p);
    // экранные координаты переводит в координаты клиентской области
    if (GetCursor() == Screen->Cursors[crHSplit]) {
        TGridCoord gc = StringGrid1->MouseCoord(p.X - 4, p.Y);
        int w = CalcGridColWidth(StringGrid1, gc.X);
        if (StringGrid1->ColWidths[gc.X] != w)
            StringGrid1->ColWidths[gc.X] = w;
        else
            StringGrid1->ColWidths[gc.X] = MIN_COL_WIDTH;
    }
    else {
        TGridCoord gc = StringGrid1->MouseCoord(p.X, p.Y);
        if (gc.Y == 0 && gc.X >= StringGrid1->FixedCols) {
            int i = abs(FSortedColNbr) - 1;
            // индекс столба по которому была выполнена сортировка
            if (FSortedColNbr) { // была сортировка?
                StringGrid1->Cells[i][0] =
                    StringGrid1->Cells[i][0].SubString(1,
                    // убираем пометку из заголовка того столбца
                    StringGrid1->Cells[i][0].Length() - 2);
                if (cbEditAutoWidth->Checked)
                    StringGrid1->ColWidths[i] =
                        CalcGridColWidth(StringGrid1, i);
            }
            if (FSortedColNbr && i == gc.X)
                // gc.X-индекс столба,по которому пытается отсортировать
                    FSortedColNbr = -FSortedColNbr;
            else
                FSortedColNbr = gc.X + 1;
            i = gc.X;

            if (FSortedColNbr < 0)
                StringGrid1->Cells[i][0] = StringGrid1->Cells[i][0] + L" ▲";
            // L" \x25bc"

            else
                StringGrid1->Cells[i][0] = StringGrid1->Cells[i][0] + L" ▼";
            // L" \x25b2" //ловлю исключение из-за i=-1

            if (cbEditAutoWidth->Checked)
                StringGrid1->ColWidths[i] = CalcGridColWidth(StringGrid1, i);

            StringGrid1->Rows[StringGrid1->Row]->Objects[0] = (TObject*)true;

            SortGridByCol(StringGrid1, i, FSortedColNbr > 0, cmpFunc2);

            for (i = 1; StringGrid1->RowCount; i++)
                if (StringGrid1->Rows[i]->Objects[0]) {
                    StringGrid1->Rows[i]->Objects[0] = (TObject*)false;
                    StringGrid1->Row = i;
                    break;
                }
        }
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1ColumnMoved(TObject *Sender, int FromIndex,
    int ToIndex)

{
    if (!FSortedColNbr) // отсортирована ли таблица?
            return;

    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

    int i = abs(FSortedColNbr) - 1; // индекс "отсортированного столба"
    if (i == FromIndex)
        FSortedColNbr = (ToIndex + 1) * (FSortedColNbr / abs(FSortedColNbr));
    else if (FromIndex > i && ToIndex <= i)
        // перемещается налево через "отсортированнный" столбец?
            FSortedColNbr += FSortedColNbr / abs(FSortedColNbr);
    else if (FromIndex < i && ToIndex >= i)
        // перемещается направо через "отсортированнный" столбец?
            FSortedColNbr -= FSortedColNbr / abs(FSortedColNbr);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1RowMoved(TObject *Sender, int FromIndex,
    int ToIndex)

{
    // p1=0;            //чтобы min и max  убрать краску
    arrmax.Length = 0;
    // p2=0;
    arrmin.Length = 0;
    StringGrid1->Repaint();

    if (!FSortedColNbr) // отсортирована ли таблица?
            return;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::sgResultsDrawCell(TObject *Sender, int ACol, int ARow,
    TRect &Rect, TGridDrawState State) {

    if (RedColorCountGrid.Length != 0) {
        for (int i = 0; i < RedColorCountGrid.Length; i += 2) {
            if (ACol == RedColorCountGrid[i] && ARow == RedColorCountGrid
                [i + 1]) {
                StringGrid1->Canvas->Brush->Color = clRed;
                StringGrid1->Canvas->FillRect(Rect);
                StringGrid1->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
                    StringGrid1->Cells[RedColorCountGrid[i]]
                    [RedColorCountGrid[i + 1]]);
            }
        }
        /* StringGrid1->Canvas->Brush->Color = clRed;
         StringGrid1->Canvas->FillRect(Rect);
         StringGrid1->Canvas->TextRect(Rect,Rect.Left+4,Rect.Top+2,StringGrid1->Cells[ACol][ARow]);
         break; */

    }

    if (flag_mistake && ACol == col && ARow == row) {
        StringGrid1->Canvas->Brush->Color = clRed;
        StringGrid1->Canvas->FillRect(Rect);
        StringGrid1->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
            StringGrid1->Cells[ACol][ARow]);
        col = 0;
        row = 0;
        flag_mistake = false;
        return;
    }

    UnicodeString s;
    if (ARow == 0 || ACol == 0 || ACol == StringGrid1->ColCount - 1) {
        StringGrid1->Canvas /* обЪект для рисования */ ->Font->Assign
            (StringGrid1->Font);
        StringGrid1->Canvas->Font->Style =
            StringGrid1->Canvas->Font->Style << fsBold;
        StringGrid1->Canvas->Font->Color = clNavy;
        // цвет текста в фикс.ячейках.
        s = StringGrid1->Cells[ACol][ARow];
        StringGrid1->Canvas->Brush->Color = StringGrid1->FixedColor;
        // цвет фона в фикс.ячейках.
        TRect r = Rect;
        // if(StringGrid1->DefaultDrawing)
        // r.left-=4;
        StringGrid1->Canvas->TextRect(r,
            r.Left + (StringGrid1->ColWidths[ACol] -
            StringGrid1->Canvas->TextWidth(s)) / 2, r.Top + 2, s);
        return;
    }
    s = StringGrid1->Cells[1][ARow];
    if (s.IsEmpty())
        return;

    if (arrmax.Length != 0) {
        for (int i = 0; i < arrmax.Length; i += 2) {
            if (ACol == arrmax[i] && ARow == arrmax[i + 1]) {
                StringGrid1->Canvas->Brush->Color = MAX_COLOR;
                StringGrid1->Canvas->FillRect(Rect);
                StringGrid1->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
                    StringGrid1->Cells[ACol][ARow]);
            }
        }
    }

    if (arrmin.Length != 0) {
        for (int i = 0; i < arrmin.Length; i += 2) {
            if (ACol == arrmin[i] && ARow == arrmin[i + 1]) {
                StringGrid1->Canvas->Brush->Color = MIN_COLOR;
                StringGrid1->Canvas->FillRect(Rect);
                StringGrid1->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
                    StringGrid1->Cells[ACol][ARow]);
            }
        }
    }
    /* if(p1!=0){
     for(int i=0;i<p1;i+=2){
     if(ACol == arrmax[i] && ARow == arrmax[i+1]){
     StringGrid1->Canvas->Brush->Color = MAX_COLOR;
     StringGrid1->Canvas->FillRect(Rect);
     StringGrid1->Canvas->TextRect(Rect,Rect.Left+4,Rect.Top+2,StringGrid1->Cells[ACol][ARow]);
     }
     }
     }

     if(p2!=0){
     for(int i=0;i<p2;i+=2){
     if(ACol == arrmin[i] && ARow == arrmin[i+1]){
     StringGrid1->Canvas->Brush->Color = MIN_COLOR;
     StringGrid1->Canvas->FillRect(Rect);
     StringGrid1->Canvas->TextRect(Rect,Rect.Left+4,Rect.Top+2,StringGrid1->Cells[ACol][ARow]);
     }
     }
     } */

    TRect r = Rect;
    if (StringGrid1->DefaultDrawing)
        r.left -= 4;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1MouseMove(TObject *Sender, TShiftState Shift,
    int X, int Y) {
    if (Redactirovanie->Checked) {
        TPoint p;
        GetCursorPos(&p); // p=Mouse->CursorPos; это экранные координаты
        p = StringGrid1->ScreenToClient(p);
        // экранные координаты переводит в координаты клиентской области
        TGridCoord gc = StringGrid1->MouseCoord(p.X, p.Y);
        if (gc.Y == 0 && gc.X == 0)
        { // всё это сделано,чтобы нельзя было нажимать на ячейку 0:0,ведь StringGrid1SelectCell
            StringGrid1->FixedCols = 1;
            // позволяет войти в режим редактирования один раз,позволяет там писать, и уже только
        } // после выхода из ячейки она блокируется.
        else
            StringGrid1->FixedCols = 0;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button_Increase_HoursClick(TObject *Sender) {
    Increase_HoursClick(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ButtonDelete_HourClick(TObject *Sender) {
    Delete_HourClick(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ButtonAdd_TransportClick(TObject *Sender) {
    Add_TransportClick(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ButtonDelete_TransportClick(TObject *Sender) {
    Delete_TransportClick(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ButtonInsert_HourClick(TObject *Sender) {
    Insert_HourClick(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid2DrawCell(TObject *Sender, int ACol, int ARow,
    TRect &Rect, TGridDrawState State) {
    if (ACol == 0 && ARow == 0) {
        StringGrid2->Canvas->Brush->Color = MAX_COLOR;
        StringGrid2->Canvas->FillRect(Rect);
        StringGrid2->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
            StringGrid2->Cells[ACol][ARow]);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid3DrawCell(TObject *Sender, int ACol, int ARow,
    TRect &Rect, TGridDrawState State) {
    if (ACol == 0 && ARow == 0) {
        StringGrid3->Canvas->Brush->Color = MIN_COLOR;
        StringGrid3->Canvas->FillRect(Rect);
        StringGrid3->Canvas->TextRect(Rect, Rect.Left + 4, Rect.Top + 2,
            StringGrid3->Cells[ACol][ARow]);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ButtonInsert_TransportClick(TObject *Sender) {
    Insert_TransportClick(NULL);
}
// ---------------------------------------------------------------------------
