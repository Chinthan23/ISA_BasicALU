//IAS architecture implementation for a c program
//Done By Chinthan Chandra(IMT2020109) and Tejas Sharma(IMT2020548)

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define First8(n) (n)>>12; //to get first 8 bits of a number
#define Last12(n) (n)%(1<<12);
//Implementing the following program in IAS Machine
/* 
main(){
  int a,b;
  c=a-b;
  d=a+b;
  e=d/c;
}
*/
//function to load content at Mem[x] to accumulator
void loadmx(ll M[],ll &MAR,ll &AC,ll &MBR){
  MBR=M[MAR];
  AC=MBR;
}
//function to add Mem[x] to accumulator considering negative and positive numbers
void addmx(ll M[],ll &MAR,ll &MBR, ll &AC){
  MBR=M[MAR];
  ll as=MBR>>39,av=MBR%(1<<39),acs=AC>>39; //taking signs and magnitudes to compute sum
  ll acv=(AC&0x7fffffffff);
  if(as == 0 && acs == 0) AC+=MBR;
  if(as==1 && acs==1){
    ll temp=acv+av;
    AC=temp;
    AC=(AC&0x7fffffffff);
    AC=(AC|1<<40);
  }
  if(as==1 && acs==0){
    if(av>acv){
      av-=acv;AC=av;
      AC=(AC&0x7fffffffff);
      AC=(AC|1<<40);
    }
    else{
      acv-=av;AC=acv;
      AC=(AC&0x7fffffffff);
    }
  }
  if(as==0 && acs==1){
    if(av>acv){
      av-=acv;AC=av;
      AC=(AC&0x7fffffffff);
    }
    else{
      acv-=av;AC=acv;
      AC=(AC&0x7fffffffff);
      AC=(AC|0x8000000000);
    }
  }
}
//function to sub Mem[x] from accumulator considering positive and negative values
void submx(ll M[], ll &MAR,ll &MBR, ll &AC){
  MBR=M[MAR];
  ll as=MBR>>39,av=MBR%(1<<39),acs=AC>>39; //taking signs and magnitudes to find difference
  ll acv=(AC&0x7fffffffff);
  if(as==0&&acs==0){
    if(acv>av){
      acv-=av;
      AC=acv;
      AC=(AC&0x7fffffffff);
    }
    else{
      av-=acv;AC=av;
      AC=(AC&0x7fffffffff);
      AC=(AC|0x8000000000);
    }
  }
  else if(as==1&&acs==1){
    if(acv>av){
      acv-=av;
      AC=acv;
      AC=(AC&0x7fffffffff);
      AC=(AC|0x8000000000);
    }
    else{
      av-=acv;AC=av;
      AC=(AC&0x7fffffffff);
    }
  }
  else if(as==1&& acs==0){
    acv+=av;
    AC=acv;
    AC=(AC&0x7fffffffff);
  }
  else if(as==0 && acs==1){
    acv+=av;AC=acv;
    AC=(AC&0x7fffffffff);
    AC=(AC|0x8000000000);
  }
}
//function to divide to numbers and store remainder in accumulator and quotient in MQ
void divmx(ll M[],ll &MAR,ll &MBR,ll &AC, ll &MQ){
  MBR=M[MAR];
  ll ts=MBR>>39,as=AC>>39;
  ll av=AC&0x7fffffffff,tv=MBR&0x7fffffffff,a=av%MBR;
  MQ=av/tv;
  AC=a;
  if((ts==1&& as==0)||(ts==0&&as==1)){
    MQ+=1;
    MQ=(MQ|0x8000000000);
    AC=tv-AC;
  }
}
//to store value to MQ
void storemxmq(ll M[],ll &MAR,ll &MQ,ll &MBR){
  MBR=M[MAR];MQ=MBR;
}
//to load MQ to AC
void loadmq(ll M[],ll &MQ,ll &AC){
  AC=MQ;
}
//To jump to the left instruction of M[x]
void jumpl(ll &MAR,ll &PC){
  PC=MAR;
}
//to jump to the right instruction of M[x]
void jumpr(ll &MAR,ll &PC){
  PC=MAR;
}
//To jump to left instruction of M[x] if accumulator is positive
void jumplc(ll &MAR,ll &PC,ll &AC){
  ll acs=(AC>>39);
  if(acs==0) PC=MAR;
}
//To jump to right instruction of M[x] if accumulator is negative
void jumprc(ll &MAR,ll &PC,ll &AC){
  ll acs=AC>>39;
  if(acs==0){
    PC=MAR;
  }
}
//To store AC value to M[x]
void storemx(ll M[],ll &MAR,ll &AC,ll &MBR){
  MBR=AC;M[MAR]=MBR;
}
//initialize the register values to 0
ll AC=0,MQ=0,PC=0,IBR=0,IR=0,MBR=0,MAR=0;

