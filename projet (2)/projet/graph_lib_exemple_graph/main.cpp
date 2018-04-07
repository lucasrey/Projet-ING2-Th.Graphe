#include "grman/grman.h"
#include <iostream>
#include <time.h>
#include "graph.h"
using namespace std;
int main()
{

    grman::init();

    grman::set_pictures_path("pics");


    /// Le nom du répertoire où se trouvent les images à charger

   Graph g;
   /*g.make_example_mer();*/

   g=g.menu(g);

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
   while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    rest(20);
    return 0;
}
END_OF_MAIN();


