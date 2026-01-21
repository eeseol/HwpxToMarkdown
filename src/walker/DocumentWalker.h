#pragma once

#include <string>

namespace OWPML {
    class CObject;
}

// 기존 인터페이스 유지
void ExtractText(OWPML::CObject* object, std::wstring& out);
