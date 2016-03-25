#include <stdio.h>

char iMemory[1024];
char dMemory[1024];

int main()
{
    FILE *iImg , *dImg;
   unsigned char* iImgbuf,dImgbuf;
    int i = 0;
    int iImgLen , dImgLen;
    iImg = fopen("iimage.bin", "rb");
    dImg = fopen("dimage.bin", "rb");
    fseek(iImg, 0, SEEK_END);
    fseek(dImg, 0, SEEK_END);
    dImgLen = ftell(iImg);
    iImgLen = ftell(dImg);

    iImgbuf =(char*) malloc(ftell(iImg));
    dImgbuf =(char*) malloc(ftell(dImg));

    fseek(iImg, 0, SEEK_SET);
    fseek(iImg, 0, SEEK_SET);

    fread(iImgbuf , sizeof(char),dImgLen , iImg );
    fread(dImgbuf , sizeof(char),iImgLen , dImg );
    for(i = 0 ; i <ftell(iImg) ;i++)
        {
            printf("%02x" , iImgbuf[i]);
        }
    fclose(iImg);
    fclose(dImg);


}
