#include <stdexcept>
#include <string>

namespace metagen {

class AbortException
{
};

class Exception : public std::runtime_error
{
private:
	typedef std::runtime_error super;

public:
	Exception(const std::string & message) : super(message) {}

};

inline void abortApplication() {
	throw AbortException();
}

inline void fatalError(const std::string & message) {
	throw Exception(message);
}

} // namespace metagen
