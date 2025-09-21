
#include "flock.hpp"
#include "graphics.hpp"
#include "velocity.hpp"
// In questa TU vengono definite le due free-function che gestiscono
// l'evoluzione temporale dello stormo. La prima in particolare è la funzione
// più importante, e pesante, di tutto il codice: Update.

// Per prima cosa viene definita la distanza a cui i boids interagiscono per
// allineamento e coesione (vision) e la distanza a cui si repellono.
double const boids_vision{200.};
double const separation_range{30.};
// La funzione update prende come parametri: lo stormo che si vuole far
// evolvere, in numero di step per chiamata e la durata temporale di ogni step.
// Inoltre prende anche un coefficiente moltiplicativo che scala la distanza a
// cui i boids interagiscono per coesione e allineamento.
// delta_t è il tempo passato dall'ultimo frame (usi il tempo invece di un
// contatore così la simulazione va alla stessa velocità su tutti i tipi di
// computer)
void update(Flock& stormo, int steps_per_evolution, sf::Time const delta_t) {
  double const dt{delta_t.asSeconds()}; //boids si muovono a step di 1 millisecondo per volta
  // La funzione fa un iterazione per ogni step_per_evolution
  //step_per evolution è un valore in millisecondi
  
  for (int s{0}; s != steps_per_evolution; ++s) {
    // Viene fissato il vettore di Boid dello stormo su un vettore ausiliario
    assert(stormo.get_flock().size() != 0);
  auto flock{stormo.get_flock()};
    

    std::for_each(flock.begin(), flock.end(), [&](Boid& boid) {
      // L'operatore unario che viene applicato è un lambda che, per ogni
      // boid su cui transform si focalizza, applica una serie di
      // trasformazioni alla sua posizione e velocità.
      // Per prima cosa aggiorna la posizione in base all'attuale velocità.
      double new_x;
      double new_y;
      double new_vx;
      double new_vy;

      if (boid.get_is_dead()) {
        // flock.erase(std::find(flock.begin(), flock.end(), boid));

        new_x = boid.get_x() + boid.get_vx() * dt;
        new_y = boid.get_y() + boid.get_vy() * dt;
        new_vx = 0;  // need to fall
        new_vy = 1500;

      } else {
        new_x = boid.get_x() + boid.get_vx() * dt;
        new_y = boid.get_y() + boid.get_vy() * dt;
        // Il resto della lambda è dedicata alla modifica della velocità.
        // Viene definita una velocità v_esterna, che rappresenta la
        // somma delle velocità di interazione con gli altri boids nello
        // stormo.
        // I range di repulsione, allineamento e coesione sono tutti definiti
        // come
        // proporzioni di "vision" e "separation".
        double const vx_e{stormo.vx_separation(separation_range, boid) +
                          stormo.vx_alignment(
                              boids_vision * default_vision_value,
                               boid) +
                          stormo.vx_cohesion(
                              boids_vision * default_vision_value,
                               boid)};
        double const vy_e{stormo.vy_separation(separation_range,  boid) +
                          stormo.vy_alignment(
                              boids_vision * default_vision_value,
                               boid) +
                          stormo.vy_cohesion(
                              boids_vision * default_vision_value,
                              boid)};
        // Viene definita una nuova variabile new_v che rappresenta la nuova
        // velocità del boid, ottenuta sommando la velocità precedente e
        // v_esterna. Il coefficiente moltiplicativo di 1/8 è stato inserito
        // per
        // moderare l'incremento della velocità. (Da notare infatti che questa
        // simulazione è slegata dalla presenza di una velocità massima per i
        // boid, tale limite avrebbe reso molto più ostico il bilancio delle
        // interazioni, che avrebbero rischiato di escludersi a vicenda).
        new_vx = boid.get_vx() + vx_e / 8.;  // senza il diviso 8 si muovono più
                                             // come api che come uccelli
        new_vy = boid.get_vy() + vy_e / 8.;

        // Ora controlla se devono essere attivati una serie di modifiche
        // particolari sulla velocità del boid.

        // Per prima cosa applica la veloctià di slowdown se si trova nelle
        // condizioni di essere applicata (vedi la definizione di
        // p_slowdown_active in velocity.cpp). La velocità di slowdown in
        // realtà
        // non è altro che una riduzione di un fattore moltiplicatico di
        // 1/1.015
        // sulla nuova velocità del boid. Sembra poco, ma dato che verrà
        // applicato
        // 200 volte al secondo, risulta effettiva. Il grande vantaggio di
        // questa
        // soluzione sta nella poca influenza che ha la velocità del boid,
        // quando
        // oltrepassa i limiti esterni, sul tempo che ci impiegherà ad essere
        // rallentato. Questo produrrà un effetto molto fluido
        // indipendentemente
        // dallo stato iniziale del boid.
        if (p_slowdown_active_x(boid)) {
          new_vx = new_vx / 1.015;
        }

        if (p_slowdown_active_y(boid)) {
          new_vy = new_vy / 1.015;
        }

        // Se il boid si trova in condizioni di essere accelerato verso il
        // centro
        // (vedi la definizione di p_velx_active) subisce una velocità
        // aggiuntiva
        // calcolata da v_perimeter.
        if (not_in_perimeter_x(boid) && p_velx_active(boid)) {
          new_vx += v_perimeterx(0.015, boid);
        }

        if (not_in_perimeter_y(boid) && p_vely_active(boid)) {
          new_vy += v_perimetery(0.015, boid);
        }
        // Se il boid si trova nel range per essere spaventato dallo scoppio,
        // allora è capace di fuggire spaventato, grazie ad una velocità
        // aggiuntiva calcolata da expl_velocity
        if (in_explosion_range(boom_positionx, boom_positiony, 100., boid)) {
          new_vx += expl_velocity_x(10., boom_positionx, boid);
          new_vy += expl_velocity_y(10., boom_positiony, boid);
        }
      }
      // Infine setta le nuove caratteristiche del boid
      boid.set_x(new_x);
      boid.set_y(new_y);
      boid.set_vx(new_vx);
      boid.set_vy(new_vy);

      return boid;
    }); //fine for each
     
    // Quando transform ha finito di operare, setta come nuovo vettore di
    // Boid
    // dello stormo, quello modificato da transform.
    std::for_each(flock.begin(), flock.end(), [&](Boid& boid) {
      if (boid.get_is_dead() && (boid.get_y() > display_height || menu_rectangle.getGlobalBounds().contains(
            boid.get_x(),
            boid.get_y()) || stat_rectangle.getGlobalBounds().contains(
            boid.get_x(),
            boid.get_y()) )
            ) {
              
        flock.erase(std::find(flock.begin(), flock.end(), boid));
      }
    });
    
    stormo.set_flock(flock);
 }
  return; //solo quando sono passati un numero di millisecondi pari a step per evolution, restituisci il controllo
}

// Funzione che calcola l'angolo (in gradi) di
// orientamento del singolo boid rispetto all'asse x
double orientation(double vx, double vy) {
  if (vx >= 0.) {
    return atan(-vy / vx) * (180 / M_PI) - 90;
  }
  return (atan(-vy / vx) + M_PI) * (180 / M_PI) - 90;
}
