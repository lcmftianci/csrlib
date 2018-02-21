#ifndef _CSR_DEF_H_
#define _CSR_DEF_H_

//成员变量声明函数
#define TPGETSET(ObjName, ObjValue)				 \
	ObjName ObjValue;								  \
	public:												\
	inline ObjName Get##ObjValue(){ return ObjValue; }   \
	inline void Set##ObjValue(ObjName obj){ ObjValue = obj; }		\
	private:

#endif // _CSR_DEF_H_
