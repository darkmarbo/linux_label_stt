#!/bin/sh

## 原始的 
###python creat_hed.py  monof.eng  qsetf.eng  questions_qst001.hed.eng.test

#### 所有大写 转换成 小写 
#cat qsetf.eng | tr 'A-Z' 'a-z' > qsetf.eng.low
#cat monof.eng | tr 'A-Z' 'a-z' > monof.eng.low
python creat_hed_low.py  monof.eng.low  qsetf.eng.low  questions_qst001.hed.eng.test.low



