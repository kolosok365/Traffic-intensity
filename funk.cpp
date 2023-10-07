#pragma hdrstop
#include <fstream>
#include "funk.h"

bool OpenFile(const char * filename,
	/* const */ DynamicArray<DynamicArray<int> > &masavtobus, int &kol_ch,
	int &first_ch, int &step, int &kol_TimeAndVT,
	DynamicArray<String> &TimeAndVT, int &Summa, int &CurrentSumma) {
	ifstream f;
	f.open(filename, ios::in | ios::binary);
	if (!f.is_open())
		return false;

	char Signature[4];
	f.read((char *)&Signature, sizeof(Signature)); // сигнатура
	if (strcmp("KLS", Signature)) {
		f.close();
		return false;
	}
	f.read((char*)&kol_TimeAndVT, sizeof(kol_TimeAndVT));
	TimeAndVT.Length = kol_TimeAndVT;
	int length = 0;
	for (int i = 0; i < kol_TimeAndVT; i++) {
		f.read((char*)&length, sizeof(length));
		f.read((char*)TimeAndVT[i] /* s */ .c_str(), (length*2) + 2);
		// умножаем на 2,т.к один символ Unicode строки кодируется
		TimeAndVT[i] = String(TimeAndVT[i] /* s */ .c_str());
		// двумя байтами и + завершающие 2 байта(не 1 байт,т.к Unicode строка)
	} // без .c_str() не чистает первый столбик
	f.read((char*)&kol_ch, sizeof(kol_ch));
	CurrentSumma += kol_ch;
	f.read((char*)&first_ch, sizeof(first_ch));
	CurrentSumma += first_ch;
	f.read((char*)&step, sizeof(step));
	CurrentSumma += step;

	int kol_elem = 0;
	f.read((char*)&kol_elem, sizeof(kol_elem));
	CurrentSumma += kol_elem;
	masavtobus.Length = kol_TimeAndVT - 1;
	if (kol_elem) {
		for (int j = 0; j < masavtobus.Length; j++) {
			masavtobus[j].Length = kol_elem / masavtobus.Length;
			f.read((char*)&masavtobus[j][0],
				sizeof(masavtobus[0][0])*(masavtobus[0].Length));
		}
	}

	f.read((char*)&Summa, sizeof(Summa));
	f.close();
	return true;
}

bool SaveFile(const char * filename,
	 DynamicArray<DynamicArray<int> > &masavtobus, int first_ch,
	int step, int kol_ch, int kol_TimeAndVT, DynamicArray<String> &TimeAndVT,
	int Summa) {
	ofstream f;
	f.open(filename, ios::out | ios::binary);
	if (!f.is_open()) {
		return false;
	}
	char Signature[4] = "KLS";
	f.write((char *)Signature, sizeof(Signature));
	f.write((char*)&kol_TimeAndVT, sizeof(kol_TimeAndVT));
	for (int i = 0; i < kol_TimeAndVT; i++) {
		// String s = TimeAndVT[i];
		// int length = s.Length();
		int length = (TimeAndVT[i]).Length();
		f.write((char*)&length, sizeof(length));
		f.write((char*)(TimeAndVT[i]) /* s */ .c_str(), (length*2) + 2);
		// умножаем на 2,т.к один символ Unicode строки кодируется
		// двумя байтами и + завершающие 2 байта(не 1 байт,т.к Unicode строка)
	}

	f.write((char*)&kol_ch, sizeof(kol_ch));
	f.write((char*)&first_ch, sizeof(first_ch));
	f.write((char*)&step, sizeof(step));

	int kol_elem = (masavtobus[0].Length) * masavtobus.Length;
	Summa += kol_elem;
	f.write((char*)&kol_elem, sizeof(kol_elem));
	if (kol_elem) {
		for (int j = 0; j < masavtobus.Length; j++) {
			f.write((char*)&masavtobus[j][0],
				sizeof(masavtobus[0][0])*(masavtobus[0].Length));
		}
	}
	f.write((char*)&Summa, sizeof(Summa));
	f.close();
	return true;
}
