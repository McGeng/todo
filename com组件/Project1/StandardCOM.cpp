// StandardCOM.cpp - 标准 COM 组件实现
#include "StandardCOM.h"
#include <iostream>

// ========================================
// Calculator 实现
// ========================================

Calculator::Calculator() : m_refCount(1)  // 初始引用计数为 1
{
    std::cout << "[Calculator] 对象创建, RefCount = " << m_refCount << std::endl;
}

Calculator::~Calculator()
{
    std::cout << "[Calculator] 对象销毁" << std::endl;
}

HRESULT __stdcall Calculator::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;  // 参数检查
    *ppvObject = nullptr;

    if (riid == IID_IUnknown)  // 请求 IUnknown
    {
        *ppvObject = static_cast<ICalculator*>(this);
        std::cout << "[Calculator] QueryInterface -> IUnknown" << std::endl;
    }
    else if (riid == IID_ICalculator)  // 请求 ICalculator
    {
        *ppvObject = static_cast<ICalculator*>(this);
        std::cout << "[Calculator] QueryInterface -> ICalculator" << std::endl;
    }
    else  // 不支持的接口
    {
        std::cout << "[Calculator] QueryInterface -> E_NOINTERFACE" << std::endl;
        return E_NOINTERFACE;
    }

    AddRef();  // 成功返回接口，增加引用计数
    return S_OK;
}

ULONG __stdcall Calculator::AddRef()
{
    m_refCount++;
    std::cout << "[Calculator] AddRef, RefCount = " << m_refCount << std::endl;
    return m_refCount;
}

ULONG __stdcall Calculator::Release()
{
    m_refCount--;
    std::cout << "[Calculator] Release, RefCount = " << m_refCount << std::endl;

    if (m_refCount == 0)  // 引用计数为 0，销毁对象
    {
        delete this;
        return 0;
    }
    return m_refCount;
}

HRESULT __stdcall Calculator::Add(int a, int b, int* result)
{
    if (!result) return E_POINTER;  // 参数检查
    *result = a + b;
    std::cout << "[Calculator] Add: " << a << " + " << b << " = " << *result << std::endl;
    return S_OK;
}

HRESULT __stdcall Calculator::Subtract(int a, int b, int* result)
{
    if (!result) return E_POINTER;
    *result = a - b;
    std::cout << "[Calculator] Subtract: " << a << " - " << b << " = " << *result << std::endl;
    return S_OK;
}

HRESULT __stdcall Calculator::Multiply(int a, int b, int* result)
{
    if (!result) return E_POINTER;
    *result = a * b;
    std::cout << "[Calculator] Multiply: " << a << " * " << b << " = " << *result << std::endl;
    return S_OK;
}

HRESULT __stdcall Calculator::Divide(int a, int b, int* result)
{
    if (!result) return E_POINTER;
    if (b == 0) return E_INVALIDARG;  // 除数为 0
    *result = a / b;
    std::cout << "[Calculator] Divide: " << a << " / " << b << " = " << *result << std::endl;
    return S_OK;
}


// ========================================
// CalculatorFactory 实现
// ========================================

CalculatorFactory::CalculatorFactory() : m_refCount(1)
{
    std::cout << "[Factory] 工厂创建, RefCount = " << m_refCount << std::endl;
}

CalculatorFactory::~CalculatorFactory()
{
    std::cout << "[Factory] 工厂销毁" << std::endl;
}

HRESULT __stdcall CalculatorFactory::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;

    if (riid == IID_IUnknown)  // 请求 IUnknown
    {
        *ppvObject = static_cast<IClassFactory*>(this);
        std::cout << "[Factory] QueryInterface -> IUnknown" << std::endl;
    }
    else if (riid == IID_IClassFactory)  // 请求 IClassFactory
    {
        *ppvObject = static_cast<IClassFactory*>(this);
        std::cout << "[Factory] QueryInterface -> IClassFactory" << std::endl;
    }
    else
    {
        std::cout << "[Factory] QueryInterface -> E_NOINTERFACE" << std::endl;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

ULONG __stdcall CalculatorFactory::AddRef()
{
    m_refCount++;
    std::cout << "[Factory] AddRef, RefCount = " << m_refCount << std::endl;
    return m_refCount;
}

ULONG __stdcall CalculatorFactory::Release()
{
    m_refCount--;
    std::cout << "[Factory] Release, RefCount = " << m_refCount << std::endl;

    if (m_refCount == 0)
    {
        delete this;
        return 0;
    }
    return m_refCount;
}

HRESULT __stdcall CalculatorFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
    std::cout << "\n[Factory] CreateInstance 开始..." << std::endl;

    if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;  // 不支持聚合
    if (!ppvObject) return E_POINTER;

    *ppvObject = nullptr;

    Calculator* pCalc = new Calculator();  // 创建 Calculator 对象
    if (!pCalc) return E_OUTOFMEMORY;

    HRESULT hr = pCalc->QueryInterface(riid, ppvObject);  // 获取请求的接口
    pCalc->Release();  // 释放初始引用（QueryInterface 已经 AddRef）

    std::cout << "[Factory] CreateInstance 完成\n" << std::endl;
    return hr;
}

HRESULT __stdcall CalculatorFactory::LockServer(BOOL fLock)
{
    // 在真实的 COM DLL 中，这里会增加/减少全局锁计数
    // 防止 DLL 在使用时被卸载
    std::cout << "[Factory] LockServer: " << (fLock ? "LOCK" : "UNLOCK") << std::endl;
    return S_OK;
}


// ========================================
// 全局函数：模拟 DllGetClassObject
// ========================================

HRESULT DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    std::cout << "\n[DllGetClassObject] 请求类工厂..." << std::endl;

    if (rclsid != CLSID_Calculator) return CLASS_E_CLASSNOTAVAILABLE;  // 不支持的 CLSID
    if (!ppv) return E_POINTER;

    *ppv = nullptr;

    CalculatorFactory* pFactory = new CalculatorFactory();  // 创建类工厂
    if (!pFactory) return E_OUTOFMEMORY;

    HRESULT hr = pFactory->QueryInterface(riid, ppv);  // 获取工厂接口
    pFactory->Release();  // 释放初始引用

    std::cout << "[DllGetClassObject] 返回类工厂\n" << std::endl;
    return hr;
}
