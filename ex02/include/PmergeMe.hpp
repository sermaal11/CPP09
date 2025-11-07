/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:01:34 by sergio            #+#    #+#             */
/*   Updated: 2025/11/07 13:06:08 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP
	
# include <stdexcept>
# include <vector>
# include <deque>
# include <string>

class PmergeMe {
public:
    // -------------------------------------------------------------------------
    // OCF — Constructores / Asignación / Destructor
    // -------------------------------------------------------------------------
    PmergeMe();                                     // ctor por defecto
    PmergeMe(int argc, char** argv);                // ctor con argumentos
    PmergeMe(const PmergeMe& other);                // ctor de copia
    PmergeMe& operator=(const PmergeMe& other);     // operador de asignación
    ~PmergeMe();                                   // destructor

    // -------------------------------------------------------------------------
    // Métodos públicos
    // -------------------------------------------------------------------------
    void run();

private:
    std::vector<int> _inputVec; // datos de entrada (enteros positivos)

    // -------------------------------------------------------------------------
    // Métodos estáticos auxiliares
    // -------------------------------------------------------------------------
    static bool isPositiveInt(const std::string& s);
    static int  toInt(const std::string& s);

    static std::vector<int> fordJohnsonVector(const std::vector<int>& in);
    static std::deque<int>  fordJohnsonDeque(const std::deque<int>& in);

    static long sortVectorTimed(const std::vector<int>& in, std::vector<int>& out);
    static long sortDequeTimed(const std::deque<int>& in, std::deque<int>& out);

    template <typename C>
    static void printLine(const char* prefix, const C& c);

    static void buildJacobsthalOrder(size_t nPend, std::vector<size_t>& order);

    static void binaryInsert(std::vector<int>& arr, int value, size_t upperBound);
    static void binaryInsert(std::deque<int>& arr, int value, size_t upperBound);
};

#endif // PMERGEME_HPP
