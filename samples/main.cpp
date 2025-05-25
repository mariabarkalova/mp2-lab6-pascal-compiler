#include "hierarchicalList.h"
#include "arithmetic.h"
#include <vector>
int main()
{
    /*setlocale(LC_ALL, "Russian");
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
    */
    HierarchicalList program;
    program.Read("ProgramPas.txt");
    program.Print();
    cout << endl << endl;

    HierarchicalList program1;
    program1.Read("ProgramPas2.txt");
    program1.Print();
    cout << endl << endl;

    cout << "Starting Pascal-- program execution..." << endl;
    PascalLineByLine interpreter(program);
    interpreter.run();
    cout << "Program finished successfully." << endl;

    cout << "Starting Pascal-- program execution..." << endl;
    PascalLineByLine interpreter1(program1);
    interpreter1.run();
    cout << "Program finished successfully." << endl;


}