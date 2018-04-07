#include "graph.h"
#include <fstream>
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(GRISCLAIR);
}

void Graph::lirefichier(const std::string& nom)
{
    m_interface = std::make_shared <GraphInterface>(50,0,750,600);
    std::ifstream fichier (nom.c_str());
    if (fichier)
    {
        int nb_sommet, nb_arete;
        int indice;
        double value;
        int x;
        int y;
        std::string nom="";
        int aindice, s1, s2;
        double poids;
        fichier >> nb_sommet;
        std::cout << nb_sommet;

        for (int i=0; i<nb_sommet;i++)
        {
            fichier>>indice;
            fichier >>value;
            fichier>>x;
            fichier>>y;
            fichier>>nom;
            add_interfaced_vertex(indice, value, x, y, nom);
        }
        fichier>>nb_arete;
            for (int i=0; i<nb_arete;i++)
            {
                fichier>>aindice;
                fichier>>s1;
                fichier>>s2;
                fichier>>poids;
                add_interfaced_edge(aindice,s1,s2,poids);
            }
    }
}
/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example_mer()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0,  0.0, 400, 100, "requin.jpg");
    add_interfaced_vertex(1,  0.0, 200, 300, "tortue.jpg");
    add_interfaced_vertex(2,  0.0, 400, 300, "dauphin.jpg");
    add_interfaced_vertex(3,  0.0, 600, 300, "thon.jpg");
    add_interfaced_vertex(4,  0.0, 100, 500, "meduse.jpg");
    add_interfaced_vertex(5,  0.0, 300, 500, "crevette.jpg");
    add_interfaced_vertex(6,  0.0, 500, 500, "sardine.jpg");
    add_interfaced_vertex(7,  0.0, 700, 500, "calmar.jpg");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // Ajouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 0, 1,  0.0);
    add_interfaced_edge(1, 0, 2,  0.0);
    add_interfaced_edge(2, 0, 3,  0.0);
    add_interfaced_edge(3, 1, 4,  0.0);
    add_interfaced_edge(4, 1, 5,  0.0);
    add_interfaced_edge(5, 1, 6,  0.0);
    add_interfaced_edge(6, 2, 5,  0.0);
    add_interfaced_edge(7, 2, 6,  0.0);
    add_interfaced_edge(8, 2, 7,  0.0);
    add_interfaced_edge(9, 3, 6,  0.0);
    add_interfaced_edge(10, 3, 7,  0.0);
}

void Graph::make_example_air()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0,  0.0, 300, 100, "aigle.jpg");
    add_interfaced_vertex(1,  0.0, 500, 100, "buse.jpg");
    add_interfaced_vertex(2,  0.0, 200, 300, "pie.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "mesange.jpg");
    add_interfaced_vertex(4,  0.0, 600, 300, "geai.jpg");
    add_interfaced_vertex(5,  0.0, 200, 500, "mille.jpg");
    add_interfaced_vertex(6,  0.0, 400, 500, "chenille.jpg");
    add_interfaced_vertex(7,  0.0, 600, 500, "gland.jpg");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // Ajouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 0, 2,  0.0);
    add_interfaced_edge(1, 0, 3,  0.0);
    add_interfaced_edge(2, 1, 3,  0.0);
    add_interfaced_edge(3, 1, 4,  0.0);
    add_interfaced_edge(4, 2, 5,  0.0);
    add_interfaced_edge(5, 2, 6,  0.0);
    add_interfaced_edge(6, 3, 5,  0.0);
    add_interfaced_edge(7, 3, 6,  0.0);
    add_interfaced_edge(8, 4, 7,  0.0);
}
void Graph::make_example_terre()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0,  0.0, 300, 50, "tamanoir.jpg");
    add_interfaced_vertex(1,  0.0, 500, 50, "jaguar.jpg");
    add_interfaced_vertex(2,  0.0, 100, 200, "mygale.jpg");
    add_interfaced_vertex(3,  0.0, 300, 200, "fourmi.jpg");
    add_interfaced_vertex(4,  0.0, 300, 350, "mante.jpg");
    add_interfaced_vertex(5,  0.0, 500, 350, "tatou.jpg");
    add_interfaced_vertex(6,  0.0, 700, 350, "tapir.jpg");
    add_interfaced_vertex(7,  0.0, 300, 500, "phasme.jpg");
    add_interfaced_vertex(8,  0.0, 500, 500, "vegetaux.jpg");

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // Ajouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 0,  0.0);
    add_interfaced_edge(1, 0, 3,  0.0);
    add_interfaced_edge(2, 1, 5,  0.0);
    add_interfaced_edge(3, 1, 6,  0.0);
    add_interfaced_edge(4, 2, 4,  0.0);
    add_interfaced_edge(5, 3, 4,  0.0);
    add_interfaced_edge(6, 4, 7,  0.0);
    add_interfaced_edge(7, 5, 8,  0.0);
    add_interfaced_edge(8, 6, 8,  0.0);
    add_interfaced_edge(9, 7, 8,  0.0);
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
}

