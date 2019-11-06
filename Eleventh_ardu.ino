#include <SharpIR.h>
#include "U8glib.h"
#include <math.h>

#define ir A0 //sensor's pin
#define model 20150 //sensor's model

SharpIR SharpIR(ir, model);
int point = 0;
const int ArrSize = 300; //array's size (always bigger then Delta)
const int CUnfail = 50; //time in centisecond (if object interrupt laser's beam - it's not count)
int ArrDis[ArrSize]; //Massive holding distance
const int Limit = 200; // Limit distance
int InGoer = 0; //Incoming
int OutGoer = 0; //Outcoming
bool Right = true;
bool Stop = false;
float Min = 201; //global minimum
int NumMin; //minimum's number
float Min1, dMin;  // Minimal elements (local)
int RightC, LeftC, LeftLenght, RightLenght, x; //variables that allow detect minimum's rmoteness from the change of distance
int Unfail;
const int CdMin = 20; //difference between minimums that allow detect object go throght the enter or no)
bool UnLong = true;
int StopPoint = 0;
const int Delta = 200; // local interval's size 

void setup() {
  Serial.begin(9600);
  for (point; point < ArrSize; ++point)
  {
    ArrDis[point] = Limit;
  }
  point = 0;
}

void loop() {
  
  int dis=SharpIR.distance();  // get distance from sensor
  if (dis > Limit) 
  dis = Limit;
  ArrDis[point] = dis; //запись значений в массив
  Right = true;
  Min = 201;
  
  x = point - Delta;
  if (x < 0)
  x += ArrSize;
  
  if ((ArrDis[x] != Limit) && (Stop == false))  //Initialization laser interrupt's test 
  {
    LeftC = x; //number of first element of interruption
    for (int i=0; i<= Delta; i++)
    {
      x = point - Delta + i;
      if (x < 0)
      x += ArrSize; 
      if (ArrDis[x] < Min) 
      {
        Min = ArrDis[x];
        NumMin = x;  
        LeftLenght = i;
      }
      x = point -  i;
      if (x < 0)
      x += ArrSize; 
      if ((ArrDis[x] != Limit) && (Right == true))
      {
        RightC = x;
        Right = false;
      }
    }
    if ((ArrDis[point] == Limit)&&(UnLong))
    {
    RightLenght = RightC - NumMin;
    if (RightLenght < 0)
    RightLenght += ArrSize;
    
    Unfail = RightC - LeftC; 
    if (Unfail < 0)
    Unfail += ArrSize;
    
    if ((LeftLenght < RightLenght)&&(Unfail > CUnfail))  
    InGoer++;
    if ((LeftLenght > RightLenght)&&(Unfail > CUnfail))
    OutGoer++;
    
    StopPoint = RightC + Delta;
    if (StopPoint >= ArrSize)
    StopPoint = RightC + Delta - ArrSize;
      
    Stop = true;
    }
    if ((ArrDis[point] != Limit)&&(UnLong))
    {
    Min1 = Min;
    UnLong = false;
    }
    if ((ArrDis[point] == Limit)&&(!UnLong))
    {
    dMin = fabs(Min - Min1);
      
    RightLenght = RightC - NumMin; 
    if (RightLenght < 0)
    RightLenght += ArrSize;
    
    Unfail = RightC - LeftC; 
    if (Unfail < 0)
    Unfail += ArrSize;
    
    if ((LeftLenght < RightLenght)&&(Unfail > CUnfail)&&(dMin > CdMin))   
    InGoer++;
    if ((LeftLenght > RightLenght)&&(Unfail > CUnfail)&&(dMin > CdMin))   
    OutGoer++;
    
    StopPoint = RightC + Delta;   
    if (StopPoint >= ArrSize)
    StopPoint = RightC + Delta - ArrSize;
      
    Stop = true;
    UnLong = true;
    }
  }
  if (point == StopPoint)
  Stop = false;
  point++;
  if (point == ArrSize)
    point = 0;
}
