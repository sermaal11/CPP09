/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:26:09 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 20:07:26 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RPN.hpp"

int main(int argc, char **argv) 
{
	// Comprobamos is nos pasan dos argumentos.
    if (argc != 2) {
        std::cerr << RED << "Error: bad arguments." << RESET << std::endl;
        return 1;
    }

    try 
	{
        RPN calculator;
        calculator.evaluate(argv[1]);
    } 
	catch (const std::exception &e) 
	{
        std::cerr << RED << e.what() << RESET << std::endl;
    }

    return 0;
}
