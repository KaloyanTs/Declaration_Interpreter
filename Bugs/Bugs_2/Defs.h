
const struct{
    char Version,    Name[16];
    unsigned int RAM_Checksum;
    char COM1_Address; BoudRate_Type COM1_BoudRate;
    char COM2_Address; BoudRate_Type COM2_BoudRate;
    char ETH_IpAddr[4];
    char ETH_gwIpAddr[4];
    char ETH_ipMask[4];
    char ETH_MacAddr[6];
}Application
={//     "                "   -  16 characters max
     10, "TRON_DRYER",           // Version & Name of Application
      0,                         // RAM Checksum
      1, br115200,               // COM1 Address & Baudrate
      0, br38400 ,               // COM2 Address & Baudrate
    192, 168,   0,  61,          // IP Address
    192, 168,   0,   1,          // Gate way
    255, 255, 255,   0,          // Mask
    0x0,0x14,0xA5,0x29,0x18,0x23 // MAC Address
    
}absolute PROG_OFFSET+8;


//*********************** Application Definitions *****************************

#define  inInv_1_OK       (I0._0)
#define  inInv_2_OK       (I0._1)
#define  inTH_Prot        (I0._2)
#define  inFeed_Aiger     (I0._3)
#define  inFeed_Gate      (I0._4)
#define  in    (I0._5)
#define  inDrum           (I0._6)
#define  in    (I0._7)
#define  inAiger_1        (I0._8)
#define  inGate_12        (I0._9)
#define  inAiger_2        (I0._10)
#define  in    (I0._11)
#define  inOutHiTemp_BM   (I0._12)
#define  inEmSTOP         (I0._13)
#define  inExtTH_Prot     (I0._14)
#define  inFun_Delta      (I0._15)

#define  outInv_Feed_ON   (Q0._0)
#define  outInv_Feed_Back (Q0._1)
#define  outInv_Drum_ON   (Q0._2)
#define  outFeed_Gate_ON  (Q0._3)
#define  outFun_START     (Q0._4)
#define  outFun_STOP      (Q0._5)
#define  outAiger_1_ON    (Q0._6)
#define  outGate_12_ON    (Q0._7)
#define  outAiger_2_ON    (Q0._8)
#define  out   (Q0._9)
#define  out   (Q0._10)
#define  out   (Q0._11)
#define  outAiger         (Q0._12)
#define  outEnable_Fuel   (Q0._13)
#define  outWater_ON      (Q0._14)
#define  outSiren         (Q0._15)

#define  outRedLamp       (Q1._14)
#define  outAlarmSound    (Q1._15)


#define  i_Always_1       (I2._8)
#define  i_Aiger_2_ON     (I2._9)
#define  i_    (I2._10)
#define  i_    (I2._11)
#define  i_    (I2._12)
#define  i_    (I2._13)
#define  i_    (I2._14)
#define  i_    (I2._15)

#define  i_    (I2._0)
#define  i_    (I2._1)
#define  i_    (I2._2)
#define  i_    (I2._3)
#define  i_    (I2._4)
#define  i_    (I2._5)



#define  o_Always_1       (Q2._8)
#define  o_Manual_Mode    (Q2._9)
#define  o_AutoStart      (Q2._10)
#define  o_AutoMode       (Q2._11)
#define  o_Small_Drum     (Q2._12)
#define  o_Siren_A        (Q2._13)
#define  o_out1_Mixer     (Q2._14)
#define  o_out11_ShneckNaklonen (Q2._15)
#define  o_      (Q2._0)
#define  o_      (Q2._1)
#define  o_      (Q2._2)
#define  o_      (Q2._3)
#define  o_      (Q2._4)


struct{
// unsigned int Status;
    unsigned stReadyToSwitchOn   :1;
    unsigned stSwitchedOn        :1;
    unsigned stOperationEnabled  :1;
    unsigned stMalfunction       :1;
    unsigned stVoltageDisabled   :1;
    unsigned stQuickStop         :1;
    unsigned stSwitchOnDisabled  :1;
    unsigned stAlarmPresent      :1;
    unsigned stReserved_1        :1;
    unsigned stNoForcedLocalMode :1;
    unsigned stReferenceReached  :1;
    unsigned stLFRD_ReferenceExceeded :1;
    unsigned stReserved_2        :1;
    unsigned stReserved_3        :1;
    unsigned stStopImposed_      :1;
    unsigned stReverseRotation   :1;
    int FreqOut, FreqRef, Current;// [0.1Hz], [0.1Hz], [0.1A]

