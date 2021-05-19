#pragma once
#include <vector>
#include <string>

struct _QUERY_STR
{
	std::wstring url;
	std::vector<std::pair<std::wstring, std::wstring>> tag;
};

class CHttpRequest
{
public:
	CHttpRequest(const _QUERY_STR& query);
	~CHttpRequest();
private:
	_QUERY_STR m_query;
public:
	std::tuple<std::vector<std::pair<std::wstring, std::wstring>>, std::wstring> SendQuery();
private:
	std::wstring Utf8ToUnicode(const std::wstring& wstr_utf8);
	std::vector<std::pair<std::wstring, std::wstring>> xmlParcer(const std::wstring str);
};




