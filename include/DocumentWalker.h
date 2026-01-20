#pragma once
#include <string>
#include "SDK_Wrapper.h"
#include "HtmlRenderer.h"

inline void ExtractText(OWPML::CObject* obj, std::wstring& out)
{
    if (!obj) return;

    Html::OnPreProcess(obj, out);
    Html::OnProcess(obj, out);

    const int n = SDK::GetChildCountObj(obj);
    for (int i = 0; i < n; ++i)
    {
        ExtractText(SDK::GetChildAtObj(obj, i), out);
    }

    Html::OnPostProcess(obj, out);
}
