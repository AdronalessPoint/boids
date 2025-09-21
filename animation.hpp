#ifndef ANIMATION
#define ANIMATION
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

class Animation
{
private:
sf::Vector2u image_count_;//quante immagini nella griglia (vuole per il largo e per il lungo)
//u perchè unsigned basta, non hai numeri negativi
sf::Vector2u current_image_;

//speed of switch
double total_time_{0.};
double switch_time_; //dopo quanto sposti il rettangolo

public:
sf::IntRect rect;
sf::Clock time;
bool just_called{false};
public:
Animation (sf::Texture texture, sf::Vector2u image_count, double switch_time) //gli dici ogni quanto scorrere il rettangolo
      : image_count_{image_count}
      , switch_time_{switch_time}
      
  {
    rect.width = texture.getSize().x / image_count_.x; //crei il quadrato in modo che sia uguale a 1 singolo disegno della striscia
    rect.height = texture.getSize().y / image_count_.y;
  }

void Update(int row){ //row è quale riga della griglia vuoi venga animata (0per la prima)
    current_image_.y=row;
    
    if (time.getElapsedTime().asSeconds() >= switch_time_)
    {
           current_image_.x++; //aggiorni il contatore
            if (current_image_.x >= image_count_.x || just_called) {current_image_.x=0;} //serve per evitare che scorri fino a finire l'immagie
    time.restart();
    };
   /* if (total_time_ >= switch_time_)
        {
            total_time_ -= switch_time_; //invecce che zero, in modo da essere più regolari
            current_image_.x++; //aggiorni il contatore
            if (current_image_.x >= image_count_.x || just_called) {current_image_.x=0;} //serve per evitare che scorri fino a finire l'immagie
        }*/

    rect.left = current_image_.x * rect.width; //sposti il rettangolo
    rect.top = current_image_.y * rect.height; //nel caso in cui tu abbia griglia
    
}

};


#endif