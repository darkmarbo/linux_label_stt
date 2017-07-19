#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getCharInfo.h"

const int MAX_LEN = 1200;

short Split(char *line,char ** seg, short *nSeg)
{
    int i,j,k,len;
    len=strlen(line);
    i=0;j=0;
    *nSeg=0;
    while(j<len)
    {
        for(i=j; line[i]==' ' && i<len; i++);

        if(i==len)
        {
            break;
        }

        for(j=i;line[j]!=' ' && j<len; j++);

        for(k=i;k<j;k++)
        {
            seg[*nSeg][k-i]=line[k];
        }
        seg[*nSeg][k-i]=0;
        *nSeg=*nSeg + 1;
    }
    return *nSeg;
}

short char2short(char *ctag,short *tag){
    int i,len;
    len=strlen(ctag);
    for(i=0;i<len;i++){
        tag[i]=ctag[i]-(short)'0';
    }
    return len;
}

void dropReturnTag(char *c){
    if(strlen(c)==0){
        return;
    }
    if(c[strlen(c)-1]=='\n'){
        c[strlen(c)-1]=0;
    }
}
int main(int argc, char *argv[])
{

    if(argc < 2)
    {

        printf("usage: %s in_file \n",argv[0]);
        printf("输出结果到 full 目录下 \n");
        return 0;
    }

    FILE *fp;
    fp = fopen(argv[1],"r");
    if(NULL == fp)
    {
        printf("open file(%s) failed!\n",argv[1]);
        return 0;
    }

    char *line_1, *line_2, *lname, *sname, *line_1_tmp;
    line_1  =   (char *)malloc(sizeof(char)*MAX_LEN);
    line_2  =   (char *)malloc(sizeof(char)*MAX_LEN);
    lname   =   (char *)malloc(sizeof(char)*120);
    sname   =   (char *)malloc(sizeof(char)*120);
    int ii;


    TtsLabelCharInfo * cif;
    TTS_Label_Init();
    char **pinyin;
    short *tag;
    short nSeg;
    cif     =   (TtsLabelCharInfo *)malloc(sizeof(TtsLabelCharInfo)*300);
    pinyin  =   (char **)malloc(sizeof(char *)*300);

    for(ii=0; ii<300; ii++)
    {
        pinyin[ii]=(char *)malloc(sizeof(char)*12);
    }
    tag = (short *)malloc(sizeof(short)*300);


    printf("开始处理...\n");
    ii = 0;
    while( fgets(line_1, MAX_LEN,fp)  != NULL)
    {
        if(fgets(line_2, MAX_LEN,fp) == NULL)
        {
            break;
        }
        dropReturnTag(line_1);
        dropReturnTag(line_2);

        // split line_1 
        int flag = 0;
        int jj = 0;
        for(jj=0; jj<strlen(line_1); jj++)
        {
            if(line_1[jj] == '\t')
            {
                line_1[jj] = '\0';
                line_1_tmp = line_1+jj+1;
                flag = 1;
            }
        }

        if(flag == 0)
        {
            printf("input line format err: %s\n",line_1);
            return 0;
        }

        Split(line_1_tmp, pinyin,&nSeg);
        if(char2short(line_2,tag) != nSeg)
        {
            //printf("warning: %s \n",line_1);
        }
        TtsLabel_ObtainLabelCharSeq(cif,pinyin,nSeg,tag);

        //itoa(ii+1, lname, 10);
        snprintf(lname, 100, "%s", line_1);
        strcat(lname, ".lab");
        strcpy(sname, "full/");
        strcat(sname, lname);
        PrintLabel(cif, nSeg, sname);

        ii++;

    }
    printf("结束处理...\n");

    free(line_1);
    free(line_2);


    return 0;
}
