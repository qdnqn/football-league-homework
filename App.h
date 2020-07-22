#ifndef APP_H_
#define APP_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <stack>

#include "Team.h"
#include "Match.h"
#include "League.h"
#include "Result.h"

class App {
  public:
    App() = default;

    void addTeam(std::istream&, std::ostream&);
    void addLeague(std::istream&, std::ostream&);
    void playMatch(std::istream&, std::ostream&);
    void undoMatch(std::ostream&);
    void detailsTeam(std::istream& inp, std::ostream& out);
    void detailsLeague(std::istream& inp, std::ostream& out);
    void detailsMatches(std::ostream&);
    void dummyData(std::istream&, std::ostream&);
  private:
    std::list<Team*> teams_;
    std::list<League*> leagues_;
    std::stack<Result*> results_;
};

void App::addTeam(std::istream& inp, std::ostream& out){
  std::string name, country, city, stadium;

  out << "Team name: ";
  inp >> name;
  out << "Country: ";
  inp >> country;

  auto it = teams_.begin();

  while(it != teams_.end()){
    if(name == (*it)->name() && country == (*it)->country()){
      throw std::invalid_argument("Team with that name already exists!");
    }

    ++it;
  }

  out << "City: ";
  inp >> city;
  out << "Stadium: ";
  inp >> stadium; 

  teams_.push_back(new Team(name, country, city, stadium));
  out << std::endl << "[--New team added!--]" << std::endl;
};

void App::addLeague(std::istream& inp, std::ostream& out){
  std::string name, country;

  out << "League name: ";
  inp >> name;

  out << "Country: ";
  inp >> country;
  
  if(teams_.size() > 1){
    auto it = leagues_.begin();
    std::vector<Team*> countryBasedTeams;

    while(it != leagues_.end()){
      if(name == (*it)->name() && country == (*it)->country()){
        throw std::invalid_argument("League with that name already exists!");
      }

      ++it;
    }

    auto it2 = teams_.begin();

    while(it2 != teams_.end()){
      if(country == (*it2)->country()){
        countryBasedTeams.push_back(*it2);
      }

      ++it2;
    }

    leagues_.push_back(new League(name, country, countryBasedTeams));
    out << std::endl  << "[--New league added!--]" << std::endl;

    leagues_.back()->generate(inp, out);
  } else {
    throw std::invalid_argument("You need atleast 2 teams to create new league.");
  }
}

void App::playMatch(std::istream& inp, std::ostream& out){
  int id, homeGoals, awayGoals;
  std::string name, country;

  out << "Please enter league name and country to list matches: ";
  inp >> name >> country;

  auto it = leagues_.begin();

  while(it != leagues_.end()){
    if((*it)->name() == name && (*it)->country() == country){
      (*it)->displayMatches(out);
    
      out << std::endl;
      out << "--------------------------------------------------------------" << std::endl; 
      
      break;
    }

    ++it;
  }
  
  auto c = 1;
  auto match = (*it)->getMatch(c);

  while(match != nullptr){
    out << "Playing match: " <<  match->home()->name() << " - " << match->away()->name() << std::endl;

    out << "Enter home goals: ";
    inp >> homeGoals;

    out << "Enter away goals: ";
    inp >> awayGoals;

    results_.push(new Result(match, homeGoals, awayGoals));
    results_.top()->apply();

    match = (*it)->getMatch(++c);
    
    inp.clear();
    out.clear();

    homeGoals = 0;
    awayGoals = 0;
  }
}

void App::undoMatch(std::ostream& out){
  if(results_.top()->undo()){
    out << "Poništili ste zadnje igranu utakmicu!" << std::endl;
  } else {
    out << "Utakmica nije poništena." << std::endl;
  }
}

void App::detailsTeam(std::istream& inp, std::ostream& out){
  std::string name, country;

  inp.clear();

  out << "Please enter name and country: ";
  inp >> name >> country;
  out << std::endl;

  auto it = teams_.begin();
  bool found = false;
  
  if(teams_.size() > 0){
  while(it != teams_.end()){
    if((*it)->name() == name && (*it)->country() == country){
      auto t = *it;
      out << "-------------------------------------------" << std::endl;
      out << "Team information:" << std::endl;
      out << "Team: " << t->name() << std::endl;
      out << "Country: " << t->country() << std::endl;
      out << "City: " << t->city() << std::endl;
      out << "Stadium: " << t->stadium() << std::endl;
      out << "Wins: " << t->wins() << " - Losses: " << t->losses() << " - Draws: " << t->draws();
      out << std::endl;
      out << "Goal diff: " << t->goalDiff() << std::endl;
      
      out << std::endl;
      out << "Team matches: " << std::endl << std::endl;

      auto lgIt = leagues_.begin();

      while(lgIt != leagues_.end()){
        (*lgIt)->teamMatches(*it, out);
        ++lgIt;
      }
      
      out << "---------------------------------------------" << std::endl;
      
      found = true;
      break;
    }

    ++it;
  }

  if(!found){
    out << "Team not found.";
  }
  } else {
    out << "Team list is empty.";
  }
}

void App::detailsMatches(std::ostream& out){  
  auto it = leagues_.begin();

  while(it != leagues_.end()){
    out << "--------------------------------------------------------------" << std::endl;
    out << "League: " << (*it)->name() << " [" << (*it)->country() << "] " << std::endl << std::endl;

    (*it)->displayMatches(out);
    
    out << std::endl;
    out << "--------------------------------------------------------------" << std::endl;
    
    ++it; 
  }
}

void App::detailsLeague(std::istream& inp, std::ostream& out){
  std::string name, country;

  inp.clear();

  out << "Please enter league name and country to list matches: ";
  inp >> name >> country;
  out << std::endl;

  auto it = leagues_.begin();

  while(it != leagues_.end()){
    if((*it)->name() == name && (*it)->country() == country){
      (*it)->generateTable(out);
      break;
    }

    ++it;
  }
}

void App::dummyData(std::istream& inp, std::ostream& out){
  out << std::endl;
  out << "Adding league: La-Liga" << std::endl;
  out << "Teams: Real-Madrid, Barcelona, Sevilla, Valencia" << std::endl;

  teams_.push_back(new Team("Real-Madrid","Spain","Real-Madrid","Santiago-Bernabeu"));
  teams_.push_back(new Team("Barcelona", "Spain", "Barcelona","Camp Nou"));
  teams_.push_back(new Team("Sevilla","Spain","Seville","Ramon-Sanchez-Pizjuan"));
  //teams_.push_back(new Team("Valencia","Spain","Valencia","Mestalla"));
  
  auto it = teams_.begin();
  std::vector<Team*> countryBasedTeams;

  while(it != teams_.end()){
    countryBasedTeams.push_back(*it);
    ++it;
  }

  leagues_.push_back(new League("La-Liga","Spain", countryBasedTeams));
  leagues_.back()->generate(inp, out);
}

#endif
