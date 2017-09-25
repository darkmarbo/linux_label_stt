#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <map>
#include "getCharInfo.h"

using namespace std;

const int MAX_LEN = 1200;

typedef map<string, string> map_pp;
map_pp map_py2phone;
map_pp map_phone;

PinyinInfo pinyin2phone[411];


/*
 * 获取cif的 最后一个phoneme  
 * */
int get3ph(const TtsLabelCharInfo &cif, char *ph)
{

    return 0;
}

/*
 // tt ui n
 * */
short split_str(const char *line, char ** seg, short *nSeg)
{
    int i,j,k,len;
    len=strlen(line);
    i=0;j=0;
    *nSeg=0;
    while(j<len)
    {
        // 找到第一个非 空格 
        for(i=j; line[i]==' ' && i<len; i++);

        if(i==len)
        {
            break;
        }

        // 继续找到第一个空格 
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

/*
 * 读取 ko-kr_rr2ph.phmap 到 map_py2phone 
 * 读取 ko-kr.phone  到 map_phone
 * */
int TTS_Label_Init()
{

    int ret = 0;
    FILE *fp;
    char *line;
    char *line_tmp;
    line  =   (char *)malloc(sizeof(char)*MAX_LEN);

    //  读取 ko-kr_rr2ph.phmap 到 map_py2phone 
    fp = fopen("ko-kr_rr2ph.phmap","r");
    if(NULL == fp)
    {
        printf("open file(ko-kr_rr2ph.phmap) failed!\n");
        return 0;
    }

    while( fgets(line, MAX_LEN, fp)  != NULL)
    {
        line[strlen(line)-1] = '\0';
        int jj = 0;
        int flag = 0;
        for(jj=0; jj<strlen(line); jj++)
        {
            if(line[jj] == '\t')
            {
                line[jj] = '\0';
                line_tmp = line + jj + 1;
                flag = 1;
            }
        }

        if(flag == 0)
        {
           printf("load map format err: %s\n",line); 
           continue;
        }

        map_py2phone[line] = line_tmp;
        //printf("load map_py2phone=[%s,%s]\n",line, line_tmp);

    }
    fclose(fp);
    

    // 读取 ko-kr.phone  到 map_phone
    fp = fopen("ko-kr.phone","r");
    if(NULL == fp)
    {
        printf("open file(ko-kr.phone) failed!\n");
        return 0;
    }

    while( fgets(line, MAX_LEN, fp)  != NULL)
    {
        line[strlen(line)-1] = '\0';
        int jj = 0;
        int flag = 0;
        for(jj=0; jj<strlen(line); jj++)
        {
            if(line[jj] == '\t')
            {
                line[jj] = '\0';
                line_tmp = line + jj + 1;
                flag = 1;
            }
        }

        if(flag == 0)
        {
           printf("load map format err: %s\n",line); 
           continue;
        }

        map_phone[line] = line_tmp;
        //printf("load map_=[%s,%s]\n",line, line_tmp);

    }
    fclose(fp);

    return ret;

}


/*
 * 使用 map_phone 
 * 按照最大匹配  将syllable 拆分成 phone 
 * */
int split_syll2phone(const string syll, string &phones)
{
    int ret = 0;
    int ii = 0;
    int flag = 0;
    int len = syll.size();

    for(ii=len-1; ii>0; ii--)
    {
        string tmp_1 = syll.substr(0,ii);
        string tmp_2 = syll.substr(ii,len-ii);
        string phs_1 = "";
        string phs_2 = "";
        phones = "";
        
        
        if(map_phone.count(tmp_1) > 0 )
        {
            phs_1 = tmp_1;
            // tmp1 tmp2 都在
            if(map_phone.count(tmp_2) > 0)
            {
                phs_2 = tmp_2;
                phones = phs_1 + " " + phs_2;
                return 0;
            }
            // tmp_2 可拆分 
            else if(split_syll2phone(tmp_2, phs_2) == 0)
            {
                phones = phs_1 + " " + phs_2;
                return 0;

            }
            
        }
    }

    return  -1;
}


/*
 *  获取 syllable 对应的 phoneme 序列 
 *  不在map的syll  按照最大匹配 进行拆分 
 *  没有返回 -1 
 * */
int getPhones(const string syll, string &phones)
{
    int ret = 0;
    if(map_py2phone.count(syll) > 0)
    {
        phones = map_py2phone[syll];
    }
    else
    {
        //printf("map_py2phone 中不存在key=%s\n", syll.c_str());
        ret = split_syll2phone(syll, phones);
        if(ret != 0)
        {
            printf("getPhones err:%s\n",syll.c_str());
            return -1;
        }
        //printf("\ttest:%s   -->  %s\n", syll.c_str(), phones.c_str());
    }
        
    return ret;
}



/*
 * TtsLabelCharInfo:存储每个 phone的信息 
 * pinyinSeq: 输入的syllable 序列 
 * sNum: syllable 的个数
 * tag:
 * */
int TtsLabel_ObtainLabelCharSeq(TtsLabelCharInfo * cif, char **pinyinSeq,
            short sNum, short * tag)
{
    //tag: 0,1,2,3,4
    int i;
    int ret = 0;
    string phones;

    // 个数全是 -1  后面还需要修改的 
    cif[0].CharInPwNum=-1;
    cif[0].CharInPpNum=-1;
    cif[0].CharInIpNum=-1;
    cif[0].CharInSentNum=-1;
    cif[0].PwInPpNum=-1;
    cif[0].PwInIpNum=-1;
    cif[0].PwInSentNum=-1;
    cif[0].PpInIpNum=-1;
    cif[0].PpInSentNum=-1;
    cif[0].IpInSentNum=-1;

    // 位置 全是1  全是开始第1个 
    cif[0].CharInPwPos=1;
    cif[0].CharInPpPos=1;
    cif[0].CharInIpPos=1;
    cif[0].CharInSentPos=1;
    cif[0].PwInPpPos=1;
    cif[0].PwInIpPos=1;
    cif[0].PwInSentPos=1;
    cif[0].PpInIpPos=1;
    cif[0].PpInSentPos=1;
    cif[0].IpInSentPos=1;



    for(i=0; i<sNum; i++)
    {
        // 每一个 syllable (pinyin) 
        strcpy(cif[i].pinyin, pinyinSeq[i]);

        // 获取对应的 phone 序列 
        ret = getPhones(pinyinSeq[i], phones); 
        if(ret != 0)
        {
            printf("err: getPhones(%s) \n", pinyinSeq[i]);
            continue;
        }
    
        // 拆分 phones 到 arr
        char* phs[3];
        phs[0] = cif[i].ph1;
        phs[1] = cif[i].ph2;
        phs[2] = cif[i].ph3;
        short num_ph;
    
        if(split_str(phones.c_str(), phs, &num_ph) < 1)
        {
            printf("err: split_str failed!\n");
            return -1;
        }
        
        cif[i].num_ph = num_ph;
        // 添加到 phs 
        for(int ii=0; ii<num_ph; ii++)
        {
            if(ii==0)
              strcpy(cif[i].phs[ii], cif[i].ph1);
            else if(ii==1)
              strcpy(cif[i].phs[ii], cif[i].ph2);
            else if(2==ii)
              strcpy(cif[i].phs[ii], cif[i].ph3);
        }
    
        //printf("test:%s+%s+%s+%s+%d\n", pinyinSeq[i], 
        //            cif[i].ph1, cif[i].ph2, cif[i].ph3, cif[i].num_ph);
    
    
    }


    // 第 2 3... 个 syllable 
    for(i=1; i<sNum; i++)
    {

        if(tag[i-1]==0)
        {
            // 新Char 开始 新 Char   
            // 4-3-2-1  
            cif[i].CharInPwPos = cif[i-1].CharInPwPos + 1;
            cif[i].CharInPpPos = cif[i-1].CharInPpPos + 1;
            cif[i].CharInIpPos = cif[i-1].CharInIpPos + 1;
            cif[i].CharInSentPos = i+1;

            // 同一个 PW
            cif[i].PwInPpPos = cif[i-1].PwInPpPos;
            cif[i].PwInIpPos = cif[i-1].PwInIpPos;
            cif[i].PwInSentPos = cif[i-1].PwInSentPos;

            cif[i].PpInIpPos = cif[i-1].PpInIpPos;
            cif[i].PpInSentPos = cif[i-1].PpInSentPos;

            cif[i].IpInSentPos = cif[i-1].IpInSentPos;

        }
        else if(tag[i-1]==1)
        {
            // 新的韵律词开始  新 PW  
            cif[i].CharInPwPos = 1;
            cif[i].CharInPpPos = cif[i-1].CharInPpPos + 1;
            cif[i].CharInIpPos = cif[i-1].CharInIpPos + 1;
            cif[i].CharInSentPos = i+1;

            cif[i].PwInPpPos = cif[i-1].PwInPpPos +1;
            cif[i].PwInIpPos = cif[i-1].PwInIpPos +1;
            cif[i].PwInSentPos=cif[i-1].PwInSentPos+1;

            // 同一个 韵律短语内部  同一个 PP
            cif[i].PpInIpPos=cif[i-1].PpInIpPos;
            cif[i].PpInSentPos=cif[i-1].PpInSentPos;
            cif[i].IpInSentPos=cif[i-1].IpInSentPos;

        }
        else if(tag[i-1]==2)
        {
            // 新的韵律短语开始 新 PP
            cif[i].CharInPwPos=1;
            cif[i].CharInPpPos=1;
            cif[i].CharInIpPos=cif[i-1].CharInIpPos+1;
            cif[i].CharInSentPos=i+1;
            cif[i].PwInPpPos=1;
            cif[i].PwInIpPos=cif[i-1].PwInIpPos+1;
            cif[i].PwInSentPos=cif[i-1].PwInSentPos+1;
            cif[i].PpInIpPos=cif[i-1].PpInIpPos+1;
            cif[i].PpInSentPos=cif[i-1].PpInSentPos+1;

            // 同一个句子 
            cif[i].IpInSentPos=cif[i-1].IpInSentPos;
        }
        else
        {
            // 新的语调短语开始 新 IP  
            cif[i].CharInPwPos=1;
            cif[i].CharInPpPos=1;
            cif[i].CharInIpPos=1;
            cif[i].CharInSentPos=i+1;
            cif[i].PwInPpPos=1;
            cif[i].PwInIpPos=1;
            cif[i].PwInSentPos=cif[i-1].PwInSentPos+1;
            cif[i].PpInIpPos=1;
            cif[i].PpInSentPos=cif[i-1].PpInSentPos+1;
            cif[i].IpInSentPos=cif[i-1].IpInSentPos+1;
        }
    }


    // 把所有暂时不能确定的  全部设置为 -1 
    for(i=0; i<sNum; i++)
    {
        if(tag[i]==0)
        {
            cif[i].CharInPwNum=-1;
            cif[i].CharInPpNum=-1;
            cif[i].CharInIpNum=-1;
            cif[i].CharInSentNum=-1;
            cif[i].PwInPpNum=-1;
            cif[i].PwInIpNum=-1;
            cif[i].PwInSentNum=-1;
            cif[i].PpInIpNum=-1;
            cif[i].PpInSentNum=-1;
            cif[i].IpInSentNum=-1;
        }
        else if(tag[i]==1)
        {
            // 韵律词包含音节数
            cif[i].CharInPwNum = cif[i].CharInPwPos;

            cif[i].CharInPpNum=-1;
            cif[i].CharInIpNum=-1;
            cif[i].CharInSentNum=-1;
            cif[i].PwInPpNum=-1;
            cif[i].PwInIpNum=-1;
            cif[i].PwInSentNum=-1;
            cif[i].PpInIpNum=-1;
            cif[i].PpInSentNum=-1;
            cif[i].IpInSentNum=-1;
        }
        else if(tag[i]==2)
        {
            // 韵律词包含音节数
            cif[i].CharInPwNum=cif[i].CharInPwPos;
            // 韵律短语包含音节数
            cif[i].CharInPpNum=cif[i].CharInPpPos;

            cif[i].CharInIpNum=-1;
            cif[i].CharInSentNum=-1;

            // 韵律短语包含韵律词个数
            cif[i].PwInPpNum=cif[i].PwInPpPos;

            cif[i].PwInIpNum=-1;
            cif[i].PwInSentNum=-1;
            cif[i].PpInIpNum=-1;
            cif[i].PpInSentNum=-1;
            cif[i].IpInSentNum=-1;
        }
        else if(tag[i]==3){
            cif[i].CharInPwNum=cif[i].CharInPwPos;
            cif[i].CharInPpNum=cif[i].CharInPpPos;
            cif[i].CharInIpNum=cif[i].CharInIpPos;
            cif[i].CharInSentNum=-1;
            cif[i].PwInPpNum=cif[i].PwInPpPos;
            cif[i].PwInIpNum=cif[i].PwInIpPos;
            cif[i].PwInSentNum=-1;
            cif[i].PpInIpNum=cif[i].PpInIpPos;
            cif[i].PpInSentNum=-1;
            cif[i].IpInSentNum=-1;
        }
        else{
            cif[i].CharInPwNum=cif[i].CharInPwPos;
            cif[i].CharInPpNum=cif[i].CharInPpPos;
            cif[i].CharInIpNum=cif[i].CharInIpPos;
            cif[i].CharInSentNum=cif[i].CharInSentPos;
            cif[i].PwInPpNum=cif[i].PwInPpPos;
            cif[i].PwInIpNum=cif[i].PwInIpPos;
            cif[i].PwInSentNum=cif[i].PwInSentPos;
            cif[i].PpInIpNum=cif[i].PpInIpPos;
            cif[i].PpInSentNum=cif[i].PpInSentPos;
            cif[i].IpInSentNum=cif[i].IpInSentPos;
        }
    }

    //for(i=0;i<7;i++)
    //{
    //    printf("%d %d %d %d\n",cif[i].CharInPwNum,cif[i].CharInPpNum,
    //                cif[i].PwInPpNum,cif[i].PwInSentNum);
    //}

    // 从倒数第2个 开始  不能确定的值  全部用其后面的值 代替 
    for(i=sNum-2; i>=0; i--)
    {
        if(cif[i].CharInPwNum<0)    { cif[i].CharInPwNum= cif[i+1].CharInPwNum;}
        if(cif[i].CharInPpNum<0)    {cif[i].CharInPpNum= cif[i+1].CharInPpNum;}
        if(cif[i].CharInIpNum<0)    {cif[i].CharInIpNum= cif[i+1].CharInIpNum;}
        if(cif[i].CharInSentNum<0)  {cif[i].CharInSentNum= cif[i+1].CharInSentNum;}
        if(cif[i].PwInPpNum<0)      {cif[i].PwInPpNum=cif[i+1].PwInPpNum;}
        if(cif[i].PwInIpNum<0)      {cif[i].PwInIpNum=cif[i+1].PwInIpNum;}
        if(cif[i].PwInSentNum<0)    {cif[i].PwInSentNum=cif[i+1].PwInSentNum;}
        if(cif[i].PpInIpNum<0)      {cif[i].PpInIpNum=cif[i+1].PpInIpNum;}
        if(cif[i].PpInSentNum<0)    {cif[i].PpInSentNum=cif[i+1].PpInSentNum;}
        if(cif[i].IpInSentNum<0)    {cif[i].IpInSentNum=cif[i+1].IpInSentNum;}
    }

    return 0;
}


/*
 * 写label序列 
 *
 * */
int PrintLabel(TtsLabelCharInfo * cif, short sNum, char *fname)
{
    int i,k,ret;
    FILE *fp;
    fp = fopen(fname,"w");
    if(!fp)
    {
        printf("打开文件失败:%s\n", fname);
        return -1;
    }

    // nu^nu-sil+g=ua@x_x/A:x_x_x/B:x-x-x@x-x&x-x#x-x$x-x!
    // 0-10;x-x|x/C:x+x+x/D:x_x/E:0+5@x+x&x+x
    // #0+3/F:0_0/G:2

    // 根据 cif[0] 对应有几个 phoneme 来写第1行 
    if(cif[0].num_ph > 1)
    {
        fprintf(fp,"nu^nu-sil+%s=%s@x_x/A:x_x_x/B:x-x-x@x-x&x-x#x-x$x-x!",
                    cif[0].ph1, cif[0].ph2);
    }
    else
    {
        // syllable 只有一个phoneme 对应
        //printf("err: num_ph ==1 pinyin=%s \n", cif[0].pinyin);
        fprintf(fp,"nu^nu-sil+%s=%s@x_x/A:x_x_x/B:x-x-x@x-x&x-x#x-x$x-x!",
                    cif[0].ph1, cif[1].ph1);
    }
    fprintf(fp,"0-0;x-x|x/C:x+x+x/D:x_x/E:0+%d@x+x&x+x", cif[0].PwInSentNum);
    fprintf(fp,"#0+%d/F:0_0/G:%d\n", cif[0].PpInSentNum, cif[0].IpInSentNum);

    // 第2行  // 第1个syllable 的 第1个phoneme 
    // nu^sil-g+ua=n@1_0/A:0_0_1/B:2-4-1@2-1&2-1#5-1$10-2!
    // 2-5;10-1|1/C:1+3+1/D:5_1/E:3+5@1+2&1+3
    // #2+3/F:1_2/G:2

    // 第3行 // 第1个syllable 的 第 2个phone
    //       // 或者是第2个syllable的第 1个phoneme
    // sil^g-ua+n=ong@2_1/A:0_0_1/B:2-4-1@2-1&2-1#5-1$10-2!
    // 2-5;10-1|1/C:1+3+1/D:5_1/E:3+5@1+2&1+3
    // #2+3/F:1_2/G:2
    //

    // 每一个cif[i] 
    for(i=0; i<sNum; i++)
    {
        // 每个cif[i] syllable 对应的 num_ph 个 phoneme  
        for(k=1; k <= cif[i].num_ph; k++)
        {

            //////////////////////////// //p1  前面 -2 个phoneme  
            if(k==1)  // cif[i].ph1
            {
                // 第1个syllable 的 第1个phoneme 
                if(i==0)
                {
                    fprintf(fp,"nu");
                }
                else
                // 第 2.3.4 个 syllable 的 第1个 phoneme 
                {
                    // 新的IP开始  前面实际上是sil 
                    if(cif[i].CharInIpPos == 1)
                    {
                        // 需要写入前面syllable的最后一个phoneme 
                        fprintf(fp,"%s", cif[i-1].phs[ cif[i-1].num_ph-1 ] );
                    }
                    else
                    {
                        // 需要写入前面syllable的 倒数第2个phoneme 
                        if(cif[i-1].num_ph > 1)
                        {
                            fprintf(fp,"%s", cif[i-1].phs[ cif[i-1].num_ph-2 ] );
                        }
                        else
                        // 如果前面syllable 只有一个 phoneme  
                        {
                            // 没有 i-2个syllable  
                            if(i == 1)
                            {
                                fprintf(fp,"sil");
                            }
                            else
                            // 需要查看i-2个 syllable的最后一个phoneme 
                            {
                                fprintf(fp,"%s", cif[i-2].phs[ cif[i-2].num_ph-1 ] );

                            }

                        }
                        
                    }
                }
            }
            else if(k==2)
            {
                // 第1个syllable 的 第2个phoneme 
                if(i==0)
                {
                    fprintf(fp,"sil");
                }
                else
                // 第2.3.4个syllable 的 第2个phoneme 
                {
                    // IP分界线  前面是静音sil 
                    if(cif[i].CharInIpPos == 1)
                    {
                        fprintf(fp,"sil");
                    }
                    else
                    {
                        // 前一个syllable的 最后一个phoneme
                        fprintf(fp,"%s",cif[i-1].phs[ cif[i-1].num_ph -1 ]);
                    }
                }
            }
            else if(k == 3)
            {
                // 当前syllable 的 第3个phoneme 
                fprintf(fp,"%s", cif[i].phs[0]);

            }

            //////////////////////////// //p2  前面 -1 个phoneme  
            if(k==1) // 第1个 phoneme 
            {
                if(i==0)
                {
                    fprintf(fp,"^sil");
                }
                else
                {
                    if(cif[i].CharInIpPos==1)
                    {
                        fprintf(fp,"^sil");
                    }
                    else
                    {
                        fprintf(fp,"^%s", cif[i-1].phs[ cif[i-1].num_ph -1 ] );
                    }
                }
            }
            else  // 第2.3 个 phoneme  取其前面的phoneme 
            {
                fprintf(fp,"^%s", cif[i].phs[k-2] );
            }



            /////////////////   p3  当前的phoneme 
            fprintf(fp,"-%s", cif[i].phs[k-1]);

            //p4  后面的phoneme 
            if(k < cif[i].num_ph)  // 同一个syllable内部 
            {
                fprintf(fp,"+%s", cif[i].phs[k]);
            }
            else
            {
                // 下一个 syllable的 第一个phoneme 
                if(i<sNum-1)
                {
                    if(cif[i].CharInIpPos == cif[i].CharInIpNum)
                    {
                        fprintf(fp,"+sil");
                    }
                    else
                    {
                        fprintf(fp,"+%s",cif[i+1].phs[0]);
                    }
                }
                else
                {
                    fprintf(fp,"+sil");
                }
            }

            ////////////////    p5  后面第 2 个phoneme 
            // 同一个syllable内部 
            if(cif[i].num_ph > k+1)  // k==1  num_ph==3
            {
                fprintf(fp,"=%s",cif[i].phs[k+1]);
            }
            else if(cif[i].num_ph == k+1) 
            // syllable 内部  后面还有一个phoneme 
            // k==1  &&  num_ph ==2 或者 k==2 && num_ph == 3
            {
                // 后面有 syllable 
                if(i<sNum-1 &&  cif[i].CharInIpPos != cif[i].CharInIpNum )
                    fprintf(fp,"=%s",cif[i+1].phs[0]);
                else
                    fprintf(fp,"=sil");

            }
            else  // num_ph == k  syllable内部无phoneme了 
            {
                // 后面是 sil
                if(cif[i].CharInIpPos == cif[i].CharInIpNum)
                {
                    // 一定在下一个syllable内部
                    if(i<sNum-1)  
                        fprintf(fp,"=%s",cif[i+1].phs[0]);
                    else
                        fprintf(fp,"=nu");
                }
                else if(i<sNum-1 && cif[i+1].num_ph > 1 )  
                // 后面是syllable  并且有2个phoneme 
                {
                    fprintf(fp,"=%s", cif[i+1].phs[1]);
                }
                else if(i<sNum-1 && cif[i+1].num_ph == 1)  
                // 后面是syllable  并且有1个phoneme 
                {
                    // i+2 个syllable 存在 并且 i+1处不是sil
                    if(i<sNum-2 && cif[i+1].CharInIpPos != cif[i+1].CharInIpNum)
                        fprintf(fp,"=%s", cif[i+2].phs[0]);
                    else
                        fprintf(fp,"=nu");
                }
            }


            //p6  音素类型  1声母，2韵母
            // 现在变成: 当前phoneme 是syllable的第几个phoneme  
            fprintf(fp,"@%d",k);

            //p7    清浊类型    0-清， 1-浊,   
            fprintf(fp,"_0");

            //a1_a2_a3_b1_b2    前后调型    0-5
            fprintf(fp,"/A:0_0_0/B:0-0");

            //-b3@b4    音节在韵律词位置
            fprintf(fp,"-%d", cif[i].CharInPwPos);
            fprintf(fp,"@%d", cif[i].CharInPwNum - cif[i].CharInPwPos +1 );

            //-b5&b6    音节在韵律短语位置
            fprintf(fp,"-%d", cif[i].CharInPpPos);
            fprintf(fp,"&%d", cif[i].CharInPpNum - cif[i].CharInPpPos +1);

            //-b7#b8    音节在语调短语位置
            fprintf(fp,"-%d", cif[i].CharInIpPos);
            fprintf(fp,"#%d", cif[i].CharInIpNum - cif[i].CharInIpPos +1);

            //-b9$b10   音节在句子位置
            fprintf(fp,"-%d",cif[i].CharInSentPos);
            fprintf(fp,"$%d",cif[i].CharInSentNum - cif[i].CharInSentPos +1);

            //-b11!b12-b13;b14  韵律词|韵律短语|语调短语|句子  包含音节数
            fprintf(fp,"-%d",cif[i].CharInPwNum);
            fprintf(fp,"!%d",cif[i].CharInPpNum);
            fprintf(fp,"-%d",cif[i].CharInIpNum);
            fprintf(fp,";%d",cif[i].CharInSentNum);

            //-b15|b16/C:c1+c2+c3/D:d1   韵律词在  韵律短语|语调短语|句子  位置
            fprintf(fp,"-%d",cif[i].PwInPpPos);
            fprintf(fp,"|%d", cif[i].PwInPpNum- cif[i].PwInPpPos+1 );
            fprintf(fp,"/C:%d",cif[i].PwInIpPos);
            fprintf(fp,"+%d",cif[i].PwInIpNum - cif[i].PwInIpPos +1);
            fprintf(fp,"+%d",cif[i].PwInSentPos);
            fprintf(fp,"/D:%d",cif[i].PwInSentNum - cif[i].PwInSentPos +1);

            //_d2/E:e1+e2    韵律短语|语调短语|句子    包含韵律词个数
            fprintf(fp,"_%d", cif[i].PwInPpNum);
            fprintf(fp,"/E:%d", cif[i].PwInIpNum);
            fprintf(fp,"+%d", cif[i].PwInSentNum);

            //@e3+e4&e5+e6     PP在  IP|ST  位置
            fprintf(fp,"@%d", cif[i].PpInIpPos);
            fprintf(fp,"+%d", cif[i].PpInIpNum- cif[i].PpInIpPos+1);
            fprintf(fp,"&%d", cif[i].PpInSentPos);
            fprintf(fp,"+%d", cif[i].PpInSentNum- cif[i].PpInSentPos+1);

            //#e7+e8
            fprintf(fp,"#%d", cif[i].PpInIpNum);
            fprintf(fp,"+%d", cif[i].PpInSentNum);

            // /F:f1_f2
            fprintf(fp,"/F:%d", cif[i].IpInSentPos);
            fprintf(fp,"_%d", cif[i].IpInSentNum- cif[i].IpInSentPos+1);

            //g1
            fprintf(fp,"/G:%d", cif[i].IpInSentNum);

            //line end
            fprintf(fp,"\n");


            /////////////// 当前是sil 语调短语边界处 
            if(cif[i].CharInIpPos == cif[i].CharInIpNum && k == cif[i].num_ph)
            {
                //   c^un-sil+sh=ib@x_x/A:x_2_x/B:4-x-x@x-x&x-x#x-x$x-x!x-
                //   5;10-x|x/C:x+x+x/D:d1_d2/E:3+5@x+x&x+x#2+3/F:1_2/G:2
                //
                // 其前面 至少有一个 syllable 
                // 前面syllable  够用了 
                if(cif[i].num_ph > 1)
                {
                    // 取前面syllable的 倒数第1.2个phoneme 
                    fprintf(fp,"%s^%s-sil+", cif[i].phs[cif[i].num_ph-2], 
                                cif[i].phs[cif[i].num_ph-1] );
                }
                //cif[i] 只有一个phoneme 
                else if(i>0 && cif[i-1].CharInIpPos != cif[i-1].CharInIpNum)
                    //cif[i-1] 的 syllable 存在 并且非IP边界 
                {
                    fprintf(fp,"%s^%s-sil+", cif[i-1].phs[cif[i].num_ph-1], 
                                cif[i].phs[0] );

                }
                else // cif[i] 只有一个phoneme  前面是sil 
                {
                    fprintf(fp, "sil^%s-sil+", cif[i].phs[0] );
                }


                // 后面还有 syllable 
                if(i<sNum-1 )
                {
                    // i+1 够用了 
                    if(cif[i+1].num_ph > 1)
                    {
                        fprintf(fp,"%s=%s@x_x/A:x_%d_x/B:%d-x-x@x-x&x-x#x-x$x-x!x-",
                        cif[i+1].phs[0], cif[i+1].phs[1], cif[i].yindiao, cif[i+1].yindiao);
                    }
                    else // i+1 只有一个phoneme 
                    {
                        // 后面的syllable 只有一个phoneme 
                        // 是否有i+2个syllable 存在 并且 i+1处 非IP边界  
                        if(i+2 < sNum &&  cif[i+1].CharInIpPos != cif[i+1].CharInIpNum)
                        {
                            fprintf(fp,"%s=%s@x_x/A:x_%d_x/B:%d-x-x@x-x&x-x#x-x$x-x!x-",
                            cif[i+1].phs[0], cif[i+2].phs[0],cif[i].yindiao,cif[i+1].yindiao);

                        }
                        else
                            fprintf(fp,"nu=nu@x_x/A:x_%d_x/B:x-x-x@x-x&x-x#x-x$x-x!x-",
                                cif[i].yindiao);

                    }
                }
                else
                {
                    fprintf(fp,"nu=nu@x_x/A:x_%d_x/B:x-x-x@x-x&x-x#x-x$x-x!x-",
                                cif[i].yindiao);
                }

                fprintf(fp,"%d;%d-x|x/C:x+x+x/D:d1_d2/E:%d+%d@x+x&x+x#",
                            cif[i].CharInIpNum,cif[i].CharInSentNum,
                            cif[i].PwInIpNum,cif[i].PwInSentNum);
                fprintf(fp,"%d+%d/F:%d_%d/G:%d\n",cif[i].PpInIpNum,
                            cif[i].PpInSentNum,cif[i].IpInSentPos,
                        cif[i].IpInSentNum-cif[i].IpInSentPos+1,cif[i].IpInSentNum);
            }
        }

    }


    fclose(fp);

    return 0;

}


int isQing(char *phone)
{
    char *qlist[]={"zh","sil","ch","c","b","d","g","f","h","k","j","q","p","s","sh","t","x","z"};
    int i;
    for(i=0;i<18;i++){
        if(strcmp(phone,qlist[i])==0){
            return 1;
        }
    }
    return 0;

}



void getShengYun(char *pinyin,char *shengmu,char *yunmu,short *yindiao)
{
    PinyinInfo *pif;//
    char py[9];
    int len=strlen(pinyin);
    int pyID;
    if(pinyin[len-1]=='1'){*yindiao=1;}
    if(pinyin[len-1]=='2'){*yindiao=2;}
    if(pinyin[len-1]=='3'){*yindiao=3;}
    if(pinyin[len-1]=='4'){*yindiao=4;}
    if(pinyin[len-1]=='5'){*yindiao=5;}
    strcpy(py,pinyin);
    py[len-1]=0;
    pyID=getPinyinInfoID(py);
    pif=pinyin2phone+pyID;
    if(pyID<0){
        printf("warning: pinyin %s does not exist!\n",py);
    }
    strcpy(shengmu,pif->shengmu);
    strcpy(yunmu,pif->yunmu);
};

short getPinyinInfoID(char *pinyin)
{
    short l=0,u=411;
    short m,comp;
    while(u-l>=2)
    {
        m=(u+l)/2;
        comp=strcmp(pinyin,pinyin2phone[m].pinyin);
        if(comp==0){return m;}
        else if(comp<0){u=m;}
        else{(l=m);}
    }
    if(strcmp(pinyin,pinyin2phone[l].pinyin)==0){
        return l;
    }
    if(strcmp(pinyin,pinyin2phone[u].pinyin)==0){
        return u;
    }
    return -1;
}





