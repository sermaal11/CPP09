/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:46:14 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 13:05:53 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/BitcoinExchange.hpp"

// OCF
BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
        this->_rates = other._rates;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// Metodos
bool BitcoinExchange::loadDatabase(const std::string& csvPath)
{
	// Abrir "data.csv"
	std::ifstream csvDb(csvPath.c_str());
	if (!csvDb)
	{
		std::cout << RED << "Error: could not open file 'data.csv'." << RESET << std::endl;
		return false;
	}

	std::string header;
	if (!std::getline(csvDb, header))
	{
	    std::cout << RED << "Error: 'data.csv' is empty." << RESET << std::endl;
	    return false;
	}
	
	// Verificar que la primera línea contenga las palabras esperadas
	if (header.find("date") == std::string::npos || header.find("exchange_rate") == std::string::npos)
	{
	    std::cout << RED << "Error: missing header 'date,exchange_rate' in 'data.csv'." << RESET << std::endl;
	    return false;
	}

	std::string line;
	while (getline(csvDb, line))
	{
		// Saltar cabecera
		if ((line.find("date") != std::string::npos) || (line.find("exchange_rate") != std::string::npos))
			continue;
			
		// Procesar linea
		if (line.find(",") != std::string::npos)
		{
			// localizamos la poscion de la ','
			int pos = line.find(",");
			// Extraemos la date
			std::string date = line.substr(0, pos);
			// Extraemos 'exchange_rate'
			std::string exchangeRate = line.substr(pos + 1);
			// Limpiamos espacios de date;
			date = trimSpace(date);
			// Limpiamos espacios de exchangeRate 
			exchangeRate = trimSpace(exchangeRate);
			// si 'date' || 'exchangeRate' == "" -> Salimos
			if (date.empty() || exchangeRate.empty())
			{
				std::cout << YELLOW << "Warning: Invalid format line, 'date' or 'exchange_rate' is void: " << line << RESET << std::endl;
				std::cout << RED << "Error: Invalid 'data.csv'" << RESET << std::endl;
				return false;
			}
			// Validamos fecha dentro de data.csv
			if (!isValidDate(date))
			{
				std::cout << RED << "Error: Invalid 'data.csv'" << RESET << std::endl;
				return false;
			}
			// Validamos exchangeRate dentro de data.csv
			if (!isValidExchangeRateDb(exchangeRate))
			{
				std::cout << RED << "Error: Invalid 'data.csv'" << RESET << std::endl;
				return false;
			}
			double rate = std::atof(exchangeRate.c_str());
			_rates[date] = rate;
			// ! std::cout << CYAN << "Loaded: " << date << " => " << rate << RESET << std::endl;
		}
		// Si no encontramos una ',' en la line, -> Salimos
		else
		{
			std::cout << YELLOW << "Warning: Invalid format line, ',' not found: " << line << RESET << std::endl;
			std::cout << RED << "Error: Invalid 'data.csv'" << RESET << std::endl;
			return false;
		}
	}
	return true;
}

bool BitcoinExchange::processInput(const std::string& inputPath)
{
	// Verificar extensión .txt
	if (inputPath.size() < 5 || inputPath.substr(inputPath.size() - 4) != ".txt")
	{
		std::cout << RED << "Error: Input file extension." << RESET << std::endl;
		return false;
	}

	// Abrir el archivo
	std::ifstream inputFile(inputPath.c_str());
	if (!inputFile)
	{
		std::cout << RED << "Error: could not open input file." << RESET << std::endl;
		return false;
	}

	// Leer primera línea (cabecera)
	std::string header;
	if (!std::getline(inputFile, header))
	{
		std::cout << RED << "Error: input file is empty." << RESET << std::endl;
		return false;
	}

	// Verificar formato exacto de cabecera
	if (header.find("date") == std::string::npos || header.find("value") == std::string::npos)
	{
		std::cout << RED << "Error: missing header 'date | value' in input file." << RESET << std::endl;
		return false;
	}

    std::string line;
    while (std::getline(inputFile, line))
    {
        if ((line.find("date") != std::string::npos) || (line.find("value") != std::string::npos))
			continue;
		
		// Procesar linea
		if (line.find("|") != std::string::npos)
		{
			// localizamos la poscion de '|'
			int pos = line.find("|");
			// Extraemos la date
			std::string date = line.substr(0, pos);
			// Extraemos 'value'
			std::string value = line.substr(pos + 1);
			// Limpiamos espacios de date;
			date = trimSpace(date);
			// Limpiamos espacios de exchangeRate 
			value = trimSpace(value);
			// si 'date' || 'value' == "" -> Salimos
			if (date.empty() || value.empty())
			{
				std::cout << YELLOW << "Warning: Invalid format line, 'date' or 'value' is void: " << line << RESET << std::endl;
				std::cout << RED << "Error: Invalid 'file.txt'" << RESET << std::endl;
				return false;
			}
			// Validamos fecha dentro de file.txt
			if (!isValidDate(date)) 
			{
    			std::cout << "Error: bad input => " << line << std::endl;
    			continue;
			}
			// Validamos value dentro de file.txt
			if (!isValidValue(value))
				continue;
	
			double amount = std::atof(value.c_str());

			// buscar el rate en la base de datos (_rates)
			std::map<std::string, double>::iterator it = _rates.lower_bound(date);
			if (_rates.empty()) 
			{
    			std::cout << "Error: database is empty." << std::endl;
    			continue;
			}
					
			// si no existe una fecha igual o posterior
			if (it == _rates.begin() && it->first != date)
			{
				std::cout << "Error: no earlier date available for " << date << std::endl;
				continue;
			}
			
			// si no hay coincidencia exacta, retrocede una posición
			if (it == _rates.end() || it->first != date)
				--it;
			
			double rate = it->second;

			// Calcular e imprimir resultado
			double result = amount * rate;
			
			// Determinar si amount es entero
			bool amountIsInt = (amount == static_cast<long>(amount));
			// Determinar si result es entero o tiene decimales significativos
			bool resultIsInt = (result == static_cast<long>(result));
			
			// Imprimir amount
			std::cout << date << " => ";
			if (amountIsInt)
				std::cout << static_cast<long>(amount);
			else
				std::cout << amount;
			
			// Imprimir result
			std::cout << " = ";
			if (resultIsInt)
				std::cout << static_cast<long>(result);
			else
				std::cout << result;
			std::cout << std::endl;

		}
		else
		{
			std::cout << "Error: bad input => " << line << std::endl;
		}
    }

    return true;
}

