/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:26:12 by sergio            #+#    #+#             */
/*   Updated: 2025/10/22 20:07:46 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/RPN.hpp"

// OCF
RPN::RPN() {}

RPN::RPN(const RPN& other) 
{
	*this = other;
}

RPN& RPN::operator=(const RPN& other) 
{
	if (this != &other)
        this->_stack = other._stack;
    return *this;
}

RPN::~RPN() {}

//Metodos
void RPN::evaluate(const std::string& input)
{
	// Eliminamos espacios del inicio y del final de la cadena
	std::string expression = trimSpaces(input);
	
	// Comprobamos si la cadena de entrada esta vacia.
	if (expression.empty())
		throw std::runtime_error("Error: void argv[1] argument");

	// Dejamos la pila vacia por si hay algo de una evaluacion anterior
	while (!_stack.empty())
		_stack.pop();
	
	// Convertimos la expresion en un flujo legible, palabra por palabra
	std::istringstream stream(expression);
	
	// Declaramos una variable para guardar cada token leido
	std::string token;

	// En un bucle, leemos todos los tokens separados por espacios o por tabs
	while (stream >> token)
	{
		// Verificamos si el token es un digito de una sola cifra
		if (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
		{
			_stack.push(token[0] - '0');
			continue;
		}
		
		// Verificamos si es un operador valido ("+ - / *")
		if (token.size() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'))
		{
			if (_stack.size() < 2)
				throw std::runtime_error("Error: bad expression (not enough operands)");
			
			int a = _stack.top();
			_stack.pop();
			int b = _stack.top();
			_stack.pop();

			int result = 0;
			switch (token[0])
			{
				case '+': result = b + a; 
					break;
        		case '-': result = b - a;
					break;
        		case '*': result = b * a;
					break;
				case '/':
					if (a == 0)
						throw std::runtime_error("Error: division by zero");
					result = b / a;
					break;
			}
			_stack.push(result);
    		continue;
		}
		// si no es ni número de 1 dígito ni operador válido -> error
		throw std::runtime_error(std::string("Error: invalid token '") + token + "'");	
	}
	
	// Comprobación final (modo "calculadora": un solo número es válido)
	if (_stack.empty())
    	throw std::runtime_error("Error: bad expression (no result)");

	if (_stack.size() > 1)
    	throw std::runtime_error("Error: bad expression (too many values)");

	// Resultado OK
	std::cout << GREEN << _stack.top() << RESET << std::endl;
}

// Funciones auxiliares
std::string trimSpaces(const std::string& str)
{
	size_t start = str.find_first_not_of(" \t");
	size_t end = str.find_last_not_of(" \t");
	
	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}
