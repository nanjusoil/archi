#include <stdio.h>

char iMemory[1024];
char dMemory[1024];

int main()
{
    FILE *iImg , *dImg;
   unsigned char* iImgbuf, *dImgbuf;
    int i = 0;
    int iImgLen , dImgLen;
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
    for(i = 0 ; i <dImgLen ;i++)
        {
            printf("%02x" , dImgbuf[i]);
        }
    fclose(iImg);
    fclose(dImg);


}
