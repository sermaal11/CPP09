/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:26:18 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 20:06:39 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define CYAN    "\033[36m"
# define MAGENTA "\033[35m"
# define RESET   "\033[0m"

# include <string>
# include <iostream>
# include <stack>
# include <sstream>

class RPN 
{
private:
    std::stack<int> _stack; // para evaluar la expresión

public:
    RPN();                          // constructor
    RPN(const RPN& other);          // constructor de copia
    RPN& operator=(const RPN& other); // operador de asignación
    ~RPN();                         // destructor

    void evaluate(const std::string& input); // método principal
};

// Funciones auxiliares.
std::string trimSpaces(const std::string& expression);

#endif // RPN_HPP
