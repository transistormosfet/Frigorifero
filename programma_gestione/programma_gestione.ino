//programma frigorifero daewoo
unsigned long t0, tdebounce = 10 , t1, t2 , tcifre = 20, t3 , silentperiod = 7800000, silentpreventiontime = 2400000 , t4, t5, t6, t7, t8; //per debounce tasti
unsigned long tcronometrocompon, tcronometrocompoff, tempocompon, tempocompoff, tempoporta, tcronometroporta;

unsigned long tempoallarme, ttone, ciclo = 0;

int rstato, fstato, silent = 0; //modalità di funzionamento (1,2,3,silent)
int cont = 1 , cont2 = 1; //variabili delle cifre led 1 , 2 , 3 , qf o qr frigo e freetzer
bool Door, silentst = 0, primaporta = 0, firststopcomp = 1, first4stf = 1, first4str = 1, defrost = 0, firstdefrost = 1;
float Fsens, Rsens, Dsens;
float FTon, FToff, RTon, RToff;


#define n 1
#define Vd 0.5
#define G 2.11

void tasti();
void led(int rst, int fst, int sil);
void letturasensori();
void pilotaggio();
void defrosting();

void setup() {
  pinMode(2, OUTPUT);   //Ffan output
  pinMode(3, OUTPUT);   //Comp(e Cfan) output
  pinMode(4, OUTPUT);   //Rfan output
  pinMode(5, OUTPUT);   //HTR output
  pinMode(A3, INPUT);   //Door input analog to digitalize threshold 800 porta aperta == alto
  pinMode(6, OUTPUT);   //buzzer digital output
  pinMode(7, OUTPUT);   //riga1 digital output
  pinMode(8, OUTPUT);   //riga2 digital output
  pinMode(9, OUTPUT);   //riga3 digital output
  pinMode(10, INPUT);   //tasti digital input
  pinMode(11, OUTPUT);  //ScanA digital output
  pinMode(12, OUTPUT);  //ScanB digital output
  pinMode(13, OUTPUT);  //ScanC digital output
  pinMode(A0, INPUT);   //Fsens analog input
  pinMode(A1, INPUT);   //Dsens analog input
  pinMode(A2, INPUT);   //Rsens analog input
}

void loop() {
  tasti();
  led(rstato, fstato, silent);
  switch (fstato) {  //mod 1, 2, 3, 4 (1, 2, 3, 4 ,5= silent)
    case 1:
      FTon = -15.5;
      FToff = -20.5;
      break;
    case 2:
      FTon = -17.5;
      FToff = -22.5;
      break;
    case 3:
      FTon = -19.5;
      FToff = -24.5;
      break;
  }
  switch (rstato) {  //mod 1, 2, 3, 4 (1, 2, 3, 4 ,5= silent)
    case 1:
      RTon = 1.35;
      RToff = 1;
      break;
    case 2:
      RTon = -0.65;
      RToff = -1;
      break;
    case 3:
      RTon = -2.65;
      RToff = -3;
      break;
  }

  letturasensori();
  pilotaggio();
  if (defrost) {
    defrosting();
  }

}


void tasti() {
  digitalWrite(7, LOW);                                             //spegni riga 1
  digitalWrite(8, LOW);                                             //spegni riga 2
  digitalWrite(9, LOW);                                             //spegni riga 3
  digitalWrite(11, HIGH);                                           //accendi scanA
  digitalWrite(12, LOW);                                            //spegni scanB
  digitalWrite(13, LOW);                                            //spegni scanC
  if ((!digitalRead(10)) && (millis() - t0 > tdebounce)) {          //tasto2
    t0 = millis();
    rstato ++;            //modifica stato
    if (rstato > 4) rstato = 1;
  }
  digitalWrite(11, LOW);                                            //spegni scanA
  digitalWrite(12, HIGH);                                           //accendi scanB
  if ((!digitalRead(10)) && (millis() - t0 > tdebounce)) {          //tasto3
    t0 = millis();
    silent = !silent;      //modifica stato
  }
  digitalWrite(12, LOW);                                            //spegni scanB
  digitalWrite(13, HIGH);                                           //accendi scanC
  if ((!digitalRead(10)) && (millis() - t0 > tdebounce)) {          //tasto1
    t0 = millis();
    fstato ++;            //modifica stato
    if (fstato > 4) fstato = 1;
  }
  digitalWrite(13, LOW);                                            //spegni scanC
}

