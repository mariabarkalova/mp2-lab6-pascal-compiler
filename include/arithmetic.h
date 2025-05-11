#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "stack.h"
#include < set >

using namespace std;

class PascalLineByLine {
    vector<string> programLines;
    size_t currentLine = 0;
    bool inBeginBlock = false;

    map<string, double> variables;
    map<string, bool> varTypes; // true - real, false - integer

    // Приоритеты операторов
    const map<string, int> opPriority = {
        {"or", 1}, {"and", 2},
        {"=", 3}, {"<>", 3}, {"<", 3}, {">", 3}, {"<=", 3}, {">=", 3},
        {"+", 4}, {"-", 4}, {"*", 5}, {"/", 5}, {"div", 5}, {"mod", 5}
    };

public:
    PascalLineByLine(const vector<string>& program) : programLines(program) {}

    void run() {
        while (currentLine < programLines.size()) {
            processLine(programLines[currentLine]);
            currentLine++;
        }
    }

private:
    void processLine(string line) {

        // Удаляем лишние пробелы
        line = delSpaces(line);
        if (line.empty()) return;

        // Обработка разных типов строк
        if (line.find("var") == 0) {
            processVarDeclaration(line);
        }
        else if (line == "begin") {
            inBeginBlock = true;
        }
        else if (line == "end" || line == "end.") {
            inBeginBlock = false;
        }
        else if (inBeginBlock) {
            processStatement(line);
        }
    }

    void processVarDeclaration(const string& line) {
        // Удаляем "var" в начале строки
        string content = line.substr(3);
        content = delSpaces(content);

        // Разбиваем на отдельные объявления по точкам с запятой
        vector<string> declarations;
        size_t start = 0;
        size_t end = content.find(';');

        while (end != string::npos) {
            declarations.push_back(delSpaces(content.substr(start, end - start)));
            start = end + 1;
            end = content.find(';', start);
        }

        // Обрабатываем каждое объявление
        for (const auto& decl : declarations) {
            size_t colonPos = decl.find(':');
            if (colonPos == string::npos) continue;

            // Получаем часть с переменными
            string varsPart = delSpaces(decl.substr(0, colonPos));
            // Получаем тип
            string type = delSpaces(decl.substr(colonPos + 1));

            // Разбиваем переменные по запятым
            vector<string> varNames;
            size_t varStart = 0;
            size_t varEnd = varsPart.find(',');

            while (varEnd != string::npos) {
                string varName = delSpaces(varsPart.substr(varStart, varEnd - varStart));
                if (!varName.empty()) {
                    varNames.push_back(varName);
                }
                varStart = varEnd + 1;
                varEnd = varsPart.find(',', varStart);
            }
            // Добавляем последнюю переменную
            string lastName = delSpaces(varsPart.substr(varStart));
            if (!lastName.empty()) {
                varNames.push_back(lastName);
            }

            // Определяем тип (real/integer)
            bool isReal = (type == "real" || type == "double");

            // Добавляем переменные в хранилище
            for (const auto& var : varNames) {
                variables[var] = 0.0;       
                varTypes[var] = isReal;     
            }
        }
    }
    void processStatement(const string& stmt) {
        if (stmt.find("writeln") == 0) {
            processWriteln(stmt);
        }
        else if (stmt.find("readln") == 0) {
            processReadln(stmt);
        }
        else if (stmt.find("if") == 0) {
            processIf(stmt);
        }
        else if (stmt.find(":=") != string::npos) {
            processAssignment(stmt);
        }

    }

    void processWriteln(const string& stmt) {
        size_t start = stmt.find('(');
        size_t end = stmt.rfind(')');

        if (start != string::npos && end != string::npos) {
            string content = stmt.substr(start + 1, end - start - 1);
            vector<string> parts = splitArguments(content);

            string output;
            for (const string& part : parts) {
                if (part[0] == '\'') {
                    // Строковый литерал
                    output += part.substr(1, part.size() - 2);
                }
                else {
                    // Выражение
                    double val = evaluateExpression(part);
                    if (variables.count(part) && !varTypes[part]) {
                        output += to_string(static_cast<int>(val));
                    }
                    else {
                        output += to_string(val);
                    }
                }
            }
            cout << output << endl;
        }
    }

