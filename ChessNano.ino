
byte touche = 1; // 1 white on top 2 white on bottom

const byte ROWS = 8; 
const byte COLS = 8;

byte bitMap[COLS]; 

byte rowPins[ROWS] = {19,20,21,22,23,24,25,26}; //connect to the row pinouts of the keypad

byte colPins[COLS] ={5,6,7,8,9,10,11,12}; //connect to the column pinouts of the keypad


unsigned long startTime;



// la position 
String strPos ="195.195.195.195.195.195.195.195.time";
// Nombre de positon dans l'enregistrement
int nbPos=0;


//*************************************************************************************************************************
// SCAN LE CLAVIER
bool getKeys() {
  bool keyActivity = false;

  
  if ( (millis()-startTime)>10 ) { // DebounceTime
              
    keyActivity = scanKeys(); //REGARDE S IL Y A EU DU CHANGEMENT SUR LE CLAVIER EN SCANNANT LE CLAVIER
    startTime = millis();
  }

  return keyActivity;
}


//*************************************************************************************************************************
bool scanKeys() {

  bool anyActivity = false; // INITIALISE L ETAT
  byte etat=0;
  // RE INITIALISE LES PORTS A CHAQUE FOIS 
  // CELA PERMET DE LES UTILISER AVEC UN AUTRE MATERIEL
  for (byte r=0; r<ROWS; r++) {
    pinMode(rowPins[r],INPUT_PULLUP);
  }

  // STOCK DANS LA VARIABLE bitMap L ETAT DE CHAQUE TOUCHE
  for (byte c=0; c<COLS; c++) {
    pinMode(colPins[c],OUTPUT);
    digitalWrite(colPins[c], LOW);  // Begin column pulse output.
    for (byte r=0; r<ROWS; r++) {
      
      etat=!digitalRead(rowPins[r]);

      if (etat!=bitRead(bitMap[r],c))  // SI UNE TOUCHE DIFFERE DE SON ANCIEN ETATT
      {
        anyActivity=true;      
            
      }       
      bitWrite(bitMap[r], c, etat);  // ECRIS L ETAT DU BOUTON EN INVERSANT CAR 0 = APPUYE
    }
    // Set pin to high impedance input. Effectively ends column pulse.
    // REMET LE PORT EN ETAT POUR LA PROCHAINE LECTURE
    digitalWrite(colPins[c],HIGH);
    pinMode(colPins[c],INPUT);
  }

  return anyActivity;
}
//*************************************************************************************************************************




//-----------------------------------------------------------
// effectue les permutations de bits pour renvoyer toujours 195. ...
// facultatif si on place cela directement sur le prog PC
String permut() {

  byte TempoBitMap=0;
  strPos="";

  if (touche==1) { 
      for(int i=0;i<COLS;i++) {
        strPos = strPos+ bitMap[i];
        strPos = strPos+ ".";
        
      }       
    } else if (touche==2) { 
      for(int i=7;i>=0;i--)
      {
        for (int b=7;b>=0;b--)
        {
          bitWrite( TempoBitMap, 7-b, bitRead(bitMap[i],b) );       
        }
        strPos = strPos+ TempoBitMap;
        strPos = strPos + ".";
        
      }      
    }    

    return strPos;    
}

//******************************************************************************************
//                          SETUP
//******************************************************************************************

void setup(){

    Serial.begin(19200);
    
    
    Serial.flush();    
    touche=2;

}

//******************************************************************************************
//                          LOOP
//******************************************************************************************
  
void loop(){

 // ChessBoard.getKeys() renvoie true en cas de changement de l'état du clavier
 if ( getKeys() ) {
    
    Serial.println( millis()-startTime );   //temps de stabilité de la position
    //calcul la chaine de la position
    permut();

    //affiche les infos sur le port serie
   
    Serial.println(strPos);                 //la position 195. ...

    nbPos ++;

    //initialise le compteur pour mesurer le temps
    startTime = millis(); 
  }
}

