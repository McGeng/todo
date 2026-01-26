#pragma once

// 前置声明
class innerClass;

class faceClass
{
private:
	innerClass* d_ptr;

public:
	faceClass();
	~faceClass();

	// 深拷贝：拷贝构造函数和赋值运算符
	faceClass(const faceClass& other);
	faceClass& operator=(const faceClass& other);

	int getID();
	void setID(int id);
};

