#include "hierarchicalList.h"
#include "arithmetic.h"
#include <vector>
/*
int main() {
    setlocale(LC_ALL, "Russian");
    vector<string> program = {
      "program Example;",//1
"var num1, num2: integer; Res, d: double;",//2
"begin",//3
"  num1:= 5;",//4
"  num2:= 5;",//5
"  Res := 0.0;",//6
"  d := 0.0;",//7
"  writeln('Введите четное целое число: ');",//8
"  readln(num2);",//9
"  writeln('Введите вещественное число: ');",//10
"  readln(d); ",//11
"  if (num2 mod 2 = 0) then begin",//12
"    Res := (num1 - num2 * 5 div 2) / (d * 2);",//13
"    writeln('Результат = ', Res);",//14
"    end",//15
"  else",//16
"   writeln('Неверный ввод');",//1
"end."//1
    };
    vector<string> program1 = {
      "program Test;",
      "var    a: integer; b: real; c: integer;",
      "begin",
      "    a := 10;",
      "    b := 3.5;",
      "    c := a div 2;",
      "    ",
      "    writeln('Line by line Pascal--');",
      "    writeln('a = ', a, ', b = ', b);",
      "    writeln('c = a div 2 = ', c);",
      "    ",
      "    if (a > 5) then writeln('a > 5 is true');",
      "    ",
      "    writeln('Enter new a:');",
      "    readln(a);",
      "    writeln('New a = ', a);",
      "end."
    };

    try {
        cout << "Starting Pascal-- program execution..." << endl;
        PascalLineByLine interpreter(program);
        interpreter.run();
        cout << "Program finished successfully." << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}*/

int main()
{
    setlocale(LC_ALL, "Russian");
    HierarchicalList list;
    HierarchicalList::hl_iterator it = list.begin();
    
    it = list.insert_next(it);
    (*it) = "Строка 1";
    HierarchicalList::hl_iterator it1 = list.insert_down(it);
    (*it1) = "Строка 1.1";
    it = list.insert_next(it);
    (*it) = "Строка 2";
    cout << list << endl;
    
    cout << endl << endl;

    list.Print();

    cout << endl << endl;

    HierarchicalList program;
    program.Read("ProgramPas.txt");

    program.Print();

    cout << endl << endl;

    cout << program << endl;
}