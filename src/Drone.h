#ifndef __DRONE_H__
#define __DRONE_H__

// les formes
FormeCube *cube = NULL;
FormeQuad *quad = NULL;
FormeSphere *sphere = NULL;
FormeCylindre *cylindre = NULL;
// (partie 1) Vous devez vous servir des quatre fonctions ci-dessous (*sans les modifier*) pour tracer tous les parties des objets.
// affiche un cylindre de rayon 1.0 et de longueur 1.0, dont la base est centrée en (0,0,0)
void afficherCylindre( ) { cylindre->afficher(); }
// affiche une sphère de rayon 1.0, centrée en (0,0,0)
void afficherSphere( ) { sphere->afficher(); }
// affiche un cube d'arête 1.0, centrée en (0,0,0)
void afficherCube( ) { cube->afficher(); }
// affiche un quad d'arête 1
void afficherQuad( ) { quad->afficher(); }

// affiche la position courante du repère (pour débogage)
void afficherRepereCourant( int num = 0 )
{
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    FenetreTP::afficherAxes( 1.5, 3.0 );
}

// partie 1: le drone
class Drone
{
public:
    Drone()
    {
        initVar();
        // créer le drone graphique
        initialiserGraphique();

        // créer quelques autres formes
        cube = new FormeCube( 1.0, true );
        quad = new FormeQuad( 1.0, true );
        sphere = new FormeSphere( 1.0, 8, 8, true );
        cylindre = new FormeCylindre( 1.0, 1.0, 1.0, 16, 1, true );
    }
    ~Drone()
    {
        conclureGraphique();
        delete cube;
        delete quad;
        delete sphere;
        delete cylindre;
    }

