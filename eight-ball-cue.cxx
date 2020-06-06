#include "controller.hxx"

#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
        try {

            if (argc == 1) {
                Controller(6).run();
                }
            else if (argc == 2) {
                int n = stoi(argv[1]);
                    Controller(n).run();
                }
            else {
                throw ge211::Client_logic_error(
                            "wrong number of arguments.");
            }
        }
        catch (exception const& e) {
            cerr << argv[0] << ": " << e.what() << "\n";
            return 2;
        }
}
