#!/bin/sh

dir_out=tts_pipeline
rm -rf ${dir_out}   &&  cp -r tts_pipeline.ori  ${dir_out}

cp -r hts_train  extract_feature  front_pro   ${dir_out} 
cp -r interval_pro/zh    ${dir_out}/interval_pro 
cp -r TtsLabel/zh/Ttslabel_bin  ${dir_out} 