    void processReadln(const string& stmt) {
        size_t start = stmt.find('(');
        size_t end = stmt.rfind(')');

        if (start != string::npos && end != string::npos) {
            string varName = stmt.substr(start + 1, end - start - 1);
            varName = delSpaces(varName);

            if (variables.count(varName)) {
                cout << "Enter value for " << varName << ": ";
                double value;
                cin >> value;

                if (!varTypes[varName]) {
                    variables[varName] = static_cast<int>(value);
                }
                else {
                    variables[varName] = value;
                }
            }
        }
    }

    void processIf(const string& stmt) {
        size_t thenPos = stmt.find("then");
        if (thenPos == string::npos) return;
        size_t endPos;
        size_t onelsePos;
        size_t offelsePos = 0;
        // Извлекаем условие
        string cond = delSpaces(stmt.substr(2, thenPos - 2));
        bool conditionResult = evaluateCondition(cond);

        // Обрабатываем then-часть
        string thenPart = delSpaces(stmt.substr(thenPos + 4));

        if (conditionResult) {
            // Обработка then-блока
            if (thenPart.find("begin") == 0) {
                currentLine++; // Переходим на строку после begin
                while (currentLine < programLines.size() &&
                    delSpaces(programLines[currentLine]) != "end") {
                    processLine(programLines[currentLine]);
                    currentLine++;
                }
               
                if ((currentLine < programLines.size()) && delSpaces(programLines[currentLine]) != "end")
                {
                    endPos = currentLine;   
                }
            }
            else {
                processStatement(thenPart);
                endPos = currentLine;
            }

        }
        else {
            // Поиск else только в пределах текущего блока
            size_t originalLine = currentLine;
            bool elseFound = false;

            // Ищем else после then-части
            for (size_t i = currentLine + 1; i < programLines.size(); i++) {
                string line = delSpaces(programLines[i]);

                // Проверяем на наличие else
                if (line.find("else") == 0) {
                    elseFound = true;
                    onelsePos = i;
                    currentLine = i;
                    string elsePart;
                    if(line.size() != 4)
                    elsePart = delSpaces(line.substr(4));
                    else {
                        i++;
                       
                        elsePart = delSpaces(programLines[i]);
                    }

                    // Обработка else-блока
                    if (elsePart.find("begin") == 0) {
                        currentLine++;
                        while (currentLine < programLines.size() &&
                            delSpaces(programLines[currentLine]) != "end") {
                            processLine(programLines[currentLine]);
                            currentLine++;
                        }
                      
                        if ((currentLine < programLines.size()) && delSpaces(programLines[currentLine]) != "end")
                            offelsePos = currentLine;
                    }
                    else {
                        processStatement(elsePart);
                        offelsePos = currentLine;
                    }
                    break;
                }
            }
                if (!elseFound) {
                  currentLine = endPos +1;
                }
            }
        if ((offelsePos == 0))
        {
            bool elseFound = false;
            for (size_t i = currentLine + 1; i < programLines.size(); i++) {
                string line = delSpaces(programLines[i]);
                
                // Проверяем на наличие else
                if (line.find("else") == 0) {
                    onelsePos = i;
                    elseFound = true;
                    currentLine = i;
                    string elsePart;
                    if (line.size() != 4)
                        elsePart = delSpaces(line.substr(4));
                    else {
                        i++;

                        elsePart = delSpaces(programLines[i]);
                    }
                    // Обработка else-блока
                    if (elsePart.find("begin") == 0) {
                        currentLine++;
                        while (currentLine < programLines.size() &&
                            delSpaces(programLines[currentLine]) != "end") {
                            currentLine++;
                        }

                        if ((currentLine < programLines.size()) && delSpaces(programLines[currentLine]) != "end")
                            offelsePos = currentLine;
                    }
                    else {
                        offelsePos = currentLine;
                    }
                    break;
                }
                
            }
            if (!elseFound) {
                currentLine = endPos + 1;
            }
            else
            currentLine = offelsePos + 1;
        }
        else currentLine = offelsePos + 1;
                
    }