int main(){
  ll Mem[1000]; //my data starts from 100

  for(int i=0;i<1000;i++){
    Mem[i]=0;
  }
  Mem[0]=0x0106406065;//LOAD Mem[100] ,SUB Mem[101]
  Mem[1]=0x2106601065;//STORE Mem[102],LOAD Mem[101]
  Mem[2]=0x0506421067;//ADD Mem[100], STOR Mem[102]
  Mem[3]=0x010670c066;//LOAD Mem[102],DIV Mem[103]
  Mem[4]=0x0a00021068;        //HALT
  Mem[5]=0x00;
  Mem[100]=0x0000000018; // value a
  Mem[101]=0x000000000C; // value b
  Mem[102]=0x0000000000; // value C
  Mem[103]=0x0000000000; // value d
  Mem[104]=0x0000000000; // value e

  bool RUN=true;
  cout<<"Program starts"<<endl;
  cout<<endl;
  while(RUN){
    //fetch the instruction at PC
    if(IBR!=0){
      //fetch right instruction to decode and execute
      IR=First8(IBR);
      MAR=Last12(IBR);
      IBR=0;
      switch(IR){
      case 0:
        RUN=false;
        break;
      case 1:
        loadmx(Mem,MAR,AC,MBR);
        break;
      case 5:
        addmx(Mem,MAR,MBR,AC);
        break;
      case 6:
        submx(Mem,MAR,MBR,AC);
        break;
      case 9:
        storemxmq(Mem,MAR,MQ,MBR);
        break;
      case 10:
        loadmq(Mem,MQ,AC);
        break;
      case 12:
        divmx(Mem,MAR,MBR,AC,MQ);
        break;
      case 33:
        storemx(Mem,MAR,AC,MBR);
      }
      //tried to implement jump too.
      /*
      switch(IR){
      case 0:
        RUN=false;
        break;
      case 1:
        loadmx(Mem,MAR,AC,MBR);
        break;
      case 5:
        addmx(Mem,MAR,MBR,AC);
        break;
      case 6:
        submx(Mem,MAR,MBR,AC);
        break;
      case 9:
        storemxmq(Mem,MAR,MQ,MBR);
        break;
      case 10:
        loadmq(Mem,MQ,AC);
        break;
      case 12:
        divmx(Mem,MAR,MBR,AC,MQ);
        break;
      case 13:
        cout<<"Jump Encountered"<<endl;
        cout<<endl;
        jumpl(MAR,PC);
        IBR=(Mem[PC]>>20);
        cout<<"New PC :"<<dec<<PC<<endl;
        continue;
        break;
      case 14:
        cout<<"Jump Encountered"<<endl;
        cout<<endl;
        jumpr(MAR,PC);
        IBR=(Mem[PC]%(1<<20));
        cout<<"New PC :"<<dec<<PC<<endl;
        continue;
        break;
      case 15:
        cout<<"Jump Encountered"<<endl;
        jumplc(MAR,PC,AC);
        IBR=(Mem[PC]>>20);
        cout<<"New PC :"<<dec<<PC<<endl;
        continue;
        break;
      case 16:
        cout<<"Jump Encountered"<<endl;
        cout<<endl;
        jumprc(MAR,PC,AC);
        IBR=(Mem[PC]%(1<<20));
        cout<<"New PC :"<<dec<<PC<<endl;
        continue;
        break;
      case 33:
        storemx(Mem,MAR,AC,MBR);
      }*/
    }
    else{
    cout<<"Instruction Being executed is Mem["<<dec<<PC<<"]"<<endl;
    cout<<endl;
      //fetch the instruction at PC
      MAR=PC;
      MBR=Mem[MAR];
      IBR=MBR%(1<<20);
      MAR=Last12(MBR>>20);
      IR=First8(MBR>>20);
    // to decode
      switch(IR){
        case 0:
          RUN=false;
          break;
        case 1:
          loadmx(Mem,MAR,AC,MBR);
          break;
        case 5:
          addmx(Mem,MAR,MBR,AC);
          break;
        case 6:
          submx(Mem,MAR,MBR,AC);
          break;
        case 9:
          storemxmq(Mem,MAR,MQ,MBR);
          break;
        case 10:
          loadmq(Mem,MQ,AC);
          break;
        case 12:
          divmx(Mem,MAR,MBR,AC,MQ);
          break;
        case 33:
          storemx(Mem,MAR,AC,MBR);
        }
      PC++;
      //increment PC for next instruction
      //printing values of register after execution
      cout<< "IBR   :0x"<<hex<<IBR<<endl;
      cout<< "IR    :0x"<<hex<<IR<<endl;
      cout<< "MAR   :0x"<<hex<<MAR<<endl;
      cout<< "MBR   :0x"<<hex<<MBR<<endl;
      cout<< "AC    :0x"<<hex<<AC<<endl;
      cout<< "MQ    :0x"<<hex<<MQ<<endl;
      cout<<endl; 
    }
  }
  cout<<"C="<<dec<<Mem[102]<<endl;
  cout<<"D="<<dec<<Mem[103]<<endl;
  cout<<"E="<<dec<<Mem[104]<<endl;
}

  

