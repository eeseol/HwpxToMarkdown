#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "SDK_Wrapper.h"
#include "HtmlRenderer.h"

// forward decl
inline void ExtractTextImpl(OWPML::CObject* object, std::wstring& out, int depth);

namespace WalkConfig
{
    // ====== 개발 모드 토글 ======
    static constexpr bool SCAN_MODE = false;      // ID once 스캔
    static constexpr bool DUMP_MODE = false;      // 서브트리 덤프
    static constexpr bool TABLE_LOG = false;      // 테이블 렌더 로그

    // ====== 5단계(표 루트 확정) ID ======
    static constexpr unsigned int TABLE_ROOT_ID = 805306373;   // 진짜 테이블 루트
    static constexpr unsigned int COL_ID = 805306463;          // 열(컬럼) 묶음
    static constexpr unsigned int CELL_WRAPPER_ID = 805306465; // 열 내부에서 row index 역할
    static constexpr unsigned int CELL_CONTENT_ID = 805306406; // 셀 실제 컨텐츠(문단 들어있음)

    // ====== 덤프 관련(필요하면 다시 켜서 사용) ======
    static constexpr unsigned int TARGET_ID = TABLE_ROOT_ID;
    static constexpr int DUMP_MAX_REL_DEPTH = 12;

    // ====== 안전장치 ======
    static constexpr int MAX_DEPTH = 5000;

    inline std::map<unsigned int, int>& SeenIdOnce()
    {
        static std::map<unsigned int, int> m;
        return m;
    }

    inline int CountChildrenByObjectList(OWPML::CObject* obj)
    {
        if (!obj) return 0;
        int childCount = 0;
        if (auto list = obj->GetObjectList())
        {
            for (auto* _ : *list) { (void)_; childCount++; }
        }
        return childCount;
    }

    inline void ScanLogOnce(int depth, OWPML::CObject* obj)
    {
        if (!SCAN_MODE || !obj) return;

        const unsigned int id = SDK::GetID(obj);
        int& cnt = SeenIdOnce()[id];
        cnt++;

        if (cnt == 1)
        {
            const int childCount = CountChildrenByObjectList(obj);

            std::wcout
                << L"[SCAN] depth=" << depth
                << L" id=" << id
                << L" childCount=" << childCount
                << L"\n";
        }
    }

    inline const wchar_t* KnownIdName(unsigned int id)
    {
        switch (id)
        {
        case ID_PARA_PType: return L"ID_PARA_PType(Paragraph)";
        case ID_PARA_T: return L"ID_PARA_T(TextRun)";
        case ID_PARA_Char: return L"ID_PARA_Char(Char)";
        case ID_PARA_LineSeg: return L"ID_PARA_LineSeg(LineSeg)";
        case ID_PARA_LineBreak: return L"ID_PARA_LineBreak(LineBreak)";
        default: return L"";
        }
    }

    inline void DumpSubtree(OWPML::CObject* root, int absDepth, int relDepth)
    {
        if (!root) return;
        if (relDepth > DUMP_MAX_REL_DEPTH) return;

        const unsigned int id = SDK::GetID(root);
        const int childCount = CountChildrenByObjectList(root);
        const wchar_t* name = KnownIdName(id);

        for (int i = 0; i < relDepth; ++i) std::wcout << L"  ";

        std::wcout
            << L"[DUMP] absDepth=" << absDepth
            << L" rel=" << relDepth
            << L" id=" << id
            << L" childCount=" << childCount;

        if (name && name[0] != 0)
            std::wcout << L"  (" << name << L")";

        std::wcout << L"\n";

        auto list = root->GetObjectList();
        if (!list) return;

        for (auto* child : *list)
        {
            if (!child) continue;
            DumpSubtree(child, absDepth + 1, relDepth + 1);
        }
    }

    inline OWPML::CObject* FindFirstChildById(OWPML::CObject* parent, unsigned int targetId)
    {
        if (!parent) return nullptr;
        auto list = parent->GetObjectList();
        if (!list) return nullptr;

        for (auto* ch : *list)
        {
            if (!ch) continue;
            if (SDK::GetID(ch) == targetId) return ch;
        }
        return nullptr;
    }

