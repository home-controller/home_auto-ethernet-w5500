// pascal type strings.
// Arrays of char with s[0] being the string length. So max length of 255
#include "pString.h"
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

byte StrLenZ(const char z[]){
  byte i;
  for(i=0; i< 0xFF; i++){
    if(z[i] == 0){ return i; }
  }
  return 0xFF; //string is longer than can be returned in a byte or no 0 termination for the string.
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
void CopyFirst(char a[], byte n, char r[], byte rLength){
  byte i;
  if(n>rLength){ n = rLength; }
  if(n>a[0]){ n = a[0]; }
  for(i=1; i <= n; i++){
    r[i] = a[i];
  }
  r[0] = n;
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
  if (l != s1[i]) return false;
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
//Split at k. Delete's char at k. Return first part in r and del it from a.
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



//
