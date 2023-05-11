// apps/LIFAMI/TPParticleSpring.cpp
#include<Grapic.h>
#include<iostream>
#include<math.h>
#include<dos.h>
#include<cmath>

using namespace grapic;
using namespace std;

//Mes constantes utilis�es
const int DimW=800;
const int DimH=400;
const float FRICTION = 0.5;


const int MAX = 20;
const int MAX_PART = 100;
const int MAX_SPRING = 100;

struct Complex
{
    float x,y;
};

struct Color
{
    int R;
    int V;
    int B;
};

struct Particule
{
    Complex p;
    Complex v;
    Complex f;
    float m;
};

//Mes 2 structures utilis�es
struct Boules
{
    Color c;
    Complex pos;
    Complex vit;
    Complex f;
    int vie;
    float m;
    float r;

};


struct Jeu
{
    int nb_B;
    float dt;
    Boules tab[MAX];
};







Complex make_complex (float x, float y)
{
    Complex c ;
    c.x= x ;
    c.y= y ;
    return c;
}

Complex make_complex_exp(float r, float theta_deg)
{
    Complex c;
    float theta=(theta_deg*M_PI)/180.0;
    c.x= r*cos(theta);
    c.y= r*sin(theta);
    return c;
}

Complex operator+ (Complex c1, Complex c2)
{
    Complex c;
    c.x = c1.x + c2.x ;
    c.y = c1.y + c2.y ;
    return c;
}

Complex operator- (Complex c1, Complex c2)
{
    Complex c;
    c.x = c1.x - c2.x ;
    c.y = c1.y - c2.y ;
    return c;
}

Complex operator* (Complex c1, Complex c2)
{
    Complex c;
    c.x = c1.x * c2.x - c1.y * c2.y;
    c.y = c1.x * c2.y + c1.y * c2.x;
    return c;
}

Complex operator* (float lambda, Complex c1)
{
    Complex c;
    c.x = lambda * c1.x;
    c.y = lambda * c1.y;
    return c;
}

Complex operator* (Complex c1, float lambda)
{
    Complex c;
    c.x = c1.x * lambda ;
    c.y = c1.y * lambda ;
    return c;
}

Complex operator/ (Complex c1, Complex c2)
{
    Complex c;
    c.x = c1.x / c2.x ;
    c.y = c1.y / c2.y ;
    return c;
}

Complex operator/ (Complex c1, float scalar)
{
    Complex c;
    c.x = c1.x / scalar;
    c.y = c1.y / scalar;
    return c;
}

Complex Rotate (Complex p, float cx, float cy, float theta_deg)
{
    Complex c = make_complex(cx,cy);
    Complex r = make_complex_exp(1,theta_deg);
    return (p-c)*r+c;
}

Complex translate (Complex p, float dx, float dy)
{
    Complex v = make_complex(dx,dy);
    return p+v;
}


Color operator*(Color C1, float f)
{
    Color C;
    C.R=C1.R*f;
    C.V=C1.V*f;
    C.B=C1.B*f;
    return C;
}

Color operator+(Color C1, Color C2)
{
    Color C;
    C.R=C1.R+C2.R;
    C.V=C1.V+C2.V;
    C.B=C1.B+C2.B;
    return C;
}

float interpolation (float a, float b, float &t)
{
    if (t<0 || t>1)
    {
        cout<<"Entrez une valeur de t compris entre 0 et 1 ;"<<endl;
        cin>>t;
    }
    return a*(1-t)+b*t;
}

Color interpol_Color(Color A, Color B, float t)
{
    Color C;
    C.R=A.R*(1-t)+B.R*t;
    C.V=A.V*(1-t)+B.V*t;
    C.B=A.B*(1-t)+B.B*t;
    return C;
}

Complex interpol_Complex (Complex Ca,Complex Cb, float t)
{
    Complex C;
    C.x=Ca.x*(1-t)+Cb.x*t;
    C.y=Ca.y*(1-t)+Cb.y*t;
    return C;
}

