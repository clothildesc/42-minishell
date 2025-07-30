#!/bin/bash

# Script pour tester minishell avec Valgrind sans les fuites de readline

echo "=== Test Valgrind avec suppression des fuites readline ==="
echo "echo hello" | valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible --suppressions=readline.supp ./minishell

echo ""
echo "=== Test Valgrind complet (avec fuites readline) ==="
echo "echo hello" | valgrind --leak-check=full --show-leak-kinds=all ./minishell
