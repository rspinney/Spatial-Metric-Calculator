/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/draw_window.hpp"
#include "./include/base.hpp"

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max


int draw_window::intersect_test(double x1,double x2,double y1,double y2, double x3,double x4,double y3,double y4){//doesnt't need to be so fast
    
    int intersect=0;
    
    double x=((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    double y=((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    
    if ((y>=std::min(y1,y2))&&(y<=std::max(y1,y2))&&(x>=std::min(x1,x2))&&(x<=std::max(x1,x2))&&(y>=std::min(y3,y4))&&(y<=std::max(y3,y4))&&(x>=std::min(x3,x4))&&(x<=std::max(x3,x4))){
        intersect=1; 
    }    
    
    return intersect;
    
}

void draw_window::calculate_areas(std::vector<std::vector<vec> >* polygons){
    
    
    std::vector<double> temp_area(current->floor_index.size(),0);

    
    for (unsigned int i=0;i<polygons->size()-1;i++){ //we always have an additional dangling polygon for editing
        
        double sum=0;
        
        
        for (unsigned int ii=0;ii<(*polygons)[i].size();ii++){
            
            unsigned int next=ii+1;
            if (next>=(*polygons)[i].size()){
                next=0;
            }
            
            sum+=((*polygons)[i][ii].y+(*polygons)[i][next].y)*((*polygons)[i][next].x-(*polygons)[i][ii].x);
            
        }
        
        sum*=0.5;
        
        if (sum<0){ //cannot guarantee clockwise or counter clockwise
            sum*=-1.0;
        }
        
        int k=(*polygons)[i][0].floor;
        
        if ((k<static_cast<int>(temp_area.size()))&&(k>=0)){
            temp_area[k]+=sum;
        }
        
    }
    
    for (unsigned int i=0;i<temp_area.size();i++){
        if (i<current->floor_areas.size()){
            current->floor_areas[i]=temp_area[i];
        }
    }
    
}

int draw_window::test_in_triangle(double x, double y,triangle tri){
    double area,s,t;
    
    area=0.5*(-tri.y2*tri.x3 +  tri.y1*(-tri.x2+tri.x3)  +   tri.x1*(tri.y2-tri.y3)  +tri.x2*tri.y3 );
    
   
    
    if (area<0){
        area*=-1;
    }
    
    s = 1.0/(2.0*area)*(tri.y1*tri.x3 - tri.x1*tri.y3 + (tri.y3 - tri.y1)*x + (tri.x1 - tri.x3)*y);
    t = 1.0/(2.0*area)*(tri.x1*tri.y2 - tri.y1*tri.x2 + (tri.y1 - tri.y2)*x + (tri.x2 - tri.x1)*y);
    
    if ((s+t)<=1){
        if ((s<=1)&&(s>=0)&&(t>=0)&&(t<=1)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
    
}

bool draw_window::point_in_polygon(double x, double y,const std::vector<vec>& polygon) {
    
    int i, j, nvert = polygon.size();
    bool c = false;
    
    for(i = 0, j = nvert - 1; i < nvert; j = i++) { //loops over edges
        if (( ( (polygon[i].y) >= y) != (polygon[j].y >= y) ) &&(x <= (polygon[j].x - polygon[i].x) * (y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            
            c = !c;
        }
    }
    
    return c;
}


int draw_window::pnpoly(double testx, double testy,const std::vector<vec> &vert)
{
    int nvert=vert.size();
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vert[i].y>=testy) != (vert[j].y>=testy)) &&
            (testx <= (vert[j].x-vert[i].x) * (testy-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x) )
            c = !c;
    }
    return c;
}


void draw_window::ear_clipping(std::vector<vec> polyline,std::vector<triangle>* triangles){
    
    std::vector<vec>polyline_copy=polyline;
    
    triangles->clear();
    
    int old_size = polyline.size();
    
    while (static_cast<int>(polyline.size())>3){
        
        old_size=polyline.size();
        
        for (unsigned int i=0;i<polyline.size();i++){
            
            int i_mid,i_plus,i_minus;
            
            if (i==0){
                i_mid=i;
                i_plus=i+1;
                i_minus=polyline.size()-1;
            }
            else if (i==(polyline.size())-1){
                i_mid=i;
                i_plus=0;
                i_minus=i-1;
                
            }
            else{
                i_mid=i;
                i_plus=i+1;
                i_minus=i-1;
            }
            
            triangle temp;
            temp.x1=polyline[i_mid].x;
            temp.y1=polyline[i_mid].y;
            temp.x2=polyline[i_plus].x;
            temp.y2=polyline[i_plus].y;
            temp.x3=polyline[i_minus].x;
            temp.y3=polyline[i_minus].y;
            
            
            
            std::vector<vec> temp_poly;
            temp_poly.clear();
            vec temp_vec;
            
            temp_vec.x=temp.x1;
            temp_vec.y=temp.y1;
            temp_poly.push_back(temp_vec);
            temp_vec.x=temp.x2;
            temp_vec.y=temp.y2;
            temp_poly.push_back(temp_vec);
            temp_vec.x=temp.x3;
            temp_vec.y=temp.y3;
            temp_poly.push_back(temp_vec);
            
            int found_ear=1;
            
            double cx,cy;
            
            cx=(1.0/3.0)*(temp.x1+temp.x2+temp.x3);
            cy=(1.0/3.0)*(temp.y1+temp.y2+temp.y3);
            
            
            
           
            if (!pnpoly(cx,cy,polyline)){
                found_ear=0;
            }
           
            
            if (found_ear){
                
                for (int j=0;j<static_cast<int>(polyline.size());j++){
                    if ((j!=i_mid)&&(j!=i_plus)&&(j!=i_minus)){
                        
                        
                        
                        
                        if (pnpoly(polyline[j].x,polyline[j].y,temp_poly)){
                            found_ear=0;
                        }
                        
                  
                        
                    }
                }
                
            }
            
            if (found_ear){
                polyline.erase(polyline.begin()+i_mid);
                triangles->push_back(temp);
                break;
            }
            
        }
        
        if (static_cast<int>(polyline.size())==old_size){ //something is not working! //leave loop
            break;
        }
        
    }
    
    if (static_cast<int>(polyline.size())==3){
        
        triangle temp;
        temp.x1=polyline[0].x;
        temp.y1=polyline[0].y;
        
        temp.x2=polyline[1].x;
        temp.y2=polyline[1].y;
        
        temp.x3=polyline[2].x;
        temp.y3=polyline[2].y;
        
        triangles->push_back(temp);
        
    }
    
}


void draw_window::draw_areas(double depth){
    
    if ((program->three_d_choice->value()<3)||(!root->floor_ceil)){
        
        if (program->three_d_choice->value()){
            depth=minz-z_gap+0.01;
        }
           
        
        if (root->bow){
            glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.33);
        }
        else{
            glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.66);
        }
   
        if (root->bow){
            glBlendFunc(GL_ZERO,GL_SRC_COLOR);
        }
        else{
            glBlendFunc(GL_ONE,GL_ONE);
        }
        glEnable( GL_BLEND );
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
  

        double xx,yy;
        
        if (snapon){
            xx=snapx;
            yy=snapy;
        }
        else{
            xx= (  static_cast<double> (x_mouse-program->win->x()-this->x() )  /  (  static_cast<double>(this->w())  )  )   *  (maxx-minx)  +  minx;
            yy= (  static_cast<double> (y_mouse-program->win->y()-this->y() )  /  (  static_cast<double>(this->h())  )  )   *  (miny-maxy)  +  maxy;
        }

        int current_selected = 0;

        glBegin(GL_TRIANGLES);

        for (unsigned int j=0;j<current->floorarea_triangles.size();j++){
        
            if ((all_flrs)||((program->floor_choice->value()-1)==(current->floorarea_polygons[j][0].floor))){
                
                if ((selector==14)&&(selected==static_cast<int>(j))){
                    
                    
                    if (root->bow){
                        glColor4f((255.0/256.0),(255.0/256.0),(0.0/256.0),0.33);
                    }
                    else{
                        glColor4f((75.0/256.0),(75.0/256.0),(0.0/256.0),0.33);
                    }
                }
                else{
                    if (root->bow){
                        if ( current->floorarea_type[j]==0){
                            glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.33);
                        }
                        else{
                            glColor4f((256.0/256.0),(220.0/256.0),(220.0/256.0),0.33);
                        }
                    }
                    else{
                        if ( current->floorarea_type[j]==0){
                            glColor4f((0.0/256.0),(50.0/256.0),(0.0/256.0),0.33);
                        }
                        else{
                            glColor4f((50.0/256.0),(0.0/256.0),(0.0/256.0),0.33);
                        }
                    }
                }
                

                if ((selector==14)&&(group_selection_list.size()>0)){

                    if (static_cast<int>(j) == group_selection_list[current_selected]){
                    
                        if (root->bow){
                            glColor4f((255.0/256.0),(255.0/256.0),(0.0/256.0),0.33);
                        }
                        else{
                            glColor4f((75.0/256.0),(75.0/256.0),(0.0/256.0),0.33);
                        }

                        if (current_selected < static_cast<int>(group_selection_list.size())-1){
                            current_selected++;
                        }
                    
                    }
                }


                int fl = current->floorarea_polygons[j][0].floor;

                double dd =current->floor_index[fl];

                for (unsigned int i=0;i<current->floorarea_triangles[j].size();i++){

                    glVertex3d(current->floorarea_triangles[j][i].x1,current->floorarea_triangles[j][i].y1,dd);
                    glVertex3d(current->floorarea_triangles[j][i].x2,current->floorarea_triangles[j][i].y2,dd);
                    glVertex3d(current->floorarea_triangles[j][i].x3,current->floorarea_triangles[j][i].y3,dd);
                }
                

            }
        }

        glEnd();

        glColor3f((255.0/256.0),(0.0/256.0),(0.0/256.0));
        glLineWidth(2.0);
        
        for (unsigned int i=0;i<(current->floorarea_polygons.size()-1);i++){
            
            if ((all_flrs)||((program->floor_choice->value()-1)==(current->floorarea_polygons[i][0].floor))){
                
                int fl = current->floorarea_polygons[i][0].floor;
                
                double dd =current->floor_index[fl];
                
                glBegin(GL_LINE_LOOP);
                for (unsigned int j=0;j<current->floorarea_polygons[i].size();j++){
                    glVertex3d(current->floorarea_polygons[i][j].x,current->floorarea_polygons[i][j].y,dd);
                   
                    
                }
                glEnd();
            }
            
        }
       
    

        glBegin(GL_LINE_STRIP);
        int ii=current->floorarea_polygons.size()-1;
        for (unsigned int j=0;j<current->floorarea_polygons[ii].size();j++){
            glVertex3d(current->floorarea_polygons[ii][j].x,current->floorarea_polygons[ii][j].y,depth+1.1);
        }
        if (mark_locations_choice->value()==14){
            glVertex3d(xx,yy,depth);
        }
        glEnd();
       
        
    }

    
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    
}