//部分復元可能な(2,3)しきい値秘密分散法 restore.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define k 2
#define n 3
#define p 19999999
//#define p 999999999999999989
#define DATAS 3
#define WLEN 1

long long int R;
long long int w[n][k];
long long int W[k][n];
long long int X[n];
long long int u[DATAS];
long long int Wu[k];
long long int keyX[k][k];
long long int invX[k][k];
long long int SR[k];
long long int reS[DATAS];
long long int bit = 255;


//関数の宣言
long long int mod_p(long long int num);
long long int rrand();
void create_keyX();
void create_matWu();
void create_inv_matX();
void create_matSR();
void create_matreS();
void file_input();

//main function
int main(void){
  int i,j;

  //シェアファイルを読み込む
  file_input();
  printf("hoge4\n");
  //シェア行列の確認
  printf("シェア行列W\n");
  for(i=0;i<k;i++){
    for(j=0;j<n;j++){
      printf("%lld\t",W[i][j]);
    }
    printf("\n");
  }

  //紐付け情報の確認
  printf("紐付け情報\n");
  for(i=0;i<DATAS;i++){
    printf("%lld\n",u[i]);
  }

  //行列Wuの確認
  create_matWu();

  //行列keyXの確認
  create_keyX();

  //逆行列の確認
  create_inv_matX();
  //SRの確認
  create_matSR();
  //reSの確認
  create_matreS();

  return 0;
}

//関数の定義
void file_input(){
  int i,j,m = 0;
  int l = 0;
  char fname0[] = ("share0.txt");
  char fname1[] = ("share1.txt");
  FILE *fp_input1;
  FILE *fp_input2;

//初期化
  for(i=0;i<k;i++){
    for(j=0;j<n;j++){
      W[i][j] = 0;
      X[j] = 0;
    }
  }
  for(i=0;i<DATAS;i++){
    u[i] = 0;
  }

  i=0;
  j=0;

  fp_input1 = fopen(fname0, "r");
  if(fp_input1 == NULL) {
    printf("%s file not open\n", fname0);
    exit(1);
  }
  while((fgets(fname0,256,fp_input1)) != NULL) {
    if(i == 0 || i == 1 || i == 2){
      w[i][j] = atoll(fname0);
      i++;
    }else if(i == 3){
        X[l] = atoll(fname0);
        i++;
        l++;
    }else {
      u[m] = atoll(fname0);
      i++;
      m++;
    }
  }
  i = 0;
  m = 0;
  j++;

  fp_input2 = fopen(fname1, "r");
  if(fp_input2 == NULL) {
    printf("%s file not open\n", fname1);
    exit(1);
  }
  while((fgets(fname1,256,fp_input2)) != NULL) {
    if(i == 0 || i == 1 || i == 2) {
      w[i][j] = atoll(fname1);
      i++;
    }else if(i == 3){
      X[l] = atoll(fname1);
      l++;
      i++;
    }else {
      u[m] = atoll(fname1);
      i++;
      m++;
    }
  }

  //転置行列
  W[0][0] = w[0][0];
  W[0][1] = w[1][0];
  W[0][2] = w[2][0];

  W[1][0] = w[0][1];
  W[1][1] = w[1][1];
  W[1][2] = w[2][1];

  printf("シェア行列W\n");
  for(i=0;i<k;i++){
    for(j=0;j<n;j++){
      printf("%lld\t",W[i][j]);
    }
    printf("\n");
  }

  /*printf("X[0] -> %lld\n", X[0]);
  printf("X[1] -> %lld\n", X[1]);
  printf("hogehogehogehogehogehogehoge");
  int z;
  printf("hogehogehogehoge");
  for(z = 0; z < n; z++){
    printf("%lld ", X[z]);
  }*/

  fclose(fp_input1);
  fclose(fp_input2);
  /*W[0][0] = 246706607414321945;
  W[0][1] = 113594921781470985;
  W[0][2] = 356135299915159865;
  W[1][0] = 112158681655322950;
  W[1][1] = 512088345647957573;
  W[1][2] = 605465576280199282;
  X[0] = 1341417039;
  X[1] = 617649301;
  u[0] = 65536;
  u[1] = 0;
  u[2] = 1;*/

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

//create_matWu関数の定義
void create_matWu(){
  int i,j;
  for(i=0;i<k;i++){
    for(j=0;j<n;j++){
      Wu[i] += mod_p(W[i][j]*u[j]);
    }
    Wu[i] = mod_p(Wu[i]);
  }
  printf("行列Wu\n");
  for(i=0;i<k;i++){
    printf("%lld\n",Wu[i]);
  }
}
//create_keyX関数の定義
void create_keyX(){
  int i,j;
  for(i=0;i<k;i++){
    for(j=0;j<k;j++){
      keyX[i][j]=(int)pow(X[i],j);
    }
  }

  printf("keyX行列\n");
  for(i=0;i<k;i++){
    for(j=0;j<k;j++){
      printf("%lld\t",keyX[i][j]);
    }
    printf("\n");
  }
}
//create_inv_matX関数の定義
void create_inv_matX(){
  long long int det;
  long long int tmp;
  long long int sqr = 0;
  int i,j;
  det = (keyX[0][0] * keyX[1][1] - keyX[0][1] * keyX[1][0]);
  if(det<0){
    det = det * -1;
    while(mod_p(det*sqr)!=1){
      sqr += 1;
    }
    sqr = mod_p(sqr);

    tmp = mod_p(-1*keyX[0][0]);
    keyX[0][0] = mod_p(-1*keyX[1][1]);
    keyX[1][1] = tmp;

    for(i=0;i<k;i++){
      for(j=0;j<k;j++){
        invX[i][j] = mod_p(keyX[i][j]*sqr);
      }
    }
  }else{
    while(mod_p(det*sqr)!=1){
      sqr += 1;
    }
    sqr = mod_p(sqr);

    tmp = keyX[0][0];
    keyX[0][0] = keyX[1][1];
    keyX[1][1] = tmp;

    keyX[0][1] = mod_p(-1*keyX[0][1]);
    keyX[1][0] = mod_p(-1*keyX[1][0]);

    for(i=0;i<k;i++){
      for(j=0;j<k;j++){
        invX[i][j] = mod_p(keyX[i][j]*sqr);
      }
    }
  }

  //print
  printf("逆行列X\n");
  for(i=0;i<k;i++){
    for(j=0;j<k;j++){
      printf("%lld\t",invX[i][j]);
    }
    printf("\n");
  }
}

//create_matSR関数の定義
void create_matSR(){
  int i,j;
  for(i=0;i<k;i++){
    for(j=0;j<k;j++){
      SR[i] += mod_p(invX[i][j]*Wu[j]);
    }
    SR[i] = mod_p(SR[i]);
  }

  //print
  printf("行列SR\n");
  for(i=0;i<k;i++){
    printf("%lld\n",SR[i]);
  }
}
//create_matreS関数の定義
void create_matreS(){
  int i=0;
  int j;
  while(SR[0] != 0){
    reS[i] = SR[0] & bit;
    SR[0] = SR[0] >> 8;
    i++;
  }

  //printf
  printf("部分復元データ\n");
  for(j=i-1;j>=0;j--){
    printf("%lld\t",reS[j]);
  }
  printf("\n");
  //printf
  char reS2[DATAS];
  printf("部分復元データ\n");
  for(j=i-1;j>=0;j--){
    reS2[j] = (char)reS[j];
    printf("%c",reS2[j]);
  }
  printf("\n");
}
