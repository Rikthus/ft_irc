#include <iostream>
#include <string>
#include <vector>

int	main(void)
{
	std::vector<std::string>  vec;
	std::vector<std::string>::iterator	it;

	vec.push_back("lol");
	vec.push_back("bouh");
	// vec.push_back(5);
	// vec.push_back(5);
	// vec.push_back(5);
	// vec.push_back(5);
	// vec.push_back(10);

	// for (it = vec.begin(); it != vec.end(); ++it)
	// 	std::cout << *it << std::endl;

	it = vec.end();
	std::cout << *it << std::endl;

	// vec[0] = 1;


	// std::cout << vec.front() << std::endl;
	// std::cout << vec[0] << std::endl;
	// std::cout << *it << std::endl;

	// int tab[4] = {1, 2, 3, 4};
	// int tab2[2] = {-1, 6};
	return (0);
}
