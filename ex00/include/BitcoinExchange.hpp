/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:46:15 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 12:56:57 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define CYAN    "\033[36m"
# define MAGENTA "\033[35m"
# define RESET   "\033[0m"

# include <string>
# include <iostream>
# include <fstream>
# include <ctime>
# include <map>
# include <iomanip>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _rates;
		
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		//Metodos
		bool loadDatabase(const std::string& csvPath);
		bool processInput(const std::string& inputPath);
};


std::string trimSpace(std::string str);
bool isValidDate(std::string date);
bool isValidExchangeRateDb(const std::string& exchangeRate);
bool isValidValue(const std::string& value);

#endif // BITCOINEXCHANGE_HPP
