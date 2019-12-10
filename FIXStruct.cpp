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
#include <sstream>

#include <netdb.h>
#include <iomanip>
#include <ctype.h>
#include <fstream>

#include <thread>


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


// BOOST IMPORTS
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>



using namespace std::chrono;
using namespace std::this_thread;
using namespace std;
using namespace boost;





////////////////////////////////////////////////////////////////////////////////
int ORD_ID;
bool INCL_DT = false;

class FV{
private:
  std::map<int,string> fmap;
  std::map<int,string>::iterator fidx;
  std::pair<int,string> fval;




public:
  int sum;

  void print(){
    for(auto i : fmap ){
      cout << i.first << "=" << i.second << "|";
    }
  }

  void add_pair(int n, string value){
    fmap.insert(std::pair<int,string>(n,value));
    cout << "\n Added " << n << "=" << value;
    sum += value.length();
    sum += sizeof(n)/sizeof(0);
  }

  void change_pair(int n, string value){
    //fmap.n = value;
    int init_sum = 0;
    fidx = fmap.find(n);
    if (fidx == fmap.end()) {
      cout << "Error, no key of " << n << endl;
    }
    else {
      init_sum = fidx->second.length();
      fidx->second = value;
      cout << "Value changed to " << value << endl;
    }
    int new_len = value.length();
    sum  += new_len - init_sum;  //Initially 10, now 8, so change is 2

  }


  int get_ord_id(){
    ORD_ID += 1;
    return ORD_ID;
  }

  struct F{
    pair<int,string> BeginString = {8,"FIX.4.2"};
    pair<int,string> Account = {1,"U12345"};
    pair<int,string> MsgType = {35,"D"};
    pair<int,string> TargetCompID = {56, "IB"};
    pair<int,string> ClOrdID = {11, "="}; // + string(get_next_OID());
    pair<int,string> Symbol = {55 ,"IBM"};
    pair<int,string> Side = {54,"1"}; //Buy
    pair<int,string> TransactTime = {56,""};
    pair<int,string> Qty = {38,"10"};
    pair<int,string> OrdType = {40,"0"}; //HB?
    pair<int,string> CheckSum = {10,"75"}; // calc_sum();
  }fstruct;

  stringstream leanBuy(string symbol, int qty){
    int oid = get_ord_id();
    /*string FIX_str;
    if(INCL_DT == true){
      string t = getDateTime();
      FIX_str = "8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=" + to_string(oid) + "|55=" + symbol +
        "|54=5|56=" + t + "|38=" + to_string(qty) + "|40=0|10=";
    }
    else*/ //To Incl DateTime in msg (not needed!)
    string FIX_str = "8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=" + to_string(oid) + "|55=" + symbol +
      "|54=5|38=" + to_string(qty) + "|40=1|10=";

    int len = FIX_str.length();
    FIX_str += to_string(len);

    cout << FIX_str << endl;
    std::stringstream buffer;
    buffer << FIX_str << endl;
    return buffer;
  }

  stringstream leanSell(string symbol,int qty){
    int oid = get_ord_id();
    string t = getDateTime();
    string FIX_str = "8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=" + to_string(oid) + "|55=" + symbol +
      "|54=5|56=" + t + "|38=" + to_string(qty) + "|40=0=1|10=";
    FIX_str += to_string(FIX_str.length());

    cout << FIX_str << endl;
    std::stringstream buffer;
    buffer << FIX_str << endl;
    return buffer;

  }

  map<int,string> baseOrder(string symbol,int side, int qty){
    //fstruct.Symbol = pair<int,string>(55,symbol); Works, but not needed
    //fstruct.Side = pair<int,string>(54,to_string(side));
    //fstruct.Qty = pair<int,string>(38,to_string(qty));
    change_pair(55,symbol);
    change_pair(54,to_string(side));
    change_pair(38,to_string(qty));
    change_pair(56,getDateTime());
    //return fstruct;
    stringstream ss;
    for(auto i : fmap){
      cout << i.first << "=" << i.second << "|";
      ss << i.first << "=" << i.second << "|";
    }
    //CHECKSUM CALCULATION
    ss << "10=" << to_string(sizeof(ss)/sizeof("a"));
    cout << "10=" <<  sizeof(ss)/sizeof("i");

    return fmap;
  }

  stringstream leanHB(){
    int oid = get_ord_id();
    string t = getDateTime();
    string FIX_str = "8=FIX.4.2|9=51|35=0|1=U123456|56=IB|11=" + to_string(oid) + "|55=None" +
      "|54=5|56=" + t + "|38=" + "0" + "|40=0|10=";
    FIX_str += to_string(FIX_str.length());

    cout << FIX_str << endl;
    std::stringstream buffer;
    buffer << FIX_str << endl;
    return buffer;
  }

