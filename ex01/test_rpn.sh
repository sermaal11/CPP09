#!/bin/bash

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Contador de tests
TOTAL=0
PASSED=0
FAILED=0

# Función para ejecutar un test
run_test() {
    local description=$1
    local input=$2
    local expected=$3
    local should_fail=$4
    
    TOTAL=$((TOTAL + 1))
    
    echo -e "${BLUE}Test #$TOTAL:${NC} $description"
    echo -e "  Input: ${YELLOW}\"$input\"${NC}"
    
    if [ "$should_fail" = "error" ]; then
        # Esperamos que falle
        output=$(./RPN "$input" 2>&1)
        exit_code=$?
        
        if [ $exit_code -ne 0 ] || echo "$output" | grep -q "Error"; then
            echo -e "  ${GREEN}✓ PASS${NC} - Error detectado correctamente"
            echo -e "  Output: $output"
            PASSED=$((PASSED + 1))
        else
            echo -e "  ${RED}✗ FAIL${NC} - Debería haber dado error"
            echo -e "  Output: $output"
            FAILED=$((FAILED + 1))
        fi
    else
        # Esperamos que funcione
        output=$(./RPN "$input" 2>&1)
        exit_code=$?
        
        # Extraer solo el número del output (ignorar colores ANSI)
        result=$(echo "$output" | sed 's/\x1b\[[0-9;]*m//g' | tail -n 1)
        
        if [ "$result" = "$expected" ]; then
            echo -e "  ${GREEN}✓ PASS${NC} - Expected: $expected, Got: $result"
            PASSED=$((PASSED + 1))
        else
            echo -e "  ${RED}✗ FAIL${NC} - Expected: $expected, Got: $result"
            FAILED=$((FAILED + 1))
        fi
    fi
    echo ""
}

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}   RPN CALCULATOR - TEST SUITE${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

# ==========================================
# TESTS VÁLIDOS - Ejemplos del subject
# ==========================================
echo -e "${YELLOW}=== TESTS DEL SUBJECT ===${NC}"
echo ""

run_test "Example 1 from subject" "8 9 * 9 - 9 - 9 - 4 - 1 +" "42" "success"
run_test "Example 2 from subject" "7 7 * 7 -" "42" "success"
run_test "Example 3 from subject" "1 2 * 2 / 2 * 2 4 - +" "0" "success"

# ==========================================
# TESTS VÁLIDOS - Operaciones básicas
# ==========================================
echo -e "${YELLOW}=== OPERACIONES BÁSICAS ===${NC}"
echo ""

run_test "Simple addition" "1 2 +" "3" "success"
run_test "Simple subtraction" "5 3 -" "2" "success"
run_test "Simple multiplication" "3 4 *" "12" "success"
run_test "Simple division" "8 2 /" "4" "success"

# ==========================================
# TESTS VÁLIDOS - Números de un solo dígito
# ==========================================
echo -e "${YELLOW}=== NÚMEROS DE UN SOLO DÍGITO ===${NC}"
echo ""

run_test "All single digits (0-9)" "0 1 2 3 4 5 6 7 8 9 + + + + + + + + +" "45" "success"
run_test "Operations with 0" "0 5 +" "5" "success"
run_test "Zero result" "5 5 -" "0" "success"
run_test "Multiplication by zero" "9 0 *" "0" "success"

# ==========================================
# TESTS VÁLIDOS - Operaciones complejas
# ==========================================
echo -e "${YELLOW}=== OPERACIONES COMPLEJAS ===${NC}"
echo ""

run_test "Multiple operations chain" "2 3 + 4 *" "20" "success"
run_test "Complex expression 1" "1 2 + 3 + 4 +" "10" "success"
run_test "Complex expression 2" "9 8 * 7 6 * +" "114" "success"
run_test "Nested operations" "5 1 2 + 4 * + 3 -" "14" "success"
run_test "All operators" "2 3 + 4 * 5 - 6 /" "2" "success"

# ==========================================
# TESTS VÁLIDOS - Resultados negativos
# ==========================================
echo -e "${YELLOW}=== RESULTADOS NEGATIVOS ===${NC}"
echo ""

run_test "Negative result" "3 5 -" "-2" "success"
run_test "Negative intermediate values" "1 2 - 3 +" "2" "success"
run_test "Multiple negatives" "0 9 - 5 -" "-14" "success"

# ==========================================
# TESTS VÁLIDOS - División
# ==========================================
echo -e "${YELLOW}=== DIVISIÓN ===${NC}"
echo ""

run_test "Integer division (rounds down)" "7 2 /" "3" "success"
run_test "Division exact" "9 3 /" "3" "success"
run_test "Division result 1" "5 5 /" "1" "success"

# ==========================================
# TESTS DE ERROR - Argumentos inválidos
# ==========================================
echo -e "${YELLOW}=== ERRORES - ARGUMENTOS INVÁLIDOS ===${NC}"
echo ""

run_test "Parentheses (not allowed)" "(1 + 1)" "" "error"
run_test "No arguments" "" "" "error"
run_test "Only spaces" "   " "" "error"

# ==========================================
# TESTS DE ERROR - Tokens inválidos
# ==========================================
echo -e "${YELLOW}=== ERRORES - TOKENS INVÁLIDOS ===${NC}"
echo ""

run_test "Number with multiple digits" "12 3 +" "" "error"
run_test "Invalid operator %" "5 3 %" "" "error"
run_test "Invalid operator ^" "2 3 ^" "" "error"
run_test "Letters in expression" "a b +" "" "error"
run_test "Special characters" "1 @ 2" "" "error"
run_test "Decimal numbers" "1.5 2 +" "" "error"
run_test "Negative number as input" "-5 3 +" "" "error"

# ==========================================
# TESTS DE ERROR - Expresiones mal formadas
# ==========================================
echo -e "${YELLOW}=== ERRORES - EXPRESIONES MAL FORMADAS ===${NC}"
echo ""

run_test "Not enough operands" "1 +" "" "error"
run_test "Only operator" "+" "" "error"
run_test "Too many numbers" "1 2 3 +" "" "error"
run_test "Missing operand at start" "+ 1 2" "" "error"
run_test "Double operator" "1 2 + +" "" "error"
run_test "Operator before numbers" "+ 1 2" "" "error"

# ==========================================
# TESTS DE ERROR - División por cero
# ==========================================
echo -e "${YELLOW}=== ERRORES - DIVISIÓN POR CERO ===${NC}"
echo ""

run_test "Division by zero direct" "5 0 /" "" "error"
run_test "Division by zero from operation" "5 3 3 - /" "" "error"

# ==========================================
# TESTS DE EDGE CASES
# ==========================================
echo -e "${YELLOW}=== EDGE CASES ===${NC}"
echo ""

run_test "Multiple spaces between tokens" "1    2     +" "3" "success"
run_test "Tabs between tokens" "1	2	+" "3" "success"
run_test "Leading and trailing spaces" "  1 2 +  " "3" "success"
run_test "Maximum single digit" "9 9 *" "81" "success"
run_test "Minimum values" "0 0 +" "0" "success"
run_test "Long valid expression" "1 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +" "9" "success"

# ==========================================
# RESUMEN
# ==========================================
echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}   RESULTADOS${NC}"
echo -e "${BLUE}=====================================${NC}"
echo -e "Total tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}¡Todos los tests pasaron! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}Algunos tests fallaron 😞${NC}"
    exit 1
fi
