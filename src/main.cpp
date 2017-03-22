#include <chrono>
#include <iostream>
#include <boost/format.hpp>
#include <string>

namespace
{
using duration_microsec = std::chrono::duration< double, std::micro >;

constexpr size_t n { 9999999 };
std::vector<char> test_vector( n );

template < typename FN >
inline duration_microsec  measure( FN& fn )
{
    auto ts1 = std::chrono::high_resolution_clock::now();
    fn();
    auto ts2 = std::chrono::high_resolution_clock::now();

    return ( ts2 - ts1 );
}

void print_table_header()
{
    using namespace std;
    cout << std::string ( 80, '-' ) << "\n";
    cout << ( boost::format( "|%|3t|%s %|30t|%s %|60t|%s %|79t||\n" )
                        % "std::for_each"
                        % "simple for loop"
                        % "difference"
            ).str();
    cout << std::string ( 80, '-' ) << "\n";
}

void run_test()
{
    using namespace std;

    auto dummpy_fn = [](char c)
    {
    };

    auto test1 = [&]() {
        std::for_each( std::begin( test_vector ), std::end( test_vector ), dummpy_fn );
    };
    auto test2 = [&]() {
        for ( auto it = test_vector.begin(); it != test_vector.end(); ++it )
        {
            dummpy_fn( *it );
        }
    };

    auto len1 = measure( test1 );
    auto len2 = measure( test2 );

    cout << ( boost::format( "%|3t|%.1f microsec %|30t|%.1f microsec %|60t|%s %%\n" )
                        % len1.count()
                        % len2.count()
                        % std::to_string( ( len2.count() / len1.count() - 1 ) * 100 )
            ).str();
}
} // namespace

int main(int argc, char **argv)
{
    using namespace std;

    try
    {
        print_table_header();
        for( auto i = 0; i < 11; ++i )
        {
            run_test();
        }
        cout << std::string ( 80, '-' ) << "\n";

        return 0;
    }
    catch ( std::exception& e )
    {
        cerr << e.what() << endl;
    }

    return 1;
}
