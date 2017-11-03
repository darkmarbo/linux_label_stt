#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <map>
#include "getCharInfo.h"

using namespace std;

MYMAP map_ph;

const int MAX_LEN = 1200;

PinyinInfo pinyin2phone[411];


/*
 * 存储 phoneme 到map中 声韵母 
 * */
int load_map_ph(const char *file, MYMAP &map_ph)
{
    // HH 2
    int ret = 0;
    FILE *fp = fopen(file, "r");
    char line[MAX_LEN] = {0};

    while(fgets(line, MAX_LEN, fp))
    {
        if(strlen(line) == 0)
          continue;
        line[strlen(line)-1] = 0;

        vector<string> res;
        split(res, line, '\t');
        if(res.size() < 2 )
          continue;
        map_ph[res[0]] = res[1];
    }


    return ret;
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

int TTS_Label_Init()
{

    int ret = 0;

    ret = load_map_ph("eng.phone", map_ph);


    return ret;

}

/*
 *  获取 syllable 对应的 phoneme 序列 
 *  aa_bb_cc  按照_拆分即可 
 *  没有返回 -1 
 * */
int getPhones(const string syll, string &phones)
{
    // HH_AY1_N_D3
    int ret = 0;
    phones="";
    vector<string> res;
    split(res, syll, '_');
    for(int ii=0; ii<res.size(); ii++)
    {
        if (ii != 0)
        {
            phones = phones + " " + res[ii];
        }
        else
        {
            phones = res[ii];
        }
    }
    ret = res.size();

    return ret;
}

/*
 *  // 得到处理后的 new_ph 
// stress phoneme的重音情况 012  没有重音记为4(辅音) 
// accent 是句重音标记为 1  不是标记为 0 
// 返回值 ret: 0(什么都没有的辅音)  1(只有012) 2(只有3) 3(03 13 23组合)
 * */
int getStress(const string &ph, string &new_ph, char &sts, char &acc)
{
    int ret = 0;
    // 默认sts为4  acc为0 
    sts = '4';
    acc = '0';

    // sts 与 phs 对应 
    string ph_tmp = ph;
    // string ph_tmp2 = ph;

    char last = ph_tmp[ph_tmp.length()-1];

    // 没有3 只有012      AA0  AA1  AA2   
    // 那么 acc 默认就是没有3了 赋值为0 
    if(last == '0' || last == '1' || last == '2')
    {
        sts = last;
        ph_tmp = ph_tmp.substr(0, ph_tmp.length()-1);;
        ret = 1;
    }
    else if(last == '3')
    {
        acc = '1';

        // 删掉当前的 3 
        ph_tmp = ph_tmp.substr(0, ph_tmp.length()-1);
        ret =2;

        // 在往前取一位  看看是不是 012 
        last = ph_tmp[ph_tmp.length()-1];
        if(last == '0' || last == '1' || last == '2')
        {
            sts = last;
            ph_tmp = ph_tmp.substr(0, ph_tmp.length()-1);;
            ret =3;
        }
    }

    new_ph = ph_tmp;

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



    // 读取每个syllable 
    // 获取对应的 每个 phoneme 存储到 cif[i].phs 中 
    for(i=0; i<sNum; i++)
    {
        // 每一个 syllable (pinyin) 
        strcpy(cif[i].pinyin, pinyinSeq[i]);

        // 获取对应的 phone 序列 
        vector<string> res;
        split(res, cif[i].pinyin, '_');

        // syllable内部的每一个 phoneme 
        cif[i].num_ph = res.size();
        for(int j=0; j<res.size(); j++)
        {
            // stress phoneme的重音情况 012  
            char sts = '4';
            // accent 句子的重音情况 '0' '1'  
            char acc = '0';

            string ph_tmp = res[j];
            string new_ph;
            ret = getStress(ph_tmp, new_ph, sts, acc);

            // 判断真的phoneme 是否合法 
            if(map_ph.find(new_ph) != map_ph.end())
            {
                snprintf(cif[i].phs[j], 10, "%s", new_ph.c_str());
                cif[i].sts[j] = sts;
                cif[i].acc = acc;
            }
            else
            {
                printf("error map_ph2:::%s:::\n", ph_tmp.c_str());
            }

        }


    }
    //return 0;


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


    // 第一行 
    //// 根据 cif[0] 对应有几个 phoneme 来写第1行 
    if(cif[0].num_ph > 1)
    {
        fprintf(fp,"nu^nu-sil+%s=%s@x_x/A:x_x_x/B:x-x-x@x-x&x-x#x-x$x-x!",
                    cif[0].phs[0], cif[0].phs[1]);
    }
    else // syllable 只有一个phoneme 对应
    {
        //printf("err: num_ph ==1 pinyin=%s \n", cif[0].pinyin);
        fprintf(fp,"nu^nu-sil+%s=%s@x_x/A:x_x_x/B:x-x-x@x-x&x-x#x-x$x-x!",
                    cif[0].phs[0], cif[1].phs[0]);
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

    //a1_a2_a3_b1_b2    前后调型    0-5
    char dx[5];

    // 每一个cif[i] 
    for(i=0; i<sNum; i++)
    {
        // 每个cif[i] syllable 对应的 num_ph 个 phoneme  
        for(k=1; k <= cif[i].num_ph; k++)
        {

            // p1.  前面 -2 个phoneme  
            if(k==1)  // cif[i].ph1
            {
                // 第1个syllable 的 第1个phoneme 
                if(i==0)
                {
                    fprintf(fp,"nu");
                    dx[0]='x';
                }
                else
                  // 第 2.3.4 个 syllable 的 第1个 phoneme 
                {
                    // 新的IP开始  前面实际上是sil 
                    if(cif[i].CharInIpPos == 1)
                    {
                        // 需要写入前面syllable的最后一个phoneme 
                        fprintf(fp,"%s", cif[i-1].phs[ cif[i-1].num_ph-1 ] );
                        dx[0] = cif[i-1].sts[ cif[i-1].num_ph-1 ];
                    }
                    else
                    {
                        // 需要写入前面syllable的 倒数第2个phoneme 
                        if(cif[i-1].num_ph > 1)
                        {
                            fprintf(fp,"%s", cif[i-1].phs[ cif[i-1].num_ph-2 ] );
                            dx[0] = cif[i-1].sts[ cif[i-1].num_ph-2 ];
                        }
                        else
                          // 如果前面syllable 只有一个 phoneme  
                        {
                            // 没有 i-2个syllable  
                            if(i == 1)
                            {
                                fprintf(fp,"sil");
                                dx[0] = 'x';
                            }
                            else
                              // 需要查看i-2个 syllable的最后一个phoneme 
                            {
                                fprintf(fp,"%s", cif[i-2].phs[ cif[i-2].num_ph-1 ] );
                                dx[0] = cif[i-2].sts[ cif[i-2].num_ph-1 ];

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
                    dx[0] = 'x';
                }
                else
                  // 第2.3.4个syllable 的 第2个phoneme 
                {
                    // IP分界线  前面是静音sil 
                    if(cif[i].CharInIpPos == 1)
                    {
                        fprintf(fp,"sil");
                        dx[0] = 'x';
                    }
                    else
                    {
                        // 前一个syllable的 最后一个phoneme
                        fprintf(fp,"%s",cif[i-1].phs[ cif[i-1].num_ph -1 ]);
                        dx[0] = cif[i-1].sts[ cif[i-1].num_ph-1 ];
                    }
                }
            }
            else
            {
                // 当前syllable 的 第3个phoneme 
                fprintf(fp,"%s", cif[i].phs[0]);
                dx[0] = cif[i].sts[0];

            }

            //  p2.  前面 -1 个phoneme  
            if(k==1) // 第1个 phoneme 
            {
                if(i==0)
                {
                    fprintf(fp,"^sil");
                    dx[1] = 'x';
                }
                else
                {
                    if(cif[i].CharInIpPos==1)
                    {
                        fprintf(fp,"^sil");
                        dx[1] = 'x';
                    }
                    else
                    {
                        fprintf(fp,"^%s", cif[i-1].phs[ cif[i-1].num_ph -1 ] );
                        dx[1] = cif[i-1].sts[ cif[i-1].num_ph-1 ];
                    }
                }
            }
            else  // 第2.3 个 phoneme  取其前面的phoneme 
            {
                fprintf(fp,"^%s", cif[i].phs[k-2] );
                dx[1] = cif[i].sts[k-2];
            }



            //  p3.  当前的phoneme 
            fprintf(fp,"-%s", cif[i].phs[k-1]);
            dx[2] = cif[i].sts[k-1];

            //p4  后面的phoneme 
            if(k < cif[i].num_ph)  // 同一个syllable内部 
            {
                fprintf(fp,"+%s", cif[i].phs[k]);
                dx[3] = cif[i].sts[k];
            }
            else
            {
                // 下一个 syllable的 第一个phoneme 
                if(i<sNum-1)
                {
                    if(cif[i].CharInIpPos == cif[i].CharInIpNum)
                    {
                        fprintf(fp,"+sil");
                        dx[3] = 'x';
                    }
                    else
                    {
                        fprintf(fp,"+%s",cif[i+1].phs[0]);
                        dx[3] = cif[i+1].sts[0];
                    }
                }
                else
                {
                    fprintf(fp,"+sil");
                    dx[3] = 'x';
                }
            }

            //  p5.  后面第 2 个phoneme 
            // 同一个syllable内部 
            if(cif[i].num_ph > k+1)  // k==1  num_ph==3
            {
                fprintf(fp,"=%s",cif[i].phs[k+1]);
                dx[4] = cif[i].sts[k+1];
            }
            else if(cif[i].num_ph == k+1) 
              // syllable 内部  后面还有一个phoneme 
              // k==1  &&  num_ph ==2 或者 k==2 && num_ph == 3
            {
                // 后面有 syllable 
                if(i<sNum-1 &&  cif[i].CharInIpPos != cif[i].CharInIpNum )
                {
                    fprintf(fp,"=%s",cif[i+1].phs[0]);
                    dx[4] = cif[i+1].sts[0];
                }
                else
                {
                    fprintf(fp,"=sil");
                    dx[4] = 'x';
                }

            }
            else  // num_ph == k  syllable内部无phoneme了 
            {
                // 后面是 sil
                if(cif[i].CharInIpPos == cif[i].CharInIpNum)
                {
                    // 一定在下一个syllable内部
                    if(i<sNum-1)  
                    {
                        fprintf(fp,"=%s",cif[i+1].phs[0]);
                        dx[4] = cif[i+1].sts[0];
                    }
                    else
                    {
                        fprintf(fp,"=nu");
                        dx[4] = 'x';
                    }
                }
                else if(i<sNum-1 && cif[i+1].num_ph > 1 )  
                  // 后面是syllable  并且有2个phoneme 
                {
                    fprintf(fp,"=%s", cif[i+1].phs[1]);
                    dx[4] = cif[i+1].sts[1];
                }
                else if(i<sNum-1 && cif[i+1].num_ph == 1)  
                  // 后面是syllable  并且有1个phoneme 
                {
                    // i+2 个syllable 存在 并且 i+1处不是sil
                    if(i<sNum-2 && cif[i+1].CharInIpPos != cif[i+1].CharInIpNum)
                    {
                        fprintf(fp,"=%s", cif[i+2].phs[0]);
                        dx[4] = cif[i+2].sts[0];
                    }
                    else
                    {
                        fprintf(fp,"=nu");
                        dx[4] = 'x';
                    }
                }
            }


            //p6  音素类型  1声母，2韵母
            // 现在变成: 当前phoneme 是syllable的第几个phoneme  
            fprintf(fp,"@%d",k);

            // p7 当前syllable 是否是句重音 accent (0|1)
            // 需要改成当前word 是不是 
            fprintf(fp,"_%c", cif[i].acc);

            // todo 
            //a1_a2_a3_b1_b2    前后调型    0-5
            //fprintf(fp,"/A:0_0_0/B:0-0");
            fprintf(fp,"/A:%c_%c_%c/B:%c-%c", dx[0], dx[1], dx[2], dx[3], dx[4]);

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
                    dx[0] = cif[i].sts[cif[i].num_ph-2];
                    dx[1] = cif[i].sts[cif[i].num_ph-1];
                }
                else if(i>0 && cif[i-1].CharInIpPos != cif[i-1].CharInIpNum)
                //cif[i] 只有一个phoneme 
                  //cif[i-1] 的 syllable 存在 并且非IP边界 
                {
                    fprintf(fp,"%s^%s-sil+", cif[i-1].phs[cif[i].num_ph-1], 
                                cif[i].phs[0] );
                    dx[0] = cif[i-1].sts[cif[i].num_ph-1];
                    dx[1] = cif[i].sts[0];

                }
                else // cif[i] 只有一个phoneme  前面是sil 
                {
                    fprintf(fp, "sil^%s-sil+", cif[i].phs[0] );
                    dx[0] = 'x';
                    dx[1] = cif[i].sts[0];
                }

                dx[2] = 'x';
                dx[3] = 'x';
                dx[4] = 'x';

                // todo
                // 后面还有 syllable 
                if(i<sNum-1 )
                {
                    // i+1 够用了 
                    if(cif[i+1].num_ph > 1)
                    {
                        fprintf(fp,"%s=%s@x_x", cif[i+1].phs[0], cif[i+1].phs[1]);
                        dx[3] = cif[i+1].sts[0];
                        dx[4] = cif[i+1].sts[1];
                    }
                    else // i+1 只有一个phoneme 
                    {
                        // 后面的syllable 只有一个phoneme 
                        // 是否有i+2个syllable 存在 并且 i+1处 非IP边界  
                        if(i+2 < sNum &&  cif[i+1].CharInIpPos != cif[i+1].CharInIpNum)
                        {
                            fprintf(fp,"%s=%s@x_x", cif[i+1].phs[0], cif[i+2].phs[0]);
                            dx[3] = cif[i+1].sts[0];
                            dx[4] = cif[i+2].sts[0];

                        }
                        else
                        {
                            fprintf(fp,"nu=nu@x_x");
                        }

                    }
                }
                else
                {
                    fprintf(fp,"nu=nu@x_x");
                }

                // a1 a2 a3 b1 b2 
                fprintf(fp,"/A:%c_%c_%c/B:%c-%c", dx[0], dx[1], dx[2], dx[3], dx[4]);

                fprintf(fp,"-x@x-x&x-x#x-x$x-x!x-");

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




