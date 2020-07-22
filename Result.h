#ifndef RESULT_H_
#define RESULT_H_

class Result {
  public:
    Result(Match* match, int homeGoals, int awayGoals) : match_{match}, homeGoals_{homeGoals}, awayGoals_{awayGoals}{};
    void apply();
    bool undo();
  private:
    Match* match_;
    int homeGoals_ = 0;
    int awayGoals_ = 0;
};

void Result::apply(){
  if(!match_->isPlayed()){
    match_->homeScore(homeGoals_);
    match_->awayScore(awayGoals_);
    match_->process();
  }
}

bool Result::undo(){
  if(match_->isPlayed()){
    match_->undo();

    return true;
  } else {
    return false;
  }
}

#endif
