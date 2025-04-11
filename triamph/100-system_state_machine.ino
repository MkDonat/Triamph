#include <Action.h>
#include <AgileStateMachine.h>
#include <LinkedList.h>
#include <State.h>
#include <Transition.h>

// New finite state  machine
StateMachine ssm; // sm -> system state machine

// Transitions implementation
bool transistion(){
  return false;
};
long startingTime = 2000; // Délai d’attente dans l’état de démarrage

void setup_ssm(){
  //fsm -> adding new states
  State* STARTING = fsm.addState(
    "Etat Transitoire, nécessaire pour
    un bon fonctionnement de
    la machine à état...",
    nullptr, //onEnter_...
    onExit_STARTING,
    nullptr //onRun_
  );

  State* SETTING = fsm.addState(
    "Arrêt total, modification 
    des paramètres en cours...",
    onEnter_SETTING,
    onExit_SETTING,
    onRun_SETTING
  );

  State* IDLE = fsm.addState(
    "Tous les actionneurs en position défaut,
    Aucun instrument (capteur) activé/utilisé",
    onEnter_IDLE,
    onExit_IDLE,
    onRun_IDLE
  );

  State* COLLECTING = fsm.addState(
    "Collecte des déchets, ensuite
    retour à la position défaut",
    onEnter_COLLECTING,
    onExit_COLLECTING,
    onRun_COLLECTING
  );

  State* OC_CLAMPS = fsm.addState( //OC -> CLOSING/OPENING CLAMPS
    "Ouverture/fermeture des pinces",
    onEnter_OC_CLAMPS,
    onExit_OC_CLAMPS,
    onRun_OC_CLAMPS
  );

  State* BACK2HOME = fsm.addState(
    "Retour à la maison",
    onEnter_BACK2HOME,
    onExit_BACK2HOME,
    onRun_BACK2HOME
  );

  State* LU = fsm.addState( // LU -> Loading Unloading
    "Chargement/Déchargement du bac",
    onEnter_LU,
    onExit_LU,
    onRun_LU
  );

  State* YAW_CONTROL = fsm.addState(
    "Rotation",
    onEnter_YAW_CONTROL,
    onExit_YAW_CONTROL,
    onRun_YAW_CONTROL
  );

  State* THRUST_CONTROL = fsm.addState(
    "POUSSÉE",
    onEnter_THRUST_CONTROL,
    onExit_THRUST_CONTROL,
    onRun_THRUST_CONTROL
  );

  //ssm -> transitions linking
  STARTING->addTransition(IDLE,startingTime);
  //ssm -> init
  ssm.setInitialState(STARTING);
  ssm.start();
}
void system_state_machine_execute(){
  ssm.execute();
}
