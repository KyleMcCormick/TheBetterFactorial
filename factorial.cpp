
/*  The Better Factorial Function
 *  ----------------------------------------------------------------------------
 *
 *  Typically, one of the primary examples used to display a programming
 *  language's features and syntax is the factorial function. Often shown as a
 *  basic linear( O(n) ) recursive or iterative function, it will typically lack
 *  many important features of good software design.
 *
 *  The factorial function below exhibits C++'s power by incorporating these
 *  often forgotten features of quality software:
 *    - Input validation
 *    - Parametric polymorphism
 *    - Type property validation
 *    - An optimal asymptotic running time ( via memoization )
 *    - An efficient run time ( by choosing iteration over recursion )
 *
 *  The following factorial function runs in amortized constant time and doesn't
 *  use a global array or any form of precomputation. It manages this by using a
 *  local static (static meaning local scope & full program duration) vector of
 *  memoized factorial values, and only performing new computations when
 *  necessary.
 *
 *  Unlike many implementations, it has an overflow check and a check for
 *  negative input.
 *
 *  It is templated, and has static assertions to ensure that both the input and
 *  output types are integral.
 *
 *  When extension of the static vector is required, it uses an iterative
 *  technique, keeping this function from requiring any more than one stack
 *  frame, and saving time by avoiding pushing on extra stack frames.
 *
 *  Key disadvantages:
 *    - Requires linear( O(n) ) storage space
 *    - Extra features require a bit more code than a typical implementation
 *
*/


#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <stdexcept>


/*
   compile with: g++ factorial.cpp -o factorial -std=c++14 -Wall
   test with:    ./factorial -20 -1 0 1 2 3 4 5 6 7 8 13 19 20 21
                 98765432109876543210 hello
*/


template< class Output_Type, class Input_Type = Output_Type >
Output_Type factorial( Input_Type Input ){

    static_assert(
        std::is_integral< Output_Type >::value,
        "Bad factorial output type"
    );

    static_assert(
        std::is_integral< Input_Type >::value,
        "Bad factorial input type"
    );

    static std::vector< Output_Type > results( 2, 1 );

    while( ( size_t ) Input >= results.size() ){
        Output_Type new_val = results.back() * ( Output_Type ) results.size();

        if( new_val / results.back() != ( Output_Type ) results.size() ){
            // Zero signifies an error - overflow in this case.
            return 0;
        }

        results.push_back( new_val );
    }

    return Input >= 0 ? results[ Input ] : 0;
    // Zero signifies an error - negative input in this case.
}


int main( int argc, char* argv[] ){

    for( int i = 1 ; i < argc ; i++ ){

        int input;

        try{
            input = std::stoi( argv[ i ] );
        }
        catch( std::invalid_argument& err ){
            std::cerr << "Argument " << i <<  " = `" << argv[i]
                      << "` isn't an integer.\n";
            continue;
        }
        catch( std::out_of_range& err ){
            std::cerr << "Argument " << i << " = " << argv[i]
                      << " is out of the 'int' range.\n";
            continue;
        }

        if( input < 0 ){
            std::cerr << "Argument " << i << " = " << argv[ i ]
                      << " is negative\n";
            continue;
        }

        auto result = factorial< uint64_t, int >( input );

        if( result == 0 ){
            std::cerr << "Argument " << i << " = " << input
                      << " forced the factorial function to overflow\n";
        }else{
            std::cout << "The factorial of " << input
                      << " is " << result << '\n';
        }

    }

}