void addForce(Particule &Pa, Complex F)
{
    Pa.f=Pa.f+F;
}

Color make_color (int r, int g, int b)
{
    Color c;
    c.R = r ;
    c.V = g ;
    c.B = b ;
    return c;
}


//Initialisation de tout mes parametres
void initJeu (Jeu &j)
{
    j.nb_B=16;
    for ( int i = 0 ; i < j.nb_B; i++)
    {
        j.tab[i].r=9;
        j.tab[i].vit=make_complex(0.f,0.f);
        j.tab[i].f=make_complex(1.f,1.f);
        j.tab[i].m=1.0;
        j.tab[i].vie=1;
    }
    j.tab[0].pos.x=DimW-DimW/4;
    j.tab[0].pos.y=DimH/2;
    int n = 1;
    for (int a = 0 ; a < 5 ; a++)
    {
        for(int b = 0 ; b < a+1 ; b++)
        {
            j.tab[n].pos.x=DimW/4-a*2*j.tab[n].r;
            j.tab[n].pos.y=DimH/2+b*2*j.tab[n].r-j.tab[n].r*a;
            n++;
        }
    }

    j.tab[0].c=make_color(255,255,255);
    j.tab[5].c=make_color(0,0,0);
    j.tab[1].c=make_color(255,0,0);
    j.tab[2].c=make_color(255,0,0);
    j.tab[3].c=make_color(230,230,30);
    j.tab[4].c=make_color(230,230,30);
    j.tab[6].c=make_color(255,0,0);
    j.tab[7].c=make_color(255,0,0);
    j.tab[8].c=make_color(230,230,30);
    j.tab[9].c=make_color(255,0,0);
    j.tab[10].c=make_color(230,230,30);
    j.tab[11].c=make_color(230,230,30);
    j.tab[12].c=make_color(230,230,30);
    j.tab[13].c=make_color(255,0,0);
    j.tab[14].c=make_color(230,230,30);
    j.tab[15].c=make_color(255,0,0);

}

//interpolation cercles
void drawcircle(Complex A, int N)
{
    Color CA = {200, 200, 200};
    Color CB = {0, 0, 0};
    Complex Csuiv;
    Color Cint;
    int i;
    Complex Cprec = A;
    for (i = 1; i <= N; i++)
    {
        float t = i * (1.0 / N);
        Cint = interpol_Color(CA, CB, t);
        color(Cint.R, Cint.V, Cint.B);
        circle(A.x,A.y,i);
        Cprec = Csuiv;
    }
}

//Draw general de mon jeu
void Draw (Jeu j)
{
    //Dessin des trous noirs
    Complex trous[6] =
    {
        make_complex(20, 20),
        make_complex(20, DimH - 20),
        make_complex(DimW - 20, 20),
        make_complex(DimW - 20, DimH - 20),
        make_complex(DimW / 2, 15),
        make_complex(DimW / 2, DimH - 15)
    };
    for (int i = 0; i < 6 ; i++)
    {
        drawcircle(trous[i],15);
    }

    for (int i = 0 ; i < j.nb_B ; i++)
    {
        if (j.tab[i].vie==1)
        {
            color(j.tab[i].c.R,j.tab[i].c.V,j.tab[i].c.B);
            circleFill(j.tab[i].pos.x,j.tab[i].pos.y,j.tab[i].r);
            color(0,0,0);
            circle(j.tab[i].pos.x,j.tab[i].pos.y,j.tab[i].r);
        }
    }
    color(255,255,255);
    fontSize(13);
    print(j.tab[5].pos.x-2,j.tab[5].pos.y-9,"8");




    color(90,50,50);
    rectangleFill(0,0,DimW,15);
    rectangleFill(0,DimH-15,DimW,DimH);
    rectangleFill(0,0,15,DimH);
    rectangleFill(DimW-15,0,DimW,DimH);

}

