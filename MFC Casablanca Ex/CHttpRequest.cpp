#include "pch.h"
#include "CHttpRequest.h"
#include "cpprest/http_client.h"
#include "Markup.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;

CHttpRequest::CHttpRequest(const _QUERY_STR& query)
{
    m_query = query;
}

CHttpRequest::~CHttpRequest()
{
}

std::wstring CHttpRequest::Utf8ToUnicode(const std::wstring& wstr_utf8)
{
    // wstring -> string 변환 (현재까지 utf8)
    std::string str_utf8;
    str_utf8.assign(wstr_utf8.begin(), wstr_utf8.end());

    // utf8 -> unicode 변환
    utility::string_t wstr_unicode = utility::conversions::to_string_t(str_utf8);

    return wstr_unicode;
}

std::tuple<std::vector<std::pair<std::wstring, std::wstring>>, std::wstring> CHttpRequest::SendQuery()
{
    http_client client(m_query.url);

    uri_builder builder(U(""));
    //std::vector<std::pair<std::wstring, std::wstring>>::iterator itr;
    auto itr = m_query.tag.begin();
    for (itr = m_query.tag.begin(); itr != m_query.tag.end(); ++itr)
    {
        builder.append_query(itr->first, itr->second, false);
        //std::wcout << itr->first << "\t" << itr->second << std::endl;
    }

    // 쿼리 문자열 생성
    utility::string_t  strQuery = builder.to_string();
    // 쿼리 문자열 전송 및 응답
    http_response response = client.request(methods::GET, strQuery).get();

    // 응답에서 utf8 문자열 얻기
    utility::string_t wstr_utf8 = response.extract_string().get();

    // 콘솔 출력 설정 UTF8 설정
    SetConsoleOutputCP(CP_UTF8);
    std::wcout << wstr_utf8 << std::endl;

    // utf8 -> unicode 변환
    utility::string_t wstr_unicode = Utf8ToUnicode(wstr_utf8);

    // xml parsing
    auto xml = xmlParcer(wstr_utf8);

    return std::make_tuple(xml, wstr_unicode);
}


std::vector<std::pair<std::wstring, std::wstring>> CHttpRequest::xmlParcer(const std::wstring str)
{
    // 파싱된 xml 데이터 저장 벡터
    std::vector<std::pair<std::wstring, std::wstring>> data;

    CMarkup xml;
    if (xml.SetDoc(str))
    {
        xml.Save(L"result.xml");

        if (xml.FindChildElem(L"body"))
        {
            xml.IntoElem();
            if (xml.FindChildElem(L"items"))
            {
                xml.IntoElem();
                while (xml.FindChildElem(L"item"))
                {
                    xml.IntoElem();
                    while (xml.FindChildElem())
                    {
                        std::wstring tag = xml.GetChildTagName();
                        std::wstring val = xml.GetChildData();

                        // utf8 -> unicode 변환
                        utility::string_t wstr_unicode = Utf8ToUnicode(val);

                        // 벡터 저장
                        data.push_back(std::make_pair(tag, wstr_unicode));
                        //std::wcout << tag << "\t" << val << std::endl;
                    }
                    xml.OutOfElem();
                }
            }
        }
    }
    return data;
}