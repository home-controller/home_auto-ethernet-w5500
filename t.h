//gcc's -E 


#define t(a,b,c) a##b##c
#define t2(a,b,c) t(a,b,c) #a
char s[] = t2( ( t(a ,as, ad) ) );
char x[] = t("h1/c","2","/");
//char y[] = t(1t/, c2/, f1);
int square(int num) {
    return num * num;
}
