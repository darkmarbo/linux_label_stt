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

            //// 判断真的phoneme 是否合法 
            //    snprintf(cif[i].phs[j], 10, "%s", new_ph.c_str());
            //    cif[i].sts[j] = sts;
            //    cif[i].acc = acc;

            //// 判断真的phoneme 是否合法 
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
 * 获取cif[i] 对应syllable的 重音情况 
 * 012 无主次 4是辅音  
 * 所以 1和2 表示有重音  return 1 
 * */
int get_stress(TtsLabelCharInfo * cif)
{
    // syllable中的所有 phoneme 
    for(int ii=0; ii<cif->num_ph; ii++)
    {
        char stress = cif->sts[ii];
        if(stress == '1' || stress == '2')
        {
            return 1;
        }
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
    // p1^p2-p3+p4=p5
    // @a1_b1/A:c1_a3_b3/B:c3-p6-p7
    // @e2-b4&b5-h1#b6-b7$h2-e3!e4-j1;j2-j3|h3/C:h4
    //
    //  sil 对应 pau 
    //  nu  对应 xx
    //  x   对应 xx
    //  cif[0].PpInSentNum 和 cif[0].IpInSentNum  韵律短语和语调短语 只保留一个
    //
    // p1-p5    前后5个phoneme  
    //
    // a1   前一个syllable 是否有stress 012（无|主|次）、4（辅音）  x是空
    // b1   当前syllable 是否有stress   现在使用1
    // c1   后一个syllable 是否有stress 
    //
    // a3   前一个syll包含ph个数    
    // b3   当前的syll包含ph个数    
    // c3   下一个syll包含ph个数    
    //
    // p6   Phone在syllable的位置（左） 
    // p7   Phone在syllable的位置（右） 
    //
    // e2   Number：syllables in word   CharInPwNum
    // b4   syllable在word 的位置（左） CharInPwPos
    // b5   syllable在word位置（右）    
    //
    // h1   Number：syllables  in phrase    PwInPpNum
    // b6   syllable在phrase位置（左数）    CharInPpPos
    // b7   syllable在phrase位置（右数）    
    // h2   Number：word  in phrase PwInSentNum
    // e3   word在phrase位置（左数）    PwInPpPos
    // e4   word在phrase位置（右数）    
    //
    // j1   Number syllable  in  utterance  
    // j2   number  words  in  utterance    
    // j3   number  phrases in  utterance   
    // h3   phrase在utterance位置（左数）   PpInSentPos
    // h4   phrase在utterance位置（右数）   


    ////////////////////////////////////////////////////////////////////////////
    // 第一行  根据 cif[0] 对应有几个 phoneme 来写第1行 
    // p1^p2-p3+p4=p5
    if(cif[0].num_ph > 1)
    {
        fprintf(fp,"nu^nu-sil+%s=%s", cif[0].phs[0], cif[0].phs[1]);
    }
    else 
    {
        fprintf(fp,"nu^nu-sil+%s=%s", cif[0].phs[0], cif[1].phs[0]);
    }

    // @a1_b1/A:c1  // c1  前中后 一个syllable 是否有stress 
    fprintf(fp,"@x_x/A:%d", get_stress(&cif[0]));

    // _a3_b3/B:c3  // 前中后 syll包含ph个数
    fprintf(fp,"_x_x/B:%d", cif[0].num_ph);

    // -p6-p7@e2-b4&b5-h1#b6-b7$h2-e3!e4       
    // Phone在syllable的位置（左右）   word中syll个数 左右位置 
    // phrase 中 syll个数 位置 ， phrase中word个数 位置 
    fprintf(fp,"-x-x@x-x&x-x#x-x$x-x!x");

    // -j1;j2-j3|h3/C:h4
    // 句子中 syll个数  word个数 phrase个数
    // phrase在句子中的 左右位置 
    // mod-1
    //fprintf(fp,"-%d;%d-%d|x/C:x", cif[0].CharInSentNum,
    //            cif[0].PwInSentNum, cif[0].IpInSentNum);

    fprintf(fp,"\n");

    ///////////////////////////////////////////////////////////////////////////////
    // p1^p2-p3+p4=p5
    // @a1_b1/A:c1
    // _a3_b3/B:c3-p6-p7
    // @e2-b4&b5-h1#b6-b7$h2-e3!e4-j1;j2-j3|h3/C:h4

    // 第2行: 第1个syllable 的 第1个phoneme 
    // 第3行: 第1个syllable 的 第 2个phone 或者是第2个syllable的第 1个phoneme


    // 每一个cif[i] 
    for(i=0; i<sNum; i++)
    {
        // 当前 syll 的 num_ph = 1 2 3 >>> 
        for(k=1; k <= cif[i].num_ph; k++)
        {

            ////////////////////////////  p1.  前面 -2 个phoneme  
            // 当前syll  只有一个phoneme 
            if(k==1)  
            {
                // 第1个syllable 的 第1个phoneme 
                if(i==0)
                {
                    fprintf(fp,"nu");
                }
                else
                {
                    // 第 2.3.4 个 syllable 的 第1个 phoneme 
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
                        {
                            // 如果前面syllable 只有一个 phoneme  
                            // 没有 i-2个syllable  
                            if(i == 1)
                            {
                                fprintf(fp,"sil");
                            }
                            else
                            {
                                // 需要查看i-2个 syllable的最后一个phoneme 
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
                {
                    // 第2.3.4个syllable 的 第2个phoneme 
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
            else
            {
                // 当前syllable 的 第3个phoneme 
                fprintf(fp,"%s", cif[i].phs[k-3]);

            }

            ////////////////////////////  p2.  前面 -1 个phoneme  
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
            else  
            {
                // 第2.3 个 phoneme  取其前面的phoneme 
                fprintf(fp,"^%s", cif[i].phs[k-2] );
            }



            ////////////////////////////////////////  p3.  当前的phoneme 
            fprintf(fp,"-%s", cif[i].phs[k-1]);

            ////////////////////////////////////////p4  后面的phoneme 
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

            //////////////////////////////  p5.  后面第 2 个phoneme 
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
                {
                    fprintf(fp,"=%s",cif[i+1].phs[0]);
                }
                else
                {
                    fprintf(fp,"=sil");
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
                    }
                    else
                    {
                        fprintf(fp,"=nu");
                    }
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
                    {
                        fprintf(fp,"=%s", cif[i+2].phs[0]);
                    }
                    else
                    {
                        fprintf(fp,"=nu");
                    }
                }
            }

            ///////////////////////////////////////////////////////////
            //// 前中后  是否有stress // 前中后 syll中包含phoneme的个数 
            
            char dx[3];
            char numPhInSyll[3];
            dx[0] = 'x';
            dx[1] = get_stress(&cif[i]) + '0';
            dx[2] = 'x';
            numPhInSyll[0]='x';
            numPhInSyll[1] = cif[i].num_ph + '0';
            numPhInSyll[2]='x';

            // 只要不是第一个 
            if(i > 0)
            {
                dx[0] = get_stress(&cif[i-1]) + '0';
                numPhInSyll[0] = cif[i-1].num_ph + '0';

            }

            // 只要不是最后一个 syllable 
            if(i < sNum-1)
            {
                dx[2] = get_stress(&cif[i+1]) + '0';
                numPhInSyll[2] = cif[i+1].num_ph + '0';
            }

            fprintf(fp,"@%c_%c/A:%c", dx[0], dx[1], dx[2]);
            fprintf(fp,"_%c_%c/B:%c", numPhInSyll[0], numPhInSyll[1], numPhInSyll[2]);

            ////   Phone在syllable的位置
            fprintf(fp,"-%d-%d", k, cif[i].num_ph - k +1);

            ////  word 中 syll个数 左右位置 
            fprintf(fp,"@%d-%d&%d", cif[i].CharInPwNum, cif[i].CharInPwPos,
                        cif[i].CharInPwNum - cif[i].CharInPwPos +1 );

            // phrase 中 syll 个数 左右位置
            // phrase 中 word 个数 左右位置
            fprintf(fp,"-%d#%d-%d$%d-%d!%d", cif[i].CharInIpNum, cif[i].CharInIpPos,
                        cif[i].CharInIpNum - cif[i].CharInIpPos +1,
                        cif[i].PwInIpNum, cif[i].PwInIpPos, 
                        cif[i].PwInIpNum - cif[i].PwInIpPos +1);


            // 句子中 syll word phrase 个数
            // 句子中 phrase的左右位置 
            // mod-1
            //fprintf(fp,"-%d;%d-%d|%d/C:%d", cif[i].CharInSentNum, cif[i].PwInSentNum,
            //            cif[i].IpInSentNum, cif[i].IpInSentPos, 
            //            cif[i].IpInSentNum- cif[i].IpInSentPos+1);

            //line end
            fprintf(fp,"\n");


            /////////////// 当前是sil 语调短语边界处 
            // s^t-pau+dh=ax@1_xx/A:0_5_xx/B:2
            // ay^d-pau+xx=xx@1_xx/A:xx_3_xx/B:xx
            //
            if(cif[i].CharInIpPos == cif[i].CharInIpNum && k == cif[i].num_ph)
            {
                
                ///////////////////////////// p1^p2-p3+p4=p5
                // 其前面 至少有一个 syllable 
                if(cif[i].num_ph > 1)
                {
                    fprintf(fp,"%s^%s-sil+", cif[i].phs[cif[i].num_ph-2], 
                                cif[i].phs[cif[i].num_ph-1] );

                }
                else if(i>0 && cif[i-1].CharInIpPos != cif[i-1].CharInIpNum)
                {
                    //cif[i] 只有一个phoneme 
                    //cif[i-1] 的 syllable 存在 并且非IP边界 
                    fprintf(fp,"%s^%s-sil+", cif[i-1].phs[cif[i].num_ph-1], 
                                cif[i].phs[0] );
                }
                else 
                {
                    // cif[i] 只有一个phoneme  前面是sil 
                    fprintf(fp, "sil^%s-sil+", cif[i].phs[0] );
                }

                dx[0] = get_stress(&cif[i]) + '0';
                dx[1] = 'x';

                // 后面还有 syllable 
                if(i<sNum-1 )
                {
                    dx[3] = get_stress(&cif[i+1]) + '0';
                    // i+1 够用了 
                    if(cif[i+1].num_ph > 1)
                    {
                        fprintf(fp,"%s=%s", cif[i+1].phs[0], cif[i+1].phs[1]);
                    }
                    else // i+1 只有一个phoneme 
                    {
                        // 后面的syllable 只有一个phoneme 
                        // 是否有i+2个syllable 存在 并且 i+1处 非IP边界  
                        if(i+2 < sNum &&  cif[i+1].CharInIpPos != cif[i+1].CharInIpNum)
                        {
                            fprintf(fp,"%s=%s", cif[i+1].phs[0], cif[i+2].phs[0]);

                        }
                        else
                        {
                            fprintf(fp,"nu=nu");
                        }

                    }
                }
                else
                {
                    dx[3] = 'x';
                    fprintf(fp,"nu=nu");
                }

                // @a1_b1/A:c1
                fprintf(fp,"@%c_%c/A:%c", dx[0], dx[1], dx[2]);
                // _a3_b3/B:c3  前中后syll 包含phoneme 个数 
                if(i<sNum-1)
                {
                    fprintf(fp,"_%d_x/B:%d", cif[i].num_ph, cif[i+1].num_ph);
                }
                else
                {
                    fprintf(fp,"_%d_x/B:x", cif[i].num_ph);
                }


                // -p6-p7@e2-b4&b5-h1#b6-b7$h2-e3!e4
                fprintf(fp,"-x-x@x-x&x-x#x-x$x-x!x");

                // mod-1
                // -j1;j2-j3|h3/C:h4
                //fprintf(fp,"-%d;%d-%d|x/C:x", cif[i].CharInSentNum, cif[i].PwInSentNum,
                //            cif[i].IpInSentNum);

                fprintf(fp,"\n");
            }
            ////////////////end   当前是sil  


        }

    }


    fclose(fp);

    return 0;

}




