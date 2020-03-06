//
//  main.cpp
//  sFIX
//
//  Created by Zachary Oakes on 1/5/20.
//  Copyright © 2020 Zachary Oakes. All rights reserved.
//

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
#include <iterator> //WOW I MIGHT NotHAVE HAD THIS !! t
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
//#include <boost>

//imports
#include "test.hpp"
//#include "test.cpp" //I THINK THE ISSUE IS I'M NOT SUPPOSED TO INCLUDE CPP FILE ! 
#include "socket.hpp"
//#include "socket.cpp"

using namespace std;
using namespace std::chrono;

//using namespace boost;
//Basic project testing functions -- Can delete, not vital to sFIX.

int is_palindrome(int i){
    string sv = to_string(i);
    string init = sv;
    //cout << sv << endl;
    reverse(sv.begin(),sv.end());
    //string rv = reverse(sv.begin(),sv.end());
    
    if(init == sv)
        return 1;
    return 0;
}


template<typename T>
int palindrome(T t){
    string sv = to_string(t);
    string init = sv;
    reverse(sv.begin(),sv.end());
    if(init == sv)
        return 1;
    return 0;
}
 
template<typename T>
int palindrome(string s){
    //string sv = to_string(
    string init = s;
    reverse(s.begin(),s.end());
    if(init == s)
        return 1;
    return 0;
}


unsigned long binary_search(vector<int> tv,int x){
    unsigned long L,H,M;
    L = 0; H = tv.size() - 1;
    
    sort(tv.begin(),tv.end());
    while(L <= H){
        M = L + (H-L)/2;
        if(tv[M] < x)
            L = M + 1;
        else if(tv[M] > x)
            H = M - 1;
        else
            return M;
    }
    return -1;
}



///--------------------------------------------------------------------------------------------------------------------------------------------------------------------------//



int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    FIX f;
    
    //f.run(8,22); //Temporarily LATE just to test late! (leave blank --> defaults)
    auto hb = f.hb();
    auto bits = f.string_to_bytes(hb);
    cout << "Bits" << endl;
    
    for(auto i : bits){
        cout << i;
    }
    cout << '\n';
    cout << "Printing using a pointer" << endl;
    char *c = &bits[0];
    cout << c << endl;
    
    
    cout << bits.size(); //FOR SIZE / LEN !



    //BITS COUNT
    char *c_ptr = &bits[0];
    cout << "BITS:" << c_ptr << endl;
    
    cout << hb << endl; //string
    int len = f.count_bytes(hb);
    cout << "LEN:" << len << endl;
    cout << "SIZE:" << hb.size() << endl;
    cout << "SIZEOF BITS ptr:" << sizeof(c_ptr);
    cout << "SIZE ? LEN BITS: " << bits.size() << endl; //Same!

    
    // -----------------------
    // SOCKET TESTING
    //Not real socket yet, just module tsting
    Socket s;
    //s.cnct("127.0.0.1");
    
    f.test();
    f.test_function(); // NOWWWW these work !! 
    
    //cout << f.HB;
    s.send_str(hb);
    
    //Send w/ ptr (crazy ptr, pting straight to hell! 
    s.send_ptr(c_ptr);
    
    f.make_logOn();
    

    s.force_connect(); //-- works !
    cout << s.check_connect();
    
    return 0;
}


///--------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//Define functions -- can be done here? (though not in test.cpp?)


/*Copied from MAIN function in main.cpp
vector<int> iv = {1,2,3,4,5,6};

cout << iv[0] << endl;
auto i = 0;


string st = "Testing a string subscript ability ...";
for(i = 0; i < st.length();i++){
    cout << st[i] << endl;
}

cout << st[0];
cout << st.at(0); //This also works, likely faster !

vector<int> ii = {1,2,3,4,5};
//vec_pl<int>(ii);
vector<string> sv = {"a","b","c","d"};

//vec_pal(sv);
sv = {"O","T","T","O"};


int res = is_palindrome(101); //Why cant I access these?
cout << "Is Pal: " << res << endl;
cout << is_palindrome(10010) << endl;
 
cout << "String pal:" << palindrome<string>("OTTO") << endl;
cout << "Int pal:" << palindrome<int>(1010) << endl;
cout << "BS: " << binary_search(ii,3);
 ENDIF copied from main function
*/