  static stringstream sHB(){
    FV fl; //Must create a local Class object to call these
    int oid = fl.get_ord_id();
    string t = fl.getDateTime();
    string FIX_str = "8=FIX.4.2|9=51|35=0|1=U123456|56=IB|11=0|55=None|54=5|56=" + t + "|38=" + "0" + "|40=0|10=";
    FIX_str += to_string(FIX_str.length());

    cout << FIX_str << endl;
    std::stringstream buffer;
    buffer << FIX_str << endl;
    return buffer;
  }

  void every(int delay){
    time_t now = time(0);
    time_t next = time(0) + delay;
    while(true){
      //time.sleep(max(0,next - time(0)));
      //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
      sleep_until(system_clock::now() + seconds(30));
      leanHB();
      /*
      try{
        //cout << "HB" << endl;
        leanHB();
      }
      catch (const std::exception& e){
        cerr << "Error calling HB" << endl;
        cout << e.what() << endl;
      }*/
      next += (time(0) - next) / delay * delay + delay;
    }
  }



  string getDateTime(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    auto y = to_string(1900 + ltm->tm_year);
    auto m = to_string(ltm->tm_mon);
    auto d =  to_string(ltm->tm_mday);
    auto h = to_string(ltm->tm_hour);
    auto mn = to_string(ltm->tm_min);
    auto s = to_string(ltm->tm_sec);
    string time = y +  m  + d + " " + h  + ":" + mn + ":" + s;
    return time;
  }

};


void every(int delay){ //incl function
  time_t now = time(0);
  time_t next = time(0) + delay;
  while(true){
    //time.sleep(max(0,next - time(0)));
    //std::this_thread::sleep_for(std::chrono::milliseconds(delay));  //This line alone may allow threading?
    sleep_until(system_clock::now() + seconds(30));
    try{
      cout << "HB" << endl;
      FV::sHB();
    }
    catch (const std::exception& e){
      cerr << "Error calling HB" << endl;
      cout << e.what() << endl;
    }
    next += (time(0) - next) / delay * delay + delay;
  }
}


//int FV::ORD_ID = 0; //Proper use of STATIC just in case

int main(){

  /*

  */
  //WTF?
  std::map<int,string> testMap;
  //testMap.insert(std::pair<int,int>(8,1000));



  FV F;
  F.add_pair(8,"FIX.4.2");
  cout << '\n';
  F.print();

  //This is close,not quite there yet.... BINGO
  F.change_pair(8,"FIX.4.4");
  cout << '\n';
  F.print();
  F.add_pair(1,"U12345");
  F.add_pair(35,"D");
  F.add_pair(56,"IB");
  auto OID = F.get_ord_id(); //Maybe needs to be static?
  F.add_pair(11,to_string(OID));
  F.add_pair(55,"T"); //Bad_Alloc somehow... (with values > 1 digit !!)
  F.add_pair(54,"1");
  F.add_pair(30,"5"); //Bad alloc somehow... (with values > 1 digit)
  F.add_pair(40,"0");
  F.add_pair(38,"5");
  F.add_pair(10,to_string(F.sum)); //Not right! Fuck...

  F.change_pair(55,"AMD");  //No alloc error with changing?

  cout << '\n';
  F.print();

  F.leanBuy("AAPL",100);


  F.leanSell("AMZN",10);


  F.getDateTime();

  F.baseOrder("TSLA",1,10);
  cout << '\n';
  F.print();

  //F.baseOrder("AAPL",1,100); //30 micros
  //F.change_pair(55,"AAPL"); //5 us

  //Time it
  auto start = std::chrono::high_resolution_clock::now();
  F.leanBuy("AAPL",100); //4.5 us
  F.leanSell("AMZN",10);
  F.leanBuy("AMD",20);
  F.leanSell("AAPL",100);
  auto elapsed = std::chrono::high_resolution_clock::now() - start;

  long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  cout << "Micros: " <<  microseconds/4 << endl;


  /////////////////// BEGIN HEARTBEAT ////////////////////////////////
  //every(30); BE CAREFUL WITH THIS SHIT!

  thread t;
  //thread t5(F.every,30);  //Must be static to call like this? (or leanHB must be)
  F.leanHB();

  FV::sHB(); //Bingo -- Static works if needed later !
  F.sHB(); //Works either way.

  ////////////// Begin Threading HB ///////////////////
  //HEARTBEAT -- FINAL ! (KEEP IT STATIC, keep it seperate)
  //every(30); //Works !

  thread t2(every,30); ///Works -- think it's a unix issue.


return 0;
}
