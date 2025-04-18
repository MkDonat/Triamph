#include <Action.h>
#include <AgileStateMachine.h>
#include <LinkedList.h>
#include <State.h>
#include <Transition.h>

// New finite state  machine
StateMachine ssm; // ssm -> system state machine

// Transitions implementation
bool transistion(){
  return false;
};
long startingTime = 2000; // Délai d’attente dans l’état de démarrage

void setup_ssm(){
  //fsm -> adding new states
  State* STARTING = ssm.addState(
    "Etat Transitoire, nécessaire pour"
    "un bon fonctionnement de"
    "la machine à état...",
    nullptr, //onEnter_...
    onExit_STARTING,
    nullptr //onRun_
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
    "POUSSÉE",
    onEnter_THRUST_CONTROL,
    onExit_THRUST_CONTROL,
    onRun_THRUST_CONTROL
  );

  //ssm -> transitions linking

  /*T0**/STARTING->addTransition(IDLE,startingTime);

  /*T1**/SETTING->addTransition(IDLE,false);//Click B
  /*T2**/IDLE->addTransition(SETTING,false);//Click PAD

  /*T3**/IDLE->addTransition(COLLECTING,false);
  /*T4**/COLLECTING->addTransition(IDLE,false);

  /*T5**/IDLE->addTransition(LU,false);
  /*T6**/LU->addTransition(IDLE,false);

  /*T7**/IDLE->addTransition(BACK2HOME,false);
  /*T8**/BACK2HOME->addTransition(IDLE,false);

  /*T9**/IDLE->addTransition(THRUST_CONTROL,false);
  /*T10*/THRUST_CONTROL->addTransition(IDLE,false);

  /*T11*/IDLE->addTransition(YAW_CONTROL,false);
  /*T12*/YAW_CONTROL->addTransition(IDLE,false);

  /*T12*/IDLE->addTransition(OC_CLAMPS,false); //OC -> OPEN/CLOSE CLAMPS
  /*T14*/OC_CLAMPS->addTransition(IDLE,false);

  //ssm -> init
  ssm.setInitialState(STARTING);
  ssm.start();
}
void system_state_machine_execute(){
  ssm.execute();
}
