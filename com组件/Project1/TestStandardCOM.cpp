// TestStandardCOM.cpp - 测试标准 COM 组件
#include "StandardCOM.h"
#include <iostream>

using namespace std;

// 设置控制台 UTF-8 编码
void SetupConsoleUTF8()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

int main()
{
    SetupConsoleUTF8();

    cout << "\n========================================" << endl;
    cout << "   标准 COM 组件示例" << endl;
    cout << "   Standard COM Component" << endl;
    cout << "========================================\n" << endl;

    // ========================================
    // 步骤 1: 获取类工厂
    // ========================================
    cout << "【步骤 1】获取类工厂\n" << endl;

    IClassFactory* pFactory = nullptr;
    HRESULT hr = DllGetClassObject(
        CLSID_Calculator,      // 请求 Calculator 的类工厂
        IID_IClassFactory,     // 请求 IClassFactory 接口
        (void**)&pFactory
    );

    if (FAILED(hr) || !pFactory)
    {
        cout << "错误：无法获取类工厂！" << endl;
        return -1;
    }

    // ========================================
    // 步骤 2: 通过类工厂创建对象
    // ========================================
    cout << "【步骤 2】通过类工厂创建对象\n" << endl;

    ICalculator* pCalc = nullptr;
    hr = pFactory->CreateInstance(
        nullptr,               // 不使用聚合
        IID_ICalculator,       // 请求 ICalculator 接口
        (void**)&pCalc
    );

    if (FAILED(hr) || !pCalc)
    {
        cout << "错误：无法创建 Calculator 对象！" << endl;
        pFactory->Release();
        return -1;
    }

    // ========================================
    // 步骤 3: 使用对象
    // ========================================
    cout << "【步骤 3】使用 Calculator 对象\n" << endl;

    int result = 0;

    pCalc->Add(100, 50, &result);        // 100 + 50
    cout << "结果: " << result << "\n" << endl;

    pCalc->Subtract(100, 50, &result);   // 100 - 50
    cout << "结果: " << result << "\n" << endl;

    pCalc->Multiply(100, 50, &result);   // 100 * 50
    cout << "结果: " << result << "\n" << endl;

    pCalc->Divide(100, 50, &result);     // 100 / 50
    cout << "结果: " << result << "\n" << endl;

    // ========================================
    // 步骤 4: 测试 QueryInterface
    // ========================================
    cout << "【步骤 4】测试 QueryInterface\n" << endl;

    IUnknown* pUnk = nullptr;
    hr = pCalc->QueryInterface(IID_IUnknown, (void**)&pUnk);
    if (SUCCEEDED(hr) && pUnk)
    {
        cout << "成功获取 IUnknown 接口\n" << endl;
        pUnk->Release();  // 释放 IUnknown 接口
    }

    // ========================================
    // 步骤 5: 释放对象
    // ========================================
    cout << "【步骤 5】释放对象\n" << endl;

    pCalc->Release();      // 释放 Calculator 对象
    pFactory->Release();   // 释放类工厂

    cout << "\n========================================" << endl;
    cout << "程序执行完毕" << endl;
    cout << "========================================\n" << endl;

    system("pause");
    return 0;
}

/*
========================================
标准 COM 模式说明：
========================================

1. 类工厂模式 (Class Factory Pattern)
   - 不直接 new 对象
   - 通过工厂创建对象
   - 符合 COM 标准

2. 创建流程：
   DllGetClassObject
        ↓
   获取 IClassFactory
        ↓
   CreateInstance
        ↓
   获取目标接口

3. 优势：
   - 统一的创建接口
   - 支持远程对象创建
   - 支持进程外服务器
   - 可控制对象生命周期

4. 对比简单模式：
   简单: new Calculator()
   标准: Factory->CreateInstance()

========================================
*/
