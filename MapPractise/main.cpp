#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <cmath>



// math functions
double dodawanie(double x, double y);
double odejmowanie(double x, double y);
double mnozenie(double x, double y);
double dzielenie(double x, double y);
double potegowanie(double x, double y);

// translate functions
void clean_up(std::string& problem);
double calculate(std::string problem, bool modul = false);
double separate(std::string& problem);

int main()
{
	std::string inp{};
	std::cout << "Podaj problem" << std::endl;
	getline(std::cin, inp);
	do
	{
		try {
			clean_up(inp);
			std::cout << "= " << separate(inp) << std::endl;
		}
		catch (int& ex) {
			std::cerr << "Problem nie do rozwiazania" << std::endl;
		}
		catch (std::string& ex) {
			std::cerr << "Nie dozwolony znak " << ex << std::endl;
		}
		std::cout << "Podaj problem" << std::endl;
		getline(std::cin, inp);
		
	} while (inp != "q" && inp != "Q");
	
	return 0;
}

void clean_up(std::string& problem) {
	char allowedOper[10]{ '-', '+', '*', '/', '^', ')', '(', '|', '.', ','};
	problem.erase(std::remove_if(problem.begin(), problem.end(), std::isspace), problem.end());
	for (int i{}; i < problem.length(); i++) {
		char* operIndex = std::find(std::begin(allowedOper), std::end(allowedOper), problem[i]);
		if (operIndex == std::end(allowedOper) && !isdigit(problem[i]))
			throw std::string{ problem[i] };
	}
}

double separate(std::string& problem) {

	std::string problems;

	int i{};
	char x{};
	bool modul{ 0 };
	int nawIndex{}, modIndex{};
	double wynik{};
	while (i < problem.length()) {
		x = problem[i];
		if (x == '(') {
			nawIndex = i;
		} 
		if (x == '|' && modul && isdigit(problem[i-1])) {
			//wyznacza sekcje w module, oblicza je osobno nastepnie wrzuca wynik zamiast nawiasu do problemu
			wynik = calculate(problem.substr(modIndex + 1, i - modIndex - 1), modul);
			problem.replace(modIndex, i - modIndex + 1, std::to_string(wynik));
			std::cout << problem << std::endl;
			modIndex = 0;
			modul = false;
			i = 0;
		}
		else if (x == ')') {
			//wyznacza sekcje w nawiasach, oblicza je osobno nastepnie wrzuca wynik zamiast nawiasu do problemu
			wynik = calculate(problem.substr(nawIndex + 1, i - nawIndex - 1));
			problem.replace(nawIndex, i - nawIndex + 1, std::to_string(wynik));
			std::cout << problem << std::endl;
			nawIndex = 0;
			modul = false;
			i = 0;
		}
		else if (x == '|') {
			modul = true;
			modIndex = i;
			i++;
		}
		else {
			i++;
		}
	}
	return calculate(problem, modul);
}

double calculate(std::string problem, bool modul) {

	// create a func template
	using func = std::function<double(double, double)>;

	std::map<char, func> mp{
		{'+', dodawanie},
		{'-', odejmowanie},
		{'*', mnozenie},
		{'/', dzielenie},
		{'^', potegowanie}
	};

	std::map<char, int> PRN{
		{'+', 1},
		{'-', 1},
		{'*', 2},
		{'/', 2},
		{'^', 3},
	};

	std::vector<double> num;
	std::vector<char> oper;
	bool number{ false }, ujemna{ false };
	double helpNum{};
	double j{ 1 };
	char lastChar{};

	// rozdzielenie liczb i operatorów
	for (const auto& x : problem) {
		//char x = problem[i];
		if ((isdigit(x) || x == '.' || x == ',') && !number) {
			number = true;
			helpNum = x - 48;
			lastChar = '0';
			continue;
		}
		if (number) {
			if (x == '.' || x == ',') {
				j = -1;
				continue;
			}
			if (!isdigit(x)) {
				if (ujemna)
					helpNum *= -1;
				number = false;
				num.push_back(helpNum);
			}
			else if (j < 0) {
				helpNum += (x - 48) * pow(10, j);
				j--;
			}
			else {
				helpNum *= 10;
				helpNum += x - 48;
			}
		}
		if (!number)
		{
			if (!(x == '-' && lastChar != '0')) {
				oper.push_back(x);
				ujemna = false;
			}
			else
				ujemna = true;
			lastChar = x;
		}
	}
	if (number) {
		if (ujemna)
			helpNum *= -1;
		num.push_back(helpNum);
		lastChar = '0';
	}
		

	if (oper.size() == 0) {
		if (modul)
			return abs(num.at(0));
		return num.at(0);
	}

	//korzystajac z wlasnosci znakow i kolejnosci zadan rozwiazanie problemu
	double wynik{};
	int i{0};
	while (num.size() > 1) {
		if ((i + 1 < oper.size() && PRN[oper.at(i + 1)] <= PRN[oper.at(i)]) || i + 1 >= oper.size()) {
			wynik = mp[oper.at(i)](num.at(i), num.at(i + 1));
			num.erase(num.begin() + i + 1);
			num.at(i) = wynik;
			oper.erase(oper.begin() + i);
			i = 0;
		}
		else {
			i++;
		}
	}
	if (modul)
		return abs(num.at(0));
	return num.at(0);
}

double dodawanie(double x, double y) {
	return x + y;
}

double odejmowanie(double x, double y) {
	return x - y;
}

double mnozenie(double x, double y) {
	return x * y;
}

double dzielenie(double x, double y) {
	if (y == 0) {
		throw 0;
	}
		
	return x / y;
}

double potegowanie(double x, double y) {
	if (x == 0 && y == 0) {
		throw 0;
	}
	return pow(x, y);
}