//Fonction qui calcule la distance entre deux complexes
float distance(Complex a, Complex b)
{
    Complex ab=b-a;
    return sqrt(ab.x*ab.x+ab.y*ab.y);
}

//Interpolation ligne appel� ci dessous
void drawline(Complex A, Complex B, int N)
{
    Color CA = {255, 0, 0};
    Color CB = {0, 0, 255};
    Complex Csuiv;
    Color Cint;
    int i;
    Complex Cprec = A;
    for (i = 1; i <= N; i++)
    {
        float t = i * (1.0 / N);
        Csuiv = interpol_Complex(A, B, t);
        Cint = interpol_Color(CA, CB, t);
        color(Cint.R, Cint.V, Cint.B);
        line(Cprec.x, Cprec.y, Csuiv.x, Csuiv.y);
        Cprec = Csuiv;
    }
}




//Fonction qui permet de controler la balle blanche
bool BallClick = false;

void updateWhiteBall(Jeu &j)
{
    int x, y;
    mousePos(x, y);
    Complex souris = make_complex(x, y);
    float dt;
    Complex null = make_complex(0,0);
    if (j.tab[0].f.x==null.x && j.tab[0].f.y==null.y)
    {
        if (isMousePressed(SDL_BUTTON_LEFT) && !BallClick &&
                (x >= j.tab[0].pos.x - 10 && x <= j.tab[0].pos.x + 10) &&
                (y >= j.tab[0].pos.y - 10 && y <= j.tab[0].pos.y + 10))
        {
            BallClick = true;
        }
        else if (!isMousePressed(SDL_BUTTON_LEFT) && BallClick)
        {
            BallClick = false;
            /*P2.pos=souris;
            Complex vP1P2=P2.pos-P1.pos;
            float dP1P2=sqrt(pow(P2.pos.x-P1.pos.x,2)+pow(P2.pos.y-P1.pos.y,2));
            Complex er= vP1P2*(1/dP1P2);
            P1.f=P1.f+(-0.5*(dP1P2-10)*(-1*er));
            */
            float dis = (distance(j.tab[0].pos, souris));
            if (dis <= 150)
            {
                j.tab[0].f =  j.tab[0].pos - souris;
            }
            else
            {
                float lien = 150 / dis;
                float newX = j.tab[0].pos.x + (souris.x - j.tab[0].pos.x) * lien;
                float newY = j.tab[0].pos.y + (souris.y - j.tab[0].pos.y) * lien;
                Complex newSouris = make_complex(newX, newY);
                j.tab[0].f =  j.tab[0].pos - newSouris;
            }
            //cout << P1.f.x << " " << P1.f.y << endl;
            j.tab[0].f = j.tab[0].f * 9;
        }
    }
    dt = 0.0235;
    // Mettre � jour la position de la balle � chaque it�ration, ind�pendamment de l'�tat du clic
    j.tab[0].vit = j.tab[0].vit + dt * (j.tab[0].f * (1 / j.tab[0].m)) ; //*-1 pour l'oppos�
    j.tab[0].pos = j.tab[0].pos + j.tab[0].vit*dt;
    j.tab[0].f = make_complex(j.tab[0].f.x * 0.9994,j.tab[0].f.y * 0.9994) ;
    j.tab[0].vit=j.tab[0].vit*FRICTION;
    j.tab[0].f.x = j.tab[0].f.x - (j.tab[0].f.x * (2/3));
    //cout << j.tab[0].vit.x << "   " << j.tab[0].vit.y << endl;
    //j.tab[0].f = make_complex(0,0);




    if (BallClick)
    {
        Complex oppositeSouris = j.tab[0].pos - (souris - j.tab[0].pos)*100;
        float dis = (distance(j.tab[0].pos, souris));
        if (dis <= 150)
        {
            drawline(j.tab[0].pos, souris, 50);
        }
        else
        {
            float lien = 150 / dis;
            float newX = j.tab[0].pos.x + (souris.x - j.tab[0].pos.x) * lien;
            float newY = j.tab[0].pos.y + (souris.y - j.tab[0].pos.y) * lien;
            Complex newSouris = make_complex(newX, newY);
            drawline(j.tab[0].pos, newSouris, 50);
        }
        color(255,255,255);
        line(j.tab[0].pos.x,j.tab[0].pos.y,oppositeSouris.x,oppositeSouris.y);
    }
}

