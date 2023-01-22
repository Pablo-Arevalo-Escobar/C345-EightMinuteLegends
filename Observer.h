//
// Created by Derek James on 2021-04-09.
//

#ifndef C345_ASS2_OBSERVER_H
#define C345_ASS2_OBSERVER_H


class Observer {
  public:
    Observer()=default;
    virtual ~Observer()=default;
    virtual void update()=0;
};


#endif //C345_ASS2_OBSERVER_H
