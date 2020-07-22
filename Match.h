#ifndef MATCH_H_
#define MATCH_H_

#include <string>
#include "Team.h"

struct Date {
  int day;
  int month;
  int year;

  void setDate(int d, int m, int y){
    day = d;
    month = m;
    year = y;
  }

  int d(){ return day; }
  int m(){ return month; }
  int y(){ return year; }

  std::string human(){
    std::string d = std::to_string(day);
    std::string m = std::to_string(month);
    std::string y = std::to_string(year);

    return d+"-"+m+"-"+y;
  }
};

class Match {
  public:
    Match(Team* home, Team* away, int day, int month, int year){
      home_ = home;
      away_ = away;

      date_.setDate(day, month, year);
    };

    void homeScore(int goals){ homeScored_ += goals; };
    void awayScore(int goals){ awayScored_ += goals; };
    
    int homeGoals(){ return homeScored_; };
    int awayGoals(){ return awayScored_; };
    std::string score(){ return std::to_string(homeScored_) + ":" + std::to_string(awayScored_);  };

    Team* home(){ return home_; }
    Team* away(){ return away_; }
    Date date(){ return date_; }

    bool matchDate(int d, int m, int y){
      if(d == date_.d() && m == date_.m() && y == date_.y()){
        return true;
      } else {
        return false;
      }
    }

    bool awayOrHome(Team* t1, Team* t2){
      if(t1->name() == home()->name() ||  t2->name() == away()->name() ||
         t1->name() == away()->name() || t2->name() == home()->name() ){
        return true;      
      } else {
        return false;
      }
    };

    bool isPlayed(){ return played_; }

    void process(){
      played_ = true;

      home()->score(homeScored_);
      home()->recieve(awayScored_);
      
      away()->score(awayScored_);
      away()->recieve(homeScored_);

      if(homeScored_ > awayScored_){
        home()->addWin();
        away()->addLoss();
      } else if(homeScored_ == awayScored_){
        home()->addDraw();
        away()->addDraw();
      } else {
        home()->addLoss();
        away()->addWin();
      }
    }

    void undo(){
      played_ = false;

      home()->unscore(homeScored_);
      home()->unrecieve(awayScored_);

      away()->unscore(awayScored_);
      away()->unrecieve(homeScored_);
      
      if(homeScored_ > awayScored_){
        home()->removeWin();
        away()->removeLoss();
      } else if(homeScored_ == awayScored_){
        home()->removeDraw();
        away()->removeDraw();
      } else {
        home()->removeLoss();
        away()->removeWin();
      }
    }
  private:
    Team *home_;
    Team *away_;

    int homeScored_ = 0;
    int awayScored_ = 0;
    bool played_ = false;

    Date date_;
};

#endif