void led(int rst, int fst, int sil) { //si passa la variabile stato e si capisce quale led accendere della matrice

  if (sil) {                    //ilent mode
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(13, HIGH);
  }
  if ((rst == 1) && (!sil)) {   //1mode refrigerator
    if ((cont == 1) && (millis() - t1 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 2) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 3) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 4) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((rst == 2) && (!sil)) {   //2mode refrigerator
    if ((cont == 1) && (millis() - t1 > tcifre)) {            //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 2) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 3) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 4) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((rst == 3) && (!sil)) {   //3mode refrigerator
    if ((cont == 1) && (millis() - t1 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 2) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 3) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 4) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((rst == 4) && (!sil)) {   //4mode refrigerator
    if ((cont == 1) && (millis() - t1 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 2) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 3) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = cont + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont == 4) && (millis() - t1 > tcifre)) {
      t1 = millis();
      cont = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }

  if ((fst == 1) && (!sil)) {   //1mode refrigerator
    if ((cont2 == 1) && (millis() - t2 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 2) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 3) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 4) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((fst == 2) && (!sil)) {   //2mode refrigerator
    if ((cont2 == 1) && (millis() - t2 > tcifre)) {            //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 2) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 3) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 4) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((fst == 3) && (!sil)) {   //3mode refrigerator
    if ((cont2 == 1) && (millis() - t2 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 2) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 3) && (millis() - t2 > tcifre)) {
      t1 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 4) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
  }
  if ((fst == 4) && (!sil)) {   //4mode refrigerator
    if ((cont2 == 1) && (millis() - t2 > tcifre)) {           //cont è una variabile che fa scorrere l'accensione sui vari led (es in mod 3 si accenderanno alternativamente 1 2 3 led, ma appariranno accesi contemporaneamente perché a velocità maggiore di 24 fps)
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 2) && (millis() - t2 > tcifre)) {
      t2 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(13, LOW);
    }
    if ((cont2 == 3) && (millis() - t2 > tcifre)) {
      t1 = millis();
      cont2 = cont2 + 1;
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(9, LOW);
      digitalWrite(13, HIGH);
    }
    if ((cont2 == 4) && (millis() - t2 > tcifre)) {
      t1 = millis();
      cont2 = 1;
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(13, LOW);
    }
  }

}
void letturasensori() {
  //porta chiusa == alto
  if (analogRead(A3) > 800) {//Door input analog to digitalize threshold 800
    Door = 0; //chiusa
    if (!primaporta) {
      primaporta = 1;
      t4 = millis();
    }
  } else {
    Door = 1; //aperta
    if (primaporta) {
      primaporta = 0;
    }
  }
  Fsens = ((((analogRead(A0) / 1023, 00) * 5, 00) - (n * Vd * G)) / (G * 0.01)); //Fsens analog input
  Dsens = ((((analogRead(A0) / 1023, 00) * 5, 00) - (n * Vd * G)) / (G * 0.01)); //Dsens analog input
  Rsens = ((((analogRead(A0) / 1023, 00) * 5, 00) - (n * Vd * G)) / (G * 0.01)); //Rsens analog input
}

