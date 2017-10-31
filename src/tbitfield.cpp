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
	MemLen = (BitLen/ 8 * sizeof(TELEM)) + 1;
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
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
} 

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))

		throw n;

	if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == 0) return 0;

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
	if (BitLen != bf.BitLen) return 0;
	int i = 0;
	for (i = 0; i < MemLen - 1; i++)
		if (pMem[i] != bf.pMem[i]) return 0;
	for (i = (MemLen - 1) * sizeof(TELEM); i < BitLen; i++)
		if (GetBit(i) != bf.GetBit(i)) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxlen, minlen, i;                         //   Протестировать позже
	if (BitLen >= bf.BitLen){

		maxlen = BitLen;
		minlen = bf.BitLen;
	}
	else {
		maxlen = bf.BitLen;
		minlen = BitLen;
	}
	TBitField res(maxlen);
	for (i = 0; i < minlen; i++) {
		//res.pMem[i] = pMem[i] | bf.pMem[i];
		if ((GetBit(i) | bf.GetBit(i)) == 1) {
			res.SetBit(i);
		}
		else {
			res.ClrBit(i);
		}
	}
	for (i = minlen; i < maxlen; i++) {
		if (BitLen >= bf.BitLen) {
			//res.pMem[i] = pMem[i];
			if (GetBit(i) == 1) {
				res.SetBit(i);
			}
			else {
				res.ClrBit(i);
			}
		}
		else {
			//res.pMem[i] = bf.pMem[i];
			if (bf.GetBit(i) == 1) {
				res.SetBit(i);
			}
			else {
				res.ClrBit(i);
			}
		}
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxlen, minlen, i;                            // Протестировать позже
	if (BitLen >= bf.BitLen) {
		maxlen = BitLen;
		minlen = bf.BitLen;
	}
	else {
		maxlen = bf.BitLen;
		minlen = BitLen;
	}
	TBitField res(maxlen);
	for (i = 0; i < minlen / (8 * sizeof(TELEM)); i++) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	for (i = (minlen / (8 * sizeof(TELEM))) * 32; i < minlen; i++) {
		if (GetBit(i) & bf.GetBit(i) == 1) res.SetBit(i);
		else res.ClrBit(i);
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++) {
		tmp.pMem[i] = ~(pMem[i]);
	}
	/*int index = BitLen - (MemLen - 1) * sizeof(TELEM) * 8;
	for (int i = 0; i < index; i++) {
		if (GetBit(i) == 1)
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);
	}*/
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
