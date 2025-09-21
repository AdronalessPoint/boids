#ifndef R_NUMBERS
#define R_NUMBERS

#include <cassert>
#include <chrono>
#include <cmath>
#include <random>
#include <array>

// In questo Header File sono definite le free-function responsabili della
// generazione pseudo-casuale dei numeri necessari a inizializzare i boids.

// Spigoli del rettangolo in cui sono contenuti i boid senza essere accelerati
// verso il centro
 //fare una distribuzione gaussiana è scomoda perchè devi troncare 
float const display_width{1280};
float const display_height{720};

inline double const bound_xmin{display_width *
                               (18. / 100.)};
inline double const bound_xmax{display_width *
                               (82. / 100.)};
inline double const bound_ymin{
    (display_height * (33. / 100.))}; //lasci margine di 33% sopra e sotto
inline double const bound_ymax{
    (display_height * (67. / 100.))};

// Random engine e distribuzione necessari per generare numeri casuali riguardo alle posizioni
// nel range del rettangolo
inline std::uniform_real_distribution<double> dist_x(bound_xmin, bound_xmax);
inline std::default_random_engine eng;


// Funzione che ritorna una coordinata X random
inline double r_position_x()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count()); //setta il seed del motore in base all'ora in cui hai fatto partire la simulazione
  assert(dist_x(eng) >= bound_xmin && dist_x(eng) <= bound_xmax);
  return dist_x(eng); //chiami il motore come parametro della distribuzione
}
// Distribuzione per generare numeri casuali sulle Y nel range del rettangolo
inline std::uniform_real_distribution<double> dist_y(bound_ymin, bound_ymax);

// Funzione che returna una coordinata Y random
double inline r_position_y()
{
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  assert(dist_y(eng) >= bound_ymin && dist_y(eng) <= bound_ymax);
  return dist_y(eng);
}

// Qui è definito un generatore casuale che returna 1 o -1 per
// rendere casuale il segno delle velocità,  sono solo numeri arbitrari
inline std::array<int, 2> const sign{1, -1};//array di 2 interi chiamato sign, inizializzato con 1 e -1
inline std::uniform_int_distribution<int> int_dist(0, 200);
// la funzione returna 1 o -1 richiamando l'i-esimo elemento dell'array, dove i
// è determitata dal resto della divisione per 2 dell'intero generato dalla
// distribuzione
inline int r_sign() {
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  return sign[(int_dist(eng) % 2)]; //restituisce una referenza all'elemento al posto 0 oopure 1 dell'array sign, in base al resto dell'operazione di divisione per due del numero casuale
}


// fissati i limiti della velocità iniziale e li uso come limiti di una
// distribuzione
inline double const initial_vel{1500.};
inline std::uniform_real_distribution<double> vel_dist(initial_vel - 500, initial_vel); //distribuz uniforme di valori tra 1500 e 2000
// funzione che returna una velocità iniziale con intensità e segno random
double inline r_velocity() {
  eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
  assert(std::abs(vel_dist(eng) * r_sign()) <= initial_vel);
  assert(std::abs(vel_dist(eng) * r_sign()) >= (initial_vel-500));
  return vel_dist(eng) * r_sign();
}

#endif