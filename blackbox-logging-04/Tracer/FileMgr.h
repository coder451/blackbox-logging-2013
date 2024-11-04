#ifndef HEADER_Gbp_Tra_FileMgr_h
#define HEADER_Gbp_Tra_FileMgr_h
#pragma once
#include <string>
namespace Gbp { namespace Tra {
	class FileMgr
	{
	public: 
		FileMgr(const std::string& baseName, const char* suffix, const char* options);
		~FileMgr();
		bool isValid() const {return f_!= 0;}
		FILE* f() const {return f_;}
	private:
		FILE* f_;
	};
}}

#endif // HEADER_Gbp_Tra_FileMgr_h
