#define llm 1
float Xpoint = 0;
float Ypoint = 0;
float Zpoint = 0;
float Feedrate1;
const int Eilgang = 250;
  float Feedrateo;
  boolean verbose = false;
float Speed;
char* indexS;
struct point {
  float x;
  float y;
  float z;
};
int StepDelay = 0;
struct point actuatorPos;
int Gactivation=0;
//benutzt du Easydriver oder ähnliches? wenn du eine H-bridge benutzt trage in cs eine 0 ein wenn du einen Easydriver benutzt dann eine 1
bool cs = 1;
//hier die Maximale Auflösung des Schrittmotors eintragen!
#define stepsperMillimeterX  320
#define stepsperMillimeterY  320
#define stepsperMillimeterZ  320
#define Xmin  0
#define Xmax  300
#define Ymin  0
#define Ymax  250
#define Zmin  0
#define Zmax  20
#define d 1
const int stpx =2;
const int sttpx =3;
const int stpy =4;
const int sttpy=5;
const int stpz =11;
const int sttpz =12;
float Xpos = Xmin;
float Ypos = Ymin;
float Zpos = Zmax;
const int lol1 = 5;
const int lol2 = 6;
const int lol4 = 11;
const int enx = 8;
const int eny = 9;
const int enz = 13;
#define Baudrate 115200
#define Enableall 1
const int StepsRevo = 200;
#define Spindel 12
#define LINE_BUFFER_LENGTH 150


