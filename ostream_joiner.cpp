#include "ostream_joiner.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int
main()
{

	vector<int> fib = { 0, 1, 1, 2, 3, 5, 8, 13, 21};
	vector<string> hello = { "hello", "world" };
    vector<int> empty;
 
	// Prints (0, 1, 1, 2, 3, 5, 8, 13, 21)
	cout << "(";
	copy(fib.begin(), fib.end(), make_ostream_joiner(cout, ", "));
	cout << ")" << endl;

    // Prints (0, 1, 1, 2, 3, 5, 8, 13, 21) 
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, "(", ", ", ")"));
    cout << endl;

    // Prints (0, 1, 1, 2, 3, 5, 8, 13, 21)
    auto oj = make_ostream_joiner(cout, "(", ", ", ")");
    copy(fib.begin(), fib.end(), oj);
    oj.release(); 
    cout << endl;

    // Prints a counted list
	// 1. hello
	// 2. world
	copy(hello.begin(), hello.end(), make_ostream_joiner(cout, ostream_counter{}, prefix));
    cout << endl;

    // Prints | 0 | 1 | 1 | 2 | 3 | 5 | 8 | 13 | 21 |
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, " | ", " | ", " | "));
    cout << endl;

	// Also prints | 0 | 1 | 1 | 2 | 3 | 5 | 8 | 13 | 21 |
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, " | ", prefix | suffix));
    cout << endl;

    // Prints | | 
    copy(empty.begin(), empty.end(), make_ostream_joiner(cout, " | ", " | ", " | ")); // prints | |
    cout << endl;

	// Prints nothing
    copy(empty.begin(), empty.end(), make_ostream_joiner(cout, " | ", prefix | suffix)); // prints nothing
    cout << endl;

    // This behave the same as ostream_iterator
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, ", ", suffix));
    cout << endl;
    copy(fib.begin(), fib.end(), std::ostream_iterator<int>(cout, ", "));
    cout << endl;
    return 0;
}