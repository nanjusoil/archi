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
    int cycle;
    char iMemory[1024];
    char dMemory[1024];
    int PC;
    int reg[32];
public :
    void printreg(FILE* );
    int mapopcode();
    void setreg(int ,int );
    int getreg(int);
    void lsreg(int , int);
    void rsreg(int , int);
    int getfuct();
    int getrs();
    int getrt();
    int getrd();
    int getimmd();
    int getdMemory(int);
    void setPC( int);
    int getPC();
    simulator(char * ,char * , int ,int);
    ~simulator();
};
int simulator::getdMemory(int index)
{
    return dMemory[index];
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
    int addr=0;
    int temp;
    cycle=0;
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
    for(int i=8;i<iImgLen;i++)
    iMemory[addr++]= (unsigned char)iImgbuf[i];
}
void  simulator::setreg(int index,int set_num)
{
    reg[index]=set_num;
}
int  simulator::getreg(int index)
{
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
    //printf("%d" ,temp1);
    return temp1;
}
int simulator::getrs()
{
int temp1,temp2=0;
temp1=iMemory[PC];
temp2=iMemory[PC+1];
temp1=(temp1<<30>>27);
temp2=(temp2>>5);
return temp1+temp2;
}
int simulator::getrt()
{
int temp1,temp2=0;
temp1=iMemory[PC+1];
temp1=((temp1<<27)>>27);
return temp1;
}
int simulator::getrd()
{
int temp1,temp2=0;
temp1=iMemory[PC+2];
temp1=temp1>>3;
return temp1;
}
void simulator::printreg(FILE* output)
{
    fprintf(output,"cycle %d\n", cycle++);
    int i;
    for (i = 0; i < 32; ++i) {
        fprintf(output, "$%02u: 0x", i);
        fprintf(output, "%08X\n", reg[i]);
    }
    fprintf(output, "PC: 0x");
    fprintf(output, "%08X\n\n\n", PC);
}

int simulator::getimmd()
{
int temp1,temp2=0;
temp1=iMemory[PC+2];
temp2=iMemory[PC+3];
temp1= temp1<<8;
return temp1+temp2;
}
int main()
{
    FILE *iImg , *dImg , *output;
    char* iImgbuf, *dImgbuf;
    int i = 0;
    int iImgLen , dImgLen;
    int opcode;
    int fuct;
    int rs;
    int rt;
    int rd;
    int immd;
    int temp5;
    int temp6;
    iImg = fopen("iimage.bin", "rb");
    dImg = fopen("dimage.bin", "rb");
    output = fopen("snapshot.rpt", "wb");

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
    rs=sim->getrs();
    rt=sim->getrt();
    rd=sim->getrd();
    immd=sim->getimmd();
   // printf("%d %x %d ",rs,rt ,rd);
    switch(opcode){
        case RTYPE:
            fuct =sim->getfuct();
            switch(fuct)
            {
            case add:
                printf("add");
                break;
                case addu:
                printf("addu");
                break;
                case sub:
                printf("sub");
                break;
                case _and:
                printf("_and");
                break;
                case _or:
                printf("_or");
                break;
                case _xor:
                printf("_xor");
                break;
                case nor:
                printf("nor");
                break;
                case nand:
                printf("nand");
                break;
                case slt:
                if(sim->getreg(rs)<sim->getreg(rt))
                    sim->setreg(rd,1);
                else
                    sim->setreg(rd,0);
                printf("slt");
                break;
                case sll:
                printf("sll");
                break;
                case srl:
                printf("srl");
                break;
                case sra:
                printf("sra");
                break;
                case jr:
                printf("jr");
                break;


            }
        break;
        case lhu:
        temp5=0;
        temp6=0;
        temp5= sim->getdMemory(sim->getreg(rs));
        temp6= sim->getdMemory(sim->getreg(rs)+1);
        temp5= temp5<<8;
        sim->setreg(rt,temp5+temp6);
        printf("lhu\n");
        break;
        case addi:
        printf("addi\n");
        break;
        case addiu:
        printf("addiu\n");
        break;
        case lw:
        printf("lw\n");
        break;
        case lh:
        temp5=0;
        temp6=0;
        temp5=  sim->getdMemory(sim->getreg(rs));
        temp6=  sim->getdMemory(sim->getreg(rs)+1);
        temp5= temp5<<8;
        sim->setreg(rt,temp5+temp6);
        printf("lh\n");
        break;
        case lb:
        sim->setreg(rt, sim->getdMemory(immd));
        printf("lb %x",sim->getreg(rt));
        break;
        case lbu:
        sim->setreg(rt, sim->getdMemory(rs+immd));
        sim->lsreg(rt,24);
        sim->rsreg(rt,24);
        printf("lbu %x",sim->getreg(rt));
        break;
        case sw:
        printf("sw\n");
        break;
        case sh:
        printf("sh\n");
        break;
        case sb:
        printf("sb\n");
        break;
        case lui:
        printf("lui\n");
        break;
        case andi:
        printf("andi\n");
        break;
        case ori:
        sim->setreg(rt,sim->getreg(rs)|immd);
        printf("ori\n");
        break;
        case nori:
        printf("nori\n");
        break;
        case slti:
        printf("slti\n");
        break;
        case beq:
        if(sim->getreg(rs)==sim->getreg(rt))
            sim->setPC( sim->getPC() +4*immd );
        printf("beq\n");
        break;
        case bne:
        if(sim->getreg(rs)!=sim->getreg(rt))
            sim->setPC( sim->getPC() +4*immd );
            printf("bne\n");
        break;
        case bgtz:
        printf("bgtz\n");
        break;
        case j:
        printf("j\n");
        break;
        case jal:
        printf("jal\n");
        break;
        case halt:
       exit(0);

    }
    sim->setPC( sim->getPC() +4);
    }

}
