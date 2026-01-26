/*
 * Pimpl（Private Implementation）模式 - 私有实现示例
 *
 * 结构：
 *   faceClass.h  (公共接口) ──d_ptr──▶ faceClass.cpp (私有实现 innerClass)
 *
 * 优点：
 *   1. 隐藏实现细节：用户只看到 faceClass.h，看不到 innerClass 的任何信息
 *   2. 减少编译依赖：修改 innerClass 不需要重新编译使用 faceClass 的代码
 *   3. 二进制兼容：可以修改私有实现而不破坏 ABI（应用二进制接口）
 *   4. 加快编译速度：头文件更简洁，减少 #include 传递
 *
 * 关键点：
 *   - 头文件中只有前向声明：class innerClass;
 *   - cpp 文件中才有 innerClass 的完整定义
 *   - innerClass 必须在使用之前定义（编译器从上往下读）
 */

#include "faceClass.h"

class innerClass
{
private:
	faceClass* p_ptr;
	int m_nId; // 记录一个数据
public:
	// 供外部调用的函数都要public
	int getID();
	void setID(int id);

public:
	innerClass(faceClass* parent);
	~innerClass();
};


faceClass::faceClass()
	:d_ptr(new innerClass(this))
{
}

faceClass::~faceClass()
{
	// 这里需要 delete，否则会内存泄露
	delete d_ptr;
}

/*
 * 拷贝构造函数 vs 赋值运算符 使用场景对比：
 *
 *   faceClass a;
 *   a.setID(100);
 *
 *   faceClass b = a;   // 拷贝构造：b 刚创建，d_ptr 不存在，需要 new
 *
 *   faceClass c;       // c 构造时已经 new 了 d_ptr
 *   c = a;             // 赋值运算符：c.d_ptr 已存在，直接复制数据即可
 */

// 拷贝构造函数 - 深拷贝
// 场景：对象刚被创建，d_ptr 还不存在，必须 new
faceClass::faceClass(const faceClass& other)
	:d_ptr(new innerClass(this))  // 创建新的 innerClass
{
	// 复制数据
	if (other.d_ptr) {
		d_ptr->setID(other.d_ptr->getID());
	}
}

// 赋值运算符 - 深拷贝
// 场景：对象已经存在，d_ptr 在构造时就创建好了，复用它即可
faceClass& faceClass::operator=(const faceClass& other)
{
	if (this != &other) {  // 防止自赋值
		// 复制数据（不需要重新创建 d_ptr，复用现有的）
		if (d_ptr && other.d_ptr) {
			d_ptr->setID(other.d_ptr->getID());
		}
	}
	return *this;
}

int faceClass::getID()
{
	if (d_ptr) {
		return d_ptr->getID();
	}
}

void faceClass::setID(int id)
{
	if (d_ptr)
	{
		d_ptr->setID(id);
	}
}


innerClass::innerClass(faceClass* parent)
	:p_ptr(parent)
	,m_nId(999)
{
}

innerClass::~innerClass()
{
	// 这里不能 delete，这个指针是外部传入的，这里 delete 会导致双重释放
	//delete p_ptr;
}

int innerClass::getID()
{
	return m_nId;
}

void innerClass::setID(int id)
{
	m_nId = id;
}














