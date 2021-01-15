#ifndef _pString_h
#define _pString_h

#include <Arduino.h>

// remember "a" is an array of char, char a[]; 'c' only works for 1 char not an array ie char c = 'c';
// so any k value should be in this type of quotes "ab"
// and any c value should be in this type of quotes 'a'

//#define "\4GET "
;
void pPrint(const char s[]);
void pPrintln(const char s[]);

byte StrLenZ(const char s[]);

//{ the k String is looped through a char at a time }
byte InStringS(const char s1[], const char s2[], boolean start = true);
boolean StrCom(const char s1[], const char s2[] );// True if the string array have the same values. (i.e. s1[0..s1[0]] == s2[0..s1[0]] )
boolean StrComSZ(const char s1[], const char z1[] );
byte FindFirstC(const char s[], const char c);
byte FindFirstK(const char s[], const char k[]);


void DelFirst(char s[], byte n);
void DelToC(char s[], byte c);
void DelToC1(char s[], byte c);

#define SplitC1_(A,C,R) SplitC1(A, C, R, sizeof(R) - 1 );
void Split1(char s[], byte n, char r[], byte rLength);
#define SplitC2_(A,C,R) SplitC2(A, C, R, sizeof(R) - 1 );
void SplitC2(char a[], char c, char r[], byte rLength);
#define SplitK2_(A,C,R) SplitK2(A, C, R, sizeof(R) - 1 );
void SplitK2(char a[], const char k[], char r[], byte rLength);

#define SplitK1_(A,C,R) SplitK1(A, C, R, sizeof(R) - 1 );
void SplitK1( char s[],  const char k[], char r[], byte rLength);


;
#endif
