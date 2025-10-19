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
double test(std::string pomocy);
double evalRPN(std::vector<char>& A, std::vector<double>& numb);

int main()
{
	std::string inp{};
	
	do
	{
		std::cout << "Podaj problem" << std::endl;
		getline(std::cin, inp);
		std::cout << "= " << test(inp) <<  std::endl;
	} while (inp != "q" && inp != "Q");
	
	return 0;
}

// fukncja zmienia dane wjesciowe od uzytkownika na dwa wektory: numerow i operacji
double test(std::string pomocy) {
	std::vector<double> num{};
	std::vector<char> oper{};
	double helpNum{};
	double i{0};
	bool number = false;
	while (i < pomocy.length()) {
		if (pomocy[i] == ' ') {
			i++;
			continue;
		}
		if ((pomocy[i] >= 48 && pomocy[i] <= 57) || pomocy[i] == '.' || pomocy[i] == ',')
			number = true;
		if (number) {
			if (pomocy[i] != '.' && pomocy[i] != ',')
				helpNum = pomocy[i] - 48;
			else
				i--;
			double j = 1;
			i++;
			while (pomocy[i] != ' ' && pomocy[i] != ')' && pomocy[i] != '|' && i < pomocy.length()) {
				if (pomocy[i] == '.' || pomocy[i] == ',') {
					j = -1;
					i++;
				}
				if (j < 0) {
					helpNum += (pomocy[i] - 48) * pow(10, j);
					j--;
				}
				else {
					helpNum *= 10;
					helpNum += pomocy[i] - 48;
					j++;
				}
				i++;
			}
			number = false;
			num.push_back(helpNum);
			helpNum = 0;
		}
		else {
			if (pomocy[i] >= 48 && pomocy[i] <= 57)
				number = true;
			else {
				if (pomocy[i] == '-' && num.size() == 0)
					num.push_back(0);
				oper.push_back(pomocy[i]);
				i++;
			}
		}
	}

	return evalRPN(oper, num);
}

double evalRPN(std::vector<char>& oper, std::vector<double>& num)
{
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
		{')', 0},
		{'+', 1},
		{'-', 1},
		{'*', 2},
		{'/', 2},
		{'^', 3},
		{'(', 4},
		{'|', 0},
	};



	double wynik{ 0 };
	int poczNaw{ 0 }, poczModul{ 0 };
	int i{0};
	int iloscNaw{ 0 }, iloscModul{ 0 };
	bool modul = false;
	while(num.size() > 1) {
		if (oper.at(i) == '(') {
			poczNaw = fmax(poczNaw, i + 1);
			i++;
			iloscNaw++;
		}
		else if ((oper.at(i) == '|' && !modul && poczModul != i) || oper.at(i)  == ')'){
			
			oper.erase(oper.begin() + i);

			if (modul) {
				oper.erase(oper.begin() + poczModul - 1);
				num.at(poczModul - 1) = abs(num.at(poczModul - 1));
			}
			else {
				oper.erase(oper.begin() + poczNaw - 1);
			}
			
			modul = 0;
			poczModul = 0;
			iloscModul = 0;
			i = 0;
			poczNaw = 0;
			iloscNaw = 0;
			i = 0;
		}
		else if (oper.at(i) == '|') {
			modul = 1;
			poczModul = i + 1;
			iloscModul++;
			i++;
		}
		else if ((i + 1 < oper.size() && PRN[oper.at(i + 1)] <= PRN[oper.at(i)]) || i + 1 >= oper.size()) {
			wynik = mp[oper.at(i)](num.at(i - iloscNaw - iloscModul), num.at(i - iloscNaw - iloscModul + 1));
			num.erase(num.begin()  + i - iloscNaw - iloscModul + 1);
			num.at(i - iloscNaw - iloscModul) = wynik;
			oper.erase(oper.begin() + i);
			i = fmax(poczModul, poczNaw);
		}
		else {
			i++;
		}	
	}
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
	if (y == 0)
		return 0;
	return x / y;
}

double potegowanie(double x, double y) {
	if (x == 0 && y == 0)
		return 0;
	if (y == 0)
		return 1;
	return pow(x, y);
}