    int rdSP2, rdSP3, rdSP4;
    int wrSP2, wrSP3, wrSP4;
}Inv_1, Inv_2; // Inv_1-Feeder;   Inv_2-Drum;

char AlarmSound_ds;
int Temp_Under_s, Temp_Over_s;
      
typedef struct{
  unsigned i_On_Delay_NextObj  :1;
  unsigned i_Off_Delay_PrevObj :1;
  unsigned i_Manual_On         :1;
  unsigned i_Enable_FB         :1;
  unsigned i_FB_Pulses         :1;
  unsigned i_STOP              :1;
  unsigned o_ON                :1;
  unsigned o_Alarm             :1;
  
  unsigned Not_Protection      :1;
  unsigned FB_Pulses_Rise      :1;
  
  unsigned On_Time_Q           :1;
  unsigned Off_Time_Q          :1;
  unsigned ReOn_Prot_Time_Q    :1;
  unsigned Alarm_Time_Q        :1;
  unsigned On_2sec_Q           :1;
  
  int On_Time_ds;
  int Off_Time_ds;
  int ReOn_Prot_Time_ds;
  int Alarm_Time_cs;
  int Flags;
  
  int Last_Alarm_Time_cs;
  int Int_On_Time_ds;
  int Int_Off_Time_ds;
  int Int_Prot_Time_ds;
  int Int_Alarm_Time_cs;
  int Int_On_2sec_ds;
  int Animation;
  int Index;

}type_Motion;

type_Motion BL;

type_Motion Feed_Aiger absolute 0x600;
type_Motion Feed_Gate absolute 0x600+30;
type_Motion Drum absolute 0x600+60;
type_Motion Cyclon absolute 0x600+90;
type_Motion Aiger_1 absolute 0x600+120;
type_Motion Gate_12 absolute 0x600+150;
type_Motion Aiger_2 absolute 0x600+180;

struct{
  unsigned Auto_Start  :1;
  unsigned Auto_Mode   :1;
  unsigned Manual_Mode :1;
  unsigned Acknowledge :1;
  unsigned Not_EmSTOP  :1;
  unsigned Mode_1      :1;
  unsigned Mode_2      :1;
  unsigned Mode_3      :1;
  
  unsigned OutTemp_OK  :1;
  unsigned LowTempOut  :1;
  unsigned ProtectFuel :1;
  unsigned OutTemp_Hi  :1;
  unsigned OutTemp_Fire:1;
  unsigned OutBuMetal_Fire:1;
  unsigned Alarm_Common:1;
  unsigned En_Man_Water:1;
  
  int SP_Temp_Out;
  
  type_Motion Feed_Aiger;
  type_Motion Feed_Gate;
  type_Motion Drum;
}GL;

struct{
  int CH_1; // [0.1*C]
  int CH_2; // [0.1*C]
  int CH_3; // [0.1*C]
  int CH_4; // [0.1*C]
}WD;

struct{
  int Offset_In;
  int Offset_Out;
  float F_In;
  float F_Out;
  float In;
  float Out;
}Temp;


struct{
  unsigned Enable_BiMetal  :1;
  char x;
  
  char Mode;
  int Inv_Feed_Hz;
  int Inv_Drum_Hz;
  int SP_Temp_M1;
  int SP_Temp_M2;
  int SP_Temp_M3;
  int Min_Speed_01Hz;
  int Max_Speed_01Hz;
  int Speed_Step_01Hz;
  int Speed_Corr_Time_s;
  int Input_HiTemp_Limit;
  int Output_HiTemp_Limit;
  int Output_FireTemp_Limit;
  int Stop_Feed_Time_s;
}Par;


//*************************** Retentive Memory ********************************
typedef struct{
  char * Pointer;
  int Length;
  int eeAddress;
  int Repeat_Offset;
  char Repeats;
  char reserved;
}RetDescrType;

#define AUTO_eePointer (4096)

const RetDescrType RetTalbe[]={
//Pointer,                         Length,   eeAddress,  Repeat_Offset,   Repeats
{(char*)&Par,                      sizeof(Par),  0,          0,             0},
{(char*)&Feed_Aiger.On_Time_ds,    10,          60,          0,             0},
{(char*)&Feed_Gate.On_Time_ds,     10,          70,          0,             0},
{(char*)&Drum.On_Time_ds,          10,          80,          0,             0},
{(char*)&Cyclon.On_Time_ds,        10,          90,          0,             0},
{(char*)&Aiger_1.On_Time_ds,       10,         100,          0,             0},
{(char*)&Gate_12.On_Time_ds,       10,         110,          0,             0},
{(char*)&Aiger_2.On_Time_ds,       10,         120,          0,             0},

{0,0,0,0,0},
};