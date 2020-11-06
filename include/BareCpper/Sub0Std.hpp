#pragma once

#include <type_traits> //< std::is_void_v

namespace sub0
{
    /** SFINAE type for T==void overload functions
    */
    template<typename T, typename TypeIfVoid = void>
    using if_void = std::enable_if_t<std::is_void_v<T>, TypeIfVoid>;

} //<END: sub0