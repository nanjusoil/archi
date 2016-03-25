#include <stdio.h>
#include <stdio.h>
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
#define lhu 37
#define nori 14
#define slti 10
#define beq 4
#define bne 5
#define bgtz 7
#define j 2
#define jal 3
#define halt 63

unsigned char iMemory[1024];
unsigned char dMemory[1024];
int PC;
int reg[32];

int mapopcode();
iImageInit(unsigned char *,int );
dImageInit(unsigned char *,int );
int getfuct();
int getrs();
int gettr();
int getrd();
int main()

{
    FILE *iImg , *dImg;
   unsigned char* iImgbuf, *dImgbuf;
    int i = 0;
    int iImgLen , dImgLen;
    int opcode;
    int fuct;
    int rs;
    int rt;
    int rd;

    iImg = fopen("iimage.bin", "rb");
    dImg = fopen("dimage.bin", "rb");
    fseek(iImg, 0, SEEK_END);
    fseek(dImg, 0, SEEK_END);
    dImgLen = ftell(dImg);
    iImgLen = ftell(iImg);

    iImgbuf =(unsigned char*) malloc(ftell(iImg));
    dImgbuf =(unsigned char*) malloc(ftell(dImg));

    fseek(iImg, 0, SEEK_SET);
    fseek(dImg, 0, SEEK_SET);

    fread(iImgbuf , sizeof(unsigned char),iImgLen , iImg );
    fread(dImgbuf , sizeof(unsigned char),dImgLen , dImg );
    for(i = 0 ; i <iImgLen ;i++)
        {
            printf("%02x" , iImgbuf[i]);
        }
    printf("\n" , iImgbuf[i]);
  /*  for(i = 0 ; i <dImgLen ;i++)
        {
            printf("%02x" , dImgbuf[i]);
        }
*/
    iImageInit(iImgbuf ,  iImgLen);
    dImageInit(dImgbuf , dImgLen );
   while(1){
    opcode =mapopcode();
    switch(opcode){
        case RTYPE:
            fuct =getfuct();
            rs=getrs();
            rt=getrt();
            rd=getrd();
            printf("%d %d %x %d ",fuct ,rs,rt ,rd);
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
        printf("lh\n");
        break;
        case lb:
        printf("lb\n");
        break;
        case lbu:
        printf("lbu\n");
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
        printf("ori\n");
        break;
        case nori:
        printf("nori\n");
        break;
        case slti:
        printf("slti\n");
        break;
        case beq:
        printf("beq\n");
        break;
        case bne:
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
    PC+=4;
    }
    fclose(iImg);
    fclose(dImg);


}
iImageInit(unsigned char * iImgbuf ,int iImgLen)
{
    int i =0;
    int addr;
    for(i=0;i<4;i++)
     PC= (PC<<8) + iImgbuf[i];
  //  printf("%x",PC);
    addr=PC;
 //   printf("%d",addr);
    for(i=8;i<iImgLen;i++)
    iMemory[addr++]= iImgbuf[i];
}
dImageInit(unsigned char *dImgbuf , int dImgLen )
{
    int i =0;
    int temp;
    int addr=0;
    for(i=0;i<4;i++)
     temp= (temp<<8) + dImgbuf[i];
    reg [29] =temp;
    for(i=8;i<dImgLen;i++)
    dMemory[addr++]= dImgbuf[i];

}
int getfuct()
{
    int temp=0;
    temp = iMemory[PC+3];
   temp=(temp<<2>>2) ;
    return temp;
}
int mapopcode()
{
    unsigned int temp1;
    temp1=0;
    temp1 = iMemory[PC];
    temp1 = (temp1>>2);
    //printf("%d" ,temp1);
    return temp1;
}
int getrs()
{
int temp1,temp2=0;
temp1=iMemory[PC];
temp2=iMemory[PC+1];
temp1=(temp1<<30>>27);
temp2=(temp2>>5);
return temp1+temp2;
}
int getrt()
{
int temp1,temp2=0;
temp1=iMemory[PC+1];
temp1=((temp1<<27)>>27);
return temp1;
}
int getrd()
{
int temp1,temp2=0;
temp1=iMemory[PC+2];
temp1=temp1>>3;
return temp1;
}
