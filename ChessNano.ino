//numéro des pins de l'arduino pour les lignes et les colonnes
byte const rowPins[8] = {2,3,4,5,6,7,8,9}; //INPUT
byte const colPins[8] = {10,11,12,14,15,16,17,13}; //OUTPUT

//état du plateau contenant les 8 bytes
byte BitMap[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//temps du début du dernier scan
unsigned long startTime;

//signature sous forme de string a transmetre via USB
String strPos = "0.0.0.0.0.0.0.0.";

//temps minimum avant rescan de l'échiquier
byte const debounceTime =10;

//*********************************************************
// SCAN LE CLAVIER
//*********************************************************
bool getKeys() 
{
	bool keyActivity = false;
	//strPos = "";

	if ( (millis()-startTime)>debounceTime ) //si le dernier scan n'est pas trop récent
	{							
		// RE INITIALISE LES PORTS A CHAQUE FOIS 
		// CELA PERMET DE LES UTILISER AVEC UN AUTRE MATERIEL
		for (byte r=0; r<8; r++) { pinMode(rowPins[r],INPUT_PULLUP); }				
		for (byte c=0; c<8; c++) 
		{
			pinMode(colPins[c],OUTPUT);
			digitalWrite(colPins[c], LOW);	// Begin column pulse output.
			for (byte r=0; r<8; r++) 
			{
				if (!digitalRead(rowPins[r])!=bitRead(BitMap[7-c],7-r)) { keyActivity=true; } 			
				bitWrite(BitMap[7-c], 7-r, !digitalRead(rowPins[r]));  // ECRIS L ETAT DU BOUTON EN INVERSANT CAR 0 = APPUYE
			}	
			//strPos = strPos + BitMap[c];
    		//strPos = strPos + ".";  		
			// Set pin to high impedance input. Effectively ends column pulse.
			// REMET LE PORT EN ETAT POUR LA PROCHAINE LECTURE
			digitalWrite(colPins[c],HIGH);
			pinMode(colPins[c],INPUT);
		}
		
	}	
	return keyActivity;
}
//-----------------------------------------------------------
// effectue les permutations de bits pour renvoyer les bonnes signatures plateau
//-----------------------------------------------------------
void PermuToBOARD() {
  //byte TempoBitMap = 0;
  strPos = "";
	for (int i=0; i<8; i++)
  	{
  		strPos = strPos + BitMap[i];
    	strPos = strPos + ".";  
  	}
}

//******************************************************************************************
//                          SETUP
//******************************************************************************************
void setup()
{
  //initialisation du port serie
  Serial.begin(57600);   
  getKeys();
  startTime = millis();  
}

//******************************************************************************************
//                          LOOP
//******************************************************************************************

void loop() {
 
  if ( getKeys() )
  {
    PermuToBOARD(); 
    Serial.println( millis() - startTime ); 
    Serial.println(strPos);
    startTime = millis();		//stock l'heure de fin du scan
  }
}

