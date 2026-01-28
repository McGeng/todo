// SimpleCOM.h - 最简单的 COM 组件接口定义
// =====================================================

#pragma once  // 防止头文件被重复包含

// 包含 Windows COM 的基础头文件
// unknwn.h 包含了 IUnknown 接口的定义，这是所有 COM 接口的基类
#include <unknwn.h>

// 包含 GUID 定义所需的头文件
// GUID 是全局唯一标识符，用于识别 COM 接口和组件
#include <guiddef.h>


// =====================================================
// 第一步：定义接口的唯一标识符 (IID - Interface ID)
// =====================================================
// 这是一个 128 位的全局唯一标识符，用于在整个系统中唯一标识这个接口
// 你可以用工具（如 Visual Studio 的 Create GUID）生成新的 GUID
// 格式：{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
// 这里我们定义了一个名为 IID_ISimpleCalculator 的 GUID
static const IID IID_ISimpleCalculator =
{ 0x12345678, 0x1234, 0x5678, { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 } };


// =====================================================
// 第二步：定义 COM 接口
// =====================================================
// COM 接口必须继承自 IUnknown（这是 COM 的基本规则）
// IUnknown 提供了三个基础方法：QueryInterface、AddRef、Release

// DECLSPEC_NOVTABLE 告诉编译器不生成虚函数表，因为这是纯接口
class DECLSPEC_NOVTABLE ISimpleCalculator : public IUnknown
{
public:
    // 这是我们自定义的方法：两个整数相加
    // __stdcall 是 Windows 上的调用约定，COM 组件必须使用这个约定
    // STDMETHOD 是一个宏，等价于 virtual HRESULT __stdcall
    // HRESULT 是返回值类型，用于表示操作是否成功
    virtual HRESULT __stdcall Add(
        int a,           // 第一个加数
        int b,           // 第二个加数
        int* result      // 指向结果的指针（输出参数）
    ) = 0;  // = 0 表示这是纯虚函数，必须由实现类提供

    // 我们可以添加更多方法，但为了简单，只实现一个加法
};


// =====================================================
// 第三步：定义 COM 组件类的唯一标识符 (CLSID - Class ID)
// =====================================================
// CLSID 用于标识具体的组件实现类
// 这样系统可以通过 CLSID 创建对应的组件实例
static const CLSID CLSID_SimpleCalculator =
{ 0x87654321, 0x4321, 0x8765, { 0x21, 0x43, 0x65, 0x87, 0xA9, 0xCB, 0xED, 0x0F } };


// =====================================================
// 第四步：声明 COM 组件的实现类
// =====================================================
class SimpleCalculator : public ISimpleCalculator
{
private:
    // 引用计数器：记录有多少客户端在使用这个对象
    // COM 使用引用计数来管理对象的生命周期
    // 当引用计数为 0 时，对象会自动销毁
    ULONG m_refCount;

public:
    // 构造函数：初始化引用计数为 1
    SimpleCalculator();

    // 析构函数：清理资源
    virtual ~SimpleCalculator();

    // =====================================================
    // 实现 IUnknown 接口的三个必需方法
    // =====================================================

    // 1. QueryInterface：查询对象是否支持某个接口
    //    客户端通过这个方法获取不同的接口指针
    virtual HRESULT __stdcall QueryInterface(
        REFIID riid,        // 要查询的接口的 IID
        void** ppvObject    // 输出参数：返回接口指针
    ) override;

    // 2. AddRef：增加引用计数
    //    每当有新的客户端持有这个对象时调用
    virtual ULONG __stdcall AddRef() override;

    // 3. Release：减少引用计数
    //    客户端不再使用对象时调用
    //    当引用计数降为 0 时，对象会删除自己
    virtual ULONG __stdcall Release() override;

    // =====================================================
    // 实现我们自定义的接口方法
    // =====================================================

    // 实现加法功能
    virtual HRESULT __stdcall Add(int a, int b, int* result) override;
};