    void processAssignment(const string& stmt) {
        size_t assignPos = stmt.find(":=");
        if (assignPos == string::npos) return;

        // Извлекаем имя переменной
        string varName = delSpaces(stmt.substr(0, assignPos));

        // Проверяем, объявлена ли переменная
        if (variables.find(varName) == variables.end()) {
            throw runtime_error("Undeclared variable: " + varName);
        }

        // Извлекаем выражение
        string expr = delSpaces(stmt.substr(assignPos + 2));
        double value = evaluateExpression(expr);

         // Присваиваем с учетом типа
         if (!varTypes[varName]) {
             variables[varName] = static_cast<int>(value); // Для integer
         }
         else {
             variables[varName] = value; // Для real/double
         }
        
       
    }

    double evaluateExpression(const string& expr) {
        vector<string> lexems = parseLexems(expr);
        vector<string> postfix = infixToPostfix(lexems);
        return evaluatePostfix(postfix);
    }

    bool evaluateCondition(const string& cond) {
        
        vector<string> lexems = parseLexems(cond);
        vector<string> postfix = infixToPostfix(lexems);
        double result = evaluatePostfix(postfix);

        return result != 0.0;
    }

    // Вспомогательные функции
    static string delSpaces(const string& s) {
        auto start = s.begin();
        while (start != s.end() && isspace(*start)) start++;

        auto end = s.end();
        do {
            end--;
        } while (distance(start, end) > 0 && isspace(*end));

        return string(start, end + 1);
    }

    vector<string> splitArguments(const string& s) {
        vector<string> args;
        bool inString = false;
        string current;

        for (char c : s) {
            if (c == '\'') {
                inString = !inString;
                current += c;
                if (!inString) {
                    args.push_back(current);
                    current.clear();
                }
            }
            else if (inString) {
                current += c;
            }
            else if (c == ',') {
                if (!current.empty()) {
                    args.push_back(current);
                    current.clear();
                }
            }
            else if (!isspace(c)) {
                current += c;
            }
        }

        if (!current.empty()) {
            args.push_back(current);
        }

        return args;
    }

    vector<string> parseLexems(const string& expr) {
        vector<string> lexems;
        string current;
        bool inString = false;

        for (char c : expr) {
            if (c == '\'') {
                inString = !inString;
                current += c;
                if (!inString) {
                    lexems.push_back(current);
                    current.clear();
                }
                continue;
            }

            if (inString) {
                current += c;
                continue;
            }

            if (isspace(c)) {
                if (!current.empty()) {
                    lexems.push_back(current);
                    current.clear();
                }
                continue;
            }

            if (isalnum(c) || c == '_' || c == '.') {
                current += c;
            }
            else {
                if (!current.empty()) {
                    lexems.push_back(current);
                    current.clear();
                }
                lexems.push_back(string(1, c));
            }
        }

        if (!current.empty()) {
            lexems.push_back(current);
        }

        // Объединяем multi-character operators и keywords
        for (size_t i = 0; i < lexems.size(); ) {
            // Обработка mod
            if (i + 2 < lexems.size() &&
                lexems[i] == "m" && lexems[i + 1] == "o" && lexems[i + 2] == "d") {
                lexems[i] = "mod";
                lexems.erase(lexems.begin() + i + 1, lexems.begin() + i + 3);
            }
            // Обработка div
            else if (i + 2 < lexems.size() &&
                lexems[i] == "d" && lexems[i + 1] == "i" && lexems[i + 2] == "v") {
                lexems[i] = "div";
                lexems.erase(lexems.begin() + i + 1, lexems.begin() + i + 3);
            }
            // Обработка составных операторов сравнения
            else if (i + 1 < lexems.size()) {
                string combined = lexems[i] + lexems[i + 1];
                if (combined == "<=" || combined == ">=" || combined == "<>" ||
                    combined == ":=") {
                    lexems[i] = combined;
                    lexems.erase(lexems.begin() + i + 1);
                }
                else {
                    i++;
                }
            }
            else {
                i++;
            }
        }

        return lexems;
    }

