/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sergio <sergio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 13:01:29 by sergio            #+#    #+#             */
/*   Updated: 2025/11/07 13:15:15 by sergio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <climits>
#include <sys/time.h>

// ============================================================================
//                              PARSING Y VALIDACIÓN
// ============================================================================

bool PmergeMe::isPositiveInt(const std::string& s)
{
    if (s.empty())
        return false;

    size_t i = 0;
    if (s[0] == '+')
    {
        if (s.size() == 1)
            return false;
        i = 1;
    }

    for (; i < s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }

    if (s == "0" || s == "+0")
        return false;

    return true;
}

int PmergeMe::toInt(const std::string& s)
{
    long long acc = 0;
    size_t i = 0;

    if (s[0] == '+')
        i = 1;

    for (; i < s.size(); i++)
    {
        acc = acc * 10 + (s[i] - '0');
        if (acc > INT_MAX)
            throw std::overflow_error("overflow");
    }

    if (acc <= 0)
        throw std::runtime_error("non-positive");

    return static_cast<int>(acc);
}

// ============================================================================
//                                  CONSTRUCTORES
// ============================================================================

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(int argc, char** argv)
{
    if (argc < 2)
        throw std::runtime_error("Error");

    _inputVec.reserve(static_cast<size_t>(argc - 1));

    for (int i = 1; i < argc; i++)
    {
        std::string tok(argv[i]);
        if (!isPositiveInt(tok))
            throw std::runtime_error("Error");

        int v = toInt(tok);
        _inputVec.push_back(v);
    }
}

PmergeMe::PmergeMe(const PmergeMe& other)
{
    _inputVec = other._inputVec;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
        _inputVec = other._inputVec;

    return *this;
}

PmergeMe::~PmergeMe()
{
}

// ============================================================================
//                                    MÉTODOS
// ============================================================================

template <typename C>
void PmergeMe::printLine(const char* prefix, const C& c)
{
    std::cout << prefix;

    typename C::const_iterator it = c.begin();
    for (; it != c.end(); ++it)
    {
        std::cout << *it;
        typename C::const_iterator next = it;
        ++next;
        if (next != c.end())
            std::cout << " ";
    }

    std::cout << std::endl;
}

void PmergeMe::run()
{
    printLine("Before: ", _inputVec);

    std::vector<int> outVec;
    std::deque<int>  outDeq;
    std::deque<int>  inDeq(_inputVec.begin(), _inputVec.end());

    long tVec = sortVectorTimed(_inputVec, outVec);
    long tDeq = sortDequeTimed(inDeq, outDeq);

    printLine("After:  ", outVec);

    std::cout.setf(std::ios::fixed);
	std::cout.precision(6);
	
	std::cout << "Time to process a range of " << _inputVec.size()
	          << " elements with std::vector<int> : "
	          << static_cast<double>(tVec) / 1000000.0 << " us" << std::endl;
	
	std::cout << "Time to process a range of " << _inputVec.size()
          << " elements with std::deque<int>  : "
          << static_cast<double>(tDeq) / 1000000.0 << " us" << std::endl;
}

// ============================================================================
//                          JACOBSTHAL + INSERCIÓN BINARIA
// ============================================================================

void PmergeMe::buildJacobsthalOrder(size_t nPend, std::vector<size_t>& order)
{
    order.clear();

    if (nPend == 0)
        return;

    // Siempre insertar primero el 1 (pend[0])
    order.push_back(1);

    if (nPend == 1)
        return;

    // Generación de bloques Jacobsthal (3, 5, 11, 21, ...)
    size_t j_prev = 1;
    size_t j = 3;

    while (j <= nPend)
    {
        // Inserta (j ... j_prev+1) en orden decreciente
        size_t idx = j;
        while (idx > j_prev)
        {
            order.push_back(idx);
            idx--;
        }

        size_t next = j + 2 * j_prev;
        j_prev = j;
        j = next;
    }

    // Bloque final parcial si el último j se pasó de nPend
    if (nPend > j_prev)
    {
        size_t idx = nPend;
        while (idx > j_prev)
        {
            order.push_back(idx);
            idx--;
        }
    }
}

void PmergeMe::binaryInsert(std::vector<int>& arr, int value, size_t upperBound)
{
    size_t lo = 0;
    size_t hi;

    if (upperBound > arr.size())
        hi = arr.size();
    else
        hi = upperBound;

    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;

        if (value <= arr[mid])
            hi = mid;
        else
            lo = mid + 1;
    }

    arr.insert(arr.begin() + static_cast<long>(lo), value);
}

void PmergeMe::binaryInsert(std::deque<int>& arr, int value, size_t upperBound)
{
    size_t lo = 0;
    size_t hi;

    if (upperBound > arr.size())
        hi = arr.size();
    else
        hi = upperBound;

    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;

        if (value <= arr[mid])
            hi = mid;
        else
            lo = mid + 1;
    }

    arr.insert(arr.begin() + static_cast<long>(lo), value);
}

// ============================================================================
//                         FORD-JOHNSON — std::vector
// ============================================================================

static void pairifyVector(const std::vector<int>& in,
                          std::vector< std::pair<int, int> >& pairs,
                          bool& hasStraggler,
                          int& straggler)
{
    pairs.clear();
    hasStraggler = false;
    straggler = 0;

    size_t n = in.size();
    size_t i = 0;

    for (; i + 1 < n; i += 2)
    {
        int a = in[i];
        int b = in[i + 1];

        if (a >= b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }

    if (i < n)
    {
        hasStraggler = true;
        straggler = in[i];
    }
}

static void sortPairsByFirstVector(std::vector< std::pair<int, int> >& pairs)
{
    for (size_t i = 1; i < pairs.size(); i++)
    {
        std::pair<int, int> key = pairs[i];
        long j = static_cast<long>(i) - 1;

        while (j >= 0 && pairs[j].first > key.first)
        {
            pairs[static_cast<size_t>(j + 1)] = pairs[static_cast<size_t>(j)];
            j--;
        }

        pairs[static_cast<size_t>(j + 1)] = key;
    }
}

std::vector<int> PmergeMe::fordJohnsonVector(const std::vector<int>& in)
{
    if (in.size() <= 1)
        return in;

    std::vector< std::pair<int, int> > pairs;
    bool hasStraggler = false;
    int straggler = 0;

    pairifyVector(in, pairs, hasStraggler, straggler);
    sortPairsByFirstVector(pairs);

    std::vector<int> mainChain;
    for (size_t i = 0; i < pairs.size(); i++)
        mainChain.push_back(pairs[i].first);

    std::vector<int> pend;
    for (size_t i = 0; i < pairs.size(); i++)
        pend.push_back(pairs[i].second);

    std::vector<size_t> order;
    buildJacobsthalOrder(pend.size(), order);

    for (size_t k = 0; k < order.size(); k++)
    {
        size_t idx1 = order[k];
        size_t i = idx1 - 1;
        int value = pend[i];
        size_t upperBound = i + 1;

        PmergeMe::binaryInsert(mainChain, value, upperBound);
    }

    if (hasStraggler)
        PmergeMe::binaryInsert(mainChain, straggler, mainChain.size());

    return mainChain;
}

// ============================================================================
//                         FORD-JOHNSON — std::deque
// ============================================================================

static void pairifyDeque(const std::deque<int>& in,
                         std::deque< std::pair<int, int> >& pairs,
                         bool& hasStraggler,
                         int& straggler)
{
    pairs.clear();
    hasStraggler = false;
    straggler = 0;

    size_t n = in.size();
    size_t i = 0;

    for (; i + 1 < n; i += 2)
    {
        int a = in[i];
        int b = in[i + 1];

        if (a >= b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }

    if (i < n)
    {
        hasStraggler = true;
        straggler = in[i];
    }
}

static void sortPairsByFirstDeque(std::deque< std::pair<int, int> >& pairs)
{
    for (size_t i = 1; i < pairs.size(); i++)
    {
        std::pair<int, int> key = pairs[i];
        long j = static_cast<long>(i) - 1;

        while (j >= 0 && pairs[static_cast<size_t>(j)].first > key.first)
        {
            pairs[static_cast<size_t>(j + 1)] = pairs[static_cast<size_t>(j)];
            j--;
        }

        pairs[static_cast<size_t>(j + 1)] = key;
    }
}

std::deque<int> PmergeMe::fordJohnsonDeque(const std::deque<int>& in)
{
    if (in.size() <= 1)
        return in;

    std::deque< std::pair<int, int> > pairs;
    bool hasStraggler = false;
    int straggler = 0;

    pairifyDeque(in, pairs, hasStraggler, straggler);
    sortPairsByFirstDeque(pairs);

    std::deque<int> mainChain;
    for (size_t i = 0; i < pairs.size(); i++)
        mainChain.push_back(pairs[i].first);

    std::deque<int> pend;
    for (size_t i = 0; i < pairs.size(); i++)
        pend.push_back(pairs[i].second);

    std::vector<size_t> order;
    buildJacobsthalOrder(pend.size(), order);

    for (size_t k = 0; k < order.size(); k++)
    {
        size_t idx1 = order[k];
        size_t i = idx1 - 1;
        int value = pend[i];
        size_t upperBound = i + 1;

        PmergeMe::binaryInsert(mainChain, value, upperBound);
    }

    if (hasStraggler)
        PmergeMe::binaryInsert(mainChain, straggler, mainChain.size());

    return mainChain;
}

// ============================================================================
//                                   TIMERS
// ============================================================================

static long nowMicros()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return static_cast<long>(tv.tv_sec) * 1000000L + static_cast<long>(tv.tv_usec);
}

long PmergeMe::sortVectorTimed(const std::vector<int>& in, std::vector<int>& out)
{
    long t0 = nowMicros();
    out = fordJohnsonVector(in);
    long t1 = nowMicros();

    return (t1 - t0);
}

long PmergeMe::sortDequeTimed(const std::deque<int>& in, std::deque<int>& out)
{
    long t0 = nowMicros();
    out = fordJohnsonDeque(in);
    long t1 = nowMicros();

    return (t1 - t0);
}