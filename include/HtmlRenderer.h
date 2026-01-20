#pragma once
#include <string>
#include <cwctype>
#include "SDK_Wrapper.h"

namespace Html {

    // Outline → HTML 태그 매핑
    inline std::wstring MapEngNameToTag(const std::wstring& engName)
    {
        if (engName == L"Outline 1") return L"h1";
        if (engName == L"Outline 2") return L"h2";
        if (engName == L"Outline 3") return L"h3";
        if (engName == L"Outline 4") return L"h4";
        if (engName == L"Outline 5") return L"h5";
        if (engName == L"Outline 6") return L"h6";   // 추가
        return L"p";
    }

    // 의미 있는 텍스트가 있는지 체크 (공백/개행만 있는 문단 제거)
    inline bool HasMeaningfulText(const std::wstring& s)
    {
        // 1) 그냥 공백/탭/개행만 있으면 빈 문단
        for (wchar_t ch : s) {
            if (!iswspace(ch)) return true;
        }
        return false;
    }

    // 문단 상태 저장용 (간단하게 static으로 처리)
    inline bool& InPara()
    {
        static bool inPara = false;
        return inPara;
    }

    inline std::wstring& ParaTag()
    {
        static std::wstring tag;
        return tag;
    }

    inline std::wstring& ParaClass()
    {
        static std::wstring cls;
        return cls;
    }

    inline std::wstring& ParaBuffer()
    {
        static std::wstring buf;
        return buf;
    }

    // 1) 문단 들어갈 때: 태그를 "바로 출력하지 말고" 상태만 저장
    inline void OnPreProcess(OWPML::CObject* obj, std::wstring& out)
    {
        (void)out; // out은 여기선 안 씀
        const unsigned int id = SDK::GetID(obj);

        if (id == ID_PARA_PType)
        {
            auto* para = (OWPML::CPType*)obj;

            const unsigned int styleID = SDK::GetParaStyleID(para);
            const std::wstring engName = SDK::GetStyleEngName(styleID);
            const std::wstring tag = MapEngNameToTag(engName);

            InPara() = true;
            ParaTag() = tag;
            ParaClass() = engName;
            ParaBuffer().clear();
        }
    }

    // 2) 텍스트 처리: out 말고 ParaBuffer에 모은다
    inline void OnProcess(OWPML::CObject* obj, std::wstring& out)
    {
        (void)out; // out은 여기선 안 씀
        const unsigned int id = SDK::GetID(obj);

        if (id == ID_PARA_T && InPara())
        {
            auto* text = (OWPML::CT*)obj;
            const int count = SDK::GetChildCount(text);

            for (int i = 0; i < count; ++i)
            {
                OWPML::CObject* child = SDK::GetChildByIndex(text, i);
                if (!child) continue;

                const unsigned int childID = SDK::GetID(child);

                if (childID == ID_PARA_Char)
                {
                    ParaBuffer() += SDK::GetCharValue((OWPML::CChar*)child);
                }
                else if (childID == ID_PARA_LineBreak)
                {
                    ParaBuffer() += L"<br/>";
                }
            }
        }
    }

    // 3) 문단 끝: ParaBuffer 보고 비었으면 출력 안 함
    inline void OnPostProcess(OWPML::CObject* obj, std::wstring& out)
    {
        const unsigned int id = SDK::GetID(obj);

        if (id == ID_PARA_PType && InPara())
        {
            // 빈 문단이면 출력 스킵
            if (HasMeaningfulText(ParaBuffer()))
            {
                out += L"<" + ParaTag() + L" class=\"" + ParaClass() + L"\">";
                out += ParaBuffer();
                out += L"</" + ParaTag() + L">\n";
            }

            // 문단 상태 종료
            InPara() = false;
            ParaTag().clear();
            ParaClass().clear();
            ParaBuffer().clear();
        }
    }

} // namespace Html
