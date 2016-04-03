#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define RTYPE 0
#define add 32
#define addu 33
#define sub 34
#define _and 36
#define _or 37
#define _xor 38
#define nor 39
#define nand 40
#define slt 42
#define sll 0
#define srl 2
#define sra 3
#define jr 8
#define addi 8
#define addiu 9
#define lw 35
#define lh 33
#define lhu 37
#define lb 32
#define lbu 36
#define sw 43
#define sh 41
#define sb 40
#define lui 15
#define andi 12
#define ori 13
#define nori 14
#define slti 10
#define beq 4
#define bne 5
#define bgtz 7
#define j 2
#define jal 3
#define halt 63

class simulator
{
private:
    char iMemory[1024];
    char dMemory[1024];
    unsigned reg[32];
    int PC;
public :
        int cycle;
    bool writetozero, datamisalign , memoryoverflow, numberoverflow ;
    void printreg(FILE* );
    void printerror(FILE* error);
    int mapopcode();
    void setreg(int ,int );
    unsigned getreg(int);
    void lsreg(int , int);
    void rsreg(int , int);
    int getfuct();
    unsigned char  getrs();
    unsigned char  getrt();
    unsigned char  getrd();
    unsigned getimmd();
    unsigned int getshift();
    char getdMemory(int);
    char getiMemory(int);
    void setunsigndMemory(int  , int);
    void setMemory(int , int);
    int getunsigndMemory(int);
    void setPC( int);
    int getPC();
   unsigned getsignimmd();
    simulator(char * ,char * , int ,int);
    ~simulator();
};
void simulator::printerror(FILE* error)
{
    if (writetozero) {
        writetozero = 0;
        fprintf(error, "In cycle %d: Write $0 Error\n", cycle);
    }
    if (numberoverflow) {
        numberoverflow = 0;
        fprintf(error, "In cycle %d: Number Overflow\n", cycle);
    }
    if (memoryoverflow) {
        memoryoverflow = 0;
        fprintf(error, "In cycle %d: Address Overflow\n", cycle);
        if(!datamisalign)
            exit(0);
    }
    if (datamisalign) {
        datamisalign = 0;
        fprintf(error, "In cycle %d: Misalignment Error\n", cycle);
        exit(1);
    }
}
unsigned int simulator::getshift()
{
    unsigned temp1 = iMemory[PC+ 2];
    unsigned temp2 = iMemory[PC + 3];
    temp1 = temp1 << 29 >> 27;
    temp2 = temp2 >> 6 << 30 >> 30;
    return temp1 + temp2;
}
char simulator::getiMemory(int index)
{
    return iMemory[PC+index];
}
char simulator::getdMemory(int index)
{
    int temp=dMemory[index];
    return dMemory[index];
}
int simulator::getunsigndMemory(int index)
{
    //int temp=dMemory[index];
    return (unsigned char)dMemory[index];
}
void simulator::setunsigndMemory(int index , int val)
{
    dMemory[index]=val;
}
void simulator::setMemory(int index , int val)
{
    dMemory[index]=val;
}
void simulator::setPC(int num)
{
    PC= num;
}
int simulator::getPC()
{
    return PC;
}
void simulator::lsreg(int index , int shift)
{
    reg[index]<<shift;
}
void simulator::rsreg(int index, int shift)
{
    reg[index]>>shift;
}
simulator::simulator(char * iImgbuf, char* dImgbuf , int iImgLen , int dImgLen)
{
    writetozero=0;
    datamisalign=0;
    memoryoverflow=0;
    numberoverflow =0;
    int addr=0;
    int temp;
    cycle=0;
    for(int i=0;i<32;i++)
        reg[i]=0;
    for(int i = 0 ;i<1024;i++)
    {
        dMemory[i]=0;
        iMemory[i]=0;
    }
    for(int i=0;i<4;i++)
     temp= (temp<<8) + (unsigned char)dImgbuf[i];
    reg [29] =temp;
    for(int i=8;i<dImgLen;i++)
    dMemory[addr++]= (unsigned char)dImgbuf[i];
    for(int i=0;i<4;i++)
     PC= (PC<<8) + (unsigned char)iImgbuf[i];
    addr=PC;
            temp =(unsigned char) dMemory[0];
    for(int i=8;i<iImgLen;i++)
    iMemory[addr++]= (unsigned char)iImgbuf[i];


}
void  simulator::setreg(int index,int set_num)
{
    reg[index]=set_num;
}
unsigned int  simulator::getreg(int index)
{
    int temp=reg[index];
    return reg[index];
}
int simulator::getfuct()
{
    int temp=0;
    temp = iMemory[PC+3];
   temp=(temp<<2>>2) ;
    return temp;
}
int simulator::mapopcode()
{
    unsigned int temp1;
    temp1=0;
    temp1 = iMemory[PC];
    temp1 = (temp1<<24>>2>>24);
    ////printf("%d" ,temp1);
    return temp1;
}
unsigned char simulator::getrs()
{
unsigned int temp1,temp2=0;
temp1=iMemory[PC];
temp2=iMemory[PC+1];
temp1=(temp1<<30>>27);
temp2=(temp2<<24>>29);
return (unsigned char)temp1+temp2;
}
unsigned char simulator::getrt()
{
unsigned int temp1,temp2=0;
temp1=iMemory[PC+1];
temp1=((temp1<<27)>>27);
return (unsigned char)temp1;
}
unsigned char simulator::getrd()
{
unsigned int temp1,temp2=0;
temp1=iMemory[PC+2];
temp1=temp1<<24>>27;
return temp1;
}
void simulator::printreg(FILE* output)
{
        /*for(int i=0;i<1024;i++)
        //printf("%x", dMemory[i]);
        //printf("\n");*/
    fprintf(output,"cycle %d\n", cycle++);
    int i;
    for (i = 0; i < 32; ++i) {
        fprintf(output, "$%02u: 0x", i);
        fprintf(output, "%08X\n", reg[i]);
    }
    fprintf(output, "PC: 0x");
    fprintf(output, "%08X\n\n\n", PC);
}
unsigned simulator::getsignimmd()
{
unsigned temp1,temp2=0;
temp1=iMemory[PC+2];
temp2=iMemory[PC+3];
temp1= temp1<<24>>16;
temp2= temp2<<24>>24;
int temp0 =  temp1+temp2;
temp0 = temp0<<16 >>16;
return temp0;
}
unsigned simulator::getimmd()
{
unsigned temp1,temp2=0;

temp1=iMemory[PC+2];
temp2=iMemory[PC+3] ;
temp1=temp1<< 24 >> 16;
temp2=temp2<< 24 >> 24;
return temp1+temp2;
}
int main()
{
    FILE *iImg , *dImg , *output,*error;
    char* iImgbuf, *dImgbuf;
    int i = 0;
    int iImgLen , dImgLen;
    int opcode;
    unsigned int fuct;
    unsigned int shamt;
    int rs;
    int rt;
    int rd;
    int temp_1 , temp_2;
    unsigned int immd;
    int temp0;
    unsigned int temp5 ,temp1, temp2 ,temp6,temp3,temp4;
    iImg = fopen("iimage.bin", "rb");
    dImg = fopen("dimage.bin", "rb");
    output = fopen("snapshot.rpt", "wb");
    error = fopen("error_dump.rpt", "wb");

    fseek(iImg, 0, SEEK_END);
    fseek(dImg, 0, SEEK_END);
    dImgLen = ftell(dImg);
    iImgLen = ftell(iImg);

    iImgbuf =(char*) malloc(ftell(iImg));
    dImgbuf =(char*) malloc(ftell(dImg));

    fseek(iImg, 0, SEEK_SET);
    fseek(dImg, 0, SEEK_SET);

    fread(iImgbuf , sizeof(char),iImgLen , iImg );
    fread(dImgbuf , sizeof(char),dImgLen , dImg );

  simulator* sim = new simulator(iImgbuf,dImgbuf,iImgLen , dImgLen);

     while(1){
    opcode =sim->mapopcode();
    sim->printreg(output);
                if(sim->cycle==94)
            {
                printf("asd");
            }
    rs=sim->getrs();
    rt=sim->getrt();
    rd=sim->getrd();
    immd=sim->getimmd();
   // //printf("%d %x %d ",rs,rt ,rd);
    switch(opcode){
        case RTYPE:
            fuct =sim->getfuct();
            fuct = fuct<<26>>26;
            switch(fuct)
            {
            case add:
                    temp_1 = sim->getreg(rs);
               temp_2 = sim->getreg(rt);
               temp0 = temp_1+temp_2;
                if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
                sim->setreg(rd, sim->getreg(rs)+sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("add");
                break;
                case addu:
                sim->setreg(rd, sim->getreg(rs)+sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("addu");
                break;
                case sub:
                               temp_1 =  sim->getreg(rs);
           temp_2 =~(sim->getreg(rt)) ;
           temp_2++;
                temp0 = temp_1+temp_2;
                if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
                sim->setreg(rd, sim->getreg(rs)-sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("sub");
                break;
                case _and:
                sim->setreg(rd, sim->getreg(rs)&sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("_and");
                break;
                case _or:
                sim->setreg(rd, sim->getreg(rs)|sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("_or");
                break;
                case _xor:
                sim->setreg(rd, sim->getreg(rs)^sim->getreg(rt));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("_xor");
                break;
                case nor:
                sim->setreg(rd, ~(sim->getreg(rs)|sim->getreg(rt)));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("nor");
                break;
                case nand:
                sim->setreg(rd, ~(sim->getreg(rs)&sim->getreg(rt)));
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("nand");
                break;
                case slt:
                    temp0=sim->getreg(rs);
                    temp_1=sim->getreg(rt);
                if(temp0<temp_1)
                    sim->setreg(rd,1);
                else
                    sim->setreg(rd,0);
            if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(0,0);
            }
                //printf("slt");
                break;
                case sll:
                shamt = sim->getshift();
                sim->setreg(rd , sim->getreg(rt)<<shamt);
                 temp5 = sim->getreg(rt)<<shamt;
            if (rd == 0&&(rt!=0||shamt!=0)) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("sll");
                break;
                case srl:
                shamt = sim->getshift();
                sim->setreg(rd , sim->getreg(rt)>>shamt);
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("srl");
                break;
                case sra:
                shamt = sim->getshift();
                sim->setreg(rd , (int)sim->getreg(rt)>>shamt);
                                    if (rd == 0) {
           sim->writetozero = 1;
           sim->setreg(rd,0);
            }
                //printf("sra");
                break;
                case jr:
                sim->writetozero = 0;
                sim->setPC(sim->getreg(rs)-4);
                break;
            }
        break;
        case lhu:
        temp5=0;
        temp6=0;
        immd = sim->getsignimmd();
        temp5= sim->getunsigndMemory(sim->getreg(rs)+immd);
        temp6= sim->getunsigndMemory(sim->getreg(rs)+1+immd);
        temp5= temp5<<24>>16;
        temp6= temp6<<24>>24;
                temp_1 = temp5;
                temp_1 =   temp_1<<24>>24;
        temp_2 = temp6;
        temp_2 = temp_2<<24>>24;
        temp0 = temp_1+temp_2;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
        sim->setreg(rt,temp5+temp6);
                        if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        temp0 = sim->getreg(rs)+immd;
            if(temp0+1>=1024||temp0<0)
            sim->memoryoverflow = 1;
        if((sim->getreg(rs)+immd)%2!=0)
            sim->datamisalign = 1;

        //printf("lhu\n");
        break;
        case addi:
            immd=sim->getsignimmd();
            temp0 = sim->getreg(rs)+immd;
                    temp_1 = sim->getreg(rs);
        temp_2 = immd;
               temp0 = temp_1+temp_2;
        if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
        sim->setreg(rt,temp0);

                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        break;
        case addiu:
            immd = sim->getsignimmd();
            sim->setreg(rt,sim->getreg(rs)+immd);
                        if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }

        //printf("addiu\n");
        break;
        case lw:
            immd = sim->getsignimmd();
        temp_1 = sim->getreg(rs);
        temp_2 = immd;
        temp0 = temp_1 + temp_2;
        if(temp0>=1024||temp0<0)
        {
             sim->memoryoverflow = 1;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
                                            if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        if((sim->getreg(rs)+immd)%4!=0)
            sim->datamisalign = 1;
            break;
        }
        temp1 = sim->getdMemory(sim->getreg(rs)+immd);
         temp1 = temp1 << 24;
        temp2 = sim->getdMemory(sim->getreg(rs)+immd+1);
         temp2 = temp2 << 24 >> 8;
        temp5 = sim->getdMemory(sim->getreg(rs)+immd+2);
         temp5 = temp5 << 24 >> 16;
        temp6 = sim->getdMemory(sim->getreg(rs)+immd+3);
         temp6 = temp6 << 24 >> 24;
        //printf("%x", temp1 + temp2 + temp5 + temp6 );

                        if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&&temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
            if(temp_1+3+temp_2>=1024||temp0<0)
            sim->memoryoverflow = 1;
        sim->setreg(rt ,temp1 + temp2 + temp5 + temp6 );
                                if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        if((temp_1+immd)%4!=0)
            sim->datamisalign = 1;
        //printf("lw\n");
        break;
        case lh:
        temp5=0;
        temp6=0;
        immd=sim->getsignimmd();
        //printf("%x", sim->getreg(rs)+immd);
        //printf("%d", sim->getdMemory(sim->getreg(rs)+immd));
        temp0 = sim->getreg(rs)+immd;
                    if(sim->getreg(rs)+immd+1>=1024||temp0<0)
                    {
                                    sim->memoryoverflow = 1;
            if(temp5>0&&temp6>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp5<0&&temp6<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
                    if((sim->getreg(rs)+immd)%2!=0)
            sim->datamisalign = 1;
                                                            if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
            break;
                    }

        temp5=  sim->getdMemory(sim->getreg(rs)+immd);
        temp6=  sim->getdMemory(sim->getreg(rs)+1+immd);
        temp5= temp5<<24>>16;
        temp6 = temp6<<24>>24;
                        temp_1 = sim->getreg(rs);
        temp_2 = immd;
                temp0 = temp5+temp6;
            if(temp5>0&&temp6>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp5<0&&temp6<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
        sim->setreg(rt,(short)(temp5+temp6));
                        if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("%x",(short)(temp5+temp6));
        //printf("lh\n");
        temp0 = sim->getreg(rs)+immd;
    if(temp0+1>=1024 || temp0<0)
            sim->memoryoverflow = 1;
    if((sim->getreg(rs)+immd)%2!=0)
            sim->datamisalign = 1;
        break;
        case lb:
            immd = sim->getsignimmd();
            temp_1 = sim->getreg(rs);
        temp_2 = immd;
                            temp0 = temp_1+temp_2;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
            if(temp0>=1024 || temp0 < 0)
            {
                            sim->memoryoverflow = 1;
                                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
            break;
            }

        sim->setreg(rt, sim->getdMemory(sim->getreg(rs)+immd));
        //printf("lb %x",sim->getreg(rt));
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        break;
        case lbu:
                                temp_1 = sim->getreg(rs);
        temp_2 = immd;
                            temp0 = temp_1+temp_2;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
        sim->setreg(rt, sim->getunsigndMemory(sim->getreg(rs)+immd));
       // sim->lsreg(rt,24);
        //sim->rsreg(rt,24);
            temp0 = temp_1+temp_2;
            if(temp0>=1024||temp0<0)
            sim->memoryoverflow = 1;
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("lbu %x",sim->getreg(rt));
        break;
        case sw:

        immd=sim->getsignimmd();
                            temp_1 = sim->getreg(rs);
        temp_2 = immd;
                            temp0 = temp_1+temp_2;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
        if(temp0+3>=1024||temp0<0)
            sim->memoryoverflow = 1;
        sim->setMemory(sim->getreg(rs)+immd , sim->getreg(rt)>> 24);
        sim->setMemory(sim->getreg(rs)+immd +1 , sim->getreg(rt)<< 8 >> 24);
        sim->setMemory(sim->getreg(rs)+immd +2, sim->getreg(rt)<< 16 >> 24);
        sim->setMemory(sim->getreg(rs)+immd +3, sim->getreg(rt)<< 24 >> 24);
        temp0 = sim->getreg(rs)+immd;
        if(temp0+3>=1024||temp0<0)
            sim->memoryoverflow = 1;
        if((sim->getreg(rs)+immd)%4!=0)
            sim->datamisalign = 1;
        break;
        case sh:
            immd=sim->getsignimmd();
            temp5=sim->getreg(rt)>>8;
                                temp_1 = sim->getreg(rs);
        temp_2 = immd;
                            temp0 = temp_1+temp_2;
            if(temp_1>0&&temp_2>0&&temp0<=0)
                    sim->numberoverflow=1;
                else if (temp_1<0&&temp_2<0&& temp0>=0)
                    sim->numberoverflow=1;
                else
                {

                }
            //printf("sh %x" , sim->getreg(rt) & 0x0000FFFF);
         sim->setMemory(sim->getreg(rs)+immd,sim->getreg(rt)<<16>>24);
         sim->setMemory(sim->getreg(rs)+immd+1,sim->getreg(rt)<<24>>24);
        //printf("sh\n");

        if((sim->getreg(rs)+immd)%2!=0)
            sim->datamisalign = 1;
            temp0 = temp_1+temp_2;
        if(temp0+1>=1024||temp0<0)
            sim->memoryoverflow = 1;
        break;
        case sb:
            immd = sim->getsignimmd();
        temp0 =sim->getreg(rs)+immd;
        if(temp0+1>=1024||temp0<0)
            sim->memoryoverflow = 1;
            sim->setMemory(temp0 , sim->getreg(rt)<<24>>24);

        //printf("sb\n");
        break;
        case lui:
            immd = sim->getimmd();
            sim->setreg(rt , immd<<16);
            if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(0 , 0);
            }
        //printf("lui\n");
        break;
        case andi:
        sim->setreg(rt , sim->getreg(rs)&immd);
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("andi\n");
        break;
        case ori:
        sim->setreg(rt,sim->getreg(rs)|immd);
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("ori\n");
        break;
        case nori:
        sim->setreg(rt , ~(sim->getreg(rs)|immd));
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("nori\n");
        break;
        case slti:
        temp0 = sim->getsignimmd();
       temp_1 =  sim->getreg(rs);
        if(temp0>temp_1)
        {
            sim->setreg(rt , 1);
        }
        else{
            sim->setreg(rt , 0);
        }
                    if (rt == 0) {
           sim->writetozero = 1;
           sim->setreg(rt,0);
            }
        //printf("slti\n");
        break;
        case beq:
            immd=sim->getsignimmd();
        if(sim->getreg(rs)==sim->getreg(rt))
            sim->setPC( sim->getPC() +4*immd );
        //printf("beq\n");
        break;
        case bne:
            immd=sim->getsignimmd();
            //printf("\n\n%d\n",sim->getrs());
            //printf("\n%d\n",sim->getrt());
            //printf("%d\n", sim->getPC());
        if(sim->getreg(rs)!=sim->getreg(rt))
            sim->setPC( sim->getPC() +4*immd );
                        //printf("%d\n", sim->getPC());
            //printf("bne\n");
        break;
        case bgtz:
            immd = sim->getsignimmd();
                temp0 = sim->getreg(rs);
                        if (temp0 > 0) {
                            immd = immd << 2;
                            sim->setPC(immd+sim->getPC());
                        } else
                        {

                        }
        //printf("bgtz\n");
        break;
        case j:
        temp1 = sim->getiMemory(0);
        temp2 = sim->getiMemory(1);
        temp3 = sim->getiMemory(2);
        temp4 = sim->getiMemory(3);
        temp1 = temp1 << 30 >> 6;
        temp2 = temp2 << 24 >> 8;
        temp3 = temp3 << 24 >> 16;
        temp4 = temp4 << 24 >> 24;
        temp5 = temp1 + temp2 + temp3 + temp4;
        sim->setPC((sim->getPC()+4)>> 28 << 28| (temp5 << 2));
        sim->setPC((sim->getPC()-4));
        //printf("j\n");
        break;
        case jal:
        sim->setreg(31 , sim->getPC()+4);
        temp1 = sim->getiMemory(0);
        temp2 = sim->getiMemory(1);
        temp3 = sim->getiMemory(2);
        temp4 = sim->getiMemory(3);
        temp1 = temp1 << 30 >> 6;
        temp2 = temp2 << 24 >> 8;
        temp3 = temp3 << 24 >> 16;
        temp4 = temp4 << 24 >> 24;
        temp5 = temp1 + temp2 + temp3 + temp4;
        sim->setPC((sim->getPC()+4)>> 28 << 28| (temp5 << 2));
        sim->setPC((sim->getPC()-4));
        break;
        case halt:
       exit(0);

    }
    sim->printerror(error);
    sim->setPC( sim->getPC() +4);
    }

}
