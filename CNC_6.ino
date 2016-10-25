#include "Pref.h"

float sx;
float sy;
float sz;
test
int difbef;
int difaft;

char currentrecivedline[LINE_BUFFER_LENGTH];
int currentlineindex;
char oldrecivedline[LINE_BUFFER_LENGTH];
char line[ LINE_BUFFER_LENGTH ];
char c;
//int lineIndex;
//bool lineIsComment, lineSemiColon;
int lineIndex = 0;
bool lineSemiColon = false;
bool lineIsComment = false;
void setup() {
  Serial.begin(Baudrate);
  Boot();
  pinMode(stpx, 1);
  pinMode(sttpx, 1);
  pinMode(stpy, 1);
  pinMode(sttpy, 1);
  pinMode(stpz, 1);
  pinMode(sttpz, 1);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(enx, 0);
  digitalWrite(eny, 0);
  digitalWrite(enz, 0);

}

void loop() {
int Testfor = strcmp(currentrecivedline,oldrecivedline);


  if (Testfor != 0) {
    Serial.print("new");
   Serial.println(currentrecivedline);
   Serial.print("old");
   Serial.println(oldrecivedline);
    processIncomingLine(currentrecivedline, currentlineindex);
    for (int i = 0; i < currentlineindex; i++) {
      oldrecivedline[i] = currentrecivedline[i];
    }
    Serial.print("new2");
   Serial.println(currentrecivedline);
   Serial.print("old3");
   Serial.println(oldrecivedline);
  }
}

// Serial reception - Mostly from Grbl, added semicolon support

