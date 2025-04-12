#include <Action.h>
#include <AgileStateMachine.h>
#include <LinkedList.h>
#include <State.h>
#include <Transition.h>

// Controller state  machine
StateMachine csm;

// Transitions implementation
bool transition(){
  return false;
};
long startingTime = 2000;

void setup_csm(){
  //fsm -> states creation
  State* STARTING = csm.addState(
    "Etat Transitoire...",
    nullptr, //onEnter...
    onExit_STARTING, //onExit...
    nullptr //onRun...
  );

  State* NORMAL = csm.addState(
    "Mode normal",
    onEnter_NORMAL,
    onExit_NORMAL,
    onRun_NORMAL
  );

  //fsm -> transitions linking
  STARTING->addTransition(NORMAL,startingTime);
  NORMAL->addTransition(STARTING,false);
  //fsm -> init
  csm.setInitialState(STARTING);
  csm.start();
}
void csm_execute(){
  csm.execute();
}
