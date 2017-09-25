#!/bin/sh

dir_out=tts_pipeline
rm -rf ${dir_out}   &&  cp -r tts_pipeline.ori  ${dir_out}

cp -r hts_train  extract_feature  front_pro  interval_pro  TtsLabel/Ttslabel_bin  ${dir_out} 

cd ${dir_out}


cd -



