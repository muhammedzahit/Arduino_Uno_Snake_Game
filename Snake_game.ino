#include <LCD5110_Graph.h>
LCD5110 ekran(8,9,10,11,12);
enum Direction{Right,Left,Up,Down};
extern uint8_t SmallFont[];
extern uint8_t TinyFont[];

byte i,s; // sayaclar , counters
byte foodX,foodY; 
byte foodX2,foodY2;
Direction Status; // yilanin gidecegi yonu tutar , keeps next destination
Direction Prev; // yilanin onceden gittigi yonu tutar , keeps previous destination
int hiz; // oyun hizi , game speed 
int en_yuksek_1,en_yuksek_2,en_yuksek_3; // en yuksek skorlari tutan degiskenler , keep top scores

struct node{
  byte x,y,value;
  struct node * next;
};

bool control = true; // oyunun bitip bitmedigini kontrol eder , controls game playing or not 

int skor = 0;

struct node * start = NULL;

struct node * q = NULL;

typedef struct node node; 

// yilanin bulundugu pikselleri tutmak icin link list olusturuldu
void addNode(byte *x,byte *y){
  node * add = (node *)malloc(sizeof(node));
  add->x = *x;
  add->y = *y;
  add->next = NULL;
  if (start == NULL){
    start = add;
    return;
  }
  q = start;
  while (q->next != NULL){
    q = q->next;
  }
  q->next = add;
  return;
}


void removeTail(){
  q = start;
  start = q->next;
  free(q);
}


node* findHead(){
  q = start;
  while (q->next != NULL){
    q = q->next;
  }
  return q;
}

// baslangic yilanini olusturur , generate first snake
void yilanOlustur(byte *x,byte *y){
  byte c = *x;
  addNode(x,y);
  ekran.setPixel(*x,*y);
  for (i = 0 ; i<20; i++){
    c += 1;
  addNode(&c,y);
  ekran.setPixel(c,*y);
  }
  ekran.update();
}
// her oyun basladiginda bir onceki yilani temizler
// remove subsequent snake
void yilaniTemizle(){
  q->next = findHead();
  while(q->next != NULL){
    free(q->next);
    q->next = findHead();
  }
  free(q);
  start = NULL;
}
// yilanin gidecegi pikselin dolu olup olmadigini kontrol eder
// checks destination pixel filled or not
void check(byte *x,byte *y){
  q = start;
  while (q != NULL){
    if (q->x == *x && q->y == *y) control = false;
    q = q->next;
  }
}

// yemegi siler
void removeFood(byte *foodX,byte *foodY){
  ekran.clrRect(*foodX,*foodY,(*foodX)+1,(*foodY)+1);
}
// enum Direction{Right,Left,Up,Down};

