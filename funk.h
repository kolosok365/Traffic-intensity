#include "KURSWORK.h"
#pragma once
#ifndef funk_h
#define funk_h
bool OpenFile(const char * filename,
	DynamicArray<DynamicArray<int> > &masavtobus, int &kol_ch, int &first_ch,
	int &step, int &kol_TimeAndVT, DynamicArray<String> &TimeAndVT, int &Summa,
	int &CurrentSumma);
bool SaveFile(const char * filename,
	DynamicArray<DynamicArray<int> > &masavtobus, int first_ch, int step,
	int kol_ch, int kol_TimeAndVT, DynamicArray<String> &TimeAndVT, int Summa);
#endif
