// pascal type strings.
// Arrays of char with s[0] being the string length. So max length of 255
#include "pString.h"
#include "defs.h"
//{ the k String is looped through a char at a time }
// if k or c not found in DelToC etc then do nothing
//
//struct v {
//   union { // anonymous union
//      struct { int i, j; }; // anonymous structure
//      struct { long k, l; } w;
//   };
//   int m;
//} v1;

//struct pStr {
//  union{
//    struct {
//      byte l;
//      char p[];
//    };
//    struct {
//      char c[];
////      byte z;
//    };
////    char [];
//  };
//};
//
//pStr test = {4, "test"};

//class pString{
//  char s[1];
//  char c[];
//  
//};

byte StrLenZ(const char z[]){
  byte i;
  for(i=0; i< 0xFF; i++){
    if(z[i] == 0){ return i; }
  }
  return 0xFF; //string is longer than can be returned in a byte or no 0 termination for the string.
}

void cTo_pString(char s[], byte l){
  byte x;
  for(x=l;x>0;x--){
    s[x] = s[x-1];
  }
  s[0] = l;
}

void cTo_pString(char s[]){
  byte l = StrLenZ(s);
  cTo_pString(s, l);
}

void pPrint(const char s[]){
  byte i;
  byte ln = s[0];
  char c;
  Serial.print('[');
  Serial.print(ln);
  Serial.print("]'");
  for(i=1; i <= s[0]; i++){
    c = s[i];
    //Serial.print('[');
    //Serial.print(i);
    //Serial.print("]='");
    Serial.print(c);
    //Serial.print('\'');
  }
  Serial.print("'");
}
void pPrintln(const char s[]){
  pPrint(s);
  Serial.println();
}
//+++++++++++++++Copying parts of the string+++++++
void CopyFirst(const char a[], byte n, char r[], byte rLength){
  byte i;
  if(n>rLength){ n = rLength; }
  if(n>a[0]){ n = a[0]; }
  for(i=1; i <= n; i++){
    r[i] = a[i];
  }
  r[0] = n;
  if(n<rLength){r[n+1] = 0; }
}


//===============Finding stuff========
byte InStringS(const char s1[], const char s2[], boolean start){//set start to false is s2 can match part way along s1
  byte i = 1;
  byte p = 0;
  while(( s1[0] >= i + p) and  s2[0] >= i){
    if(s1[i+p] == s2[i]) {
      if(i >= s2[0]){ return p+1; }//not sure about this. maybe first pos should be 0
      i++;
    }
    else{
      if(start){ return 0; }
      p += i;
      i = 1;
    }
  }
  return 0;// for not found.
}
boolean StrCom(const char s1[], const char s2[]){// True if the string array have the same valuse.
  if(s1[0] == s2[0]){
    if(InStringS(s1, s2) > 0) return true;
  }
  return false;
}

boolean StrComSZ(const char s1[], const char z1[] ){
  byte i, l;
  l = StrLenZ(z1);
  if (l != s1[0]) return false;
  for(i=1;i<=s1[0]; i++){
    if((s1[i] != z1[i-1]) ) {return false; }
  }
  
  return true;
 
}


byte FindFirstC(const char s[], const char c){
byte  x;
  for(x=1; x <= s[0]; x++){ 
    if(s[x] == c){ return x; }
  }
  return 0;
}
byte FindFirstK(const char s[], const char k[]){
byte  x,i;
  for(x=1; x <= s[0]; x++){
    for(i=1; i <= k[i]; i++){
      if(s[x] == k[i]){ return x; }
    }
  }
  return 0;
}

//==============delete string functions===============
void DelFirst(char s[], byte n){
byte x;  
  if(s[0] > n and n > 0){
    for(x=1; x + n <= s[0]; x++){
      s[x] = s[x+n];
    }
    s[0] -= n;
  } else {
    s[0] = 0;
  }
}
void DelToC(char s[], byte c){
// delete first part of s up to and including c
byte x;
  x = FindFirstC(s, c);
  DelFirst(s, x);
}
void DelToC1(char s[], byte c){
// delete first part of s up to but not including c
byte x;
  x = FindFirstC(s, c);
  if(x > 1){
    DelFirst(s, x-1);
  }
}

