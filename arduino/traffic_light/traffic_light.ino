int _RED =  12;
int _YELLOW =  13;
int _GREEN =  15;

void setup() {
  Serial.begin(9600);
  pinMode(_RED, OUTPUT);
  pinMode(_YELLOW, OUTPUT);
  pinMode(_GREEN, OUTPUT);

}

void loop() {
  digitalWrite(_RED, HIGH);   
  delay(1000);                      
  digitalWrite(_YELLOW, HIGH);     
  delay(1000);                      
  digitalWrite(_RED, LOW); 
  digitalWrite(_YELLOW, LOW); 
  digitalWrite(_GREEN, HIGH);   
  delay(1000);                      
  digitalWrite(_GREEN, LOW);
  delay(500);
  Serial.println("loop.....");
}