void processIncomingLine( char* line, int charNB ) {
  struct point G91;
  G91.x = 0;
  G91.y = 0;
  G91.z = 0;
  int currentIndex = 0;
  char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
  struct point newPos;
  newPos.x = 0.0;
  newPos.y = 0.0;
  newPos.z = 0.0;
  int Vorschub;
  float Feedrate = 0;
//  Serial.println("ok");
  char* indexSX = strchr( line + currentIndex, 'X' );
  newPos.x = atof( indexSX + 1);

  char* indexSY = strchr( line + currentIndex, 'Y' );
  newPos.y = atof( indexSY + 1);


  char* indexSZ = strchr( line + currentIndex, 'Z' );
  newPos.z = atof( indexSZ + 1);


  char* indexF = strchr(line + currentIndex, 'F');
  Feedrate = atof (indexF + 1);
  if (indexF <= 0)
  {
    Feedrate = Feedrateo;

  }

  if ( indexSX <= 0)
  {
    newPos.x  = actuatorPos.x;
    G91.x = 1;
  }
  if (indexSY <= 0) {
    newPos.y = actuatorPos.y;
    G91.y = 1;
  }
  if (indexSZ <= 0) {
    newPos.z = actuatorPos.z;
    G91.z = 1;
  }

  float dx = actuatorPos.x - newPos.x;
  float dy = actuatorPos.y - newPos.y;
  float dz = actuatorPos.z - newPos.z;

  while ( currentIndex < charNB ) {

    switch ( line[ currentIndex++ ] ) {              // Select command, if any


      case 'G': {
          buffer[0] = line[ currentIndex++ ];          // /!\ Dirty - Only works with 2 digit commands
          buffer[1] = line[ currentIndex++ ];
          buffer[2] = '\0';
          //buffer[1] = '\0';

          switch ( atoi( buffer ) ) {                  // Select G command
            case 00:
              { Feedrate1 = Eilgang;

                break;
              }
            case 01:
              {
                Feedrate1 = Feedrate;

                break;
              case 91:
                if (G91.x == 1) {
                  newPos.x = 0;
                }
                if (G91.y == 1) {
                  newPos.y = 0;
                }
                if (G91.z == 1) {
                  newPos.z = 0;
                }

                Serial.println(G91.x);
                Serial.println(newPos.y);
                Serial.println(newPos.z);
                drawLine(newPos.x, newPos.y, newPos.z);
                actuatorPos.x = actuatorPos.x + newPos.x;
                actuatorPos.y = actuatorPos.y + newPos.y;
                actuatorPos.z = actuatorPos.z + newPos.z;
                Feedrate1 = Feedrate;
                Serial.println(actuatorPos.x);
                Serial.println(actuatorPos.y);
                Serial.println(actuatorPos.z);


                Gactivation = 1;
                break;

              }
            default:
              {}
          }
          break;
        }
      case 'M': {
          buffer[0] = line[ currentIndex++ ];        // /!\ Dirty - Only works with 3 digit commands
          buffer[1] = line[ currentIndex++ ];
          buffer[2] = line[ currentIndex++ ];
          buffer[3] = '\0';
          switch ( atoi( buffer ) ) {
            case 7:
              break
              ;
            case 114:
              Serial.print( "Absolute position : X = " );
              Serial.print( actuatorPos.x );
              Serial.print( "  -  Y = " );
              Serial.println( actuatorPos.y );
              break;// M114 - Repport position
            case 15: {
                Boot();
                actuatorPos.x = 0.0;
                actuatorPos.y = 0.0;
                actuatorPos.z = 0.0;
                newPos.x = 0.0;
                newPos.y = 0.0;
                newPos.z = 0.0;
                break;
              }
            case 03:
              { Serial.println("on");  //neue Commands hier einfügen
                delay(2000);
                break;
              }
            case 5:
              { Serial.println("off");
                delay(2000);
                break;
              }

              break;
            default:
              Serial.print( "Command not recognized : M");
              Serial.println( buffer );

          }
        }
    } //<Idle,MPos:5.529,0.560,7.000,WPos:1.529,-5.440,-0.000>


  }
  //    if (llm == 1) {
  //      digitalWrite(8, 0);
  //      digitalWrite(9, 0);
  //      digitalWrite(13, 0);
  //
  //    }

  if (Gactivation == 0) {
    drawLine(dx, dy, dz);
  } Gactivation = 0;
  if (Gactivation == 0) {
    actuatorPos.x = newPos.x;
    actuatorPos.y = newPos.y;
    actuatorPos.z = newPos.z;
  }
  Feedrateo = Feedrate;

}
void drawLine(float ax, float ay, float az) {
  if (Gactivation != 0) {
    az = -az;
  }
  activate(0);
  int cy = 0;
  int cx = 0;
  int cz = 0;

  int overx = 0;
  int overy = 0;
  float x0 = Xpos;
  float y0 = Ypos;
  float z0 = Zpos;
  if (ax < 0) {
    sx = 0; //Vorwärts? Rückwärts? gar nicht?
  }
  else if ( ax == 0) {
    sx = 0;
  }
  else {
    sx = 1;
  }
  if (ay < 0) {
    sy = 0;
  }
  else if ( ay == 0) {
    sy = 0;
  }
  else {
    sy = 1;
  }
  if (az < 0) {
    sz = 0;
  }
  else if ( az == 0) {
    sz = 0;
  }
  else {
    sz = 1;
  }

  int ix = 0;
  int iy = 0;
  int iz = 0;
  float dx = abs(ax * stepsperMillimeterX);
  float dy = abs(ay * stepsperMillimeterY);
  float dz = abs(az * stepsperMillimeterZ);
  if ( dx == 0 && dy == 0 && dz == 0) {}
  else {
    if ( dx >= dy && dx >= dz) {
      float fy = dx / 2;
      float fz = dx / 2;
      Serial.println(Feedrate1);
      Serial.println("x");

      for (unsigned long i = 0; i < dx; i++) {


        //                digitalWrite(enx, 0);

        st(stpx, sx, Feedrate1, sttpx);
        //        digitalWrite(enx, 1);


        //        int p =  i % 10;
        //        if (p == 0) {
        //          reupdatepos(ix, i, iz);
        //        }
        fy = fy - dy;
        fz = fz - dz;
        if ( fy <= 0 && dy > 0)
        { iy++;
          //          digitalWrite(eny, 0);

          st(stpy, sy, Feedrate1, sttpy);
          //          digitalWrite(eny, 1
          //                      );


          fy = fy + dx;
        }
        if  (fz < 0 && dz > 0) {
          iz++;
          //          digitalWrite(enz, 0);
          st(stpz, sz, Feedrate1, sttpz);
          //          digitalWrite(enz, 1);


          fz = fz + dx;
        }

      }

    }

    if (dy > dx && dy > dz) {

      float fx = dy / 2;
      float fz = dy / 2;
      Serial.println(Feedrate1);
      Serial.println("Y");

      for ( unsigned long i = 0; i < dy; i++) {
        //        digitalWrite(eny, 0);


        st(stpy, sy, Feedrate1, sttpy);
        //        digitalWrite(eny, 1);


        //        int p =  i % 10;
        //        if (p == 0) {
        //          reupdatepos(ix, i, iz);
        //        }
        fx = fx - dx;
        fz = fz - dz;
        if ( fx <= 0 && dx > 0)
        { ix++;
          //          digitalWrite(enx, 0);

          st(stpx, sx, Feedrate1, sttpx);
          //          digitalWrite(enx, 1);

          fx = fx + dy;
        }
        if  (fz < 0 && dz > 0) {
          iz++;
          //          digitalWrite(enz, 0);
          st(stpz, sz, Feedrate1, sttpz);
          //          digitalWrite(enz, 1);
          fz = fz + dy;
        }

      }

    }

    if (dz > dx && dz > dy) {

      float fx = dy / 2;
      float fy = dy / 2;
      Serial.println(Feedrate1);
      Serial.println("Z");


      for ( unsigned long i = 0; i < dz; i++) {
        //        digitalWrite(enz, 0);

        st(stpz, sz, Feedrate1, sttpz);
        //        digitalWrite(enz, 1);

        //        int p =  i % 10;
        //        if (p == 0) {
        //          reupdatepos(ix, i, iz);
        //        }
        fx = fx - dx;
        fy = fy - dy;
        if ( fx <= 0 && dx > 0)
        { ix++;
          //          digitalWrite(enx, 0);

          st(stpx, sx, Feedrate1, sttpx);
          //          digitalWrite(enx, 1);

          fx = fx + dz;
        }
        if  (fy < 0 && dz > 0) {
          iy++;
          //          digitalWrite(eny, 0);


          st(stpy, sy, Feedrate1, sttpy);
          //          digitalWrite(eny, 1);

          fy = fy + dz;
        }

      }
    }






  }


  activate(1);
}
void Boot() {
  Serial.println("Grbl 0.8c ['$' for help]");


}
void pinmode() {

  pinMode(Spindel, Enableall);

}
void reupdatepos(float x, float y, float z) {
  float uex = x / stepsperMillimeterX * sx * -1;
  float uey = y / stepsperMillimeterY * sy * -1;
  float uez = z / stepsperMillimeterZ * sz * -1;


  float inx = uex + actuatorPos.x;
  float iny = uey + actuatorPos.y;
  float inz = uez + actuatorPos.z;


  Serial.print("<Idle,MPos:");


  Serial.print(inx);
  Serial.print(",");
  Serial.print(iny);
  Serial.print(",");
  Serial.print(inz);
  Serial.print(",WPos:");
  Serial.print("1.529,-5.440,-0.000>");
  Serial.println("");

}
void st(int stp, bool dir, int i, int sm) {
  digitalWrite(stp, dir);
  digitalWrite(sm, HIGH);
  delayMicroseconds(i);
  //delay(3);
  digitalWrite(sm, LOW);
  delayMicroseconds(i);
  //delay(3);
  digitalWrite(stp, LOW);


}
void activate(int i) {
  if (llm == 1) {
    digitalWrite(8, i);
    digitalWrite(9, i);
    digitalWrite(13, i);

  }
}
void serialEvent( ) {
  c = Serial.read();
Serial.println(lineIndex);
  if (( c == '\n') || (c == '\r') ) {             // End of line reached
    if ( lineIndex > 0 ) {                        // Line is complete. Then execute!
      line[ lineIndex ] = '\0';                   // Terminate string
//      for (int i = 0; i <= lineIndex; i++) {
//        currentrecivedline[i] = line[i];
//      }
strcpy(currentrecivedline,line); 
      currentlineindex = lineIndex;
     
Serial.println("ok");
      
      if (verbose) {
        Serial.print( "Received : ");
        Serial.println( line );
      }

      lineIndex = 0;
    }
    else {
      // Empty or comment line. Skip block.
    }
    lineIsComment = false;
    lineSemiColon = false;
//    Serial.println("ok");

  }
  else {
    if ( (lineIsComment) || (lineSemiColon) ) {   // Throw away all comment characters
      if ( c == ')' )  lineIsComment = false;     // End of comment. Resume line.
    }
    else {
      if ( c <= ' ' ) {                           // Throw away whitepace and control characters
      }
      else if ( c == '/' ) {                    // Block delete not supported. Ignore character.
      }
      else if ( c == '(' ) {                    // Enable comments flag and ignore all characters until ')' or EOL.
        lineIsComment = true;
      }
      else if ( c == ';' ) {
        lineSemiColon = true;
      }
      else if ( lineIndex >= LINE_BUFFER_LENGTH - 1 ) {
        Serial.println( "ERROR - lineBuffer overflow" );
        lineIsComment = false;
        lineSemiColon = false;
      }
      else if ( c >= 'a' && c <= 'z' ) {        // Upcase lowercase
        line[ lineIndex++ ] = c - 'a' + 'A';
      }
      else {
        line[ lineIndex++ ] = c;
      }
    }
  }
}



