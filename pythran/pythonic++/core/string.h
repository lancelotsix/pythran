#ifndef PYTHONIC_STRING_H
#define PYTHONIC_STRING_H
#include <string>
#include <sstream>

namespace pythonic {
    namespace core {
		class string : public std::string {
			public:
				string() : std::string() {}
				string(std::string const & s) : std::string(s) {}
				string(std::string && s) : std::string(std::move(s)) {}
				string(const char*s) : std::string(s) {}
				string(char c) : std::string(1,c) {}
                core::string operator+(core::string const& s) const {
                    return core::string( (*(std::string*)this)+(std::string const&)s );
                }
				operator long() {
					long out;
					std::istringstream iss(*this);
					iss >> out;
					return out;
				}
		};
    }
}
template<typename T>
pythonic::core::string operator*(pythonic::core::string const & s, T const & t) {
    size_t n=t;
    pythonic::core::string other;
    other.resize(s.size()*n,'a');
    auto where = other.begin();
    for(size_t i=0;i<n; i++, where+=s.size())
        std::copy(s.begin(), s.end(), where);
    return other;
}

namespace std {
  template <> struct hash<pythonic::core::string>
  {
    size_t operator()(const pythonic::core::string & x) const
    {
      return hash<std::string>()(x);
    }
  };
}

#endif
