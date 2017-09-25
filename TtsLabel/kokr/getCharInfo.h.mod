#ifndef _TTS_GETCHARINFO_H_
#define _TTS_GETCHARINFO_H_

#include<string>
#include<map>

using namespace std;

typedef struct _PinyinInfo{
    char pinyin[18];
    char shengmu[6];
    char yunmu[6];
}PinyinInfo;

typedef struct _TtsLabelCharInfo{
    char pinyin[18];

    char ph1[6]; // tt ui n
    char ph2[6]; // tt ui n
    char ph3[6]; // tt ui n

    // phoneme  syllable  word phrase

    // p1^p2-p3+p4=p5@:
    // 当前pheneme和其前后2个。
    
    //  @P6_P7
    // 当前 phoneme 在 syllable 中的位置（左数和右数）。

    // /A:a1_a2_a3
    //short isS;    // 前一个syllable 是否重读？
    //short isA;    //前一个syllable 是否口音？
    short a3_nP;    // 前一个syllable 包含phoneme的个数。

    //// /B:b1-b2-b3:
    //short isS; //  当前syllable 是否重读？
    //short isA; //  当前syllable 是否口音？
    short b3_nP;  //  当前syllable 包含phoneme的个数。

    // @b4-b5&b6-b7#：
    short CharInPwPos; //  当前syllable在当前word的 位置（左数）。
                        //  当前syllable在当前word的 位置（右数）。
    short CharInPpPos; //  当前syllable在当前phrase的 位置（左数）。
                        //  当前syllable在当前phrase的 位置（右数）。

    //// #b8-b9$b10-b11!：
    //short b8 ; //  当前phrase中，前一个syllable中重读的个数。
    //short b9;  //  当前phrase中，后一个syllable中重读的个数。
    //short b10; //  当前phrase中，前一个syllable中口音的个数。
    //short b11; //  当前phrase中，后一个syllable中口音的个数。
    //
    // !b12-b13;b14-b15|：距离用syllable衡量。
    //  前一个重读的syllable到当前syllable的距离。
    //  后一个重读的syllable到当前syllable的距离。
    //  前一个口音的syllable到当前syllable的距离。
    //  后一个口音的syllable到当前syllable的距离。
    //
    // |b16 /C:
    char vowel[6]; // 当前syllable中的vowel。
    //
    // /C:c1+c2+c3：
    //  下一个syllable 是否重读？
    //  下一个syllable 是否口音？
    //  下一个syllable 包含phoneme的个数。
    //
    //  /D:d1_d2：
    //  上一个word的gpos（词性）。
    //  上一个word包含syllable个数。
    //
    //  /E:e1+e2@e3+e4&e5+e6#e7+e8 /F:：
    //  当前word的gpos（词性）。
    short e2_nS;        // 当前word包含syllable个数。
    short e3_posWP1;    //  当前word在当前phrase的位置（左数）
    short e4_posWP2;    //  当前word在当前phrase的位置（右数）
    //  当前phrase中，当前word前面content word的个数（左数位置）？
    //  当前phrase中，当前word后面content word的个数（右数位置）？
    //  上一个content word到当前word的距离？ 0表示无
    //  当前word到下一个content word的距离？0表示无。
    //
    //  /F: f1_f2：
    //  下一个word的gpos（词性）。
    //  下一个word包含syllable个数。
    //
    //  /G:g1_g2：
    //  前一个phrase，包含syllable的个数。
    //  前一个phrase，包含word的个数。
    //
    //  /H:h1=h2^h3=h4|h5 /I:：
    short h1_nS;        //  当前phrase，包含syllable的个数。
    short h2_nW;        //  当前phrase，包含word的个数。
    short h3_posPU1;    // 当前phrase，在当前utterance的位置（左数）。
    short h4_posPU2;    // 当前phrase，在当前utterance的位置（右数）。
    // TOBI endtone of the current phrase（0或者L-L%）
    //  /I:i1=i2/J:
    //  下一个phrase，包含syllable的个数。
    //  下一个phrase，包含word的个数。
    //
    //  /J: j1+ j2-j3：
    short j1_nS;    //  Utterance 包含syllable个数。
    short j2_nW;    //  Utterance 包含word个数。
    short j3_nP;    //  Utterance 包含phrase个数。
    //

    //////////////////////////////////////////////////////////////////////

    char shengmu[6];
    char yunmu[6];
    short yindiao; //1-5

    short CharInIpPos;
    short CharInSentPos;
    short PwInPpPos;
    short PwInIpPos;
    short PwInSentPos;
    short PpInIpPos;
    short PpInSentPos;
    short IpInSentPos;
    short CharInPwNum;
    short CharInPpNum;
    short CharInIpNum;
    short CharInSentNum;
    short PwInPpNum;
    short PwInIpNum;
    short PwInSentNum;
    short PpInIpNum;
    short PpInSentNum;
    short IpInSentNum;
}TtsLabelCharInfo;

/*
 // tt ui n
 * */
short split_str(const char *line, char ** seg, short *nSeg);

/*
 * 按照最大匹配  将syllable 拆分成 phone 
 * */
int split_syll2phone(const string syll, string &phones);

int getPhones(const string syll, string &phones);

int TTS_Label_Init();


short getPinyinInfoID(char *pinyin);
void getShengYun(char *pinyin,char *shengmu,char *yunmu,short *yindiao);
int TtsLabel_ObtainLabelCharSeq(TtsLabelCharInfo * cif,char **pinyinSeq,short sNum,short * tag);
int PrintLabel(TtsLabelCharInfo * cif,short sNum,char *fname);

#endif
