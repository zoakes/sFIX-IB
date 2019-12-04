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


struct sFX{
  string BeginString = "8=FIX.4.2"; //"8=FIX.4.2"; //Maybe do with vector? 0 = #, then 1 = Value
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

  /*string CheckSum = str(FIXMSG.BeginString.length + FIXMSG.Account.length + FIXMSG.MsgType.length +
  FIXMSG.TargetCompID.length + FIXMSG.ClOrdID.length + FIXMSG.Symbol.length + FIXMSG.Side.length +
  FIXMSG.Qty.length + FIXMSG.OrdType.length) ; */

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
      cout << "Value changed to " << value;
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
    pair<int,string> Qty = {38,"10"};
    pair<int,string> OrdType = {40,"0"}; //HB?
    pair<int,string> CheckSum = {10,"160"}; // calc_sum();
  };

};



//int FV::ORD_ID = 0;

int main(){


  struct sFX s;

  s.BeginString = "8=FIX.4.4";
  cout << s.BeginString << endl;

  cout << s.Symbol << endl;

  struct FIXMSG f;
  cout << f.BeginString;
  //f.BeginString[0] = "9";
  struct VF v;

  ///Map not working out well...

  v.BeginString.insert(pair<int,string>(8,"=FIX4.2")); //= {"8","=FIX.4.2"};
  auto bs = v.BeginString;
  //cout << bs->second ;

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





return 0;
}
