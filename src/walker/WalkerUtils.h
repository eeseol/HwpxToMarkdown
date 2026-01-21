#pragma once

#include <vector>

namespace OWPML {
    class CObject;
}

namespace WalkerUtils
{
    // parent의 직계 child 중 targetId와 같은 ID를 가진 첫 번째 child 반환 (없으면 nullptr)
    OWPML::CObject* FindFirstChildById(OWPML::CObject* parent, unsigned int targetId);

    // parent의 직계 child 중 targetId와 같은 ID를 가진 것들 중 n번째(0-based) 반환 (없으면 nullptr)
    OWPML::CObject* FindNthChildById(OWPML::CObject* parent, unsigned int targetId, int n);

    // parent의 직계 child 중 targetId와 같은 ID를 가진 child들을 outVec에 수집
    void CollectChildrenById(OWPML::CObject* parent, unsigned int targetId, std::vector<OWPML::CObject*>& outVec);

    // 직계 child 개수 (Objectlist 기반)
    int CountChildren(OWPML::CObject* parent);
}
