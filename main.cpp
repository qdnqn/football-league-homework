#include <iostream>
#include <string>

#include "Team.h"
#include "Match.h"
#include "League.h"
#include "App.h"
#include "Result.h"

using namespace std;

int main(){
  cout << "----------------------------------------------------" << endl;
  cout << "Team and match managment program!" << endl << endl;  
  cout << "----------------------------------------------------" << endl;
  
  int choice = 0;
  App fp;

  do{  
    cout << "1. Add team" << endl;
    cout << "2. Add league" << endl;
    cout << "3. Play match" << endl;
    cout << "4. Cancel last match" << endl;
    cout << "5. Team details" << endl;
    cout << "6. League table" << endl;
    cout << "7. List all matches" << endl;
    cout << "8. Enter dummy data" << endl;
    cout << "9. Quit" << endl << endl;
    cout << "Your choice: ";
    
    if(!(cin >> choice)){
      choice = 8;
    } else {
      if(choice == 1){
        fp.addTeam(cin, cout);
      } else if(choice == 2){
        fp.addLeague(cin, cout);
      } else if(choice == 3){
        fp.playMatch(cin, cout); 
      } else if(choice == 4){
        fp.undoMatch(cout);
      } else if(choice == 5){
        fp.detailsTeam(cin, cout);
      } else if(choice == 6){
        fp.detailsLeague(cin, cout); 
      } else if(choice == 7){
        fp.detailsMatches(cout);
      } else if(choice == 8){
        fp.dummyData(cin, cout);
      } else {
        choice = 9;
      }
    }
    
    cout << endl << endl;
    cout << "--------------[MAIN MENU]-------------------" << endl;
  } while(choice != 9);

  return 0;
}
