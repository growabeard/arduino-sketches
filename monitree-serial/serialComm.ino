const byte buffSize = 40;

char inputBuffer[buffSize];

const char startMarker = '<';

const char endMarker = '>';

byte bytesRecvd = 0;

boolean readInProgress = false;

boolean newDataFromPC = false;



char messageFromPC[buffSize] = {0};



void getDataFromPC() {



    // receive data from PC and save it into inputBuffer

    

  if(Serial.available() > 0) {



    char x = Serial.read();



      // the order of these IF clauses is significant

      

    if (x == endMarker) {

      readInProgress = false;

      newDataFromPC = true;

      inputBuffer[bytesRecvd] = 0;

      parseData();

    }

    

    if(readInProgress) {

      inputBuffer[bytesRecvd] = x;

      bytesRecvd ++;

      if (bytesRecvd == buffSize) {

        bytesRecvd = buffSize - 1;

      }

    }



    if (x == startMarker) { 

      bytesRecvd = 0; 

      readInProgress = true;

    }

  }

}



//=============

 

void parseData() {

  

  strcpy(messageFromPC, inputBuffer); // copy it to messageFromPC

  

}



//=============



void replyToPC(char *data) {



  if (newDataFromPC) {

    newDataFromPC = false;

    Serial.print("<Msg ");

    Serial.print(data);

    Serial.println(">");

  }

}



void maybeGetData() {

  if (strcmp(messageFromPC, "GET") == 0) {

    char data[200];

  

    readData(data);



    replyToPC(data);
    strcpy(messageFromPC, "");
  }

}
