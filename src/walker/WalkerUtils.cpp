#include "walker/WalkerUtils.h"

#include "sdk/OwpmSDKPrelude.h"
#include "sdk/SDK_Wrapper.h"

namespace WalkerUtils
{
    OWPML::CObject* FindFirstChildById(OWPML::CObject* parent, unsigned int targetId)
    {
        if (!parent) return nullptr;

        auto list = parent->GetObjectList();
        if (!list) return nullptr;

        for (auto* ch : *list)
        {
            if (!ch) continue;
            if (SDK::GetID(ch) == targetId)
                return ch;
        }
        return nullptr;
    }

    OWPML::CObject* WalkerUtils::FindNthChildById(OWPML::CObject* parent, unsigned int targetId, int n)
    {
        if (!parent) return nullptr;
        if (n < 0) return nullptr;

        auto list = parent->GetObjectList();
        if (!list) return nullptr;

        int hit = 0;
        for (auto* ch : *list)
        {
            if (!ch) continue;
            if (SDK::GetID(ch) != targetId) continue;

            if (hit == n)
                return ch;

            hit++;
        }

        return nullptr;
    }


    void CollectChildrenById(OWPML::CObject* parent, unsigned int targetId, std::vector<OWPML::CObject*>& outVec)
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

    int CountChildren(OWPML::CObject* parent)
    {
        if (!parent) return 0;

        int cnt = 0;
        auto list = parent->GetObjectList();
        if (!list) return 0;

        for (auto* ch : *list)
        {
            (void)ch;
            cnt++;
        }
        return cnt;
    }
}
