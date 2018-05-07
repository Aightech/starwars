#include "element.hpp"


unsigned long int* Element::s_map = NULL;
int Element::s_mapHeight = 0;
int Element::s_mapWidth = 0;
int Element::s_mapOffsetX = 0;
int Element::s_mapOffsetY = 0;

int Element::s_team = 0;



//store one of each element to get access to them through Element class.
//Element* Element::elementsType[]= ELEMENTS_ARRAY;
void Element::updatePos()
{
       updateID();
       for(int ie = 0 ; ie < m_height ; ie++)//mark the whole area of the element as marked
              for(int je = 0; je < m_width ; je++)
                     s_map[(m_y+ie)*s_mapWidth + m_x + je] = (unsigned long int)this;//m_ID;
       m_sprite.setPosition(Vector2f(s_mapOffsetX+m_x,s_mapOffsetY + m_y));
              
}

bool Element::isPlaceFree(int px,int py,int pw, int ph)
{
       bool placeFree = false;
       if(px > 0 && py > 0 && px + pw < s_mapWidth && py + pw < s_mapHeight)
       {
              placeFree = true;
              //test the whole area of the element to know if the element is at a free place
              for(int iu = 0 ; iu < ph ; iu++)
                     for(int ju = 0; ju < pw ; ju++)
                            if(s_map[(py+iu)*s_mapWidth + px + ju] != 0)
                                   placeFree = false;
       }
       return placeFree;
}

bool Element::placeAround(Element *fixed, Element *toPlaced,int *px, int *py)
{
       int uw = toPlaced->width();
       int uh = toPlaced->height();
       int fw = fixed->width();
       int fh = fixed->height();
       int fx = fixed->x();
       int fy = fixed->y();
       
       int i=0,j=-1,idir=1,jdir=0,phase=0,level=0;
       while(!Element::isPlaceFree(fx+i*uw,fy+j*uh,uw,uh))
       {
              if(phase == 0 && i*uw >= fw+level*uw)
              {   
                     phase++;
                     idir=0;
                     jdir=1;
              }
              if(phase == 1 && j*uh >= fh+level*uh)
              {   
                     phase++;
                     idir=-1;
                     jdir=0;
              }
              if(phase == 2 && i <= -(level+1))
              {   
                     phase++;
                     idir=0;
                     jdir=-1;
              }
              if(phase == 3 && j <= -(level+1))
              {   
                     phase++;
                     level++;
                     i=-(level+1);
                     j=i;
                     idir=1;
                     jdir=0;
                     phase=0;
              }
              j+=jdir;
              i+=idir;
       }
       *px=fx+i*uw;
       *py=fy+j*uh;
}
