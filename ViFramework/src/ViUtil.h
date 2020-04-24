#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <stdio.h>

#if 1
	#ifdef _MSC_VER
		#define DISABLE_WARNING_PUSH __pragma(warning(push))
		#define DISABLE_WARNING_POP __pragma(warning(pop))
		#define DISABLE_WARNING(num) __pragma(warning(disable: num))
	//TODO other compilers - GCC, CLANG
	#endif
#else
	#define DISABLE_WARNING_PUSH
	#define DISABLE_WARNING_POP
	#define DISABLE_WARNING(num)
#endif

#ifdef _DEBUG
#define vi_assert(condition, message)							\
do																\
{																\
	if (!(condition))											\
	{															\
		std::cerr << "Assertion '" << #condition << "' failed in " << \
			__FILE__ << " line " << __LINE__ << ": " << message << \
			std::endl;											\
		std::terminate();										\
	}															\
} while (false)
#else
#define vi_assert(condition, message) do { } while (false)
#endif

#define vi_property_get(type, var)	\
inline type Get_##var()				\
{									\
	return var;						\
}									\
static_assert(true, "vi_property_get macro requires a semicolon immediately proceeding.")

#define vi_property_set(type, var)	\
inline void Set_##var(type setTo)	\
{									\
	var = setTo;					\
}									\
static_assert(true, "vi_property_set macro requires a semicolon immediately proceeding.")

#define vi_property_get_named(type, var, get_set_name)	\
inline type Get##get_set_name()							\
{														\
	return var;											\
}														\
static_assert(true, "vi_property_get_named macro requires a semicolon immediately proceeding.")


#define vi_property_set_named(type, var, get_set_name)	\
inline void Set##get_set_name(type setTo)				\
{														\
	var = setTo;										\
}														\
static_assert(true, "vi_property_set_named macro requires a semicolon immediately proceeding.")

#define vi_property_exported_get_named(type, var, get_set_name)	\
inline type VIENGINE_EXPORT Get##get_set_name()					\
{																\
	return var;													\
}																\
static_assert(true, "vi_property_exported_get_named macro requires a semicolon immediately proceeding.")

#define vi_property_exported_set_named(type, var, get_set_name)	\
inline void VIENGINE_EXPORT Set##get_set_name(type setTo)		\
{																\
	var = setTo;												\
}																\
static_assert(true, "vi_property_exported_set_named macro requires a semicolon immediately proceeding.")

#define vi_property(type, var)	\
private:						\
	DISABLE_WARNING_PUSH		\
	DISABLE_WARNING(4251)		\
	type var;					\
	DISABLE_WARNING_POP			\
public:							\
	vi_property_get(type, var);	\
	vi_property_set(type, var);	\
static_assert(true, "vi_property macro requires a semicolon immediately proceeding.")

#define vi_property_named(type, var, get_set_name)	\
private:											\
	DISABLE_WARNING_PUSH							\
	DISABLE_WARNING(4251)							\
	type var;										\
	DISABLE_WARNING_POP								\
public:												\
	vi_property_get_named(type, var, get_set_name);	\
	vi_property_set_named(type, var, get_set_name);	\
static_assert(true, "vi_property_named macro requires a semicolon immediately proceeding.")

#define vi_property_exported_named(type, var, get_set_name)	\
private:													\
	DISABLE_WARNING_PUSH									\
	DISABLE_WARNING(4251)									\
	type var;												\
	DISABLE_WARNING_POP										\
public:														\
	vi_property_exported_get_named(type, var, get_set_name);\
	vi_property_exported_set_named(type, var, get_set_name);\
static_assert(true, "vi_property_exported_named macro requires a semicolon immediately proceeding.")

#ifdef ViFramework_EXPORTS
	/*Enabled as "export" while compiling the dll project*/
	#define VIENGINE_EXPORT __declspec(dllexport)  
#else
	/*Enabled as "import" in the Client side for using already created dll file*/
	#define VIENGINE_EXPORT __declspec(dllimport)  
#endif

//--------------------------------------------------------------------------------------------------
//												Typedefs
//--------------------------------------------------------------------------------------------------
DISABLE_WARNING_PUSH
DISABLE_WARNING(4251)
template struct VIENGINE_EXPORT glm::mat<4, 4, glm::f32, glm::packed_highp>;
template struct VIENGINE_EXPORT glm::vec<4, glm::f32, glm::packed_highp>;
template struct VIENGINE_EXPORT glm::vec<3, glm::f32, glm::packed_highp>;
template struct VIENGINE_EXPORT glm::vec<2, glm::f32, glm::packed_highp>;

template struct VIENGINE_EXPORT glm::vec<3, glm::int32, glm::packed_highp>;
template struct VIENGINE_EXPORT glm::vec<2, glm::int32, glm::packed_highp>;

typedef VIENGINE_EXPORT glm::mat<4, 4, glm::f32, glm::packed_highp> mat4;
typedef VIENGINE_EXPORT glm::vec<4, glm::f32, glm::packed_highp> vec4;
typedef VIENGINE_EXPORT glm::vec<3, glm::f32, glm::packed_highp> vec3;
typedef VIENGINE_EXPORT glm::vec<2, glm::f32, glm::packed_highp> vec2;

typedef VIENGINE_EXPORT glm::vec<4, glm::int32, glm::packed_highp> vec4i;
typedef VIENGINE_EXPORT glm::vec<3, glm::int32, glm::packed_highp> vec3i;
typedef VIENGINE_EXPORT glm::vec<2, glm::int32, glm::packed_highp> vec2i;

typedef VIENGINE_EXPORT glm::vec<3, glm::f64, glm::packed_highp> vec3d;
typedef VIENGINE_EXPORT glm::vec<2, glm::f64, glm::packed_highp> vec2d;

typedef VIENGINE_EXPORT std::string string;
DISABLE_WARNING_POP
