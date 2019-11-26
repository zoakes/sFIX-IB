#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <array>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <ctype.h>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <iterator> //WOW I MIGHT NIGHT HAVE HAD THIS !! this is why that shit didnt work! REGEX !!
#include <sstream>
#include <numeric>
#include <functional>
#include <valarray>
#include <complex>

#include <netdb.h>
#include <iomanip>
#include <ctype.h>
#include <fstream>


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


// BOOST IMPORTS
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>




using namespace std;
using namespace boost;


class TSocket{
private:
  int sockfd, connfd, res, out;
  struct sockaddr_in servaddr, cli;
  char buff[1024];
  //int servaddr.family; // = AF_NET;
  //servaddr.sin_addr.s_addr; // = inet_addr(127.0.0.1"");
  //servaddr.port = htons(4000);

public:
  int sock = 0; //socket(AF_INET, SOCK_STREAM, 0);

  TSocket(const char *addr="www.google.com",int port=4000){
      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(addr);                               //Argument accepted FINALLY. 
      servaddr.sin_port = htons(port);

      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if(sockfd == -1){
        cout << "Socket creation failed" << endl;
        exit(0);
      }
      else{
        cout << "Socket created succesfully " << endl;
      }
      bzero(&servaddr,sizeof(servaddr)); //Clear the buffer / bytes
      //NEW ADDITS
      res = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
      if(res == 0){                                                             //Think this is correct ?
        cout << "Connection failed" << endl;
        exit(0);
      }
      else{
        cout << "Connection Success ! ... " << endl;
      }

    }

  ~ TSocket(){
      close(sockfd);
      cout << "Closed socket " << sockfd << endl;
    }

  int forceConnect(){
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == 0){
      cout << "connection failed" << endl;
      exit(0);
    }
    else{
      cout << "Connection Success !" << endl;
    }
    return 0;
  }

  //Format input: char msg[] = "Tst";
  int sendMsg(const char * const& msg){
    const char* msgFmt = msg;
    res = send(sockfd, msgFmt, strlen(msg),0);
    if(res == 0){                                                               //I think this is correct?
      cout << "Failed sending " << msg << endl;
      out = -1;
    }
    else
      cout << "Sending " << msg << endl;
      out = 1;
  return out;
  }


};



int main(){

  char ch = 'T';
  char *ptr = &ch;
  cout << *ptr << '\n';

  const char chr[] = "TEST";
  char tst = 'T';

  const char * const& ptrHell = chr;

  string S = "1";
  string * pt = &S;
  cout << *pt << endl;

  TSocket t;

  //t.forceConnect(); //Works as well.

  //Fucking thing works!
  t.sendMsg(chr);





  return 0;
}
