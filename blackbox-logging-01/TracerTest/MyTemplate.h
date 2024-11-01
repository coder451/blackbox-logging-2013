#ifndef HEADER_MyTemplate_h
#define HEADER_MyTemplate_h
#include <Tracer/Trace.h>
template <typename T>
class MyTemplate
{
public:
	MyTemplate(): t_()
	{
		TRACEF(10, "XYZ");
	}
	~MyTemplate(){}
	size_t size() const {return sizeof(t_);}
private:
	T t_;
};

#endif // HEADER_MyTemplate_h