//Fonction qui renvoie un bool�en si les balles sont rentr�e en collision
bool Collision(const Boules &b1, const Boules &b2)
{
    float dx = b1.pos.x - b2.pos.x;
    float dy = b1.pos.y - b2.pos.y;
    float distance = dx * dx + dy * dy;
    float rayon = b1.r + b2.r;
    return distance <= rayon * rayon;
}

//Fonction qui calcule les forces au moment de la collision en utilisant des fonctions physiques
void calculCollision(Boules &b1, Boules &b2)
{
    Complex delta = b1.pos - b2.pos;
    float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    float dt = (distance - b1.r - b2.r) / 2;

    b1.pos = b1.pos - (dt * (delta / distance));
    b2.pos = b2.pos + (dt * (delta / distance));

    Complex normale = (b2.pos - b1.pos) / distance;
    Complex vitesseRelative = b1.vit - b2.vit;
    float impulsion = 6 * (vitesseRelative.x * normale.x + vitesseRelative.y * normale.y) / (b1.m + b2.m);

    b1.f = b1.f - ((impulsion * b2.m) * normale);
    b2.f = b2.f + ((impulsion * b1.m) * normale);
}

//Fonction qui mets en place mes collision et distribue les forces
void collision(Jeu &j)
{
    for (int i = 0; i < j.nb_B; i++)
    {
        if (j.tab[i].pos.y > DimH - j.tab[i].r - 15 )
        {
            j.tab[i].pos.y = DimH - j.tab[i].r - 15;
            j.tab[i].vit.y = -j.tab[i].vit.y;
            j.tab[i].f.y= j.tab[i].f.y * -1;
            j.tab[i].f = j.tab[i].f * FRICTION;
        }
        else
        {
            if (j.tab[i].pos.y < j.tab[i].r + 15)
            {
                j.tab[i].pos.y = j.tab[i].r + 15;
                j.tab[i].vit.y = -j.tab[i].vit.y;
                j.tab[i].f.y= j.tab[i].f.y * -1;
                j.tab[i].f = j.tab[i].f * FRICTION;
            }
        }

        if (j.tab[i].pos.x > DimW - j.tab[i].r - 15)
        {
            j.tab[i].pos.x = DimW - j.tab[i].r - 15;
            j.tab[i].vit.x = -j.tab[i].vit.x;
            j.tab[i].f.x= j.tab[i].f.x * -1;
            j.tab[i].f = j.tab[i].f * FRICTION;
        }
        else
        {
            if (j.tab[i].pos.x < + j.tab[i].r + 15)
            {
                j.tab[i].pos.x =j.tab[i].r + 15;
                j.tab[i].vit.x = -j.tab[i].vit.x;
                j.tab[i].f.x= j.tab[i].f.x * -1;
                j.tab[i].f = j.tab[i].f * FRICTION;
            }
        }

        for (int i = 0; i < j.nb_B; i++)
        {
            for (int k = i + 1; k < j.nb_B; k++)
            {
                if (Collision(j.tab[i], j.tab[k]))
                {
                    calculCollision(j.tab[i], j.tab[k]);
                }
            }
        }

    }
}

//Fonction qui calcule la position t+1 des balles hors Balle blanche calcul� dans updateWhiteBall
void updatePosition (Jeu &j)
{
    float dt = 0.02;
    for (int i = 1 ; i<j.nb_B ; i++)
    {
        j.tab[i].vit = j.tab[i].vit + dt * (j.tab[i].f * (1 / j.tab[i].m)) ; //*-1 pour l'oppos�
        j.tab[i].pos = j.tab[i].pos + j.tab[i].vit*dt;
        j.tab[i].f = make_complex(j.tab[i].f.x * 0.9994,j.tab[i].f.y * 0.9994) ;
        j.tab[i].vit=j.tab[i].vit*FRICTION;
    }
    for (int i = 0; i < j.nb_B ; i++)
    {
        if (abs(j.tab[i].vit.x) < 0.15 && abs(j.tab[i].vit.y) < 0.15)
        {
            j.tab[i].f=make_complex(0,0);
        }
    }
}

