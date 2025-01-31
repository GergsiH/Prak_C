#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    FILE *inp, *outp;
    unsigned char utf8;
    int offset = 0, d = 0;
    unsigned short utf16;

    for (int i=0; i<argc; i++){
        if (strcmp(argv[i], "-i")==0){
            if ((inp=fopen(argv[i+1], "r"))==NULL){
                fprintf(stderr, "NO INPUT FILE\n");
                return 1;
            }
        }else if (strcmp(argv[i], "-o")==0){
            if ((outp=fopen(argv[i+1], "w"))==NULL){
                fprintf(stderr, "NO OUTPUT FILE\n");
                return 2;
            }
        }
    }

    if (argc >= 2){
        if ((inp = fopen(argv[2], "r")) == NULL){
            fprintf(stderr, "ERROR NO READ\n");
            return 1;
        }
    }
    else{
        inp = stdin;
    }
    if (argc >= 4){
        if ((outp = fopen(argv[4], "w")) == NULL){
            fprintf(stderr,"ERROR NO WRITE\n");
            return 2;
        }
    }
    else{
        outp = stdout;
    }

    utf8 = 0xFF;
    fwrite(&utf8, sizeof(utf8), 1, outp);
    utf8 = 0xFE;
    fwrite(&utf8, sizeof(utf8), 1, outp);

    while( fread(&utf8, 1, 1, inp)){
        utf16 = 0;
        offset++;
        if ((utf8 >> 7) == 0){
            utf16 += utf8;
            d = 1;
        }
        else if ((utf8 >> 5) == 6){
            utf8 = utf8 & 31;
            utf16 += utf8;
            utf16 = utf16 << 6;
            fread(&utf8, 1, 1, inp);
            offset++;

            if((utf8 >> 6) == 2){
                utf8 = utf8 & 63;
                utf16 += utf8;
                d =1;
            }
            else{
                fprintf(stderr, "ERROR\n");
            }
        }
        else if ((utf8 >> 4) == 14){
            utf8 = utf8 & 15;
            utf16 += utf8;
            utf16 = utf16 << 6;
            fread(&utf8, 1, 1, inp);
            offset++;

            if ((utf8 >> 6) == 2){
                utf8 = utf8 & 63;
                utf16 += utf8;
                utf16 = utf16 << 6;
                fread(&utf8, sizeof(char), 1, inp);
                offset++;

                if ((utf8 >> 6) == 2){
                    utf8 = utf8 & 63;
                    utf16 += utf8;
                    d = 1;
                }
                else {
                    fprintf(stderr,"ERROR\n");
                }
            }
            else{
                fprintf(stderr,"ERROR\n");
            }
        }
        else {
            fprintf(stderr,"ERROR\n");
        }
        if (d){
            fwrite(&utf16, sizeof(utf16), 1, outp);
            d = 0;
        }
    }

    printf("\n");

    fclose(inp);
    fclose(outp);

    return 0;
}