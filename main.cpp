#include <cstdlib>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h> 
#include <cstring>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
using namespace std;
struct setup
{
    char *server =  "";
    const string bot_nick = "";
    const int port = 6667;
    const string indent = "";
    const string realname = "";
    const string channel = "#channel";
    
};

class tools : public setup
{
    public:
        void hostname_to_ip(char * hostname1, char* ip1) // http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
        {
            struct hostent *he;
            struct in_addr **addr_list;
            int i;
            if((he = gethostbyname(hostname1)) == NULL) 
            {
                herror("gethostbyname");
            }
            addr_list = (struct in_addr **) he->h_addr_list;
            for(i = 0; addr_list[i] != NULL; i++) 
            {
                strcpy(ip1, inet_ntoa(*addr_list[i]) );
            }
        }
    private:
    protected:
};

int main()
{
    setup info;
    tools tool;
    srand(time(NULL));
    struct sockaddr_in serverc0;
    time_t rawtime;
    time (&rawtime);
    char ip0[100];
    tool.hostname_to_ip(info.server, ip0);
    int s0 = socket(AF_INET , SOCK_STREAM , 0); 
    if(s0 == -1){cout << "Could not create socket" << endl;} // err msg
    serverc0.sin_addr.s_addr = inet_addr(ip0);
    serverc0.sin_family = AF_INET;
    serverc0.sin_port = htons(info.port);
    if(connect(s0, (struct sockaddr *)&serverc0 , sizeof(serverc0)) < 0){perror("connect failed. Error");return 1;} // err msg
    cout << "Connected" << endl;
    send(s0, ("NICK " + info.bot_nick + "\r\n").c_str(), ("NICK " + info.bot_nick + "\r\n").size(), 20);
    send(s0, ("USER " + info.indent + " " + info.server + " null :" + info.realname + "\r\n").c_str(), ("USER " + info.indent + " " + info.server + " null :" + info.realname + "\r\n").size(), 20);
    send(s0, ("JOIN " + info.channel + "\r\n").c_str(), ("JOIN " + info.channel + "\r\n").size(),  20);
    send(s0, ("PRIVMSG " + info.channel + " :I entered at: " + ctime(&rawtime) + "\r\n").c_str(), ("PRIVMSG " + info.channel + " :I entered at: " + ctime(&rawtime) + "\r\n").size(), 20);
    while(1)
    {
        string line;
        char data = 0; 
        while (data != '\n')
            {
                ssize_t datarecv = recv(s0, &data, 1, NULL);
                if(datarecv == -1)
                {
                        if (errno != EAGAIN && errno != EWOULDBLOCK)
                        {
                            cout  << "connection error" << errno << endl;
                            break;
                        }
                }

                if(datarecv == 0)
                {
                        cout << "the other one" << endl;
                    break;
                }

                line += data;
                if (line.size() > 2048)
                {
                   exit(-1);
                    break;
                }
            }
        if(line.size() > 0)
        {
            line.replace(line.find("\r"), 2, "");
            cout << line  << endl;
            if(line.find("PING :") != -1) // dont mess with this, it responds to pings
            {
                line.replace(line.find("PING "), 5, "");
                line.replace(line.find(":"), 1, "");
                send(s0, ("PONG :" + line + "\r\n").c_str(), ("PONG :" + line + "\r\n").size(),  20);
                cout << "ponged" << endl;
            }
            // stuff to do here!
        }
    }
    close(s0); 
    return 0;
}
