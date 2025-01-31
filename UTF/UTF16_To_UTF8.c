#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *inp, *outp;
    unsigned short utf16;
    char ucs1, ucs2, ucs3;
    int cnt = 2, BE = 0;

    for (int i=0; i<argc; i++){
        if (strcmp(argv[i], "-i")==0){
            if ((inp=fopen(argv[i+1], "r"))==NULL){
                fprintf(stderr, "INVALID\n");
                return 1;
            }
        }else if (strcmp(argv[i], "-o")==0){
            if ((outp=fopen(argv[i+1], "w"))==NULL){
                fprintf(stderr, "INVALID\n");
                return 2;
            }
        }
    }

    if (argc >= 2)
    {
        if ((inp = fopen(argv[2], "r")) == NULL)
        {
            fprintf(stderr, "ERROR\n");
            return 1;
        }
    }
    else
    {
        inp = stdin;
    }

    if (argc >= 4)
    {
        if ((outp = fopen(argv[4], "w")) == NULL)
        {
            fprintf(stderr, "ERROR\n");
            return 2;
        }
    }
    else
    {
        outp = stdout;
    }
    cnt = fread(&utf16, 1, 2, inp);

    if(utf16==0xFEFF)
        BE++;

    cnt = fread(&utf16, 1, 2, inp);

    if (cnt == 1)
    {
        fprintf(stderr, "ERROR\n");
    }

    while (cnt == 2)
    {
        if (BE == 0)
        {
            ucs1 = (char)(utf16 / 256);
            utf16 = utf16 << 8;
            utf16 += ucs1;
        }
        if (utf16 < 128)
        {
            ucs1 = (char)utf16;
            fputc(ucs1, outp);
        }
        else if (utf16 < 2048)
        {
            ucs2 = (char)(utf16 % 64);
            ucs2 = ucs2 + 128;
            utf16 = utf16 >> 6;

            ucs1 = (char)(utf16);
            ucs1 = ucs1 + 128 + 64;

            fputc(ucs1, outp);
            fputc(ucs2, outp);
        }
        else
        {
            ucs3 = (char)(utf16 % 64);
            ucs3 = ucs3 + 128;
            utf16 = utf16 >> 6;

            ucs2 = (char)(utf16 % 64);
            ucs2 = ucs2 + 128;
            utf16 = utf16 >> 6;

            ucs1 = (char)(utf16);
            ucs1 = ucs1 + 128 + 64 + 32;

            fputc(ucs1, outp);
            fputc(ucs2, outp);
            fputc(ucs3, outp);
        }
        cnt = fread(&utf16, 1, 2, inp);
    }
    if (cnt == 1)
    {
        fprintf(stderr, "ERROR\n");
    }
    printf("\n");

    fclose(inp);
    fclose(outp);

    return 0;
}
