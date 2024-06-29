#include <bimap.hpp>
#include <sstream>

using address = uint32_t;

int main()
{
	stde::bimap<uint64_t, address> my_map;
	address _6_address = 0;
	for (auto i = 0u; i < 10; i++)
	{
		auto strobj = new std::string();
		if (i == 6)
			_6_address = reinterpret_cast<address>(strobj);
		std::stringstream ss;
		ss << i << "-string";
		*strobj = ss.str();
		my_map.insert({ i, reinterpret_cast<address>(strobj) });
	}

	for (const auto& elem : my_map)
		printf_s(
			"%lld => %p\n",
			*elem.first,
			reinterpret_cast<void*>(*elem.second));

	printf_s("\n");

	for (const auto& elem : my_map)
		printf_s(
			"%lld => '%s'\n",
			*elem.first,
			(*reinterpret_cast<std::string*>(*elem.second)).data());

	printf_s("\n");

	my_map.erase_key(2);
	my_map.erase_value(_6_address);

	for (const auto& elem : my_map)
		printf_s(
			"%lld => '%s'\n",
			*elem.first,
			(*reinterpret_cast<std::string*>(*elem.second)).data());

	for (auto elem : my_map)
		delete reinterpret_cast<std::string*>(*elem.second);

	return 0;
}