Graph Graph::menu(Graph g)
{
    BITMAP *page;
    BITMAP *buffer;
    BITMAP *choix1;
    BITMAP *choix2;
    BITMAP *choix3;
    int w=0;

    page = load_bitmap("fond.bmp",NULL);
    buffer = create_bitmap(SCREEN_W,SCREEN_H);

    choix1=load_bitmap("aerienne1.bmp",NULL);
    if (!choix1)

    {
        allegro_message("pas pu trouver/charger aerienne.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    choix2=load_bitmap("terrestre1.bmp",NULL);
    if (!choix2)

    {
        allegro_message("pas pu trouver/charger terrestre.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    choix3=load_bitmap("marine1.bmp",NULL);
    if (!choix3)

    {
        allegro_message("pas pu trouver/charger marine.bmp");
        allegro_exit();
        exit(EXIT_FAILURE);
    }


    while (!w)
    {
        clear_bitmap(buffer);
        blit (page,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        masked_blit(choix1,page,0,0,100,100,SCREEN_W,SCREEN_H);
        masked_blit(choix2,page,0,0,100,200,SCREEN_W,SCREEN_H);
        masked_blit(choix3,page,0,0,100,300,SCREEN_W,SCREEN_H);
        blit(page,screen,0,0,0,0,SCREEN_W,SCREEN_H);

        if ((mouse_b&1))
        {
            if (mouse_y>100 && mouse_y<150)
            {
                clear_bitmap(buffer);
                clear_bitmap(page);
                clear_bitmap(choix1);
                clear_bitmap(choix2);
                clear_bitmap(choix3);
                Graph g;
                g.lirefichier("aerien.txt");
                while ( !key[KEY_ESC] )
                    {
                        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                        g.update();

                        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                        grman::mettre_a_jour();
                    }

                grman::fermer_allegro();
            }
            else if (mouse_y>200 && mouse_y<250)
            {
                clear_bitmap(buffer);
                clear_bitmap(page);
                clear_bitmap(choix1);
                clear_bitmap(choix2);
                clear_bitmap(choix3);
                Graph g;
                g.lirefichier("terrestre.txt");
                while ( !key[KEY_ESC] )
                    {
                        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                        g.update();


                        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                        grman::mettre_a_jour();
                    }

                grman::fermer_allegro();

            }
            else if (mouse_y>300 && mouse_y<350)
            {
                clear_bitmap(buffer);
                clear_bitmap(page);
                clear_bitmap(choix1);
                clear_bitmap(choix2);
                clear_bitmap(choix3);
                Graph g;
                g.lirefichier("marin.txt");
                while ( !key[KEY_ESC] )
                    {
                        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                        g.update();


                        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
                        grman::mettre_a_jour();
                    }

                grman::fermer_allegro();
            }
            while(!(mouse_b&1))
            {

            }
            rest(500);
        }
    }



}