void pilotaggio() {

  if (!defrost) {   //se non siamo in defrost allora si esegue il pilotaggio sui vari stati

    //--------cronometri monitoraggio tempo on off del compressore------------------------
    if (digitalRead(3) && (millis() - tcronometrocompon > 1000)) {  //ogni secondo
      tcronometrocompon = millis();
      tempocompon ++;

    }
    if (!digitalRead(3) && (millis() - tcronometrocompoff > 1000)) { //ogni secondo
      tcronometrocompoff = millis();
      tempocompoff ++;
    }

    if (Door && (millis() - tcronometroporta > 1000)) { //ogni secondo
      tcronometroporta = millis();
      tempoporta ++;
      tempoallarme ++;
    }

    //-------------------gestione buzzer------------------------
    if (tempoallarme > 60) {  //ogni minuto
      if (millis() - ttone > 1) {   //alla frequenza di 1000 Hz
        digitalWrite(6, !digitalRead(6));   //toggle 6 buzzer
        ciclo ++;
        if (ciclo > 2000) {   //per 2 secondi
          ciclo = 0;
          tempoallarme = 0;
        }
      }
    }

    //-----------controllo decisione start defrosting-----------------------------
    if ((tempoporta > (10 * 60)) || (tempocompon > (14 * 3600)) || ((tempocompon + tempocompoff) > (60 * 3600))) { //tempoporta in secondi è maggiore di 10 minuti OPPURE tempocompon in secondi è maggiore di 14 ore OPPURE tempocompon + tempocompoff in secondi è maggiore di 60 ore
      defrost = 1;
    }

    //---------------silent mode start--------------
    if ((Fsens < 15.00) && (silent) && (millis() - t3 > silentpreventiontime)) { //le condizioni devono essere tutte verificate per iniziare
      if (!silentst) { //primo start
        silentst = 1;
        t3 = millis();  //cronometro start
      }
      //tutto off
      digitalWrite(2, LOW); //Ffan
      digitalWrite(3, LOW); //comp e Cfan
      digitalWrite(4, LOW); //RFan
      digitalWrite(5, LOW); //HTR

    }

    //--------------------silent mode stop---------------------
    if (((Fsens > -9.00) && (!silent) ) || (millis() - t3 > silentperiod)) { //le condizioni devono essere tutte verificate per iniziare
      if (silentst) { //primo stop
        silentst = 0;
        t3 = millis();  //cronometro stop
      }
      //non si fa nulla perché si lascia tutto come prima comp fan e stati e led, ci pensa rstato e fsato
    }
    if (!Door) { //porta chiusa (porta chiusa-> Door = 0)
      if ((!silent) && ((Fsens > FTon) || (Rsens > RTon))) { //accensione generale

        //----------------FREETZER---------------
        if (fstato != 4) {
          if (Fsens > FTon) {
            if (millis() - t4 > 2000) { //2 sec dopo chiusura porta
              digitalWrite(2, HIGH); //Ffan acceso
            }
            digitalWrite(3, HIGH); //comp e Cfan
            firststopcomp = 1;     //il tag prima volta stop compressore si azzera
          }
        }
        //----------------FRIGO---------------
        if (rstato != 4) {
          if (Rsens > RTon) {
            if (millis() - t4 > 2000) { //2 sec dopo chiusura porta
              digitalWrite(4, HIGH); //Rfan acceso
            }
            digitalWrite(3, HIGH);  //comp e Cfan
            firststopcomp = 1;      //il tag prima volta stop compressore si azzera
          }
        }
      }
      if ((!silent) && ((Fsens < FToff) || (Rsens < RToff))) { //spegnimento generale

        //----------------FREETZER---------------
        if (fstato != 4) {
          if (Fsens < FToff) {
            if (firststopcomp) {  //prima volta che stoppi il comp
              firststopcomp = 0;
              t5 = millis();
            }
            if (millis() - t5 > 60000) { //1 minuto dopo spegnimento comp
              digitalWrite(2, LOW); //Ffan spento
            }
            digitalWrite(3, LOW); //comp e Cfan spento
          }
        }
        //----------------FRIGO---------------
        if (rstato != 4) {
          if (Rsens < RToff) {
            digitalWrite(4, LOW); //Rfan off e non il comp perché priorità freetzer
          }
        }
      }
      //-------------QF mode----------------------------
      if (fstato == 4) {  //QF mode
        if (first4stf) {
          first4stf = 0;
          t6 = millis();
        }
        //tutto acceso per 150 min
        if (millis() - t6 < (150 * 60 * 1000)) {
          digitalWrite(2, HIGH); //Ffan
          digitalWrite(3, HIGH); //comp e Cfan
        }
        else {
          fstato = 2; //dopo 150 min di QF torna a mode 2
          first4stf = 1; //resetta tag firstf stato 4
        }
      }

      //-----------QR mode----------------------
      if (rstato == 4) {  //QR mode
        if (first4str) {
          first4str = 0;
          t7 = millis();
        }
        //tutto acceso per 40 min
        if (millis() - t7 < (40 * 60 * 1000)) {
          digitalWrite(3, HIGH); //comp e Cfan
          digitalWrite(4, HIGH); //Rfan
        }
        else {
          rstato = 2; //dopo 40 min di QR torna a mode 2
          first4str = 1; //resetta tag firstr stato 4
        }
      }

    } else {  //porta aperta
      digitalWrite(4, LOW); //spegni Ffan
      digitalWrite(5, LOW); //spegni Rfan
    }
  }
}

