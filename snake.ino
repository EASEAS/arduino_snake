#include "LedControl.h"
#include "binary.h"

LedControl lc=LedControl(8,10,9,1);

unsigned int pos_x  = 0;
unsigned int pos_y  = 0;
unsigned int dir = 0;
unsigned int snake_length = 1;
int armor  = 3;

byte output[8]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int middleBoard[8][8] = { {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0}};
void lose();
void setup() {
  Serial.begin(9600);
  
  lc.shutdown(0,false);
  // Set brightness to a medium value
  lc.setIntensity(0,0);
  // Clear the display
  lc.clearDisplay(0);  

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);

  randomSeed(analogRead(0));
  add_food();
}




void loop() {
    
    if (armor < 0 )
    {
      while(1)
      {
        lose();
      }
    }

    delay(100);
    int joy_x = analogRead(A1) - 500;
    int joy_y = analogRead(A2) - 500;
    delay(100);

    //thresh hold for joy stick
    //figure out joy direction
    if (abs(joy_x) > 100)
    {
      if (dir != 1 && dir!= 4){
        if (joy_x > 0) dir = 4;
        else dir = 1;
      }
    }else if (abs(joy_y) > 100)
    {
      if (dir != 2 && dir!= 8){
        if (joy_y > 0) dir = 8;
        else dir = 2;
      }
    }

    //Propogate direction
    if (dir == 1)
    {
          if (pos_x == 7) armor--;
          else pos_x ++;
          
    }else if (dir == 2)
    {
          if (pos_y == 7) armor--;
          else pos_y ++;
          
    }else if (dir == 4 )
    {
          if (pos_x == 0) armor--;
          else pos_x --;
    }else if (dir == 8)
    {
          if (pos_y == 0) armor--;
          else pos_y --;
    }

    //clip variables to valid positions
    pos_y = max(min(pos_y,7 ),0);
    pos_x = max(min(pos_x,7 ),0);

    //manage dot matrix display and game board
    clearBoard();
    if (middleBoard[pos_x][pos_y] == 100){
      add_length();
      snake_length++;
      add_food();
    }else if (middleBoard[pos_x][pos_y] != 0 )
    {
      while(1)lose();

    }
    middleBoard[pos_x][pos_y] = snake_length;
    boardToMatrix();
    
    //output[pos_y] = (0x00)|(1<<pos_x);

}

void win()
{
  clearBoard();
  for (unsigned int j = 0 ; j < 8 ; j++)
  {
    

    for (int i = 0 ; i < 8 ; i++)
    {
      lc.setRow(0,i,output[i]);
    }
    
  }
}
void lose ()
{
    for (unsigned int i = 0 ; i < 8 ; i++)
    {
      output[i] = 0xAA <<(i%2) | 1>>((i+1)%2);
    }
    for (int i = 0 ; i < 8 ; i++)
    {
      lc.setRow(0,i,output[i]);
    }
    delay(300);
    for (unsigned int i = 0 ; i < 8 ; i++)
    {
      output[i] = 0xAA<<((i+1)%2) | 1>>((i)%2);
    }
      for (int i = 0 ; i < 8 ; i++)
    {
      lc.setRow(0,i,output[i]);
    }
    delay(300);
}
void clearBoard()
{
    for (int i = 0 ; i < 8 ; i++)
    {
      output[i] = 0x00;
    }
    for (int i = 0 ; i < 8 ; i++)
    {
      lc.setRow(0,i,output[i]);
    }
}
void boardToMatrix()
{
  for (unsigned int i = 0 ; i < 8 ; i++)
  {
      for (unsigned int j = 0 ; j < 8 ; j++)
      {
          if (middleBoard[i][j] != 0 )
          {
            if ( middleBoard[i][j] != 100)  middleBoard[i][j] = max( middleBoard[i][j]--, 0);
            output[i] |= (1<<j);
          }else{
            output[i] &= ~(1<<j);
          }
      }
  }
    
    for (int i = 0 ; i < 8 ; i++)
    {
      lc.setRow(0,i,output[i]);
    }
}
void add_food()
{
    while(1)
    {
      int randx = random(8);
      int randy = random(8);
      if (middleBoard[randx][randy] == 0)
      {
        middleBoard[randx][randy] = 100;
        break;
      }
    }

}
void add_length()
{
  for (unsigned int i = 0 ; i < 8 ; i++)
  {
      for (unsigned int j = 0 ; j < 8 ; j++)
      {
          if (middleBoard[i][j] != 0 &&  middleBoard[i][j] != 100 )  middleBoard[i][j]++;
      }
  }
}
