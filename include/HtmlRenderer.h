#pragma once

#include <string>
#include <cwctype>
#include <map>
#include <iostream>

#include "SDK_Wrapper.h"

namespace Html {

    // Extract level from styles like "Outline 4" -> 4, otherwise 0
    inline int ExtractOutlineLevel(const std::wstring& engName)
    {
        const std::wstring prefix = L"Outline ";
        if (engName.rfind(prefix, 0) != 0) return 0;

        int level = 0;
        for (size_t i = prefix.size(); i < engName.size(); ++i)
        {
            if (!iswdigit(engName[i])) break;
            level = level * 10 + (engName[i] - L'0');
        }
        return level;
    }

    // Outline -> HTML tag mapping
    inline std::wstring MapEngNameToTag(const std::wstring& engName)
    {
        const int level = ExtractOutlineLevel(engName);

        // Outline 1~6 => h1~h6
        if (level >= 1 && level <= 6) {
            return L"h" + std::to_wstring(level);
        }

        // Outline 7~10 (or anything else) => paragraph
        return L"p";
    }

    // ===========================
    // Style log (optional)
    // ===========================
    inline std::map<std::wstring, int>& StyleSeenAll()
    {
        static std::map<std::wstring, int> m;
        return m;
    }

    inline std::map<std::wstring, int>& StyleSeenMapped()
    {
        static std::map<std::wstring, int> m;
        return m;
    }

    inline std::map<std::wstring, int>& StyleSeenUnmapped()
    {
        static std::map<std::wstring, int> m;
        return m;
    }

    inline bool IsMappedEngName(const std::wstring& engName)
    {
        if (engName.rfind(L"Outline ", 0) == 0) return true;
        if (engName == L"Normal") return true;
        if (engName == L"Body") return true;
        return false;
    }

    inline void LogParaStyle(const std::wstring& engName)
    {
        StyleSeenAll()[engName]++;

        if (IsMappedEngName(engName))
            StyleSeenMapped()[engName]++;
        else
            StyleSeenUnmapped()[engName]++;
    }

    inline void DumpStyleLogToConsole()
    {
        std::wcout << L"\n================== STYLE LOG SUMMARY ==================\n";
        std::wcout << L"[ALL] count=" << StyleSeenAll().size() << L"\n";
        std::wcout << L"[MAPPED] count=" << StyleSeenMapped().size() << L"\n";
        std::wcout << L"[UNMAPPED] count=" << StyleSeenUnmapped().size() << L"\n\n";

        std::wcout << L"------------------ UNMAPPED STYLES ------------------\n";
        for (auto& kv : StyleSeenUnmapped())
        {
            std::wcout << L"- " << kv.first << L" : " << kv.second << L"\n";
        }
        std::wcout << L"======================================================\n";
    }

    // class name normalize
    // Example: "Outline 4" -> "outline-4"
    inline std::wstring NormalizeClassName(const std::wstring& engName)
    {
        const int level = ExtractOutlineLevel(engName);
        if (level >= 1 && level <= 10) {
            return L"outline-" + std::to_wstring(level);
        }

        // Keep original for now (Normal, Body, etc.)
        return engName;
    }

    // Check if a string has any non-whitespace
    inline bool HasMeaningfulText(const std::wstring& s)
    {
        for (wchar_t ch : s) {
            if (!iswspace(ch)) return true;
        }
        return false;
    }

    // ===========================
    // Paragraph state (temporary)
    // ===========================
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

    // Called when entering a paragraph
    inline void BeginParagraph(OWPML::CPType* para)
    {
        if (!para) return;

        const unsigned int styleID = SDK::GetParaStyleID(para);
        const std::wstring engName = SDK::GetStyleEngName(styleID);

        LogParaStyle(engName);

        ParaTag() = MapEngNameToTag(engName);
        ParaClass() = NormalizeClassName(engName);

        InPara() = true;
        ParaBuffer().clear();
    }

    // Handle text run (CT) inside a paragraph
    inline void ProcessText(OWPML::CT* text)
    {
        if (!text || !InPara()) return;

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

    // Some documents use LineSeg as a logical line boundary
    inline void ProcessLineSeg()
    {
        if (!InPara()) return;
        ParaBuffer() += L"<br/>";
    }

    // Called when leaving a paragraph
    inline void EndParagraph(std::wstring& out)
    {
        if (!InPara()) return;

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

} // namespace Html
