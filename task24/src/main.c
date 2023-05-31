#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool evaluateFormula(char formula[], bool values[]);

// Функция для проверки эквивалентности логических формул
bool areEquivalent(char formula1[], char formula2[]) {
    // Найдем все пропозициональные переменные в формулах
    char variables[26] = {0}; // Предполагаем, что переменных не больше 26 (A-Z)
    int varCount = 0;

    for (int i = 0; i < strlen(formula1); i++) {
        if (formula1[i] >= 'A' && formula1[i] <= 'Z') {
            bool found = false;

            for (int j = 0; j < varCount; j++) {
                if (variables[j] == formula1[i]) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                variables[varCount] = formula1[i];
                varCount++;
            }
        }
    }

    for (int i = 0; i < strlen(formula2); i++) {
        if (formula2[i] >= 'A' && formula2[i] <= 'Z') {
            bool found = false;

            for (int j = 0; j < varCount; j++) {
                if (variables[j] == formula2[i]) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                variables[varCount] = formula2[i];
                varCount++;
            }
        }
    }

    // Переберем все возможные значения пропозициональных переменных
    int max = 1 << varCount; // 2^varCount

    for (int i = 0; i < max; i++) {
        bool values[26] = {0}; // Значения переменных (A-Z)
        int temp = i;

        for (int j = 0; j < varCount; j++) {
            values[variables[j] - 'A'] = temp % 2;
            temp /= 2;
        }

        // Вычислим значения формул для текущих значений переменных
        bool result1 = evaluateFormula(formula1, values);
        bool result2 = evaluateFormula(formula2, values);

        if (result1 != result2) {
            return false;
        }
    }

    return true;
}

// Функция для вычисления значения логической формулы с заданными значениями переменных
bool evaluateFormula(char formula[], bool values[]) {
    bool stack[100];
    int top = -1;

    for (int i = 0; i < strlen(formula); i++) {
        if (formula[i] >= 'A' && formula[i] <= 'Z') {
            stack[++top] = values[formula[i] - 'A'];
        } else if (formula[i] == '~') {
            bool operand = stack[top--];
            stack[++top] = !operand;
        } else if (formula[i] == '^' || formula[i] == 'v') {
            bool operand2 = stack[top--];
            bool operand1 = stack[top--];

            if (formula[i] == '^') {
                stack[++top] = operand1 && operand2;
            } else if (formula[i] == 'v') {
                stack[++top] = operand1 || operand2;
            }
        }
    }

    return stack[top];
}

// Функция для удаления избыточных скобок из формулы
void removeRedundantParentheses(char formula[]) {
    int len = strlen(formula);
    int openCount = 0;
    int closeCount = 0;

    // Подсчет количества открывающих и закрывающих скобок
    for (int i = 0; i < len; i++) {
        if (formula[i] == '(') {
            openCount++;
        } else if (formula[i] == ')') {
            closeCount++;
        }
    }

    // Удаление избыточных скобок
    while (openCount > closeCount) {
        for (int i = 0; i < len; i++) {
            if (formula[i] == '(') {
                formula[i] = ' ';
                openCount--;
                break;
            }
        }
    }

    while (closeCount > openCount) {
        for (int i = len - 1; i >= 0; i--) {
            if (formula[i] == ')') {
                formula[i] = ' ';
                closeCount--;
                break;
            }
        }
    }

    // Удаление пробелов
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (formula[i] != ' ') {
            formula[j] = formula[i];
            j++;
        }
    }
    formula[j] = '\0';
}

// Функция для проверки программы
void testProgram(char formula[]) {
    char formulaCopy[100];
    strcpy(formulaCopy, formula);

    printf("Исходная формула: %s\n", formula);

    removeRedundantParentheses(formula);

    printf("Формула без избыточных скобок: %s\n", formula);

    bool equivalent = areEquivalent(formulaCopy, formula);

    if (equivalent) {
        printf("Исходная формула эквивалентна формуле без скобок\n");
    } else {
        printf("Исходная формула не эквивалентна формуле без скобок\n");
    }

    printf("-----\n");
}

int main() {
    // Тестовые примеры
    testProgram("A OR (B AND C)");
    testProgram("((A AND B) OR C)");
    testProgram("(A OR B) AND C");
    testProgram("(((A)))");
    testProgram("(A OR B) OR C");
    testProgram("(A AND B) AND C");

    return 0;
}