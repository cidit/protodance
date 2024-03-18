

#include <ProtoTGP.h>
char dernier_mouvement;

const uint32_t ONE_SECOND = 1000;
const uint32_t POINTS_TIMEOUT = 5 * ONE_SECOND;
const uint32_t GAME_LENGTH = 10 * ONE_SECOND;
ProtoTGP proto;

char generer_mouvement_aleatoire()
{
  const auto moves = "<v^>";
  int result = random(0, 4);
  return moves[result];
}

char lire_mouvement_clavier()
{
  if (proto.haut.isPressed())
  {
    return '^';
  }
  else if (proto.bas.isPressed())
  {
    return 'v';
  }
  else if (proto.gauche.isPressed())
  {
    return '<';
  }
  else if (proto.droite.isPressed())
  {
    return '>';
  }
  else
  {
    return '0';
  }
}

 
static uint8_t current_score = 0;
static uint8_t high_score = 0;
static uint64_t round_timer;
static uint64_t game_timer;


void reset_round()
{
  round_timer = millis();
  dernier_mouvement = generer_mouvement_aleatoire();
}

void reset_game() {
  proto.ecran.clearDisplay();
  proto.ecran.ecrire("GAME OVER! SCORE: " + String(current_score));
  proto.ecran.display();
  auto now = millis();
  while (millis() < now + 3 * ONE_SECOND) {}
  if (current_score > high_score) {
    high_score = current_score;
  }
  current_score = 0;
  round_timer = game_timer = millis();
}

void setup()
{
  Serial.begin(115200);
  proto.begin();
  game_timer = round_timer = millis();
  dernier_mouvement = generer_mouvement_aleatoire();
}


void loop()
{
  const auto current_time = millis();
  proto.refresh();

  const auto time_left = GAME_LENGTH/ONE_SECOND -(current_time - game_timer) / ONE_SECOND;
  const auto points = POINTS_TIMEOUT/ONE_SECOND -(current_time - round_timer) / ONE_SECOND;
  proto.ecran.clearDisplay();
  proto.ecran.ecrire("Score: " + String(current_score) + " HS: " + high_score, 1); 
  proto.ecran.ecrire("Time left: " + String(time_left), 2);

  proto.ecran.ecrire("\tpoints : " + String(points), 5);

  // proto.ecran.ecrire("<v^>", 2);

  proto.ecran.ecrire(String({dernier_mouvement}), 6);

  
  if (current_time - game_timer >= GAME_LENGTH) {
    reset_game();
    return;
  }
  if (lire_mouvement_clavier() == dernier_mouvement)
  {
    current_score += points;
    reset_round();
  }
  if (current_time - round_timer >= POINTS_TIMEOUT)
  {
    reset_round();
  }
}


bool isCapitalized(char &input) {
  return input >= 65 && input <= 90;
}