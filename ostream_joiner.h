#ifndef OSTREAM_JOINER_H
#  define OSTREAM_JOINER_H

#include <iostream>
#include <string>
#include <utility>
#include <type_traits>
#include <optional>
#include <tuple>
  
  namespace detail {
    struct no_close {};
    template<typename T> struct oj_close : public std::optional <T> {
      oj_close() = default;
      oj_close(T const &t) : std::optional<T>(t) {}
      oj_close(T &&t) : std::optional<T>(t) {}
      template<typename charT, typename traits>
      void close(std::basic_ostream<charT, traits> &os) {
        if (*this)
          os << this->value();
        this->reset();
      }
    };
    template<> struct oj_close <no_close> {
      operator bool() { return false;  }
      template<typename charT, typename traits>
      void close(std::basic_ostream<charT, traits> &os) {}
    };
  }

  enum delim_pos {
    prefix = 2,
    suffix = 4
  };

  template<typename DelimT, typename charT = char, typename traits = std::char_traits<charT>, typename CloseT = detail::no_close >
  class ostream_joiner : private detail::oj_close<CloseT> { // Inherit to take advantage of empty base class optimization if no open/close
    typedef ostream_joiner self; // Inspired by boost::function_output_iterator
  public:
    typedef charT               char_type;
    typedef traits              traits_type;
    typedef std::basic_ostream<charT, traits> ostream_type;
    typedef std::output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;

    template <class T> 
    ostream_joiner& operator=(T const & value) {
  
      if (first()) {
        if (flags & prefix)
          *osp << del;
        clear_first();
      } else if (!(flags & suffix)) {
        *osp << del;
      }
      *osp << value;
      if (flags & suffix)
        *osp << del;
      return *this;
    }

    ostream_joiner &operator*() { return *this; }
    self& operator++() { return *this; }
    self& operator++(int) { return *this; }

    ostream_joiner(ostream_joiner const &other) : flags(other.flags), osp(other.osp), del(other.del) {}
    ostream_joiner(ostream_type &os, DelimT &&del, char flags = 0) : osp(&os), del(del), flags(1 | flags) {}
    ostream_joiner(ostream_type &os, DelimT const &del, char flags = 0) : osp(&os), del(del), flags(1 | flags) {}
    template<typename OpenT>
    ostream_joiner(ostream_type &os, std::tuple<OpenT, DelimT, CloseT> const &t) 
		: detail::oj_close<CloseT>(std::get<2>(t)), osp(&os), del(std::get<1>(t)), flags(1) {
      *os << std::get<0>(t);
    }

    void release() {
      this->close(*osp);
    }

    ~ostream_joiner() {
      this->close(*osp);
    }

  private:
    bool first() { return flags & 1; }
    void clear_first() { flags &= ~1; }
    char flags;
    ostream_type *osp;
    DelimT del;

  };

  template<typename charT, typename traits, typename DelimT>
  ostream_joiner(std::basic_ostream<charT, traits> &, DelimT, char flags = 0)->ostream_joiner<DelimT, charT, traits>;

  template <class charT, class traits, class OpenT, class DelimT, class CloseT>
  ostream_joiner(std::basic_ostream<charT, traits> &, std::tuple<OpenT, DelimT, CloseT>)->ostream_joiner<DelimT, charT, traits, CloseT>;

  struct counting_delimiter {
	  counting_delimiter(int b = 1, char const *pref = "\n", char const *suf = ". ") : pref(pref), current(b), suf(suf) {}
	  char const *pref;
	  int current;
	  char const *suf;
	  template <typename charT, typename traits>
	  inline friend std::basic_ostream<charT, traits> &operator<<(std::basic_ostream<charT, traits> &os, ostream_counter &c) {
		  os << c.pref;
		  os << c.current << c.suf;
		  c.current++;
		  return os;
	  }
  };
#endif