    vector<string> infixToPostfix(const vector<string>& lexems) {
        vector<string> output;
        TVectorStack<string> opStack;

        for (const string& lexem : lexems) {
            // Пропускаем пустые токены
            if (lexem.empty() || lexem == ";") continue;

            // Проверяем, является ли токен оператором (ищем в opPriority)
            if (opPriority.find(lexem) != opPriority.end()) {
                // Это оператор - обрабатываем по приоритету
                while (!opStack.IsEmpty() && opStack.Top() != "(" &&
                    opPriority.at(opStack.Top()) >= opPriority.at(lexem)) {
                    output.push_back(opStack.Pop());
                }
                opStack.Push(lexem);
            }
            else if (lexem == "(") {
                opStack.Push(lexem);
            }
            else if (lexem == ")") {
                while (!opStack.IsEmpty() && opStack.Top() != "(") {
                    output.push_back(opStack.Pop());
                }
                opStack.Pop(); // Удаляем "("
            }
            else if (isalnum(lexem[0]) || lexem[0] == '_') {
                // Это число или переменная
                output.push_back(lexem);
            }
            else {
                throw runtime_error("Unknown lexem: " + lexem);
            }
        }

        // Выталкиваем оставшиеся операторы из стека
        while (!opStack.IsEmpty()) {
            output.push_back(opStack.Pop());
        }

        return output;
    }

    double evaluatePostfix(const vector<string>& postfix) {
        TVectorStack<double> stack;

        for (const string& lexem : postfix) {
            // Пропускаем пустые токены и символы ';'
            if (lexem.empty() || lexem == ";") {
                continue;
            }

            // Проверяем, является ли токен оператором (ищем в opPriority)
            auto it = opPriority.find(lexem);
            if (it != opPriority.end()) {
                // Проверяем, что в стеке достаточно операндов
                if (stack.IsEmpty()) {
                    throw runtime_error("Not enough operands for operator: " + lexem);
                }
                double b = stack.Pop();

                // Для бинарных операторов нужен второй операнд
                double a = 0;
                if (lexem != "not") {  // "not" обычно обрабатывается отдельно
                    if (stack.IsEmpty()) {
                        throw runtime_error("Not enough operands for binary operator: " + lexem);
                    }
                    a = stack.Pop();
                }

                // Выполняем операцию
                if (lexem == "+") stack.Push(a + b);
                else if (lexem == "-") stack.Push(a - b);
                else if (lexem == "*") stack.Push(a * b);
                else if (lexem == "/") {
                    if (b == 0) throw runtime_error("Division by zero");
                    stack.Push(a / b);
                }
                else if (lexem == "and") {
               
                    stack.Push(a && b);
                }
                else if (lexem == "=") {

                    stack.Push(a == b);
                }
                else if (lexem == "<") {
                   
                    stack.Push(a < b);
                }
                else if (lexem == ">") {
                    
                    stack.Push(a > b);
                }
                else if (lexem == "<=") {
                   
                    stack.Push(a <= b);
                }
                else if (lexem == ">=") {
                    
                    stack.Push(a >= b);
                }
                else if (lexem == "or") {
                   
                    stack.Push(a || b);
                }
                else if (lexem == "div") {
                    int ib = static_cast<int>(b);
                    if (ib == 0) throw runtime_error("Integer division by zero");
                    stack.Push(static_cast<int>(a) / ib);
                }
                else if (lexem == "mod") {
                    int ib = static_cast<int>(b);
                    if (ib == 0) throw runtime_error("Mod by zero");
                    stack.Push(static_cast<int>(a) % ib);
                }
                else if (lexem == "and") stack.Push(a && b);
                else if (lexem == "or") stack.Push(a || b);
            }
            // Обработка чисел
            else if (isdigit(lexem[0])) {
                try {
                    stack.Push(stod(lexem));
                }
                catch (...) {
                    throw runtime_error("Invalid number format: " + lexem);
                }
            }
            // Обработка переменных
            else if (isalpha(lexem[0])) {
                if (variables.find(lexem) == variables.end()) {
                    throw runtime_error("Undefined variable: " + lexem);
                }
                stack.Push(variables[lexem]);
            }
            else {
                throw runtime_error("Unknown lexem: " + lexem);
            }
        }

        if (stack.IsEmpty()) {
            int o = 1;
            throw runtime_error("No result in expression");
        }

        return stack.Pop();
    }
};
