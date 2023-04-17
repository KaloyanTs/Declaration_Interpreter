
char AlarmSound_ds;

struct{
  unsigned B :1;

  char x,y;
  int z;
  //int A;
  float F;

}DI;

struct{
  unsigned _:1;

  int Speed_01Hz;  // 75.0Hz <=> 52:35 [m:s]
  int Pump_SP; //0.1*c
  int Ratio_1; //0.1%
  int Ratio_2; //0.1%
  int CoolTime_min;
  int Pump_Hyst; //0.1*c
}Par;