void defrosting() {
  if (firstdefrost) {
    firstdefrost = 0;
    t8 = millis();
  }

  //--------precooling-------------
  if ((millis() - t8 < (50 * 60 * 1000)) || (Fsens < -27.00)) {
    digitalWrite(2, HIGH); //Ffan on
    digitalWrite(3, HIGH); //comp e Cfan on
    digitalWrite(5, LOW); //HTR off
    if (Rsens > RTon) {
      digitalWrite(4, HIGH); //Rfan on
    }
    else if (Rsens < RToff) {
      digitalWrite(4, LOW); //Rfan off
    }
  }
  //-------heater defrosting----------
  if ((millis() - t8 > (50 * 60 * 1000)) && (millis() - t8 < (130 * 60 * 1000))) { //tempo compreso tra 50 e 130 minuti durata 80 minti
    digitalWrite(2, LOW); //Ffan off
    digitalWrite(3, LOW); //comp e Cfan off
    digitalWrite(4, LOW); //Rfan off
    if (Dsens > 10.00) {
      digitalWrite(5, LOW); //HTR off
    }
    else {
      digitalWrite(5, HIGH); //HTR on
    }
  }

  // ----------pause------------
  if ((millis() - t8 > (130 * 60 * 1000)) && (millis() - t8 < (134 * 60 * 1000))) { //tempo compreso tra 130 e 134 minuti durata 4 minuti
    digitalWrite(2, LOW); //Ffan off
    digitalWrite(3, LOW); //comp e Cfan off
    digitalWrite(4, LOW); //Rfan off
    digitalWrite(5, LOW); //HTR off
  }

  //----fan delay---------------
  if ((millis() - t8 > (134 * 60 * 1000)) && (millis() - t8 < (139 * 60 * 1000))) { //tempo compreso tra 130 e 134 minuti durata 4 minuti
    digitalWrite(2, LOW); //Ffan off
    digitalWrite(3, HIGH); //comp e Cfan on
    digitalWrite(4, LOW); //Rfan off
    digitalWrite(5, LOW); //HTR off
  }
  if (millis() - t8 > (139 * 60 * 1000)) { //alla fine del defrost (tempo max 139 min) azzerare tempocompon, tempocompoff, tempoporta e tag defrost e ripristinare firstdefrost
    defrost = 0;
    firstdefrost = 1;
    tempocompon = 0;
    tempocompoff = 0;
    tempoporta = 0;
  }
}
