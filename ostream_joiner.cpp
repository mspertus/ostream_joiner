#include "ostream_joiner.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

// This might be a useful class to provide too
struct counter {
  counter(int b = 0, char const *pref = "\n", char const *suf = ". ") : pref(pref), current(b), suf(suf) {}
  char const *pref;
  int current;
  char const *suf;
  inline friend ostream &operator<<(ostream &os, counter &c) {
    os << c.pref;
    os << c.current << c.suf;
    c.current++;
    return os;
  }
};




int
main()
{

	vector<int> fib = { 0, 1, 1, 2, 3, 5, 8, 13, 21};
    vector<int> empty;
    // Three ways to print a parenthesized vector
    // N4257
	cout << "(";
	copy(fib.begin(), fib.end(), make_ostream_joiner(cout, ", "));
	cout << ")" << endl;
    // Passing the open and close parentheses 
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, "(", ", ", ")"));
    cout << endl;
    // Forcing the closer to be printed at a specific time (like with a mutex or unique_ptr)
    auto oj = make_ostream_joiner(cout, "(", ", ", ")");
    copy(fib.begin(), fib.end(), oj);
    oj.release(); 
    cout << endl;
    // Counting
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, counter(1), prefix));
    cout << endl;
    // The next two have the same output
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, " | ", " | ", " | "));
    cout << endl;
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, " | ", prefix | suffix));
    cout << endl;
    // But these two show why that aren't redundant
    copy(empty.begin(), empty.end(), make_ostream_joiner(cout, " | ", " | ", " | ")); // prints | |
    cout << endl;
    copy(empty.begin(), empty.end(), make_ostream_joiner(cout, " | ", prefix | suffix)); // prints nothing
    cout << endl;
    // This behave the same as ostream_iterator
    copy(fib.begin(), fib.end(), make_ostream_joiner(cout, ", ", suffix));
    cout << endl;
    copy(fib.begin(), fib.end(), std::ostream_iterator<int>(cout, ", "));
    cout << endl;
    return 0;
}