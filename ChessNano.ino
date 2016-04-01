

int keyPress;

byte touche = 0;

const byte droite=3;
const byte gauche=1;
const byte haut=5;
const byte bas=2;
const byte entree=4;
const byte aucune=0;


//-----------------------------------------------------------
// GESTION DU Pseudo CLAVIER 8x8
//-----------------------------------------------------------

#include "utility/Key.h"
#include "Keypad.h"

const byte ROWS = 8; 
const byte COLS = 8;
//define the cymbols on the buttons of the keypads

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','4','5','6','7','8'},
  {'a','b','c','d','e','f','g','h'},
  {'A','B','C','D','E','F','G','H'},
  {'&','é','#','{','(','-','è','_'},
  {'i','j','k','l','m','n','o','p'},
  {'I','J','K','L','M','N','O','P'},
  {'q','r','s','t','u','v','w','x'},
  {'Q','R','S','T','U','V','W','X'}
};


byte rowPins[ROWS] ={2,3,4,5,6,7,8,9}; //,6,5,4,3,2}; //connect to the row pinouts of the keypad
byte colPins[COLS] ={10,11,12,14,15,16,17,18}; //,13,0,1,2,3} ; //connect to the column pinouts of the keypad


byte NewbitMap[8]={195,195,195,195,195,195,195,195};

//initialize an instance of class NewKeypad
Keypad ChessBoard = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 



unsigned long startTime;

// la position 
String strPos ="195.195.195.195.195.195.195.195.time";
// Nombre de positon dans l'enregistrement
int nbPos=0;




//-----------------------------------------------------------
// effectue les permutations de bits pour renvoyer toujours 195. ...
// facultatif si on place cela directement sur le prog PC
String permut() {

  byte TempoBitMap=0;
  strPos="";

  if (touche==haut) { 
      for(int i=0;i<ROWS;i++) {
        strPos = strPos+ ChessBoard.bitMap[i];
        strPos = strPos+ ".";
        NewbitMap[i]=ChessBoard.bitMap[i];
      }       
    } else if (touche==droite) {
      
      for(int i=ROWS-1;i>=0;i--)
      {
        for (int b=ROWS-1;b>=0;b--)
        {
          bitWrite( TempoBitMap, b, bitRead(ChessBoard.bitMap[b],i) ); 
        } 
        strPos = strPos+ TempoBitMap;
        strPos = strPos + ".";
        NewbitMap[ROWS-1-i]=TempoBitMap; 
      }

    } else if (touche==gauche) {

      for(int i=0;i<ROWS;i++)
      {
        for (int b=0;b<ROWS;b++)
        {
          bitWrite( TempoBitMap, ROWS-1-b, bitRead(ChessBoard.bitMap[b],i) );   
        }
        strPos = strPos+ TempoBitMap;
        strPos = strPos + ".";
        NewbitMap[i]=TempoBitMap;
      }

    } else if (touche==bas) { 
      for(int i=ROWS-1;i>=0;i--)
      {
        for (int b=ROWS-1;b>=0;b--)
        {
          bitWrite( TempoBitMap, ROWS-1-b, bitRead(ChessBoard.bitMap[i],b) );       
        }
        strPos = strPos+ TempoBitMap;
        strPos = strPos + ".";
        NewbitMap[ROWS-1-i]=TempoBitMap;
      }      
    }    


    return strPos;
    
}

//******************************************************************************************
//                          SETUP
//******************************************************************************************

void setup()
{
//----------------------------------------------------------
//initialisation su port serie
    Serial.begin(57600);
   
//----------------------------------------------------------

    ChessBoard.setDebounceTime(10);
    ChessBoard.setHoldTime(2000);    


 touche=bas;

}

//******************************************************************************************
//                          LOOP
//******************************************************************************************
  
void loop(){

 // ChessBoard.getKeys() renvoie true en cas de changement de l'état du clavier
 if ( ChessBoard.getKeys() ) {
    
    //calcul la chaine de la position
    permut();

    //affiche les infos sur le port serie
    Serial.println( millis()-startTime );   //temps de stabilité de la position
    Serial.println(strPos);                 //la position 195. ...

   //initialise le compteur pour mesurer le temps
    startTime = millis(); 
  }
}

