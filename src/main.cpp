/**
 * @mainpage
 * @section Description
 * A client server chat application. Client and Server are both separated and communicate through commands.
 * Both client and server are high level classes and has no information about communation protocol.
 * Comunication between them is encapsulated by Command::CBase derived classes. It is possible to add new commands
 * without modifying anything just by inheriting Command::CBase.
 * @section Author
 * Lukáš Mazur
 */
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "Socket.hpp"
#include "Server/Server.hpp"
#include "Client/Client.hpp"

using namespace std;

void printHelp ()
{
    cout << "Usage:" << endl << endl;
    cout << "Server:" << endl;
    cout << "\t program_name --server port_number" << endl << endl;
    cout << "Client:" << endl;
    cout << "\t program_name address:port" << endl << endl;
    cout << "Help (prints this message):" << endl;
    cout << "\t program_name --help" << endl;
}

int main (int argc, char * argv[])
{
    try {
        if (argc == 3 && string(argv[1]) == string("--server")) {
            Server::CServer server(stoi(argv[2]));
            server.Run();
            return 0;
        } else if (argc == 3) {
            Client::CClient client(argv[1], stoi(argv[2]));
            client.Run();
            return 0;
        } else {
            printHelp();
        }
    } catch (runtime_error e) {
        cerr << e.what() << endl;
    } catch (logic_error e) {
        printHelp();
        cout << e.what() << endl;
    }

    return 0;
}