void Split1(char s[], byte n, char r[], byte rLength){
//Split at n. Return first part in r. Del returned part from s leaving n as first char.
//Split off and Return s up to but not including n
  if(n>1){
    CopyFirst(s, n - 1 , r, rLength);
    DelFirst(s, n-1);
  } else {r[0] = 0; }
}

void SplitC1(char s[], char c, char r[], byte rLength){
//Split off and Return s up to but not including char, c found in s
byte x;
  x = FindFirstC(s, c);
  Split1(s, x, r, rLength);
}

void Split2(char s[], byte n, char r[], byte rLength){
//Split at n. Delete's char at n. Return first part in r and del first n from s.
  if(n>1){
    CopyFirst(s, n -1 , r, rLength);
  } else {r[0] = 0; }
    DelFirst(s, n);
}



void SplitC2(char s[], char c, char r[], byte rLength){
//Split at c. Delete's char at c. Return first part in r and del it from s.
byte x;
  x = FindFirstC(s, c);
  Split2(s, x, r, rLength);
}

void SplitK1( char s[], const char k[], char r[], byte rLength){
//Split at k. Return first part in r and del it from a.
byte x;
  x = FindFirstK(s, k);
  Split1(s, x, r, rLength);
}

void SplitK2( char s[], const char k[], char r[], byte rLength){
//Split at k. Delete's char at k. Return first part in r and del it from a.
byte x;
  x = FindFirstK(s, k);
 /* Serial.println();
  Serial.print("k '");
  pPrint(k);
  Serial.print("' found at '");
  Serial.print(x);
  Serial.print("' in :'");
  pPrint(s);
  Serial.println("'");*/
  Split2(s, x, r, rLength);
}

//==========================Joining strings======================

//#define SplitK1_(A,C,R) SplitK1(A, C, R, sizeof(R) - 1 );
void addC(char s1[], char c, byte rLength){// Max return length is the sizeof the array pointed to by s1 in this case.
  if(rLength > s1[0]) {
    s1[0]++;
    s1[ (byte) (s1[0]) ] = c;
  }
  if(rLength > s1[0]){
    s1[s1[0] + 1] = 0;// make the string C style turminnated to if we have space.
  }
}

void addS(char s1[], const char s2[], byte rLength){// Max return length is the sizeof the array pointed to by s1 in this case.
  byte cl, i, l;
#ifdef _pString_debug
  Serial.print(F("s1 = ") );
  pPrintln(s1);
  Serial.print(F("s2 = ") );
  pPrintln(s2);
  Serial.print(F("rLength = ") );
  Serial.println(rLength);
#endif
  if( (s1[0] + s2[0]) > rLength){
    if( s1[0] >= rLength) { cl=0; }
    else { cl =  rLength - s1[0]; }
  } else{
    cl = s2[0];
  }
  l = s1[0];
  for(i = 1; i <= cl; i++){
    s1[i+l] = s2[i];
  }
  s1[0] += cl;
  if(rLength > s1[0]){
    s1[s1[0] + 1] = 0;// make the string C style turminnated to if we have space.
  }
#ifdef _pString_debug
  Serial.print(F("after adding s2, s1 = ") );
  pPrintln(s1);
#endif  

}

void JoinS1C(const char* s1, const char* s2, char c, char r[], byte rLength){
  if(r != s1){
    r[0] =0;
    addS(r, s1, rLength);
  }
  addC(r, c, rLength);
  addS(r, s2, rLength);
}

void JoinS(const char s1[], const char s2[], char r[], byte rLength){
#ifdef _pString_debug
  Serial.print(F("Entering JoinS ") );
  Serial.print(F("s1 = ") );
  pPrintln(s1);
  Serial.print(F("s2 = ") );
  pPrintln(s2);
  Serial.print(F("rLength = ") );
  Serial.println(rLength);
#endif
  if((r != s1) ){
    r[0] =0;
    addS(r, s1, rLength);
  }
  addS(r, s2, rLength);
}

//byte lengthZ(char z[]){ //ops already have 1 above
//  byte i = 0;
//  while( (i < 255) and (x[i] != 0) ){
//    i++;
//  }
//  return i;
//}
//
