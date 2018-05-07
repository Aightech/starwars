#include "tower.hpp"


int Tower::findTarget(){
//find a reachable target around the tower and return its id, return -1 if no one is reachable
	for (int i=0; i<m_reach; i++){ 
		for (int j=0; j<m_reach; j++){
			if( ((Element *) s_map[(m_y+i)*s_mapWidth + m_x + j])->type() == UNIT_TYPE){ //TODO  SUPER_UNIT_TYPE 
				return ((Element *) s_map[(m_y+i)*s_mapWidth + m_x + j])->id(); //first unit find in the area is returned
			}
		}	
	}  
	return -1;

}

Request Tower::update()
{
       //create unit
       Request req={-1,0,0};
       updatePos();
       if(clock()-m_clock>CLOCKS_PER_SEC && m_test <5)
       {
              m_clock=clock();
              req.type=R_ATTACK;
              m_test++;
              
              Tower::findTarget(this,&req.val1,&req.val2);
            
              
       }
       return req;
}
