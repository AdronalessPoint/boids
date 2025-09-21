#ifndef FLOCK
#define FLOCK

#include <algorithm>

#include "boids.hpp"
#include "iterator"
#include "r_numbers.hpp"

// Classe flock, rappresenta l'insieme di boids che interagiscono tra di loro.
//  La maggior parte dei metodi di questa classe sono definiti in altre TU, in
//  particolare il calcolo delle velocità è assegnato a velocity.cpp, mentre il
//  calcolo statistico a statistics.cpp
class Flock {
  // I dati membro sono un vettore che conterrà gli oggeti Boid, ovvero i boids
  // dello stormo, e i valori dei parametri di repulsione, allineamento e
  // coesione
  std::vector<Boid> flock;
  double sep_;
  double al_;
  double coh_;
  int size_{0};
  // Costruttore
 public:
  Flock(double s, double a, double c, int n)
      : sep_{s}, al_{a}, coh_{c}, size_{n} {}
  Flock(double s, double a, double c)
      : sep_{s},
        al_{a},
        coh_{c}

  {}
  // Metodo che riempe lo stormo in base al numero intero di boids che si decide
  // di generare
  void fill()  // potresti non dargli niente e fargli usare come n la size (devi
               // cambiare alcune cose in simulation.test però)
  {
    std::generate_n(std::back_inserter(flock), size_, []() {
      Boid boid{r_position_x(), r_position_y(), r_velocity(), r_velocity()};
      return boid;
    });
  }
  // Metodi get e set necessari per interfacciarsi con i dati membro
  auto get_flock() const { return flock; }

  void set_flock(std::vector<Boid> const& new_flock) { flock = new_flock; }

  // aggiungere un boid
  void add_a_boid() {
    Boid boid{80., 0.9 * display_height - 71, 1000,
              -1000};  // creato dove c'è il nido
    flock.push_back(boid);
    size_ = size_ + 1;
  }
  auto dead_boid_walking() {
    if (size_ > 1) {
      std::uniform_int_distribution<int> random_boid(0, size_ - 1);
      std::default_random_engine eng;
      eng.seed(std::chrono::system_clock::now().time_since_epoch().count());
      assert(random_boid(eng) >= 0. && random_boid(eng) < size_);
      /*auto dead_boid {*(flock.begin()+random_boid(eng))};
         while(dead_boid.get_is_dead()){dead_boid=*(flock.begin()+random_boid(eng));};*/
      auto it_to_the_boid{flock.begin() + random_boid(eng)};

      while ((*it_to_the_boid).get_is_dead()) {
        it_to_the_boid = flock.begin() + random_boid(eng);
      };

      (*it_to_the_boid).set_is_dead();

      // remove_if(flock.begin(), flock.end(), [](Boid const& boid){return
      // boid.get_is_dead_();}); flock.erase(it_to_the_boid);
      // std::rotate(it_to_the_boid,it_to_the_boid+1,flock.end()); //moves the
      // boid who must die to the end of the vector

      size_ = size_ - 1;
    }
  }
  /*void kill_a_boid (){
    remove_if(flock.begin(), flock.end(), [](Boid const& boid){return
  boid.get_is_dead_();});
    //flock.pop_back();



  }*/

  double get_al() const { return al_; }

  double get_sep() const { return sep_; }

  double get_coh() const { return coh_; }
  double get_size() const { return size_; }

  void set_ali(double new_al) { al_ = new_al; }

  void set_sep(double new_sep) { sep_ = new_sep; }

  void set_coe(double new_coh) { coh_ = new_coh; }
  void set_size(double new_size) { size_ = new_size; }
  // Dichiarazione dei metodi che calcolano le velocità di interazione tra i
  // boids dello stormo. La decisione di farle funzioni membro è stata presa per
  // via dei vantaggi a cui porta, specialmente l'accesso ai dati membro.
  // I corpi dei metodi sono definiti in velocity.cpp
  double vx_separation(double range, Boid const& fixed_boid) const;
  double vy_separation(double range, Boid const& fixed_boid) const;

  double vx_alignment(double range, Boid const& fixed_boid) const;
  double vy_alignment(double range, Boid const& fixed_boid) const;

  double vx_cohesion(double range, Boid const& fixed_boid) const;
  double vy_cohesion(double range, Boid const& fixed_boid) const;

  // Dichiarazione dei metodi per il calcolo statistico, il corpo è definito in
  // statistics.cpp
  double mean_distance() const;
  double mean_velocity() const;
  double stnd_deviation_distance(double mean_distance) const;
  double stnd_deviation_velocity(double mean_velocity) const;
};

#endif