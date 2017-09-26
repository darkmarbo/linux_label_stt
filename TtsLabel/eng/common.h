
// g_vad  g_dynamic_match  
//
#ifndef H_COMMON
#define H_COMMON

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

#include<string>
#include<vector>
#include<map>

#include<iostream>
#include <sstream>
#include <fstream>

using namespace std;


/*
 * 删除字符串前后的空格 
 * */
string& trim(string &);

/*
 * 使用 dem(字符) 来切分 输入字符串input
 * 结果存储在 res(vector)中 
 * */
void split(vector<string>& res, const string& input, const char dem = '\t');

/*
 * 使用 dem(字符串) 来切分 输入字符串input
 * 结果存储在 res(vector)中 
 * */
void split(vector<string>& res, const string& input, const string& dem);




#endif
