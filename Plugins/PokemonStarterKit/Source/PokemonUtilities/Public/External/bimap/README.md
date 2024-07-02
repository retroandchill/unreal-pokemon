### bimap

stde::bimap<_KeyType, _ValueType> implements a interface that behaves similar to a map where you can make reverse lookups.
A bidirectional map contains pairs with 1 to 1 relations. Every key has only one value and every value corresponds to exactly one key.
It could be used to store Ids and pointer addresses. Thus you can lookup the Id by the pointer but also the pointer by Id.

std-extensions bimap can be used as a header only implementation. The only thing you need is the std library and C++.

Example:

```C++
/*
* The IdConverter class implements some ideas on how to use the bimap and when it is useful
* It does not want to show the best way to implement something.
*
* bimap.hpp also implements a using declarative that makes it easy to use a unordered_bimap.
* It's the same as the normal bimap but using std::unordered_map to store its state.
* In some situations you would want to use stde::bimap and in some you want to
* use stde::unordered_bimap.
*/
#include <bimap.hpp>
#include <memory>

class IdConverter
{
private:
	stde::bimap<uint32_t, uint32_t> m_id_map;
public:
	template<class _Type>
	_Type* by_id(uint32_t uid) { return static_cast<_Type*>(m_id_map.get_value(uid)); }
	template<class _Type>
	uint32_t by_ptr(_Type* ptr) { return m_id_map.get_key(static_cast<uint32_t>(ptr)); }
	template<class _Type>
	uint32_t insert(_Type* ptr)
	{
		if (m_id_map.has_value(static_cast<uint32_t>(ptr))) return 0;
		auto id = generate_id();
		m_id_map.insert(id, static_cast<uint32_t>(ptr));
	}
	void remove_by_id(uint32_t uid)
	{
		m_id_map.erase_key(uid);
	}
	template<class _Type>
	void remove_by_ptr(_Type* ptr)
	{
		m_id_map.erase_value(static_cast<uint32_t>(ptr));
	}
	// Id Generator
	uint32_t generate_id()
	{
		// suppose random_generate() is a function that creates random numbers
		auto id = random_generate();
		while (m_id_map.has_key(id)) id = random_generate();
		return id;
	}
	void update_all()
	{
		// Assuming GuiObjectPtr is the base class of all elements in the IdConverter table
		for (auto& obj : m_id_map)
			static_cast<GuiObjectPtr>(obj.second)->update();
	}
	// You could also create a to_shared function that returns a smart pointer to the object
	// Remember to always keep one shared pointer so that your object isn't being destroyed by the 
	// shared pointer implementation accidently or remove the object from the converter in the
	// Base classes destructor.
	template<class _Type>
	std::shared_ptr<_Type> to_shared(uint32_t uid)
	{
		return std::make_shared<_Type>(static_cast<_Type*>(m_id_map.get_value(uid)));
	}
};

// ...

int main()
{
	IdConverter converter;
	// ...

}

```

More examples:
 * [bimap_basic](/examples/bimap_basic.hpp)
 * [bimap_gui](/examples/bimap_gui.hpp)
 
Performance tests:

```
Testing std::map<unsigned int, unsigned int> with 100000 elements.
        Initialization took: 1283127837 nanoseconds             1 seconds
        Access took        : 26110671 nanoseconds               0 seconds
        find took          : 765988895 nanoseconds              0 seconds
        erase took         : 683217572 nanoseconds              0 seconds

Testing std::map<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >, class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >> with 100000 elements.
        Initialization took: 7193567389 nanoseconds             7 seconds
        Access took        : 26496017 nanoseconds               0 seconds
        find took          : 3899122728 nanoseconds             3 seconds
        erase took         : 1055643928 nanoseconds             1 seconds

Testing stde::bimap<unsigned int, unsigned int> with 100000 elements.
        Initialization took: 2101563891 nanoseconds             2 seconds
        Access took        : 59079720 nanoseconds               0 seconds
        find took          : 821610188 nanoseconds              0 seconds
        erase took         : 2721119610 nanoseconds             2 seconds

Testing stde::bimap<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >, class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >> with 100000 elements.
        Initialization took: 12045763399 nanoseconds            12 seconds
        Access took        : 60478559 nanoseconds               0 seconds
        find took          : 3892301806 nanoseconds             3 seconds
        erase took         : 3719605167 nanoseconds             3 seconds
		
```
