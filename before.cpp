#include <vector>
#include <string>
using namespace std;
std::vector<int> scoreboard(const std::string &exam) {
  //Your code here!
  vector<int> scoreboard ;
    size_t save=0;
    while(scoreboard.size()!=2){
        string cadena="";
    for(size_t i=save;i<exam.size();++i){
        if(exam[i]==' '){
            save=i;
            break;
        }
        cadena+=exam[i];
        
    }
    
    
    
    for(size_t j = save;j<exam.size();++j){
        if(exam[j]!=' '){
            save=j;
            break;
        }
        
    }
    
    if(cadena=="nothing" or cadena=="zero" or cadena=="nil"){
        scoreboard.push_back(0);
    }
    
    if(cadena=="one"){
        scoreboard.push_back(1);
    }
    if(cadena=="two"){
        scoreboard.push_back(2);
    }
    if(cadena=="three"){
        scoreboard.push_back(3);
    }
    if(cadena=="four"){
        scoreboard.push_back(4);
    }
    if(cadena=="five"){
        scoreboard.push_back(5);
    }
    if(cadena=="six"){
        scoreboard.push_back(6);
    }
    if(cadena=="seven"){
        scoreboard.push_back(7);
    }
    if(cadena=="eight"){
        scoreboard.push_back(8);
    }
    if(cadena=="nine"){
        scoreboard.push_back(9);
    }
    
}
  return scoreboard ;
}
