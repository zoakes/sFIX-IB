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
#include <iterator> 
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


#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


// BOOST IMPORTS
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>




using namespace std;
using namespace boost;


//Tried multiple structures first, ended up with nested Struct in map Class w/ stringstream


struct sFX{
  string BeginString = "8=FIX.4.2"; //"8=FIX.4.2"; 
  string Account = {"1=U12345"};
  string MsgType = {"35=D"}; //New Order
  string TargetCompID = {"56=IB"};
  string ClOrdID = {"11="}; // + string(get_next_OID());
  string Symbol = {"55=IBM"};
  string Side = {"54=1"}; //Buy
  string Qty = {"38=10"};
  string OrdType = {"40=0"}; //HB?
  string CheckSum = {"10=160"};

};


//Maybe create a struct for each type (order, heartbeat, cancel, etc)
struct FIXMSG{
  string BeginString = {"8","=FIX.4.2"}; //"8=FIX.4.2"; //Maybe do with vector? 0 = #, then 1 = Value
  string Account = {"1","=U12345"};
  string MsgType = {"35","=D"}; //New Order
  string TargetCompID = {"56", "=IB"};
  string ClOrdID = {"11", "="}; // + string(get_next_OID());
  string Symbol = {"55=","IBM"};
  string Side = {"54","=1"}; //Buy
  string Qty = {"38=","10"};
  string OrdType = {"40","=0"}; //HB?
  string CheckSum = {"10","=160"};
  

};


struct VF{ //Dont want a map... want pairs...
  std::map<int,string> BeginString; //= {8,"=FIX.4.2"};
};


////////////////////////////////////////////////////////////////////////////////
int ORD_ID;

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
    sum  += new_len - init_sum;  //Ex: Initially 10, now 8, so adding 8, subtracting 10 (-2)

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
    cout << oid << endl;

    string t = getDateTime();
    string FIX_str = "8=FIX.4.2|9=51|35=D|1=U123456|56=IB|11=" + to_string(oid) + "|55=" + symbol +
      "|54=5|56=" + t + "|38=" + to_string(qty) + "|40=0|10=";
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
      "|54=5|56=" + t + "|38=" + to_string(qty) + "|40=0|10=";
    FIX_str += to_string(FIX_str.length());

    cout << FIX_str << endl;
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



//int FV::ORD_ID = 0;

int main(){

  //BeginIf String Structure, Map Structure, Tuple? structure 
  struct sFX s;

  s.BeginString = "8=FIX.4.4";
  cout << s.BeginString << endl;

  cout << s.Symbol << endl;

  struct FIXMSG f;
  cout << f.BeginString;
  //f.BeginString[0] = "9";
  struct VF v;

  ///Map alone not working out well...

  v.BeginString.insert(pair<int,string>(8,"=FIX4.2")); //= {"8","=FIX.4.2"};
  auto bs = v.BeginString;
  //cout << bs->second ;
   
  //Endif String Struct / Map Struct 
  
  //Beginif NESTED MAP CLASS W/ STRUCT 


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
  F.add_pair(10,to_string(F.sum)); //Not right!?

  F.change_pair(55,"AMD");  //No alloc error with changing?

  cout << '\n';
  F.print();

  F.leanBuy("AAPL",100);

  F.leanSell("AMZN",10);


  F.getDateTime();

  F.baseOrder("TSLA",1,10);
  cout << '\n';
  F.print();



return 0;
}
