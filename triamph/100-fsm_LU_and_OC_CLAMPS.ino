#include <Action.h>
#include <AgileStateMachine.h>
#include <LinkedList.h>
#include <State.h>
#include <Transition.h>

StateMachine fsm_LU;
StateMachine fsm_OC_CLAMPS;

bool onButtonClick_LU(){
  return is_button_clicked_LU;
}

bool onButtonClick_OC_CLAMPS(){
  return is_button_clicked_OC_CLAMPS;
}

long startingTime = 2000;

void setup_fsm(){

  //LOADING_UNLOADING
  State* IDLE_FU = fsm_LU.addState(
    "IDLE", nullptr, onExit_IDLE, nullptr
  );

  State* UNLOAD_TRASH = fsm_LU.addState(
    "Déchargement",
    onEnter_UNLOAD_TRASH,
    onExit_UNLOAD_TRASH,
    onRun_UNLOAD_TRASH
  );

  State* LOAD_TRASH = fsm_LU.addState(
    "Chargement",
    onEnter_LOAD_TRASH,
    onExit_LOAD_TRASH,
    onRun_LOAD_TRASH
  );

  //OPENING_CLOSING_CLAMPS
  State* IDLE_OC_CLAMPS = fsm_OC_CLAMPS.addState(
    "IDLE", nullptr, onExit_IDLE, nullptr
  );

  State* CLOSING_CLAMPS = fsm_OC_CLAMPS.addState(
    "Déchargement",
    onEnter_CLOSING_CLAMPS,
    onExit_CLOSING_CLAMPS,
    onRun_CLOSING_CLAMPS
  );

  State* OPENING_CLAMPS = fsm_OC_CLAMPS.addState(
    "Chargement",
    onEnter_OPENING_CLAMPS,
    onExit_OPENING_CLAMPS,
    onRun_OPENING_CLAMPS
  );

  // Transitions_LU
  LOAD_TRASH->addTransition(UNLOAD_TRASH, onButtonClick_LU);
  UNLOAD_TRASH->addTransition(LOAD_TRASH, onButtonClick_LU);
  IDLE_FU->addTransition(LOAD_TRASH, startingTime);

  // Transitions_OC_CLAMPS
  OPENING_CLAMPS->addTransition(CLOSING_CLAMPS, onButtonClick_OC_CLAMPS);
  CLOSING_CLAMPS->addTransition(OPENING_CLAMPS, onButtonClick_OC_CLAMPS);
  IDLE_OC_CLAMPS->addTransition(CLOSING_CLAMPS, startingTime);

  fsm_LU.setInitialState(IDLE_FU);
  fsm_LU.start();

  fsm_OC_CLAMPS.setInitialState(IDLE_OC_CLAMPS);
  fsm_OC_CLAMPS.start();
}

void fsm_execute(){
  fsm_LU.execute();
  fsm_OC_CLAMPS.execute();
}