#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>
#include <iomanip>

int daysOfTheMonth[12]{31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void add_name_birthday_to_print(std::vector<std::string>& printNames,
	std::vector<std::tm>& printBirthdays, const std::string& name, const std::tm& birthday)
{
	if (printNames.empty())
	{
		printNames.push_back(name);
		printBirthdays.push_back(birthday);
	}
	else
	{
		printBirthdays.clear();
		printNames.clear();
		printNames.push_back(name);
		printBirthdays.push_back(birthday);
	}
}

void coming_birthdays(const std::vector<std::string>& names, const std::vector<std::tm>& birthdays)
{
	std::time_t currentTime = std::time(nullptr);
	std::tm currentDate = *std::localtime(&currentTime);
	std::tm nearestBirthday = birthdays[0];

	int nameIt = 0;
	bool isTrue = false;
	std::string nameNearestBirthday = names[0];

	std::vector<std::string> printNames;
	std::vector<std::tm> printBirthdays;

	for (int i = 1; i < birthdays.size(); ++i)
	{
		if (currentDate.tm_mon <= birthdays[i].tm_mon)
		{
			if (birthdays[i].tm_mon < nearestBirthday.tm_mon)
			{
				nameIt = i;
				nameNearestBirthday = names[i];
				nearestBirthday = birthdays[i];
				add_name_birthday_to_print(printNames, printBirthdays, names[i], nearestBirthday);
				isTrue = true;
			}
			else if (currentDate.tm_mon > nearestBirthday.tm_mon)
			{
				nameIt = i;
				nameNearestBirthday = names[i];
				nearestBirthday = birthdays[i];
				add_name_birthday_to_print(printNames, printBirthdays, names[i], nearestBirthday);
				isTrue = true;
			}
			else if (nearestBirthday.tm_mon == birthdays[i].tm_mon)
			{
				if (currentDate.tm_mday <= birthdays[i].tm_mday)
				{
					if (birthdays[i].tm_mday < nearestBirthday.tm_mday)
					{
						nameIt = i;
						nameNearestBirthday = names[i];
						nearestBirthday = birthdays[i];
						add_name_birthday_to_print(printNames, printBirthdays, names[i], nearestBirthday);
						isTrue = true;
					}
					else if (birthdays[i].tm_mday == nearestBirthday.tm_mday)
					{
						if (printNames.empty())
						{
							printNames.push_back(names[i]);
							printBirthdays.push_back(birthdays[i]);
							printNames.push_back(names[nameIt]);
							printBirthdays.push_back(nearestBirthday);
						}
						else
						{
							if (isTrue)
							{
								printNames.clear();
								printBirthdays.clear();
								isTrue = false;
							}
							if (!printNames.empty())
							{
								printNames.push_back(names[i]);
								printBirthdays.push_back(birthdays[i]);
							}
							else
							{
								printNames.push_back(names[i]);
								printBirthdays.push_back(birthdays[i]);
								printNames.push_back(names[nameIt]);
								printBirthdays.push_back(nearestBirthday);
							}
						}
					}
				}
			}

		}
	}

	system("cls");
	std::cout << "Nearest birthday: " << std::endl;
	for (int i = 0; i < printBirthdays.size(); ++i)
	{
		std::cout << "Name: " << printNames[i] << "\nBirthday: " << std::setfill('0') << std::setw(2) <<
			printBirthdays[i].tm_mon + 1 << "/" << std::setfill('0') << std::setw(2) << printBirthdays[i].tm_mday <<std::endl;
		if (currentDate.tm_mon == printBirthdays[i].tm_mon && currentDate.tm_mday == printBirthdays[i].tm_mday)
			std::cout << "Birthday today. Don't forget to congratulate! " << std::endl;
	}
}

bool is_correct_name(const std::string& name)
{
	if (name.empty()) return false;

	for (int i = 1; i < name.length(); ++i)
	{
		if (name[0] < 'A' || name[0] > 'Z' || 
			name[i] < 'a' || name[i] > 'z') return false;
	}

	return true;
}

bool is_correct_year(const int& year)
{
	if (year < 1000 || year > 2023)
		return false;

	if (year % 4 == 0) daysOfTheMonth[1] = 29;
	else daysOfTheMonth[1] = 28;

	return true;
}

bool is_correct_month(const int& month)
{
	if (month < 1 || month > 12) return false;

	return true;
}

bool is_correct_day(const int& day, const int& month)
{
	if (day < 1 || day > daysOfTheMonth[month - 1]) return false;

	return true;
}

bool is_correct_birthday(const std::string& birthday)
{
	int count = 0; int temp = 0;
	int year, month, day;

	if (birthday.empty()) return false;

	for (int i = 0; i < birthday.length(); ++i)
	{
		if (birthday[i] == '/')
		{
			++count;
			if (count > 2) return false;

			if (temp == 0)
				year = std::stoi(birthday.substr(temp, i));

			if (temp != 0)
				month = std::stoi(birthday.substr(temp + 1, i - (temp + 1)));

			temp = i;
		}
		if ((birthday[i] < '0' || birthday[i] > '9') &&
			birthday[i] != '/') return false;
	}

	if (count < 2) return false;
	day = std::stoi(birthday.substr(temp + 1, (birthday.length() - temp)));

	if (!is_correct_year(year)) return false;
	if (!is_correct_month(month)) return false;
	if (!is_correct_day(day, month)) return false;

	return true;
}

int main()
{
	std::string name, birthday;
	std::vector<std::string> names;
	std::vector<std::tm> birthdays;

	std::time_t t = std::time(nullptr);

	while (true)
	{
		std::cout << "Enter the name: ";
		std::cin >> name;

		if (name == "End" || name == "end")
		{
			coming_birthdays(names, birthdays);
			return 0;
		}

		if (!is_correct_name(name))
		{
			std::cerr << "Error! Incorrect input data. Try again." << std::endl;
			continue;
		}

		std::cout << "Input format: \nBirthday: year/month/day." << std::endl;
		std::cout << "Enter the date of your birthday: ";
		std::cin >> birthday;

		if (!is_correct_birthday(birthday))
		{
			std::cerr << "Error! Incorrect input data. Try again." << std::endl;
			continue;
		}
		else
		{
			std::tm local = *std::localtime(&t);
			std::stringstream born(birthday);
			born >> std::get_time(&local, "%Y/%m/%d");

			names.push_back(name);
			birthdays.push_back(local);
		}
	}
}
