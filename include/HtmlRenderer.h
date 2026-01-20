#pragma once
#include <string>
#include <cwctype>
#include "SDK_Wrapper.h"

namespace Html {

    // "Outline 4" 같은 문자열에서 숫자만 뽑아내기
    // 매칭되면 level(1~10)을 리턴, 아니면 0 리턴
    inline int ExtractOutlineLevel(const std::wstring& engName)
    {
        const std::wstring prefix = L"Outline ";
        if (engName.rfind(prefix, 0) != 0) return 0; // 시작이 "Outline "이 아니면 탈락

        // 뒤에 숫자 파싱
        int level = 0;
        for (size_t i = prefix.size(); i < engName.size(); ++i)
        {
            if (!iswdigit(engName[i])) break;
            level = level * 10 + (engName[i] - L'0');
        }
        return level;
    }

    // Outline → HTML 태그 매핑
    inline std::wstring MapEngNameToTag(const std::wstring& engName)
    {
        const int level = ExtractOutlineLevel(engName);

        // Outline 1~6만 h1~h6
        if (level >= 1 && level <= 6) {
            return L"h" + std::to_wstring(level);
        }

        // Outline 7~10은 p로 처리
        return L"p";
    }

    // class명을 우리가 원하는 방식으로 정리
    // 예: "Outline 4" -> "outline-4"
    inline std::wstring NormalizeClassName(const std::wstring& engName)
    {
        const int level = ExtractOutlineLevel(engName);
        if (level >= 1 && level <= 10) {
            return L"outline-" + std::to_wstring(level);
        }

        // Outline 아니면 그대로 유지 (Normal 같은거)
        return engName;
    }

    // 의미 있는 텍스트가 있는지 체크 (공백/개행만 있는 문단 제거)
    inline bool HasMeaningfulText(const std::wstring& s)
    {
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
        (void)out;
        const unsigned int id = SDK::GetID(obj);

        if (id == ID_PARA_PType)
        {
            auto* para = (OWPML::CPType*)obj;

            const unsigned int styleID = SDK::GetParaStyleID(para);
            const std::wstring engName = SDK::GetStyleEngName(styleID);

            // 태그 결정 (Outline 1~6은 h1~h6, Outline 7~10은 p)
            const std::wstring tag = MapEngNameToTag(engName);

            // class명 정규화 (Outline 4 -> outline-4)
            const std::wstring cls = NormalizeClassName(engName);

            InPara() = true;
            ParaTag() = tag;
            ParaClass() = cls;
            ParaBuffer().clear();
        }
    }

    // 2) 텍스트 처리: out 말고 ParaBuffer에 모은다
    inline void OnProcess(OWPML::CObject* obj, std::wstring& out)
    {
        (void)out;
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
            if (HasMeaningfulText(ParaBuffer()))
            {
                out += L"<" + ParaTag() + L" class=\"" + ParaClass() + L"\">";
                out += ParaBuffer();
                out += L"</" + ParaTag() + L">\n";
            }

            InPara() = false;
            ParaTag().clear();
            ParaClass().clear();
            ParaBuffer().clear();
        }
    }

} // namespace Html