    void initVar() { position = glm::vec3(0.25*Etat::dimBoite, 0.0, 0.0); taille = 1; angleCorps = anglePale = 0.0; anglePatte = 70.0; }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        if ( anglePatte > 90.0 ) anglePatte = 90.0; else if ( anglePatte < 45.0 ) anglePatte = 45.0;
    }

    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // (partie 2) MODIFICATIONS ICI ...
        // créer le VBO pour les sommets
        //...

        // créer le VBO la connectivité
        //...

        glBindVertexArray(0);
    }

    void conclureGraphique()
    {
        glDeleteBuffers( 1, &vboTheiereSommets );
        glDeleteBuffers( 1, &vboTheiereConnec );
        glDeleteVertexArrays(1, &vao);
    }

    // (partie 2) Vous modifierez cette fonction pour utiliser les VBO
    // affiche une théière, dont la base est centrée en (0,0,0)
    void afficherTheiere()
    {
        glBindVertexArray( vao );
        // (partie 2) MODIFICATIONS ICI ...
        //...

        // vous pouvez utiliser temporairement cette fonction pour la première partie du TP, mais vous ferez mieux dans la seconde partie du TP
        glBegin( GL_TRIANGLES );
        for ( unsigned int i = 0 ; i < sizeof(gTheiereConnec)/sizeof(GLuint) ; i++ )
            glVertex3fv( &(gTheiereSommets[3*gTheiereConnec[i]] ) );
        glEnd( );

        glBindVertexArray(0);
    }

    // TODO:
    // afficher la boite (le corps) de le drone.
    // - Son corps est un cube légèrement aplati en y (sa hauteur) : facteurs d’échelle = ( 2.0, 1.0, 2.0 ).
    //   Pour la partie 2, la taille de la théière sera simplement modulée par ( 0.5, 0.5, 0.5 ).
    // - Le bas du cube corps est déplacé à l'origine.
    // - Recentrer la théière
    void afficherCorps()
    {
        // donner la couleur de la boite
        glVertexAttrib3f( locColor, BLANC.r, BLANC.g, BLANC.b ); // blanc

        // afficherRepereCourant( ); // débogage: montrer le repère à la position courante

        //Afficher la boite
        // afficher le bon modèle
        switch (Etat::modele)
        {
        default:
        case 1: // un cube
            matrModel.PushMatrix(); {
                matrModel.Translate( 0.0, 2.0, 0.0 ); // (bidon) À MODIFIER
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherCube();
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            break;

        case 2: // la théière
            matrModel.PushMatrix(); {
                matrModel.Scale( 0.25, 0.25, 0.25 );
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherTheiere();
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            break;
        }
    }

    // TODO:
    // afficher les rotors
    // Une pale est un quadrilatère de longueur "3" et de largeur "0.2".
    // - La première pale est à hauteur de 0.2 au-dessus de la face supérieure du corps et tourne dans le sens horaire lorsque vue du dessus.
    // - La seconde pale est à hauteur de 0.50 au-dessus de la face supérieure du corps et tourne dans le sens antihoraire.
    // L'angle de rotation des pales est donné par la variable « anglePale ».
    // et le mat
    // le mat est un cylindre de rayon 0.1 et de longueur 0.5.
    void afficherRotor()
    {
    
        // Son mat
        matrModel.PushMatrix(); {
            glVertexAttrib3f( locColor, CYAN.r, CYAN.g, CYAN.b ); // cyan
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
        
        // ajouter une ou des transformations afin de tracer des *pales carrées*, de la même largeur que le corps
        matrModel.PushMatrix();{
            // donner la couleur de la première pale
            glVertexAttrib3f( locColor, VIOLET.r, VIOLET.g, VIOLET.b ); // violet
            matrModel.Translate( 2.0, 2.0, 0.0 ); // (bidon) À MODIFIER
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherQuad();

            // donner la couleur de la seconde pale
            glVertexAttrib3f( locColor, VERT.r, VERT.g, VERT.b); // vert
            //...
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }

    // TODO:
    // afficher un support latéral
    // Le support est un cylindre de rayon 0.5 et de longueur 2.0.
    // Le support est positionné au coin du corps à un angle de 45 degré.
    void afficherSupport()
    {
        // donner la couleur du support
        glVertexAttrib3f( locColor, BRUN.r, BRUN.g, BRUN.b); // marron;

        // ajouter une ou des transformations afin de tracer chacune des supports
        matrModel.PushMatrix();{
            matrModel.Translate( -2.0, 2.0, 0.0 ); // (bidon) À MODIFIER
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        // ...
    }

    void afficher()
    {
        // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
        matrModel.PushMatrix();{ // sauvegarder la transformation courante

            // TODO:
            // ajouter une ou des transformations
            // Le drone
            //   est placé à la position courante « position[] »,
            //   est orienté selon l'angle « angleCorps » et
            //   est de taille ( taille, taille, taille ),

            // TODO: Utiliser les fonctions afficherCorps, afficherRotor et afficherSupport
            afficherCorps();
            afficherRotor();
            afficherSupport();           

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }

    void calculerPhysique()
    {
        if ( Etat::enmouvement )
        {
            static int sens[6] = { +1, +1, +1, +1, +1, +1 };
            glm::vec3 vitesse( 0.03, 0.02, 0.05 );
            // mouvement en X
            if ( position.x-taille <= -0.5*Etat::dimBoite ) sens[0] = +1;
            else if ( position.x+taille >= 0.5*Etat::dimBoite ) sens[0] = -1;
            position.x += 60*Etat::dt * vitesse.x * sens[0];
            // mouvement en Y
            if ( position.y-taille <= 0.0 ) sens[2] = +1;
            else if ( position.y+taille >= Etat::dimBoite ) sens[2] = -1;
            position.y += 60*Etat::dt * vitesse.y * sens[2];
            // mouvement en Z
            if ( position.z-taille <= -0.5*Etat::dimBoite ) sens[1] = +1;
            else if ( position.z+taille >= 0.5*Etat::dimBoite ) sens[1] = -1;
            position.z += 60*Etat::dt * vitesse.z * sens[1];

            // rotation du corps
            if ( angleCorps > 360.0 ) angleCorps -= 360.0;
            angleCorps += 60*Etat::dt * 0.35;

            // rotation des pales
            anglePale += 60*Etat::dt * 8.0 * sens[4];

            // angle des pattes et des pales
            if ( anglePatte <= 45.0 ) sens[3] = +1;
            else if ( anglePatte >= 90.0 ) sens[3] = -1;
            anglePatte += 60*Etat::dt * 1.0 * sens[3];

            // taille du corps
            if ( taille <= 0.25 ) sens[5] = +1;
            else if ( taille >= 1.0 ) sens[5] = -1;
            taille += 60*Etat::dt * 0.005 * sens[5];

        }
    }

    // partie 2: utilisation de vbo et vao
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    GLint locVertex = -1;
    GLint locColor = -1;

    glm::vec3 position;       // position courante de l'hélicoptère
    GLfloat taille;           // facteur d'échelle pour la taille globale du modèle
    GLfloat angleCorps;       // angle de rotation (en degrés) du corps de l'hélicoptère
    GLfloat anglePale;        // angle de rotation (en degrés) des pales
    GLfloat anglePatte;       // angle de rotation (en degrés) des pattes

    const glm::vec3 BLANC  = glm::vec3(1.0, 1.0, 1.0);
    const glm::vec3 CYAN   = glm::vec3(0.0, 1.0, 1.0);
    const glm::vec3 VIOLET = glm::vec3(0.5, 0.5, 1.0);
    const glm::vec3 VERT   = glm::vec3(0.0, 1.0, 0.0);
    const glm::vec3 BRUN   = glm::vec3(0.9, 0.4, 0.0);

    // TODO: Ajouté des constantes au besoin
    // ...
};

#endif
