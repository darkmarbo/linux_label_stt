
// g_vad  g_dynamic_match  
//
#include"common.h"

string&  trim(string& str)
{
	if(str.empty())
	{
		return str;
	}

	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ")+1);

	return str;
}

/*
 * 使用字符dem 进行切分 
 * 切分出的block>0 才插入 
 * */
void split(vector<string>& res, const string& input, const char dem)
{
	size_t pos = input.find(dem);
	size_t beg = 0;
	while(pos != string::npos)
	{
		string w = input.substr(beg, pos-beg);
		if(w.size() > 0)
		{
			res.push_back(w);
		}
		beg = pos+1;
		pos = input.find(dem, beg);
	}
	string w = input.substr(beg, pos-beg);
	if(w.size() > 0)
	{
		res.push_back(w);
	}
}

/*
 * 使用 字符串dem 进行切分 
 * 切分出的 block>0 才插入 
 * */
void split(vector<string>& res, const string& input, const string& dem)
{
	size_t pos = input.find(dem);
	size_t beg = 0;
	while(pos != string::npos)
	{
		string w = input.substr(beg, pos-beg);
		if(w.size() > 0)
		{
			res.push_back(w);
		}
		beg = pos+dem.length();
		pos = input.find(dem, beg);
	}
	string w = input.substr(beg, pos-beg);
	if(w.size() > 0)
	{
		res.push_back(w);
	}
}



