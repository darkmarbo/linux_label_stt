#!/bin/sh

dir_out=tts_pipeline
rm -rf ${dir_out}   &&  cp -r tts_pipeline.ori  tts_pipeline

cp -r extract_feature  front_pro  interval_pro  TtsLabel/Ttslabel_bin  ${dir_out} 

cd ${dir_out}


cd -



