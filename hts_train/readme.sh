




===========================================================================

hts_train       
    4.train.bak.8000.auto: 副本 
        从 4.train.8000.auto 拷贝过来的备份流程 
    修改 data 下:
            Makefile 中的 /home/szm/tts/train_hts_model/4.train.8000.auto
            labels/gen 中的 277*.lab 替换

    scripts/Config.pm 中的:
            /home/szm/tts/train_hts_model/4.train.8000.auto

    data 下 运行： make mlf , make list ,make scp

===========================================================================
