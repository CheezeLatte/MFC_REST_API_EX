// CDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFC Casablanca Ex.h"
#include "CDlg.h"
#include "afxdialogex.h"
#include "CHttpRequest.h"
#include <iostream>
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 

using namespace std;

BOOL CDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    printf("SDfasdf");
    // http request 쿼리 구조체 생성 (url, key 등)
    _QUERY_STR query;
    query.url = L"http://openapi.data.go.kr/openapi/service/rest/Covid19/getCovid19SidoInfStateJson?";

    query.tag.push_back(std::make_pair(L"serviceKey", L"d2RNi6TmD97pfklG3wnzZeI9NLYazc2vKYhRrUAdQt7Qli1Kgk8BrbTX6H%2BaFyYjA7iT82g9iCyBdBW1TELizw%3D%3D"));
    query.tag.push_back(std::make_pair(L"pageNo", L"1"));
    query.tag.push_back(std::make_pair(L"numOfRows", L"10"));
    query.tag.push_back(std::make_pair(L"startCreateDt", L"20200830"));
    query.tag.push_back(std::make_pair(L"endCreateDt", L"20200830"));

    
    // Http request 클래스 생성
    CHttpRequest request(query);
    // query 전송 후 리턴 (tuple type, C++17 compile ON)
    auto [xml, str] = request.SendQuery();

    printf("결과 : Hello, world!\n");

    // 리스트 컨트롤 값 추가
    int row = xml.size();
    int col = 2;

    CRect rect;
    GetClientRect(&rect);
    m_list.InsertColumn(0, _T("Tag"), LVCFMT_CENTER, int(rect.Width() * 0.5));
    m_list.InsertColumn(1, _T("Value"), LVCFMT_CENTER, int(rect.Width() * 0.5));

    m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES);

    int r = 0;
    for (auto itr = xml.begin(); itr != xml.end(); ++itr)
    {
        m_list.InsertItem(r, itr->first.c_str());
        m_list.SetItem(r, 1, LVIF_TEXT, itr->second.c_str(), 0, 0, 0, NULL);
        r++;
    }

    return TRUE;
}



