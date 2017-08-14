////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		DInternalTypesAliases
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Aliases for types
/// | \details	Using standart types may cause confusion in memory representation (depending on system) \n
///	|			Plus if some mathematical functions will be changed in the future, we want to avoid multiple modifications!
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/* Standard library includes */
#include <vector>
#include <memory>
#include <map>
#include <sstream>
#ifndef WIN32 //  ======================================================================== [ not WINDOWS ] =============================  //
#include <stdint.h> ///< Using [basictype]_fast[minsize]_t
#define GLM_FORCE_RADIANS
#endif //  =========================================================================== [ end not WINDOWS ] =============================  //
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

//// ---- Namespaces ---- ////
///* Substitution Aliases */
using pxBool = bool;				/// 1b boolean GL_TRUE or GL_FALSE
using pxInt16 = int_fast16_t;		/// Signed 16b integer [-32 768,  32 767] at least! (can be more if faster on system)
using pxUInt16 = uint_fast16_t;		/// Unsigned 16b integer [0,  65 535] at least! (can be more if faster on system)
using pxInt = int_fast32_t;			/// Signed 32b integer [-2 147 483 648,  2 147 483 647] at least! (can be more if faster on system)
using pxInt32 = int32_t;			/// Signed 32b integer [-2 147 483 648,  2 147 483 647] STRICT (usefull for third party libs)
using pxUInt32 = uint32_t;		    /// Unsigned 32b integer [0,  4 294 967 295] STRICT (usefull for third party libs)
using pxUInt = uint_fast32_t;		/// Unsigned 32b integer [0,  4 294 967 295] at least! (can be more if faster on system)
using pxInt64 = int_fast64_t;		/// Signed 64b integer [-9 223 372 036 854 775 808,  9 223 372 036 854 775 807] at least! (can be more if faster on system)
using pxUInt64 = uint_fast64_t;		/// Unsigned 64b integer [0,  18 446 744 073 709 551 615] at least! (can be more if faster on system)
using pxFloat = float;				/// Signed 32b floating-point
using pxFloat64 = double;			/// Signed 64b floating-point

using pxString = std::string;
template<class T, class U>
using pxMap = std::map<T, U>;
template<class T>
using pxVec = std::vector<T>;
template<class T, class U>
using pxPair = std::pair<T,U>;
template<class T>
using pxUniquePtr = std::unique_ptr<T>;
template<class T>
using pxSharedPtr = std::shared_ptr<T>;

#ifndef WIN32 //  ============================================================================ [ WINDOWS ] =============================  //
template<typename T, typename... Args>
std::unique_ptr<T> createUniquePtr(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#else //  ================================================================================ [ not WINDOWS ] =============================  //
template<typename T, typename... Args>
std::unique_ptr<T> createUniquePtr(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
#endif //  =========================================================================== [ end not WINDOWS ] =============================  //

using GLvramLocation = GLuint;		/// Used for OpenGL bindings

using pxVec2i = glm::i32vec2;		/// Signed 32*2b integer vector
using pxVec2f = glm::f32vec2;		/// Signed 32*2b floating-point vector
using pxVec3f = glm::f32vec3;		/// Signed 32*3b floating-point vector
using pxVec4f = glm::f32vec4;		/// Signed 32*4b floating-point vector
using pxVec3f64 = glm::f64vec3;		/// Signed 64*3b floating-point vector
using pxMat4f = glm::f32mat4;		/// Signed 32*4*4b floating-point matrix

template<class T>
pxBool is_lexically_castable(pxString const& a_string) {
	std::stringstream ss(a_string);
	T result;
	if ((ss >> result).fail() || !(ss >> std::ws).eof())
		return false;
	return true;
}

template<class T>
T lexical_cast(pxString const& a_string) {
	std::stringstream ss(a_string);
	T result;
	ss >> result;
	return result;
}
