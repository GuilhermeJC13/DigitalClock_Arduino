#include <LiquidCrystal.h> // biblioteca do LCD

#define buzzer 2 // buzzer

#define pot A0 // potenciometro

#define G2 196 // Notas
#define C 262
#define D 294
#define E 330
#define F 349
#define G3 392

LiquidCrystal LCD(3,4,5,6,7,8); // Inicializar o LCD

int cont_seg = 0; // variaveis que contam o tempo
int cont_min = 0;
int cont_hora = 12;

int tempoAtual = 0; // variaveis para o metodo de delay sem parar o codigo
int tempoSeg = 0;

int alarme_hora = 12; // variaveis do alarme
int alarme_minuto = 1;

String serialData; // Variavel do tipo string para os comandos pelo monitor serial

int tela_atual = 0; // variavel para transicao de menus

int alarme = 0; // variavel bool para dizer que o alarme foi acionado

int norpt = 1; // variavel bool usada para evitar alarme tocar mais de uma vez

// Sequencia de notas e suas duracoes no alarme (ode a alegria)
int notes[62]{E, E, F, G3, G3, F, E, D, C, C, D, E, E, D, D, E, E, F, G3, G3, F, E, D, C, C,D, E, D, C, C, D, D, E, C, D, E, F, E, C, D, E, F, E, D, C, D, G2, E, E, F, G3, G3, F, E, D, C, C, D,E, D, C, C};
int duration[62]{600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 900, 300, 1200, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 900, 300, 1200, 600, 600, 600, 600, 600, 300, 300, 
600, 600, 600, 300, 300, 600, 600, 600, 600, 1200, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 900, 300, 1200};


void horario(){ // funcao para calcular o horario de forma precisa
  tempoAtual = millis();
  if(tempoAtual - tempoSeg >= 1000 || alarme == 1){
    LCD.clear();
    cont_seg++;
    tempoSeg = tempoAtual;
    alarme = 0;
  }
  if(cont_seg >= 60){
    cont_min++;
    cont_seg -= 60;
    norpt = 1;
  }
  if(cont_min >= 60){
    cont_hora++;
    cont_min -= 60;
  }
  if(cont_hora >= 24){
    cont_hora -= 24;
  }
  if(cont_min < 0){
    cont_hora--;
    cont_min = 60;
  }
  if(cont_hora < 0){
    cont_hora = 0;
  }
}

void conf_relogio(){ // funcao para modificar a hora do relogio
  while(1){
    horario();
    LCD.clear();
    LCD.setCursor(4,0);
    if(cont_hora < 10){
      LCD.print("0");
    }
    LCD.print(cont_hora);
    LCD.print(":");
    if(cont_min < 10){
      LCD.print("0");
    }
    LCD.print(cont_min);
    LCD.print(":");
    if(cont_seg < 10){
      LCD.print("0");
    }
    LCD.print(cont_seg);
    LCD.setCursor(4,1);
    LCD.print("Relogio");
    delay(1000);

    int valor = analogRead(pot);
    valor = map(valor, 0, 1023, -20, 20);
    cont_min += valor;

    if(Serial.available() > 0){
      serialData = Serial.readString();
      if(serialData == "Voltar\n"){
        tela_atual = 0;
        break;
      }
    }
  }
}

void conf_alarme(){ // funcao para configurar o alarme
  while(1){
    horario();
    LCD.clear();
    LCD.setCursor(5,0);
    if(alarme_hora < 10){
      LCD.print("0");
    }
    LCD.print(alarme_hora);
    LCD.print(":");
    if(alarme_minuto < 10){
      LCD.print("0");
    }
    LCD.print(alarme_minuto);
    LCD.setCursor(5,1);
    LCD.print("Alarme");
    delay(1000);

    int valor = analogRead(pot);
    valor = map(valor, 0, 1023, -20, 20);
    alarme_minuto += valor;

    if(alarme_minuto < 0){
      alarme_hora--;
      alarme_minuto = 0;
    }
    if(alarme_hora < 0){
      alarme_hora = 0;
    }
    if(alarme_minuto >= 60){
      alarme_minuto = 0;
      alarme_hora++;
    }

  if (Serial.available() > 0){
    if(Serial.available() > 0){
      serialData = Serial.readString();
      if(serialData == "Voltar\n"){
        tela_atual = 0;
        break;
      }
     }
    }
   }
  }

void musica(){ // funcao que toca a musica do alarme
  LCD.clear();
  LCD.setCursor(5,0);
  LCD.print("Alarme");//38
  cont_seg += 38;
 
  for( int i = 0; i <= 61; i++){
    tone(buzzer, notes[i]);
    delay(duration[i]);
  }

  noTone(buzzer);

  noTone(buzzer);
  LCD.clear();
  alarme = 1;
}

void acorda(){ // funcao do alarme
  if(cont_hora == alarme_hora && cont_min == alarme_minuto && norpt){
    musica();
    norpt = 0;
  }
}

void setup() { // setup
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  LCD.begin(16,2);
}

void loop() {
  if(tela_atual == 0){ // parte principal(relogio)
    horario();
    acorda();
    LCD.clear();
    LCD.setCursor(4,0);
    if(cont_hora < 10){
      LCD.print("0");
    }
    LCD.print(cont_hora);
    LCD.print(":");
    if(cont_min < 10){
      LCD.print("0");
    }
    LCD.print(cont_min);
    LCD.print(":");
    if(cont_seg < 10){
      LCD.print("0");
    }
    LCD.print(cont_seg);
    LCD.setCursor(5,1);
    if(cont_hora >= 4 && cont_hora <= 11){
      LCD.print("Manha");
    }
    if(cont_hora > 11 && cont_hora <= 17){
      LCD.print("Tarde");
    }
    if(cont_hora > 17 || cont_hora >= 1 && cont_hora <= 3){
      LCD.print("Noite");
    }
    delay(1000);
    
    
    
    if(Serial.available() > 0){
      serialData = Serial.readString();
      if(serialData == "Configurar\n"){
        tela_atual = 1;
      }
    }
  }
else if (tela_atual == 1){ // menu de configuracoes
    horario();
    acorda();
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Relogio");
    LCD.setCursor(0, 1);
    LCD.print("Alarme");
    delay(1000);

    if(Serial.available() > 0){
      serialData = Serial.readString();
      if(serialData == "Relogio\n"){
        conf_relogio();
      
      }else if(serialData == "Alarme\n"){
         conf_alarme();
         
      }else if(serialData == "Voltar\n"){
        tela_atual = 0;
      }
    }
  }
}