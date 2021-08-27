/*
 *         DI PLC-S Header Fle
 * ==================================
 */
 //        INPUTS
struct {
    unsigned ManualMode   :1; // MB address: 69
    unsigned AutoMode     :1;
    unsigned fgPerformed  :1;
    unsigned fgEnManStart :1;
    unsigned fgEnManStop  :1;
    unsigned ClearLiqDosators  :1;

    signed char PerfProgBar;

    enum {wdNone, wdStart_M1, wdStart_M2, wdNetCommand, wdWaitCommand,
         wdWaitRefresh, wdWaitDosing, wdReady=10, } Dosing;
    int ReqQWater;       //27 [0.1L]
    int ReqTWater;       //28 [0.1*C]
    enum {wtcDoseStop, wtcStartDose_M1, wtcStartDose_M2,
          wtcContDose_Drain=10, wtcContDose_M1, wtcContDose_M2,
          wtClear=123, } Command;
}Water;

#define MODBUS_MASTER

#define mbmRead      (0)
#define mbmWrite     (1<<7)
#define mbmFn23      (1<<6)

#define mbmR_16      (1<<4)
#define mbmR_8       (1<<3)
#define mbmR_4       (1<<2)
#define mbmR_2       (1<<1)
#define mbmR_1       (1<<0)

#define mbmW_16      (mbmR_16 | mbmWrite)
#define mbmW_8       (mbmR_8 | mbmWrite)
#define mbmW_4       (mbmR_4 | mbmWrite)
#define mbmW_2       (mbmR_2 | mbmWrite)
#define mbmW_1       (mbmR_1 | mbmWrite)

#define mbmR23_16    (mbmR_16 | mbmFn23)
#define mbmR23_8     (mbmR_8 | mbmFn23)
#define mbmR23_4     (mbmR_4 | mbmFn23)
#define mbmR23_2     (mbmR_2 | mbmFn23)
#define mbmR23_1     (mbmR_1 | mbmFn23)

#define mmmfWaitResponce   (1<<7)
#define mmmfRWIndex        (1<<6)
#define mmmfWriteFunc      (1<<5)
#define mmmfFunc23         (1<<4)

#define mmmfSuccIndWrite   (1<<2)
#define mmmfSuccIndRead    (1<<1)
#define mmmfLastRespError  (1<<0)

typedef struct{
    unsigned  _0:1;
    unsigned  _1:1;
    unsigned  _2:1;
    unsigned  _3:1;
    unsigned  _4:1;
    unsigned  _5:1;
    unsigned  _6:1;
    unsigned  _7:1;

    unsigned  _8:1;
    unsigned  _9:1;
    unsigned _10:1;
    unsigned _11:1;
    unsigned _12:1;
    unsigned _13:1;
    unsigned _14:1;
    unsigned _15:1;
}IO_Type;

IO_Type Q[8] absolute 0x100;
IO_Type Q0 absolute 0x100;
IO_Type Q1 absolute 0x102;
IO_Type Q2 absolute 0x104;
IO_Type Q3 absolute 0x106;
IO_Type Q4 absolute 0x108;
IO_Type Q5 absolute 0x10A;
IO_Type Q6 absolute 0x10C;
IO_Type Q7 absolute 0x10E;

IO_Type I[8] absolute 0x120;
IO_Type I0 absolute 0x120;
IO_Type I1 absolute 0x122;
IO_Type I2 absolute 0x124;
IO_Type I3 absolute 0x126;
IO_Type I4 absolute 0x128;
IO_Type I5 absolute 0x12A;
IO_Type I6 absolute 0x12C;
IO_Type I7 absolute 0x12E;

char AlarmFlags[20] absolute 0x130;
bit Belts_ON;
struct{
    unsigned LogicBypass:1;
    unsigned InPower   :1;
    unsigned OutPower  :1;
    unsigned SimInputs :1;
    unsigned SoftReset :1;
    unsigned Init1sec  :1;
    unsigned Init2sec  :1;

    char Dummy_____;

