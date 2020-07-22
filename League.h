#ifndef LEAGUE_H_
#define LEAGUE_H_

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>

#include "Team.h"
#include "Match.h"

int randCustom(int min, int max) 
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) );
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

class League {
  public:
    League() = default;
    League(std::string name, std::string country, std::vector<Team*> teams) : name_{name}, country_{country}, teams_{teams} {};
    
    ~League(){
      auto it = matches_.begin();

      while(it != matches_.end()){
        delete *it;
      }
    }

    std::string name(){ return name_; }
    std::string country(){ return country_; }
    std::vector<Team*> teams(){ return teams_; }
    std::vector<Match*> matches(){ return matches_; }
    
    void generate(std::istream&, std::ostream&);
    void generateDummy(std::istream&, std::ostream&);
    void displayMatches(std::ostream&);
    void generateTable(std::ostream&);
    void teamMatches(Team* t, std::ostream&);
    void displayMatch(Match*, std::ostream&, int);
    Match* getMatch(int);
  private:
    std::string name_;
    std::string country_; 
    std::vector<Match*> matches_;
    std::vector<Team*> teams_;
    std::vector<Team*> table_;
};

void League::teamMatches(Team* t, std::ostream& out){
  if(t->country() == country_){
    auto mt = matches_.begin();
    int i = 1;

    while(mt != matches_.end()){
      if((*mt)->home()->name() == t->name() || (*mt)->away()->name() == t->name()){
        displayMatch(*mt, out, i);

        ++i;
      }

      ++mt;
    }
  }
}

void League::generate(std::istream& inp, std::ostream& out){
  auto track = teams_.begin();
  auto mt = matches_.begin();

  int c = 1;
  int found = 0;

  time_t Time = time(NULL);
  struct tm *aTime = localtime(&Time);

  int day = aTime->tm_mday;
  int month = aTime->tm_mon+1;
  int year = aTime->tm_year+1900;
  
  out << std::endl;
  out << "Schedule of the matches: " << std::endl << std::endl;
  
  bool loopSame = false;

  while(track != teams_.end()){
    auto it = teams_.begin();
    
    time_t Time = time(NULL);
    struct tm *aTime = localtime(&Time);

    int day = aTime->tm_mday;
    int month = aTime->tm_mon+1;
    int year = aTime->tm_year+1900;

    while(it != teams_.end()){
      if(it != track){
        int y,m,d;

        y = year;
        m = month;
        d = day;

        found = 0;
        mt = matches_.begin();

        while(mt != matches_.end()){
          if((*mt)->matchDate(d, m, y)){
            found++;

            if((*mt)->awayOrHome(*track, *it)){
              loopSame = true;
              break;
            }
          }

          ++mt;
        }

        aTime->tm_mday += 1;
        Time = mktime(aTime);

        day = aTime->tm_mday;
        month = aTime->tm_mon+1;
        year = aTime->tm_year+1900;

        if(found < 3 && !loopSame){
          matches_.push_back(new Match(*track, *it, d, m, y));
          
          out << "Match #" << c << std::endl;
          out << (*track)->name() << " - " << (*it)->name() << std::endl;
          out << "Match date: ";
          out <<  d << "." << m << "." << y;
          out << std::endl << std::endl;
        } else {
          loopSame = false;
          continue;
        }      

        std::sort(matches_.begin(), matches_.end(), [](Match* a, Match* b){
              if(a->date().y() < b->date().y()){
                return true;
              } else if(a->date().m() < b->date().m()){
                return true;
              } else if(a->date().d() < b->date().d()){
                return true;
              } else {
                return false;
              }
            });
        
        ++c;
      }

      ++it;
    }

    ++track;
  }

  out << "Matches scheduled for this league!" << std::endl;
}

void League::generateDummy(std::istream& inp, std::ostream& out){
  auto track = teams_.begin();
  auto mt = matches_.begin();

  int c = 1;
  int found = 0;

  out << std::endl;
  out << "Please enter schedule of matches!" << std::endl << std::endl;
  
  while(track != teams_.end()){
    auto it = teams_.begin();

    while(it != teams_.end()){
      if(it != track){
        int y,m,d;

        y = 2018;
        m = randCustom(1,12);
        d = randCustom(1,28);

        out << "Match #" << c << std::endl;
        out << (*track)->name() << " - " << (*it)->name() << std::endl;
        out << "Enter home date[eg: 25 12 2018]: ";
        out << d << " " << m << " " << y << std::endl;
      
        found = 0;
        mt = matches_.begin();

        while(mt != matches_.end()){
          if((*mt)->matchDate(d, m, y)){
            found++;
          }

          ++mt;
        }

        if(found < 3){
          matches_.push_back(new Match(*track, *it, d, m, y));
        } else {
          throw std::overflow_error("4 matches per same date is allowed!");
        }

        std::sort(matches_.begin(), matches_.end(), [](Match* a, Match* b){
              if(a->date().y() < b->date().y()){
                return true;
              } else if(a->date().m() < b->date().m()){
                return true;
              } else if(a->date().d() < b->date().d()){
                return true;
              } else {
                return false;
              }
            });
        ++c;
      }

      ++it;
    }

    ++track;
  }

  out << "Matches scheduled for this league!" << std::endl;
}

void League::generateTable(std::ostream& out){
  auto it = teams_.begin();

  table_.clear();

  while(it != teams_.end()){
    table_.push_back(*it);
    ++it;
  }

  std::sort(table_.begin(), table_.end(), [](Team* a, Team* b){
        if(a->wins() > b->wins()){
          return true;
        } else if(a->wins() == b->wins()){
          if(a->goalDiff() > b->goalDiff()){
            return true;
          } else {
            return false;
          }
        } else {
          return false;
        }
      });
  
  auto it2 = table_.begin();

  while(it2 != table_.end()){
    out << (*it2)->name() << " - Wins: " << (*it2)->wins();
    out << " Losses: " <<  (*it2)->losses() << " Draws: " << (*it2)->draws();
    out << " Goal Diff: " << (*it2)->goalDiff() << " Points: " << (*it2)->points();
    out << std::endl;
    ++it2;
  }
}

void League::displayMatches(std::ostream& out){
  auto matches = matches_.begin();
  int i = 1;

  std::sort(matches_.begin(), matches_.end(), [](Match* a, Match* b){
        if(a->date().d() < b->date().d()){
          return true;
        } else {
          return false;
        }
      });

   std::sort(matches_.begin(), matches_.end(), [](Match* a, Match* b){
        if(a->date().m() < b->date().m()){
          return true;
        } else {
          return false;
        }
      });

   std::sort(matches_.begin(), matches_.end(), [](Match* a, Match* b){
        if(a->date().y() < b->date().y()){
          return true;
        } else {
          return false;
        }
      });

 

  while(matches != matches_.end()){
    displayMatch(*matches, out, i);
      
    ++matches; 
    ++i;
  }
}

void League::displayMatch(Match* m, std::ostream& out, int i){
   std::string played = "NOT PLAYED";
      
    if(m->isPlayed()){
      played = "PLAYED";  
    }
              
    out << "#[" << i << "] " << m->home()->name();
    out << " - " << m->away()->name();
    out << " | Date: " << m->date().human();
    out << " | " << played;

    if(played == "PLAYED"){
      out << " | " << m->score();
    }
  
    out << std::endl;
}

Match* League::getMatch(int id){
  auto it = matches_.begin();
  int c = 1;

  while(it != matches_.end()){
    if(c == id){
      return *it;
    }  

    ++c;
    ++it;
  }

  return nullptr;
}

#endif
