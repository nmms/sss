//部分復元可能な(2,3)しきい値秘密分散法　backup.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define k 2
#define n 3
//#define p 99999989
#define p 999999999999999989
#define DATAS 3 //データの項目数(分割数)
#define WLEN 1 //1項目の文字数

long long int temp[DATAS][WLEN];
long long int bitlen[DATAS];
long long int bytelen[DATAS]; //分割データサイズl_i
long long int data[DATAS];
long long int X[n][k];  //vandermonde
long long int R;  //randomNumber
long long int A[DATAS][k];
long long int XA[DATAS][n];
long long int u[DATAS]; //紐付け情報
char printc[DATAS*WLEN]; //元データ


//関数の宣言
long long int mod_p(long long int num);
long long int rrand();
void create_matX();
void create_matA();
void create_matXA();
void create_matu();
void file_input();
void file_output();


//main function
int main(void){



  srand((unsigned)time(NULL));
  int i,j;
  //printf("%lld\n", p);
  //fileからデータを取り込む
  file_input();
  printf("フルデータ:%s\n",printc);
  printf("格納されているデータ\n");
  for(i=0;i<DATAS;i++){
    for(j=0;j<WLEN;j++){
      printf("分割データ%dの値:%lld\t",i+1,temp[i][j]);
    }
    printf("\n");
  }
  printf("各データサイズ\n");
  for(i=0;i<DATAS;i++){
    printf("%lld\t",data[i]);
  }
  printf("\n");
  printf("各データの長さ(byte)\n");
  for(i=0;i<DATAS;i++){
    bytelen[i] = bitlen[i] - bitlen[i -1];
  }
  for(i=0;i<DATAS-1;i++){
    bytelen[i] = bytelen[i] - 1;
    printf("%lld\t",bytelen[i]);
  }
  printf("%lld\n",bytelen[DATAS-1]);

//ここで紐付け情報を操作できる
//全て復元
  /*bytelen[0] = bytelen[1] + bytelen[2];
  bytelen[1] = bytelen[2];
  bytelen[2] = 1;*/
  //1,3復元
  bytelen[0] = bytelen[1] + bytelen[2];
  bytelen[1] = 0;
  bytelen[2] = 1;

  //vandermonde行列Xの確認
  printf("vandermode行列X\n");
  create_matX();
  for(i=0;i<n;i++){
    for(j=0;j<k;j++){
      printf("%lld\t",X[i][j]);
    }
    printf("\n");
  }

  //行列Aの確認
  printf("行列A\n");
  create_matA();
  for(i=0;i<DATAS;i++){
    for(j=0;j<k;j++){
      printf("%lld\t",A[i][j]);
    }
    printf("\n");
  }

  //行列XAの確認
  printf("行列XA\n");
  create_matXA();
  for(i=0;i<DATAS;i++){
    for(j=0;j<n;j++){
      printf("%lld\t",XA[i][j]);
    }
    printf("\n");
  }

  //紐付け情報の確認
  printf("紐付け情報\n");
  create_matu();
  for(i=0;i<DATAS;i++){
    printf("%lld\n",u[i]);
  }

  //shareを書き込む
  file_output();


  return 0;
}

//関数の定義
//file_input関数の定義
void file_input(){
  FILE *fp_input;
  FILE *fp;
  char c;
  char rc;
  long long int i = 0, j = 0, l = 0;

  fp = fopen("data.txt", "r");
  if(fp == NULL) {
    printf("file not open\n");
    exit(1);
  }

  fp_input = fopen("data.txt", "r");
  if(fp_input == NULL) {
    printf("file not open\n");
    exit(1);
  }
  //元データ表示用
  while((rc = fgetc(fp)) != EOF) {
    if(rc == ','){
    }else{
      printc[l] = rc;
      l++;
    }
  }


  //分割データ格納用
  while((c = fgetc(fp_input)) != EOF) {
    if(c != ',') {
      temp[i][j] = (long long int)c;
      if(j == 0) {
        data[i] = temp[i][j];
      }else{
        data[i] = data[i] << 8;
        data[i] += temp[i][j];
      }
      j++;
    }else{
      bitlen[i] = ftell(fp_input);
      i++;
      j = 0;
    }
  }
  bitlen[i] = ftell(fp_input);
  fclose(fp_input);
  fclose(fp);
}
//mod_p関数の定義
long long int mod_p(long long int num) {
  if (num < 0) {
    num *= -1;
    return p - (num % p);
  } else {
    return num % p;
  }
}
//rrand関数の定義
long long int rrand(){
  R = rand();
  return R;
}
//create_matX関数の定義
void create_matX(){
  int i,j;
  for(i=0;i<n;i++){
    for(j=0;j<k;j++){
      X[i][j]=mod_p((int)pow(rrand(),j));
      while(X[i][j] == 0){
        X[i][j]=mod_p((int)pow(rrand(),j));
      }
    }
  }
  while(X[0][1] == X[1][1] || X[0][1] == X[2][1]){
    X[0][1]=mod_p((int)pow(rrand(),1));
  }
  while(X[1][1] == X[2][1] || X[1][1] == X[0][1]){
    X[1][1]=mod_p((int)pow(rrand(),1));
  }
}
//create_matA関数の定義
void create_matA(){
  int i,j;
  for(i=0;i<DATAS;i++){
    for(j=0;j<k;j++){
      if(j == 0){
        A[i][j] = mod_p(data[i]);
      }else{
        A[i][j] = mod_p(rrand());
      }
    }
  }
}
//create_matXA関数の定義
void create_matXA(){
  int i,j,l;
  for(i=0;i<DATAS;i++){
    for(j=0;j<n;j++){
      for(l=0;l<k;l++){
        XA[i][j] += X[j][l]*A[i][l];
      }
      XA[i][j] = mod_p(XA[i][j]);
    }
  }
}
//file_output関数の定義
void file_output(){
  int j;
  for(j=0;j<n;j++){
    char filename[j];
    FILE *fp_output;
    sprintf(filename, "share%d.txt", j);
    fp_output = fopen(filename, "w");
    if (fp_output == NULL) {
      printf("file not open\n");
      exit(1);
    }
    if(j == 0){
      fprintf(fp_output, "%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n",XA[0][0],XA[1][0],XA[2][0],X[0][1],u[0],u[1],u[2]);
    }else if(j == 1) {
      fprintf(fp_output, "%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n",XA[0][1],XA[1][1],XA[2][1],X[1][1],u[0],u[1],u[2]);
    }else {
      fprintf(fp_output, "%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n%lld\n",XA[0][2],XA[1][2],XA[2][2],X[2][1],u[0],u[1],u[2]);
    }
    fclose(fp_output);
  }
}
//紐付け情報
void create_matu(){
  int i;
  for(i=0;i<DATAS-1;i++){
    if(bytelen[i] == 0){
      u[i] = 0;
    }else{
      u[i] = (int)pow(256,bytelen[i]);
    }
    u[DATAS-1] = 1;
  }
}
