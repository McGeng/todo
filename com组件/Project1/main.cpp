// main.cpp - COM 组件的使用示例（客户端程序）
// =====================================================

#include "SimpleCOM.h"  // 包含 COM 组件的接口定义
#include <iostream>     // 用于控制台输出
#include <Windows.h>    // Windows API

using namespace std;


// =====================================================
// 工厂函数：创建 COM 对象的实例
// =====================================================
// 在真实的 COM 系统中，对象的创建通常由 CoCreateInstance 完成
// 但为了简化学习，我们用一个简单的工厂函数来创建对象
HRESULT CreateSimpleCalculator(REFIID riid, void** ppvObject)
{
    cout << "\n========================================" << endl;
    cout << "开始创建 COM 对象..." << endl;
    cout << "========================================\n" << endl;

    // 检查输出参数是否有效
    if (ppvObject == nullptr)
    {
        return E_POINTER;
    }

    // 初始化输出参数
    *ppvObject = nullptr;

    // 创建 SimpleCalculator 对象
    // 使用 new 操作符，对象的引用计数初始化为 1
    SimpleCalculator* pCalculator = new SimpleCalculator();

    // 检查对象是否创建成功
    if (pCalculator == nullptr)
    {
        return E_OUTOFMEMORY;  // 内存不足
    }

    // 通过 QueryInterface 获取客户端请求的接口
    // 这里会再次调用 AddRef，所以引用计数会变成 2
    HRESULT hr = pCalculator->QueryInterface(riid, ppvObject);

    // 释放我们持有的引用
    // 因为 QueryInterface 已经增加了引用计数
    // 我们释放自己的引用后，引用计数会回到 1
    pCalculator->Release();

    cout << "\nCOM 对象创建完成\n" << endl;

    // 返回 QueryInterface 的结果
    return hr;
}


// =====================================================
// 主函数：演示如何使用 COM 组件
// =====================================================
int main()
{
    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║     最简单的 COM 组件学习示例                  ║" << endl;
    cout << "║     Simple COM Component Tutorial           ║" << endl;
    cout << "╚══════════════════════════════════════════════╝\n" << endl;

    // =====================================================
    // 步骤 1：声明接口指针
    // =====================================================
    // 定义一个指向 ISimpleCalculator 接口的指针
    // 初始化为 nullptr 是一个好习惯
    ISimpleCalculator* pCalculator = nullptr;

    cout << "步骤 1: 声明接口指针" << endl;


    // =====================================================
    // 步骤 2：创建 COM 对象
    // =====================================================
    cout << "\n步骤 2: 创建 COM 对象" << endl;

    // 调用工厂函数创建对象，并获取 ISimpleCalculator 接口
    // IID_ISimpleCalculator 是我们想要的接口的标识符
    // (void**)&pCalculator 是输出参数，用于接收接口指针
    HRESULT hr = CreateSimpleCalculator(
        IID_ISimpleCalculator,      // 请求的接口 ID
        (void**)&pCalculator        // 接收接口指针的地址
    );

    // 检查对象是否创建成功
    if (FAILED(hr) || pCalculator == nullptr)
    {
        // FAILED 是一个宏，用于检查 HRESULT 是否表示失败
        cout << "\n错误：COM 对象创建失败！" << endl;
        return -1;
    }


    // =====================================================
    // 步骤 3：使用 COM 对象的方法
    // =====================================================
    cout << "\n========================================" << endl;
    cout << "步骤 3: 使用 COM 对象" << endl;
    cout << "========================================\n" << endl;

    // 定义两个数和结果变量
    int num1 = 10;
    int num2 = 20;
    int result = 0;

    // 调用 COM 对象的 Add 方法
    // pCalculator 是接口指针，通过它调用方法
    cout << "调用 Add 方法计算 " << num1 << " + " << num2 << endl;
    hr = pCalculator->Add(num1, num2, &result);

    // 检查方法调用是否成功
    if (SUCCEEDED(hr))
    {
        // SUCCEEDED 是一个宏，用于检查 HRESULT 是否表示成功
        cout << "\n计算成功！结果是: " << result << endl;
    }
    else
    {
        cout << "\n错误：Add 方法调用失败！" << endl;
    }


    // =====================================================
    // 步骤 4：测试 QueryInterface（查询接口）
    // =====================================================
    cout << "\n========================================" << endl;
    cout << "步骤 4: 测试 QueryInterface" << endl;
    cout << "========================================\n" << endl;

    // 尝试获取 IUnknown 接口
    IUnknown* pUnknown = nullptr;
    hr = pCalculator->QueryInterface(IID_IUnknown, (void**)&pUnknown);

    if (SUCCEEDED(hr) && pUnknown != nullptr)
    {
        cout << "\n成功获取 IUnknown 接口" << endl;

        // 不再使用时，必须调用 Release
        // 这会减少引用计数
        pUnknown->Release();
        cout << "已释放 IUnknown 接口" << endl;
    }


    // =====================================================
    // 步骤 5：释放 COM 对象
    // =====================================================
    cout << "\n========================================" << endl;
    cout << "步骤 5: 释放 COM 对象" << endl;
    cout << "========================================\n" << endl;

    // 当我们不再需要 COM 对象时，必须调用 Release
    // 这是 COM 编程的重要规则：谁调用了 AddRef/QueryInterface，谁就要调用 Release
    if (pCalculator != nullptr)
    {
        cout << "调用 Release 释放接口指针" << endl;
        pCalculator->Release();

        // 释放后，将指针设为 nullptr 是一个好习惯
        // 防止出现悬空指针
        pCalculator = nullptr;
    }

    cout << "\n========================================" << endl;
    cout << "程序执行完毕" << endl;
    cout << "========================================" << endl;

    // 暂停，让你看到输出结果
    cout << "\n按任意键退出..." << endl;
    system("pause");

    return 0;
}


// =====================================================
// COM 组件的核心概念总结：
// =====================================================
//
// 1. **接口 (Interface)**
//    - 接口定义了一组方法（功能）
//    - 所有 COM 接口都继承自 IUnknown
//    - 接口只有纯虚函数，没有实现
//
// 2. **IUnknown - 所有 COM 接口的基类**
//    - QueryInterface: 查询对象是否支持某个接口
//    - AddRef: 增加引用计数
//    - Release: 减少引用计数
//
// 3. **引用计数 (Reference Counting)**
//    - 用于管理对象的生命周期
//    - 每次获取接口指针时调用 AddRef
//    - 每次不再使用接口指针时调用 Release
//    - 引用计数为 0 时，对象自动销毁
//
// 4. **GUID/IID/CLSID**
//    - GUID: 全局唯一标识符（128位）
//    - IID: 接口标识符 (Interface ID)
//    - CLSID: 组件类标识符 (Class ID)
//    - 用于在系统中唯一标识接口和组件
//
// 5. **HRESULT**
//    - Windows 上的标准返回类型
//    - 表示操作是否成功
//    - S_OK: 成功
//    - E_FAIL, E_POINTER 等: 各种错误代码
//
// 6. **为什么需要 COM？**
//    - 跨语言调用（C++, C#, VB 等）
//    - 二进制兼容性（不同编译器编译的代码可以互操作）
//    - 版本控制（通过接口隔离变化）
//    - 组件重用（独立的、可替换的模块）
//
// =====================================================