std::string trimSpace(std::string str)
{
	size_t start = str.find_first_not_of(" ");
	size_t end = str.find_last_not_of(" ");
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

static bool isLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(std::string date)
{
	// Validar formato "YYYY-MM-DD"
	if (date.size() != 10)
		return false;
	if (!(date[4] == '-' && date[7] == '-'))
		return false;
	for (size_t i = 0; i < 10; ++i) 
	{
        if (i == 4 || i == 7) 
			continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) 
            return false;
	}

	// Extraer YYYY, MM, DD y convertir a int
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	
	// Validacion de YYYY
	if (year < 2009)
    	return false;
	std::time_t t = std::time(NULL);
	std::tm* now = std::localtime(&t);
	int currentYear = now->tm_year + 1900;
	if (year > currentYear)
    	return false;
	
	// Validacion de MM
	if (month < 1 || month > 12)
		return false;

	// Validacion de DD
	int mdays;
	switch (month) 
	{
	    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
	        mdays = 31; 
				break;
	    case 4: case 6: case 9: case 11:
	        mdays = 30; 
				break;
	    case 2:
    		if (isLeap(year))
        		mdays = 29;
    		else
        		mdays = 28;
    		break;
	    default:
	        mdays = 31;
	}
	if (day < 1 || day > mdays)
		return false;
	return true;
}

bool isValidExchangeRateDb(const std::string& exchangeRate)
{
	size_t dot = 0;
	for(size_t i = 0; i < exchangeRate.size(); i++)
	{
		if (exchangeRate[i] == '.')
			dot++; 
		if (!isdigit(exchangeRate[i]) && exchangeRate[i] != '.')
		{
			std::cout << YELLOW << "Warning: Invalid format exchange_rate: " << exchangeRate << RESET << std::endl;
			return false;
		}
	}
	if (dot > 1)
	{
		std::cout << YELLOW << "Warning: Invalid format exchange_rate: " << exchangeRate << RESET << std::endl;
		return false;
	}
	double rate = std::atof(exchangeRate.c_str());
	if (rate < 0)
	{
		std::cout << YELLOW
				  << "Warning: exchange_rate is negative (" << exchangeRate << ")"
				  << RESET << std::endl;
		return false;
	}
	return true;
}

bool isValidValue(const std::string& value)
{
	if (value.empty())
		return false;
	
	// Primero verificar si tiene '-' (número negativo)
	if (value.find('-') != std::string::npos)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}
	
	size_t dot = 0;
	// Permitir + al principio (p. ej. "+42")
	size_t start = (value[0] == '+') ? 1 : 0;

	for (size_t i = start; i < value.size(); ++i)
	{
		if (value[i] == '.')
			dot++;
		else if (!isdigit(value[i]))
			return false;
	}

	if (dot > 1)
		return false;

	double num = std::atof(value.c_str());

	if (num < 0)
	{
		std::cout << "Error: not a positive number." << std::endl;
		return false;
	}

	if (num > 1000)
	{
		std::cout << "Error: too large a number." << std::endl;
		return false;
	}

	return true;
}