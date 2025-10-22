/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:46:04 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 13:36:50 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/BitcoinExchange.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << RED << "Usage: ./btc 'file'" << RESET << std::endl;
		std::cout << RED << "Error: could not open file." << RESET << std::endl;
		return 1;
	}
		
	BitcoinExchange btc;
	
	if (!btc.loadDatabase("data.csv"))
	{
		std::cout << RED << "Error: could not open file." << RESET << std::endl;
    	return 1;
	}
	
	if (!btc.processInput(argv[1]))
	{
		std::cout << RED << "Error: could not open file." << RESET << std::endl;
        return 1;
	}
	
	return 0;
}