// yilanin ilerleme fonksiyonlari , move function
void ilerle(enum Direction Status,enum Direction Prev){
  byte x,y,x2,y2;
  
  
  if (Status == Right && Prev == Left) control = false;
  if (Status == Left && Prev == Right) control = false;
  if (Status == Up && Prev == Down) control = false;
  if (Status == Down && Prev == Up) control = false;
  
  
  if (Status == Right && Prev != Left){
    q = findHead();
    x = q->x;
    x += 1;
    byte a = 84; // if icinde integer - byte kontrolu yapamayiz ! , we cannot compare byte types with integers 
    if (x == a){
      x = 0;
    }
    y = q->y;
  // eger yemekle karsilasirsa yemegi sil, yenisini olsutur ve kuyruk silme fonksiyonunu atla
  // if encounter with food , remove food and genarete new one and dont use removeTail func
    if (   (x == foodX || x == foodX2)  &&  (y == foodY || y == foodY2) ){
        skor += 10;
        removeFood(&foodX,&foodY);
        foodX = random(0,84);
        foodY = random(0,48);
        foodX2 = foodX+1;
        foodY2 = foodY+1;
        ekran.drawRect(foodX,foodY,foodX2,foodY2);
    }
    else {
        x2 = start->x;
        y2 = start->y;
        ekran.clrPixel(x2,y2);
        removeTail();
    }
    check(&x,&y);
    addNode(&x,&y);
  // eklenen x-y kordinatinin pixelini ekrana yazdir , print pixel for (x,y)
    ekran.setPixel(x,y);
    ekran.update();
  }
  else if (Status == Left && Prev != Right){
    q = findHead();
    x = q->x;
    x -= 1;
    byte a = -1; 
    if ( x == a ){
      x = 83;
    }
    y = q->y;
    if (   (x == foodX || x == foodX2)  &&  (y == foodY || y == foodY2) ){
        skor += 10;
        removeFood(&foodX,&foodY);
        foodX = random(0,84);
        foodY = random(0,48);
        foodX2 = foodX+1;
        foodY2 = foodY+1;
        ekran.drawRect(foodX,foodY,foodX2,foodY2);
    }
    else {
        x2 = start->x;
        y2 = start->y;
        ekran.clrPixel(x2,y2);
        removeTail();
    }
    check(&x,&y);
    addNode(&x,&y);
    ekran.setPixel(x,y);
    ekran.update();
  }
  else if (Status == Up && Prev != Down){
    q = findHead();
    x = q->x;
    y = q->y;
    y -= 1;
    byte a = -1;
    if (y == a){
      y = 47;
    }
    if (   (x == foodX || x == foodX2)  &&  (y == foodY || y == foodY2) ){
        skor += 10;
        removeFood(&foodX,&foodY);
        foodX = random(0,84);
        foodY = random(0,48);
        foodX2 = foodX+1;
        foodY2 = foodY+1;
        ekran.drawRect(foodX,foodY,foodX2,foodY2);
    }
    else {
        x2 = start->x;
        y2 = start->y;
        ekran.clrPixel(x2,y2);
        removeTail();
    }
    check(&x,&y);
    addNode(&x,&y);
    ekran.setPixel(x,y);
    ekran.update();
  }
  else if (Status == Down && Prev != Up) {
    q = findHead();
    x = q->x;
    y = q->y;
    y += 1;
    byte a = 48;
    if (y == a){
      y = 0;
    }
    if (   (x == foodX || x == foodX2)  &&  (y == foodY || y == foodY2) ){
        skor += 10;
        removeFood(&foodX,&foodY);
        foodX = random(0,84);
        foodY = random(0,48);
        foodX2 = foodX+1;
        foodY2 = foodY+1;
        ekran.drawRect(foodX,foodY,foodX2,foodY2);
    }
    else {
        x2 = start->x;
        y2 = start->y;
        ekran.clrPixel(x2,y2);
        removeTail();
    }
    check(&x,&y);
    addNode(&x,&y);
    ekran.setPixel(x,y);
    ekran.update();
  }
}
// print Menu func
void menuYaz(int page,int selection){
  if (page == 1 && selection == 1){
      ekran.clrScr();
      ekran.update();
      ekran.print("->Oyuna Basla",CENTER,0);
      ekran.print("Hizi Ayarla",CENTER,16);
    ekran.setFont(TinyFont);
    ekran.print("EN YUKSEK SKORLAR",CENTER,30);
    ekran.setFont(SmallFont);
      ekran.update();
  }
  else if (page == 1 && selection == 2){
      ekran.clrScr();
      ekran.update();
      ekran.print("Oyuna Basla",CENTER,0);
      ekran.print("->Hizi Ayarla",CENTER,16);
    ekran.setFont(TinyFont);
    ekran.print("EN YUKSEK SKORLAR",CENTER,30);
    ekran.setFont(SmallFont);
      ekran.update();
  }
  else if (page == 1 && selection == 3){
      ekran.clrScr();
      ekran.update();
      ekran.print("Oyuna Basla",CENTER,0);
      ekran.print("Hizi Ayarla",CENTER,16);
    ekran.setFont(TinyFont);
    ekran.print("->EN YUKSEK SKORLAR",CENTER,30);
    ekran.setFont(SmallFont);
      ekran.update();
  } 
  else if (page == 2 && selection ==1 ){
      ekran.clrScr();
      ekran.update();
      ekran.print("->100",CENTER,0);
      ekran.print("50",CENTER,8);
      ekran.print("25",CENTER,16);
      ekran.update();
  }
  else if (page == 2 && selection ==2 ){
      ekran.clrScr();
      ekran.update();
      ekran.print("100",CENTER,0);
      ekran.print("->50",CENTER,8);
      ekran.print("25",CENTER,16);
      ekran.update();
  }
  else if (page == 2 && selection ==3 ){
      ekran.clrScr();
      ekran.update();
      ekran.print("100",CENTER,0);
      ekran.print("50",CENTER,8);
      ekran.print("->25",CENTER,16);
      ekran.update();
  }
  if (page == 3 && selection == 1){
      ekran.clrScr();
      ekran.update();
      ekran.print("1-",LEFT,0);
    ekran.printNumI(en_yuksek_1,CENTER,0);
      ekran.print("2-",LEFT,16);
    ekran.printNumI(en_yuksek_2,CENTER,16);
    ekran.print("3-",LEFT,30);
    ekran.printNumI(en_yuksek_3,CENTER,30);
      ekran.update();
  }
}

