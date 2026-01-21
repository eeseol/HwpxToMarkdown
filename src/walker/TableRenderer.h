#pragma once

#include <string>

namespace OWPML {
    class CObject;
}

namespace TableRenderer
{
    // DocumentWalker의 내부 재귀 함수를 TableRenderer가 다시 호출해야 해서 콜백을 받는다.
    // (std::function 안 쓰고 함수 포인터로 가볍게)
    using RenderChildFn = void(*)(OWPML::CObject* object, std::wstring& out, int depth);

    // TABLE_ROOT_ID(805306373) 테이블을 HTML <table>로 렌더링
    void RenderTableFromRoot373(
        OWPML::CObject* tableRoot,
        std::wstring& out,
        int depth,
        RenderChildFn renderChild
    );
}
