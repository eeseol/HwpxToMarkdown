#pragma once

#include <string>

namespace OWPML {
    class CPType;
    class CT;
}

namespace Html {

    // ===========================
    // Cell mode + LineBreak policy
    // ===========================
    enum class CellBreakMode
    {
        Space,    // " "
        Newline,  // "\n"
        BrTag     // "<br/>"
    };

    // CellMode ฐüทร
    void SetCellMode(bool on);
    bool IsCellMode();

    void SetCellBreakMode(CellBreakMode mode);
    void SetCellParagraphMode(CellBreakMode mode);

    // Paragraph lifecycle
    void BeginParagraph(OWPML::CPType* para);
    void ProcessText(OWPML::CT* text);
    void ProcessLineSeg();
    void EndParagraph(std::wstring& out);

    // Document wrapper
    void BeginHtmlDocument(std::wstring& out);
    void EndHtmlDocument(std::wstring& out);

    // Style log
    void DumpStyleLogToConsole();

} // namespace Html