//Fonction qui verifie si les balles rentrent dans les tous noir
void TrouNoir(Jeu& j)
{
    Complex trous[6] =
    {
        make_complex(20, 20),
        make_complex(20, DimH - 20),
        make_complex(DimW - 20, 20),
        make_complex(DimW - 20, DimH - 20),
        make_complex(DimW / 2, 15),
        make_complex(DimW / 2, DimH - 15)
    };
    //Autres balles
    for (int i = 1; i < j.nb_B; i++)
    {
        if (j.tab[i].vie == 1)
        {
            for (int k = 0; k < 6; k++)
            {
                float dist = distance(j.tab[i].pos, trous[k]);
                if (dist < j.tab[i].r+5)
                {
                    j.tab[i].vie = 0;
                    j.tab[i].pos=make_complex(-100,-100);
                }
            }
        }
    }
    //Balle Blanche
    for (int k = 0; k < 6; k++)
    {
        float dist = distance(j.tab[0].pos, trous[k]);
        if (dist < j.tab[0].r+10)
        {
            j.tab[0].pos.x=DimW-DimW/4;
            j.tab[0].pos.y=DimH/2;
            j.tab[0].f=make_complex(1,1);
        }
    }
}

//Fonction qui renvoie un booleen si toutes les balles rouge sont rentr� dans les trous noirs
bool winRed (Jeu j)
{
    int mort = 0;
    for(int i = 1 ; i< j.nb_B ; i++)
    {
        if(j.tab[i].c.R==255)
        {
            if (j.tab[i].pos.x==-100)
            {
                mort+= 1;
            }
        }
    }

    if (mort == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Fonction qui renvoie un booleen si toutes les balles jaunes sont rentr� dans les trous noirs
bool winYellow (Jeu j)
{
    int mort = 0;

    for(int i = 1 ; i< j.nb_B ; i++)
    {
        if(j.tab[i].c.R==230)
        {
            if (j.tab[i].pos.x==-100)
            {
                mort+= 1;
            }
        }
    }

    if (mort == 7)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Fonction qui verifie si la balle noir est rentr� dans un trou noir
bool Ball8 (Jeu j)
{
    if (j.tab[5].pos.x==-100)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Mon main ou je fais toutes mes verifs de victoire/defaite et arr�te le jeu, sinon le jeu continue.
int main(int, char**)
{
    bool stop = false ;
    winInit("ressors",DimW,DimH);
    backgroundColor(10,120,50);
    Jeu j ;
    initJeu(j);
    float v;
    Complex pos;
    while(!stop)
    {
        winClear();
        if (winRed(j)==true && Ball8(j)==true)
        {
            Draw(j);
            color(255,70,70);
            fontSize(20);
            print(DimW/2-100,DimH/2,"L'�quipe rouge a gagn� !");
        }else if (winYellow(j)==true && Ball8(j)==true)
        {
            Draw(j);
            color(190,170,40);
            fontSize(20);
            print(DimW/2-100,DimH/2,"L'�quipe jaune a gagn� !");
        }else if (Ball8(j)==true)
        {
            Draw(j);
            color(0,0,0);
            fontSize(20);
            print(DimW/2-170,DimH/2,"La Balle noire est rentr�, vous avez perdu !");
        }
        else
        {
            updateWhiteBall(j);
            collision(j);
            updatePosition(j);
            TrouNoir(j);
            Draw(j);
        }
        stop=winDisplay();

    }
    pressSpace();
    winQuit();
    return 0;
}
