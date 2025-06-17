#include <Action.h>
#include <AgileStateMachine.h>
#include <LinkedList.h>
#include <State.h>
#include <Transition.h>

long startingTime = 2000; // Délai d’attente dans l’état de démarrage

// New finite state  machine
StateMachine ssm; // ssm -> system state machine

// Transitions implementation
bool transition_1(){
  return false;
}
bool transition_2(){
  return false;
}
bool transition_3(){
  if (strcmp(receivedData.button_msg, "COLLECT") == 0){
    return true;
  }else return false;
}
bool transition_4(){
  return Collecting_task_completed;
}
bool transition_5(){
  if (strcmp(receivedData.button_msg, "LOAD_UNLOAD") == 0){
    return true;
  }else return false;
}
bool transition_6(){
  return is_LoadUnload_task_complete;
}
bool transition_7(){
  return false;
}
bool transition_8(){
  return false;
}
bool transition_9(){
  if( *dir_y == 1 || *dir_y == -1 ){
    return true;
  }else{
    return false;
  } 
}
bool transition_10(){
  if( *dir_y == 1 || *dir_y == -1 ){
    return false;
  }else{
    return true;
  }
}
bool transition_11(){
  if( *dir_x == 1 || *dir_x == -1 ){
    return true;
  }else{
    return false;
  } 
}
bool transition_12(){
  if( *dir_x == 1 || *dir_x == -1 ){
    return false;
  }else{
    return true;
  } 
}
bool transition_13(){
  if (strcmp(receivedData.button_msg, "CLAMPS") == 0){
    return true;
  }else return false;
  //return false;
}
bool transition_14(){
  return is_OC_Clamps_task_complete;
  //return false;
}

void setup_ssm(){
  //fsm -> adding new states
  State* STARTING = ssm.addState(
    "Etat Transitoire, nécessaire pour"
    "un bon fonctionnement de"
    "la machine à état...",
    nullptr, //onEnter_...
    onExit_STARTING,
    nullptr //onRun_...
  );

  State* SETTING = ssm.addState(
    "Arrêt total, modification" 
    "des paramètres en cours...",
    onEnter_SETTING,
    onExit_SETTING,
    onRun_SETTING
  );

  State* IDLE = ssm.addState(
    "Tous les actionneurs en position défaut,"
    "Aucun instrument (capteur) activé/utilisé",
    onEnter_IDLE,
    onExit_IDLE,
    onRun_IDLE
  );

  State* COLLECTING = ssm.addState(
    "Collecte des déchets, ensuite"
    "retour à la position défaut",
    onEnter_COLLECTING,
    onExit_COLLECTING,
    onRun_COLLECTING
  );

  State* OC_CLAMPS = ssm.addState( //OC -> CLOSING/OPENING CLAMPS
    "Ouverture/fermeture des pinces",
    onEnter_OC_CLAMPS,
    onExit_OC_CLAMPS,
    onRun_OC_CLAMPS
  );

  State* BACK2HOME = ssm.addState(
    "Retour à la maison",
    onEnter_BACK2HOME,
    onExit_BACK2HOME,
    onRun_BACK2HOME
  );

  State* LU = ssm.addState( // LU -> Loading Unloading
    "Chargement/Déchargement du bac",
    onEnter_LU,
    onExit_LU,
    onRun_LU
  );

  State* YAW_CONTROL = ssm.addState(
    "Rotation",
    onEnter_YAW_CONTROL,
    onExit_YAW_CONTROL,
    onRun_YAW_CONTROL
  );

  State* THRUST_CONTROL = ssm.addState(
    "POUSSÉE/GAZ",
    onEnter_THRUST_CONTROL,
    onExit_THRUST_CONTROL,
    onRun_THRUST_CONTROL
  );

  //ssm -> transitions linking

  /*T0**/STARTING->addTransition(IDLE,startingTime);

  /*T1**/SETTING->addTransition(IDLE,transition_1);
  /*T2**/IDLE->addTransition(SETTING,transition_2);

  /*T3**/IDLE->addTransition(COLLECTING,transition_3);
  /*T4**/COLLECTING->addTransition(IDLE,transition_4);

  /*T5**/IDLE->addTransition(LU,transition_5);
  /*T6**/LU->addTransition(IDLE,transition_6);

  /*T7**/IDLE->addTransition(BACK2HOME,transition_7);
  /*T8**/BACK2HOME->addTransition(IDLE,transition_8);

  /*T9**/IDLE->addTransition(THRUST_CONTROL,transition_9);
  /*T10*/THRUST_CONTROL->addTransition(IDLE,transition_10);

  /*T11*/IDLE->addTransition(YAW_CONTROL,transition_11);
  /*T12*/YAW_CONTROL->addTransition(IDLE,transition_12);

  /*T13*/IDLE->addTransition(OC_CLAMPS,transition_13); //OC -> OPEN/CLOSE CLAMPS
  /*T14*/OC_CLAMPS->addTransition(IDLE,transition_14);

  //ssm -> init
  ssm.setInitialState(STARTING);
  ssm.start();
}
void system_state_machine_execute(){
  ssm.execute();
}
