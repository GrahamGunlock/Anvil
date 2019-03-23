#include "VMTableHook.h"

#include <memory>

namespace Tools
{
    VirtualMethodTableHook::VirtualMethodTableHook(void * pObjToBeHooked) : m_pObj((ppVMT_t)pObjToBeHooked)
    {
        if (pObjToBeHooked == nullptr)
            return;

        m_OriginalVmt = *m_pObj;

        m_VirtualFunctionCount = CountVirtualFunctions();

        m_NewVmt = new void*[m_VirtualFunctionCount + 1]();
        m_pHookedFunctions = new byte[m_VirtualFunctionCount]();

        std::memcpy(m_NewVmt, m_OriginalVmt, (m_VirtualFunctionCount + 1) * sizeof(void*));

        *m_pObj = m_NewVmt;
    }

    VirtualMethodTableHook::~VirtualMethodTableHook()
    {
        if (m_NewVmt == nullptr)
            return;

        *m_pObj = m_OriginalVmt;
        delete[] m_NewVmt;
        delete[] m_pHookedFunctions;
    }

    bool VirtualMethodTableHook::Init(void * pObjToBeHooked)
    {
        if (m_NewVmt == nullptr)
        {
            m_pObj = (ppVMT_t)pObjToBeHooked;
            m_OriginalVmt = *m_pObj;

            m_VirtualFunctionCount = CountVirtualFunctions();

            m_NewVmt = new void*[m_VirtualFunctionCount + 1]();
            m_pHookedFunctions = new byte[m_VirtualFunctionCount]();

            std::memcpy(m_NewVmt, m_OriginalVmt, (m_VirtualFunctionCount + 1) * sizeof(void*));

            *m_pObj = m_NewVmt;
            return true;
        }
        return false;
    }

    int VirtualMethodTableHook::CountVirtualFunctions(void) const
    {
        if (m_OriginalVmt == nullptr)
            return -1;

        int VirtualFunctionCount = 0;
        while (m_OriginalVmt[VirtualFunctionCount])
        {
            VirtualFunctionCount++;
        }
        return VirtualFunctionCount;
    }

    void* VirtualMethodTableHook::GetOriginal(int Index) const
    {
        return m_OriginalVmt[Index];
    }

    bool VirtualMethodTableHook::Hook(void * NewFunctionPointer, int Index)
    {
        if (m_pHookedFunctions[Index] != 0 || Index > m_VirtualFunctionCount || Index < 0)
            return false;

        m_pHookedFunctions[Index] = 1;
        m_NewVmt[Index] = NewFunctionPointer;
        return true;
    }

    bool VirtualMethodTableHook::Unhook(int Index)
    {
        if (m_pHookedFunctions[Index] != 1 || Index > m_VirtualFunctionCount || Index < 0)
            return false;

        m_pHookedFunctions[Index] = 0;
        m_NewVmt[Index] = m_OriginalVmt[Index];
        return true;
    }

    void VirtualMethodTableHook::UnhookAll(void)
    {
        for (int Index = 0; Index < m_VirtualFunctionCount; Index++)
        {
            if (m_pHookedFunctions[Index] != 0)
                Unhook(Index);
            continue;
        }
    }
}