    unsigned long Time_ms;
}Sys absolute 0x30;
enum {
    ev_1ms,  ev_2ms,  ev_4ms,  ev_8ms,  ev_16ms,  ev_32ms,  ev_64ms,  ev_128ms,//0
    ev_5ms,  ev_10ms, ev_20ms, ev_Res1, ev_50ms,  ev_100ms, ev_200ms, ev_Res2, //1
    ev_250ms,ev_500ms,ev_1sec, ev_2sec, ev_4sec,  ev_8sec,  ev_16sec, ev_32sec,//2

    ev_PwrSupplyFall,

    LastEvent
};

void ClearPLC_RAM(void)
{
asm{
    btfss   TRISB,7,ACCESS
    return
    movlw   0x0E
    lfsr    0,0
Lb_ClrMem:
    clrf    POSTINC0,ACCESS
    cpfseq  FSR0H,ACCESS
    bra     Lb_ClrMem
    btfss   FSR0L,7,ACCESS
    bra     Lb_ClrMem
    bcf     TRISB,7,ACCESS
    clrf    TOSU,ACCESS
    clrf    TOSH,ACCESS
    clrf    TOSL,ACCESS
   }
}
void InitPLC(void)
{  // Frequency = 41.666667 MHz
   OSCTUNE = 0b01000000; // PLLx5, PostScaler Disabled, PreScaler-div-by-3
   ADCON1 = 0x0F ;  // All Digital I/O
   LATA=0;  TRISA = (0b11111100);
   LATB=0;  TRISB = (0b11010000);
   LATC=0;  TRISC = (0b10011000);
   LATD=0;  TRISD = (0b11111011);
   LATE=0;  TRISE = (0b11111111);
   LATF=0;  TRISF = (0b01100001);
   LATG=0;  TRISG = (0b11100100);
   LATH=0;  TRISH = (0b10011111);
   LATJ=0;  TRISJ = (0b11001111);

   /***** Timer 1 Code ****
   *  Timer 1 is active
   *  Timer 1 derived from system clock
   *  Not synchronised with external input
   *  Oscillator turned off to save power
   *  Timer prescale ratio = 1:8
   *  Timer 1 is read in two 8-bit operations*/
   T1CON        = 0b00110101;
   /***** CCP Module 2 Code ****
   *  CCP Module 2 in Compare mode
   *  Event triggered by special event */
   CCP2CON        = 0b00001011;
  //  Comparator value set to 1302 => 1ms
   WORD(CCPR2L)=130; // 100us

   RCON.B7=1;  // Interrupt priority enable
   //Prescaler 1:4; Postscaler 1:12; TMR2 Preload = 216; Actual Interrupt Time : 999.936 us
   TMR2 = 0; T2CON=(11<<3)|5; PR2=216; TMR2IP_bit = 0; TMR2IE_bit = 1;
   WP=0; I2C1_Init(100000);
   CCP2IP_bit = 1; CCP2IE_bit = 0;
   CLRWDT();
}
char Read47L04_STATUS()
{ // A1=1; A2=0
  char Data;
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0x35);             // send byte via I2C  (device address + W)
  Data = I2C1_Rd(0u);        // Read the data (NO acknowledge)
  I2C1_Stop();               // issue I2C stop signal
  return Data;
}
void Write47L04_STATUS(char Data)
{  // A1=1; A2=0
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0x34);             // send byte via I2C  (device address + W)
  I2C1_Wr(0);                // send byte (STSTUS address)
  I2C1_Wr(Data);             // send data (data to be written)
  I2C1_Stop();               // issue I2C stop signal
}
char ReadEEP(unsigned int Address)
{ // A1=1; A2=0
  char Data;
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0xA4);             // send byte via I2C  (device address + W)
  I2C1_Wr(BYTE1(Address));   // send byte (data address)
  I2C1_Wr(BYTE0(Address));   // send byte (data address)
  I2C1_Repeated_Start();     // issue I2C signal repeated start
  I2C1_Wr(0xA5);             // send byte (device address + R)
  Data = I2C1_Rd(0u);        // Read the data (NO acknowledge)
  I2C1_Stop();               // issue I2C stop signal
  return Data;
}
void WriteEEP(unsigned int Address, char Data)
{  // A1=1; A2=0
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0xA4);             // send byte via I2C  (device address + W)
  I2C1_Wr(BYTE1(Address));   // send byte (data address)
  I2C1_Wr(BYTE0(Address));   // send byte (data address)
  I2C1_Wr(Data);             // send data (data to be written)
  I2C1_Stop();               // issue I2C stop signal
}
void WriteSeq_47L04(char * DataPtr, unsigned int Address, char NBytes)
{
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0xA4);             // send byte via I2C  (device address + W)
  I2C1_Wr(BYTE1(Address));   // send byte (data address)
  I2C1_Wr(BYTE0(Address));   // send byte (data address)
  while (NBytes--) {
    I2C1_Wr(*DataPtr++);     // send data (data to be written)
  }
  I2C1_Stop();               // issue I2C stop signal
}
void ReadSeq_47L04(char * DataPtr, unsigned int Address, char NBytes)
{
  I2C1_Start();              // issue I2C start signal
  I2C1_Wr(0xA4);             // send byte via I2C  (device address + W)
  I2C1_Wr(BYTE1(Address));   // send byte (data address)
  I2C1_Wr(BYTE0(Address));   // send byte (data address)
  I2C1_Repeated_Start();     // issue I2C signal repeated start
  I2C1_Wr(0xA5);             // send byte (device address + R)
  while (NBytes--) *DataPtr++ = I2C1_Rd(NBytes);  // Read the data + Acknolige
  I2C1_Stop();               // issue I2C stop signal
}
void Get_ROM_IP_Block(void * ETH)
{
    char i=22;
    TBLPTR=0x1CC00;
    asm{TBLRD*+};
    if (TABLAT==0xFF) return;
    FSR1PTR=ETH;
    while (i--){
      POSTINC1=TABLAT;
      asm{TBLRD*+};
    }
}
void SystemTime(void)
{
    static char time_250ms, Ev250ms;
    static char time_5ms, time_50ms, Ev5ms;

    EventFlagsBuffer[0]=BYTE(Sys.Time_ms);
    Sys.Time_ms++;
    EventFlagsBuffer[0]^=BYTE(Sys.Time_ms);

    if (++time_5ms==5){
      time_5ms=0;
      EventFlagsBuffer[1] = Ev5ms++;
      if (++time_50ms==10){
        time_50ms=0;
        Ev5ms+=0x10;
        if (++time_250ms==5) {time_250ms=0;
          EventFlagsBuffer[2] = Ev250ms++;
          EventFlagsBuffer[2]^= Ev250ms;
          if (EventFlagsBuffer[2] & 0x04) Sys.Init1sec=1;
          if (EventFlagsBuffer[2] & 0x08) Sys.Init2sec=1;
        }
      }
      Ev5ms &=0x77;
      EventFlagsBuffer[1]^= Ev5ms;
    }

}
void InputScan(void)
{
    char X;
    if (!Sys.SimInputs) {
      X=0;
      if (IN0) X.F0=1;
      if (IN1) X.F1=1;
      if (IN2) X.F2=1;
      if (IN3) X.F3=1;
      if (IN4) X.F4=1;
      if (IN5) X.F5=1;
      if (IN6) X.F6=1;
      if (IN7) X.F7=1;
      BYTE0(I0)=X;
      X=0;
      if (IN8) X.F0=1;
      if (IN9) X.F1=1;
      if (IN10) X.F2=1;
      if (IN11) X.F3=1;
      if (IN12) X.F4=1;
      if (IN13) X.F5=1;
      if (IN14) X.F6=1;
      if (IN15) X.F7=1;
      BYTE1(I0)=X;
      X=0;
      if (IN16) X.F0=1;
      if (IN17) X.F1=1;
      if (IN18) X.F2=1;
      if (IN19) X.F3=1;
      if (IN20) X.F4=1;
      if (IN21) X.F5=1;
      if (IN_OutPower) X.F7=1;
      BYTE0(I1)=X;
    }
    if (I1._7) {ResetAlarm(al_OutPower); Sys.OutPower=1;}
    else {SetAlarm(al_OutPower); Sys.OutPower=0;}
}
void EventsManager(void)
{
    di();
    WREG=EVARDIM;
 asm{
     clrwdt
     lfsr     0,_EventFlagsBuffer
     lfsr     1,_EventFlags
EventsManagerLoop:
     movff    INDF0,POSTINC1
     clrf     POSTINC0,ACCESS
     decfsz   WREG,ACCESS
     goto     EventsManagerLoop
    }
    if (Q[0]._0) OUT0=1; else OUT0=0;
    if (Q[0]._1) OUT1=1; else OUT1=0;
    if (Q[0]._2) OUT2=1; else OUT2=0;
    if (Q[0]._3) OUT3=1; else OUT3=0;
    if (Q[0]._4) OUT4=1; else OUT4=0;
    if (Q[0]._5) OUT5=1; else OUT5=0;
    if (Q[0]._6) OUT6=1; else OUT6=0;
    if (Q[0]._7) OUT7=1; else OUT7=0;
    if (Q[0]._8) OUT8=1; else OUT8=0;
    if (Q[0]._9) OUT9=1; else OUT9=0;
    if (Q[0]._10) OUT10=1; else OUT10=0;
    if (Q[0]._11) OUT11=1; else OUT11=0;

    if (Q[1]._0) RELAY0=1; else RELAY0=0;
    if (Q[1]._1) RELAY1=1; else RELAY1=0;
    if (Q[1]._2) RELAY2=1; else RELAY2=0;
    if (Q[1]._3) RELAY3=1; else RELAY3=0;
    if (Q[1]._4) RELAY4=1; else RELAY4=0;
    if (Q[1]._5) RELAY5=1; else RELAY5=0;
    ei();
}
/*
struct{
  unsigned NegDir:1;
  unsigned Last_B:1;
  unsigned Difference:1;
  unsigned ABOddParity:1;
  unsigned PrevZ:1;
  unsigned NewZ:1;

  long Position;
  int SpeedPulses;
  int RoutePosition;
  int Speed;
  int PPR;
}Enc1, Enc2, Enc3;

*/
#define EncoderScan(IN_A, IN_B, IN_Z, Z_Pos)      \
   ENC.ABOddParity=0; ENC.Last_B=0; ENC.NewZ=0; \
   if (IN_A) {ENC.ABOddParity=!ENC.ABOddParity; ENC.Difference=!ENC.Difference; ENC.NegDir=!ENC.NegDir;}\
   if (IN_B) {ENC.ABOddParity=!ENC.ABOddParity; ENC.Difference=!ENC.Difference; ENC.Last_B=1;}      \
   if (ENC.Difference) {      \
     if (ENC.NegDir) {        \
       asm{movlw  0};         \
       asm{decf   _##ENC+1};  \
       asm{subwfb _##ENC+2}; \
       asm{subwfb _##ENC+3}; \
       asm{subwfb _##ENC+4}; \
       asm{decf   _##ENC+5}; \
       asm{subwfb _##ENC+6}; \
       asm{decf   _##ENC+7}; \
       asm{subwfb _##ENC+8}; \
     }else {              \
       asm{movlw  0};         \
       asm{incf   _##ENC+1};  \
       asm{addwfc _##ENC+2}; \
       asm{addwfc _##ENC+3}; \
       asm{addwfc _##ENC+4}; \
       asm{incf   _##ENC+5}; \
       asm{addwfc _##ENC+6}; \
       asm{incf   _##ENC+7}; \
       asm{addwfc _##ENC+8}; \
     }                        \
   }                                            \
   if (IN_Z) {ENC.NewZ=1; if (!ENC.PrevZ) {ENC.PPR=ENC.RoutePosition; ENC.RoutePosition=0; Z_Pos=1;}}      \
   asm{rrncf _##ENC};
char BitPos(char N){return (1<<(N&7));}
void ClearOutNo(char N)
{
    if (N<128){
      FSR1H=1;
      FSR1L=(N>>3);
      INDF1 &=~BitPos(N);
    }
    else {
      if (N==163) {BYTE(Q3)&=0b11000000; Belts_ON=0;}
    }
}

void SetOutNo(char N)
{
    if (N<128){
      FSR1H=1;
      FSR1L=(N>>3);
      INDF1 |= BitPos(N);
    }
    else {
      if (N==163) {BYTE(Q3)|=0b00111111; Belts_ON=1;}
    }
}

void ToggleOutNo(char N)
{
    if (N<128){
      FSR1H=1;
      FSR1L=(N>>3);
      INDF1 ^= BitPos(N);
    }
}
