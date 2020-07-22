#ifndef TEAM_H_
#define TEAM_H_

#include <string>
#include <vector>

class Team {
  public:
    Team() = default;
    Team(std::string name, std::string country, std::string city, std::string stadium) :
      name_{name}, country_{country}, city_{city}, stadium_{stadium} {
        id_ = name_+country_;
      };

    void setName(std::string name){ name_ = name; };
    void setCountry(std::string country){ country_ = country; };
    void setCity(std::string city){ city_ = city; };
    void setStadium(std::string stadium){ stadium_ = stadium_; };
    
    std::string id(){ return id_; }   
    std::string name(){ return name_; };
    std::string country(){ return country_; };
    std::string city(){ return city_; }; 
    std::string stadium(){ return stadium_; };

    int wins(){ return wins_; };
    int draws(){ return draws_; };
    int losses(){ return loss_; };
    int scored(){ return scored_; };
    int recieved(){ return recieved_; };
    int goalDiff(){ return scored_-recieved_; };
    int points(){
      return wins_*3 + draws_;
    }

    void addWin(){ ++wins_; };
    void removeWin(){ --wins_; };
    void addLoss(){ ++loss_; };
    void removeLoss(){ --loss_; };
    void addDraw(){ ++draws_;};
    void removeDraw(){ --draws_; };

    void score(int scored){ scored_ += scored; };
    void recieve(int recieved){ recieved_ += recieved; };
    void unscore(int scored){ scored_ -= scored; };
    void unrecieve(int recieved){ recieved_ -= recieved; };
  private:
    std::string id_;
    std::string name_;
    std::string country_;
    std::string city_;
    std::string stadium_;

    int wins_;
    int loss_;
    int draws_;

    int scored_;
    int recieved_;
};

#endif
