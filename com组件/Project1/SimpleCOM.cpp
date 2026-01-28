// SimpleCOM.cpp - COM 组件的具体实现
// =====================================================

#include "SimpleCOM.h"  // 包含我们定义的接口和类声明
#include <iostream>     // 用于输出调试信息


// =====================================================
// 构造函数：创建 COM 对象时被调用
// =====================================================
SimpleCalculator::SimpleCalculator()
{
    // 初始化引用计数为 1
    // 为什么是 1？因为创建对象的人已经持有了一个引用
    m_refCount = 1;

    // 输出调试信息，帮助理解对象的生命周期
    std::cout << "[COM] SimpleCalculator 对象被创建，引用计数 = " << m_refCount << std::endl;
}


// =====================================================
// 析构函数：销毁 COM 对象时被调用
// =====================================================
SimpleCalculator::~SimpleCalculator()
{
    // 输出调试信息
    std::cout << "[COM] SimpleCalculator 对象被销毁" << std::endl;
}


// =====================================================
// QueryInterface：查询接口
// 这是 COM 的核心机制之一
// =====================================================
HRESULT __stdcall SimpleCalculator::QueryInterface(REFIID riid, void** ppvObject)
{
    // 首先检查输出参数是否有效
    if (ppvObject == nullptr)
    {
        // 如果传入的指针是空的，返回错误
        return E_POINTER;
    }

    // 初始化输出参数为 nullptr
    *ppvObject = nullptr;

    // 检查客户端请求的是哪个接口

    // 情况1：客户端请求 IUnknown 接口
    // 所有 COM 对象都支持 IUnknown
    if (riid == IID_IUnknown)
    {
        // 将 this 指针转换为 IUnknown* 类型
        // 注意：我们转换为 ISimpleCalculator* 再赋值
        // 因为 ISimpleCalculator 继承自 IUnknown
        *ppvObject = static_cast<ISimpleCalculator*>(this);

        std::cout << "[COM] QueryInterface: 返回 IUnknown 接口" << std::endl;
    }
    // 情况2：客户端请求我们自定义的 ISimpleCalculator 接口
    else if (riid == IID_ISimpleCalculator)
    {
        // 将 this 指针转换为 ISimpleCalculator* 类型
        *ppvObject = static_cast<ISimpleCalculator*>(this);

        std::cout << "[COM] QueryInterface: 返回 ISimpleCalculator 接口" << std::endl;
    }
    // 情况3：客户端请求的接口我们不支持
    else
    {
        std::cout << "[COM] QueryInterface: 不支持请求的接口" << std::endl;
        return E_NOINTERFACE;  // 返回"不支持此接口"的错误码
    }

    // 如果成功返回了接口指针，必须增加引用计数
    // 这是 COM 的重要规则：每次给出接口指针都要 AddRef
    AddRef();

    // S_OK 表示操作成功
    return S_OK;
}


// =====================================================
// AddRef：增加引用计数
// =====================================================
ULONG __stdcall SimpleCalculator::AddRef()
{
    // 增加引用计数（原子操作，线程安全）
    // InterlockedIncrement 是 Windows API，保证多线程环境下的安全
    // 但为了简单，我们直接使用 ++
    m_refCount++;

    std::cout << "[COM] AddRef 调用，引用计数 = " << m_refCount << std::endl;

    // 返回新的引用计数值
    return m_refCount;
}


// =====================================================
// Release：减少引用计数
// 这是 COM 对象生命周期管理的关键
// =====================================================
ULONG __stdcall SimpleCalculator::Release()
{
    // 减少引用计数
    m_refCount--;

    std::cout << "[COM] Release 调用，引用计数 = " << m_refCount << std::endl;

    // 如果引用计数降为 0，说明没有人再使用这个对象了
    if (m_refCount == 0)
    {
        std::cout << "[COM] 引用计数为 0，准备销毁对象" << std::endl;

        // 删除自己（调用析构函数）
        delete this;

        // 返回 0 表示对象已被销毁
        return 0;
    }

    // 返回当前引用计数
    return m_refCount;
}


// =====================================================
// Add：我们自定义的业务方法 - 两数相加
// =====================================================
HRESULT __stdcall SimpleCalculator::Add(int a, int b, int* result)
{
    std::cout << "[COM] Add 方法被调用: " << a << " + " << b << std::endl;

    // 检查输出参数是否有效
    if (result == nullptr)
    {
        // 如果结果指针是空的，返回错误
        return E_POINTER;
    }

    // 执行加法运算
    *result = a + b;

    std::cout << "[COM] Add 方法返回结果: " << *result << std::endl;

    // S_OK 表示操作成功
    return S_OK;
}
