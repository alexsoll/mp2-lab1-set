// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if  (len < 0)
		throw len;
	BitLen = len;
	MemLen = (BitLen/sizeof(TELEM)) + 1;
	pMem = new TELEM[MemLen];
	
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ( (n < 0) || (n>=BitLen) )
		throw n;
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ( (n < 0) || (n >= BitLen) )
		throw n;
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ( (n < 0) || (n>=BitLen))
		throw n;
	int GMI = GetMemIndex(n);
	int GMM = GetMemMask(n);
	GMM &= pMem[GMI];
	if (GMM == 0) return 0;
	else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return false;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) return false;
	for (int i = (MemLen - 1) * sizeof(TELEM); i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i)) return false;
	return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++) {
		tmp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++) {
		tmp.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++) {
		tmp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++) {
		tmp.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++) {
		tmp.pMem[i] = ~pMem[i];
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char sym;
	istr >> sym;
	while (sym == '0' || sym == '1') {
		if (sym == '0')
			bf.ClrBit(i);
		else bf.SetBit(i);
		i++;
		istr >> sym;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; i++)
		if (bf. GetBit(i))
			ostr << 1;
		else
			ostr << 0;
	return ostr;
}
