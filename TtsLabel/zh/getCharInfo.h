#ifndef _TTS_GETCHARINFO_H_
#define _TTS_GETCHARINFO_H_

typedef struct _PinyinInfo{
    char pinyin[8];
    char shengmu[6];
    char yunmu[6];
}PinyinInfo;

typedef struct _TtsLabelCharInfo{
    char pinyin[8];
    char shengmu[6];
    char yunmu[6];
    short yindiao; //1-5
    short CharInPwPos;
    short CharInPpPos;
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

short getPinyinInfoID(char *pinyin);
void getShengYun(char *pinyin,char *shengmu,char *yunmu,short *yindiao);
void TtsLabel_ObtainLabelCharSeq(TtsLabelCharInfo * cif,char **pinyinSeq,short sNum,short * tag);
void TTS_Label_Init();
void PrintLabel(TtsLabelCharInfo * cif,short sNum,char *fname);

#endif
