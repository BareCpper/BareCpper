#include <cstdint>
#include <tuple>
#include <utility>

/*template< typename PortT , int cPinIndex, typename PortRegister_t >
struct Pin
{
    typedef PortT Port_t;

    constexpr static Port_t port()
    { return Port_t(); }

    constexpr static int index()
    { return cPinIndex; }

    constexpr static PortRegister_t mask()
    { return PortRegister_t(1) << cPinIndex; }
};*/


/** Pins Entry stores the pin type using the index as the member name i.e. Pin<0> in a list if accessed at Pin3
*/
template< int cPinIndex, typename PinT > struct ListPin;
template< typename PinT > struct ListPin<0u, PinT> { typedef PinT Pin0; };
template< typename PinT > struct ListPin<1u, PinT> { typedef PinT Pin1; };
template< typename PinT > struct ListPin<2u, PinT> { typedef PinT Pin2; };
template< typename PinT > struct ListPin<3u, PinT> { typedef PinT Pin3; };
template< typename PinT > struct ListPin<4u, PinT> { typedef PinT Pin4; };
template< typename PinT > struct ListPin<5u, PinT> { typedef PinT Pin5; };
template< typename PinT > struct ListPin<6u, PinT> { typedef PinT Pin6; };
template< typename PinT > struct ListPin<7u, PinT> { typedef PinT Pin7; };
template< typename PinT > struct ListPin<8u, PinT> { typedef PinT Pin8; };
template< typename PinT > struct ListPin<9u, PinT> { typedef PinT Pin9; };
template< typename PinT > struct ListPin<10u, PinT> { typedef PinT Pin10; };
template< typename PinT > struct ListPin<11u, PinT> { typedef PinT Pin11; };
template< typename PinT > struct ListPin<12u, PinT> { typedef PinT Pin12; };
template< typename PinT > struct ListPin<13u, PinT> { typedef PinT Pin13; };
template< typename PinT > struct ListPin<14u, PinT> { typedef PinT Pin14; };
template< typename PinT > struct ListPin<15u, PinT> { typedef PinT Pin15; };
template< typename PinT > struct ListPin<16u, PinT> { typedef PinT Pin16; };
template< typename PinT > struct ListPin<17u, PinT> { typedef PinT Pin17; };
template< typename PinT > struct ListPin<18u, PinT> { typedef PinT Pin18; };
template< typename PinT > struct ListPin<19u, PinT> { typedef PinT Pin19; };
template< typename PinT > struct ListPin<20u, PinT> { typedef PinT Pin20; };
template< typename PinT > struct ListPin<21u, PinT> { typedef PinT Pin21; };
template< typename PinT > struct ListPin<22u, PinT> { typedef PinT Pin22; };
template< typename PinT > struct ListPin<23u, PinT> { typedef PinT Pin23; };
template< typename PinT > struct ListPin<24u, PinT> { typedef PinT Pin24; };
template< typename PinT > struct ListPin<25u, PinT> { typedef PinT Pin25; };
template< typename PinT > struct ListPin<26u, PinT> { typedef PinT Pin26; };
template< typename PinT > struct ListPin<27u, PinT> { typedef PinT Pin27; };
template< typename PinT > struct ListPin<28u, PinT> { typedef PinT Pin28; };
template< typename PinT > struct ListPin<29u, PinT> { typedef PinT Pin29; };
template< typename PinT > struct ListPin<30u, PinT> { typedef PinT Pin30; };
template< typename PinT > struct ListPin<31u, PinT> { typedef PinT Pin31; };
/*
template< typename PortT , int... PinList >
struct Pins : ListPin<PinList, Pin<PortT,PinList,uint32_t>>...
{
    typedef std::tuple<Pin<PortT,PinList,uint32_t>...> Array;

};*/

template< int cPortIndex >
struct Pins
{
    template<int cPinIndex>
    struct Pin;
};

template<> template<>
struct Pins<0>::Pin<0>
{
    static const int foo = 111;
};

template<> template<>
struct Pins<0>::Pin<1>
{
    static const int foo = 222;
};

template<> template<>
struct Pins<0>::Pin<2>
{
    static const int foo = 333;
};

// @todo Enable-if in range...
//template<> template<int a>
//struct Pins<0>::Pin
//{
//};

template< int cPortIndex, int ...cPinIndices >
struct PinAlias : ListPin< cPinIndices // @todo std::make_index_sequence<sizeof...(cPinIndices)>
    , typename Pins<cPortIndex>::template Pin<cPinIndices> >...
{
};

template< int cPortIndex>
struct Port;

template<>
struct Port<0> : Pins<0>
    , PinAlias<0
    , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    , 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    , 30, 31>
{
};


typedef Port<0> Port0;

int main()
{
    // Port0::Pin0 pin0;
    // Port0::Pin31 pin31;

 //TEST: Port0::Pin4 v;
     //std::cout << pin31.index() << '-';
    return Port0::Pin0::foo;
}