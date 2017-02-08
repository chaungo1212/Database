// Database.h - Contains declaration of Function class  
#pragma once  

#ifdef DATABASE_EXPORTS  
#define DATABASE_API __declspec(dllexport)   
#else  
#define DATABASE_API __declspec(dllimport)   
#endif  

namespace Database
{
	// This class is exported from the Database.dll  
	class Functions
	{
	public:
		static DATABASE_API void test();
	};
}