    inline void CollectChildrenById(OWPML::CObject* parent, unsigned int targetId, std::vector<OWPML::CObject*>& outVec)
    {
        outVec.clear();
        if (!parent) return;

        auto list = parent->GetObjectList();
        if (!list) return;

        for (auto* ch : *list)
        {
            if (!ch) continue;
            if (SDK::GetID(ch) == targetId)
                outVec.push_back(ch);
        }
    }
}

// ======================================
// 5-3: Table Root 전용 처리(실제 HTML 출력)
// - column-major -> row-major로 재조합
// ======================================
inline void RenderTableFromRoot373(OWPML::CObject* tableRoot, std::wstring& out, int depth)
{
    if (!tableRoot) return;

    const unsigned int rid = SDK::GetID(tableRoot);
    if (rid != WalkConfig::TABLE_ROOT_ID) return;

    // 1) rowGroups 모으기 (기존 columns)
    std::vector<OWPML::CObject*> rowGroups;
    WalkConfig::CollectChildrenById(tableRoot, WalkConfig::COL_ID, rowGroups);

    const int rowCount = (int)rowGroups.size();
    if (rowCount <= 0) return;

    // 2) 각 rowGroup에서 cellWrapper 수집
    std::vector<std::vector<OWPML::CObject*>> rowCells(rowCount);
    int maxColCount = 0;

    for (int r = 0; r < rowCount; ++r)
    {
        WalkConfig::CollectChildrenById(rowGroups[r], WalkConfig::CELL_WRAPPER_ID, rowCells[r]);
        maxColCount = std::max(maxColCount, (int)rowCells[r].size());
    }

    // 3) HTML 출력: row 먼저, 그 안에 col
    out += L"<table>\n";

    for (int r = 0; r < rowCount; ++r)
    {
        out += L"<tr>\n";

        for (int c = 0; c < maxColCount; ++c)
        {
            out += L"<td>\n";

            if (c < (int)rowCells[r].size())
            {
                OWPML::CObject* cellWrapper = rowCells[r][c];
                OWPML::CObject* content = WalkConfig::FindFirstChildById(cellWrapper, WalkConfig::CELL_CONTENT_ID);

                if (content)
                    ExtractTextImpl(content, out, depth + 1);
                else
                    ExtractTextImpl(cellWrapper, out, depth + 1);
            }

            out += L"</td>\n";
        }

        out += L"</tr>\n";
    }

    out += L"</table>\n";
}

// ================================
// 재귀 본체
// ================================
inline void ExtractTextImpl(OWPML::CObject* object, std::wstring& out, int depth)
{
    if (!object) return;

    if (depth > WalkConfig::MAX_DEPTH)
    {
        std::wcout << L"[WARN] Max depth exceeded. Stop recursion.\n";
        return;
    }

    OWPML::Objectlist* childList = object->GetObjectList();
    if (!childList) return;

    for (OWPML::CObject* child : *childList)
    {
        if (!child) continue;

        WalkConfig::ScanLogOnce(depth, child);

        const unsigned int id = SDK::GetID(child);

        // 덤프 모드(필요하면 켜서 사용)
        if (WalkConfig::DUMP_MODE && id == WalkConfig::TARGET_ID)
        {
            std::wcout << L"\n========== TARGET SUBTREE DUMP START ==========\n";
            std::wcout << L"TARGET_ID=" << WalkConfig::TARGET_ID << L"\n\n";
            WalkConfig::DumpSubtree(child, depth, 0);
            std::wcout << L"========== TARGET SUBTREE DUMP END ==========\n\n";
            return;
        }

        switch (id)
        {
            // ====== 5-3 핵심: 805306373은 무조건 전용 렌더로 처리 ======
        case WalkConfig::TABLE_ROOT_ID:
        {
            RenderTableFromRoot373(child, out, depth);
            break;
        }

        // ====== Paragraph/Text ======
        case ID_PARA_PType:
        {
            Html::BeginParagraph((OWPML::CPType*)child);
            ExtractTextImpl(child, out, depth + 1);
            Html::EndParagraph(out);
            break;
        }
        case ID_PARA_T:
        {
            Html::ProcessText((OWPML::CT*)child);
            break;
        }
        case ID_PARA_LineSeg:
        {
            Html::ProcessLineSeg();
            break;
        }
        default:
            ExtractTextImpl(child, out, depth + 1);
            break;
        }
    }
}

// 기존 호출부 유지
inline void ExtractText(OWPML::CObject* object, std::wstring& out)
{
    ExtractTextImpl(object, out, 0);
}