// bir butona basilana kadar bekle , hold until push a button
void butonAl(  int *up_button,int *down_button,int* left_button,int* right_button,int* ok_button){
  delay(100); // oncedene girilen butonu okumamasi icin konulmustur, dont read previous pushed button
  while (digitalRead(*up_button) == HIGH && digitalRead(*down_button) == HIGH &&  digitalRead(*left_button) == HIGH && digitalRead(*right_button) == HIGH && digitalRead(*ok_button) == HIGH){}
}
// en yuksek skorlari tut , keep top scores
void enYuksek(int *skor){
  if (*skor > 0){
    
  if (*skor >= en_yuksek_1 ){
    en_yuksek_3 = en_yuksek_2;
    en_yuksek_2 = en_yuksek_1;
    en_yuksek_1 = *skor;
  }
  else if (*skor >= en_yuksek_2){
    en_yuksek_3 = en_yuksek_2;
    en_yuksek_2 = *skor;
  }
  else if (*skor > en_yuksek_3 ){
    en_yuksek_3 = *skor;
  }
  
  }
}

int up_button = 3;
int down_button = 4;
int left_button = 5;
int right_button = 6;
int ok_button = 13;

void setup() {

  ekran.InitLCD();
  ekran.setFont(SmallFont);
  randomSeed(100); // her seferinde farkli sayilar uret , generate different random numbers each time
  en_yuksek_1 = 0;
  en_yuksek_2 = 0;
  en_yuksek_3 = 0;
  
  
  pinMode(up_button,INPUT);  // UP
  pinMode(down_button,INPUT); // DOWN 
  pinMode(left_button,INPUT); // LEFT
  pinMode(right_button,INPUT); // RIGHT
  pinMode(ok_button,INPUT); // OK
  
  
}

bool yilan = true;

void loop() {
  skor = 0;
  control = true;
  Status = Right;
  Prev = Right;
    digitalWrite(up_button,HIGH);
    digitalWrite(down_button,HIGH);
    digitalWrite(left_button,HIGH);
    digitalWrite(right_button,HIGH);
    digitalWrite(ok_button,HIGH);
    hiz = 100;
    int page=1,secPage=1,flag=1;
    while (flag){
    if (page == 1){

      if (secPage == 1){

      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(down_button) == LOW || digitalRead(up_button) == LOW) secPage++;
      if (digitalRead(ok_button) == LOW) flag = 0;
    }

    if (secPage == 2){
      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(up_button) == LOW) secPage--;
      if (digitalRead(ok_button) == LOW){
      page++;
      secPage--;
      }
      if (digitalRead(down_button) == LOW) secPage++;
    }
  
    if (secPage == 3){
      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(up_button) == LOW) secPage--;
      if (digitalRead(ok_button) == LOW){
        secPage = 1;
        page = 3;
      }
      if (digitalRead(down_button) == LOW) secPage = 1;
    }
      
    }

    if (page == 2){

      if (secPage == 1){

      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(down_button) == LOW) secPage++;
      if (digitalRead(up_button) == LOW) secPage = 3;
      if (digitalRead(ok_button) == LOW){
      hiz = 100;
      flag = 0;
      }
    }
    
    if (secPage == 2){
      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(down_button) == LOW) secPage++;
      if (digitalRead(up_button) == LOW) secPage--;
      if (digitalRead(ok_button) == LOW){
      hiz = 50;
      flag = 0;
      }
    }

    if (secPage == 3){
      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      if (digitalRead(down_button) == LOW) secPage = 1;
      if (digitalRead(up_button) == LOW) secPage--;
      if (digitalRead(ok_button) == LOW){
      hiz = 25;
      flag = 0;
      }
    }
      
    }
    
    if (page == 3){
    
      menuYaz(page,secPage);
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      page = 1;
    
    }
    
  }
  ekran.clrScr();
  ekran.update();
  delay(300); 
  foodX = random(0,84);
  foodY = random(0,48);
  foodX2 = foodX+1;
  foodY2 = foodY+1;
  ekran.drawRect(foodX,foodY,foodX2,foodY2);
  byte x = 0;
  byte y = 0;
  yilaniTemizle();
  yilanOlustur(&x,&y);
  while(control){
    if (digitalRead(up_button) == LOW){
      Prev = Status;
      Status = Up;
      ilerle(Status,Prev);
      delay(hiz);
    }
    else if (digitalRead(down_button) == LOW){
      Prev = Status;
      Status = Down;
      ilerle(Status,Prev);
      delay(hiz);
    }
    else if (digitalRead(left_button) == LOW){
      Prev = Status;
      Status = Left;
      ilerle(Status,Prev);
      delay(hiz);
    }
    else if (digitalRead(right_button) == LOW){
      Prev = Status;
      Status = Right;
      ilerle(Status,Prev);
      delay(hiz);
    }
    else {
      ilerle(Status,Prev);
      delay(hiz);
    }
  
  }
    if (control == false){
      ekran.clrScr();
      ekran.print("Kaybettiniz",CENTER,8);
      ekran.print("Skorunuz",CENTER,16);
      ekran.printNumI(skor,CENTER,24);
      enYuksek(&skor);
      ekran.update();
      butonAl(&up_button,&down_button,&left_button,&right_button,&ok_button);
      delay(100);
    }
}
