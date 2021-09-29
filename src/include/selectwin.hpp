/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "preprocessor.hpp"
#include "base.hpp"
#include "fltk_extend.hpp"

#undef min
#undef max

#ifndef SELECTWIN_H
#define SELECTWIN_H

class SelectWin{
protected:
    struct pass{
        void* point;
        base* program;
        int floor;
        int layer;
        check_extend* button;

        pass(){
            floor=0;
            layer=0;
            program=NULL;
            button=NULL;
        }

    };

    struct close_pass{
        void* point;
        base* program;
        
        close_pass(){ 
            point = NULL;
            program = NULL;
        }
    };

    int m_layers;
    int floor;
    Fl_Double_Window* m_dlg;
    //std::vector<check_extend*> check_buttons;
    
    std::vector<pass> pass_data;
    close_pass pass_close;
    container* root; //very dodgy: look into?
    base* program;

    std::string win_name;
    
public:
    SelectWin(void* data,std::string win_name_in) : m_dlg(0), program((base*) data),win_name(win_name_in)                    
    {//note these implicit initialisations have to be in the same order as initialisation within the class
        m_layers = 0;
        floor=program->floor_choice->value()-1;
        root = &(program->root);
        
        
    }
    virtual ~SelectWin()
    {
        Fl::focus(program->win);
        delete m_dlg; //will delete all children too
    }
    
    virtual void delete_win()=0; // must be virtual to ensure we delete the whole derived object
        
    static void close_layer_dialogue(Fl_Widget* /*widget*/, void* data){
             
        close_pass* pass_close =(close_pass*) data;
        SelectWin* self = (SelectWin*) (pass_close->point);
        base* program = pass_close->program;
        program->draw->first_draw=1;
        program->draw->redraw();
        self->delete_win();      
    }
    
   virtual void effect(int, int,check_extend*)=0;
    
   static void layer_check_cb(Fl_Widget *widget, void *data_){
        
      check_extend* button = (check_extend*) widget;

      pass* data = (pass*) data_;
      SelectWin* self = (SelectWin*) (data->point);
      //container* root = data->root;
      base* program = data->program;
      int floor = data->floor;
      int layer = data->layer;

      self->effect(floor,layer,button);

      program->draw->redraw();
    
   }
    
   static void hide_all_cb(Fl_Widget*, void *data){

      std::vector<pass> *pass_data = static_cast<std::vector<pass>* >(data);
      base* program = NULL;

      for (auto x:*pass_data){
         SelectWin* self = (SelectWin*) (x.point);
         program = x.program;
         int floor = x.floor;
         int layer = x.layer;
         check_extend* button = x.button;
         button->value(0);
         self->effect(floor,layer,button);
      }

      program->draw->redraw();


   }

   static void show_all_cb(Fl_Widget*, void *data){

      std::vector<pass> *pass_data = static_cast<std::vector<pass>* >(data);
      base* program = NULL;

      for (auto x:*pass_data){
         SelectWin* self = (SelectWin*) (x.point);
         program = x.program;
         int floor = x.floor;
         int layer = x.layer;
         check_extend* button = x.button;
         button->value(1);
         self->effect(floor,layer,button);
      }

      program->draw->redraw();


   }

   virtual void generate()=0;

    void Create()
    {
        
        Fl_Color back_color = fl_rgb_color(247,247,247);
        Fl_Color button_color = fl_rgb_color(BUTTON_R,BUTTON_G,BUTTON_B);

        m_dlg  = new Fl_Double_Window(program->win->x()+0.5*program->win->w()-250,program->win->y()+0.5*program->win->h()-265,500,530,win_name.c_str());//"Layers");
        m_dlg->set_modal();
        m_dlg->begin();
        
        Fl_Group *gr = new Fl_Group(0,0,m_dlg->w(),m_dlg->h());
        gr->box(FL_FLAT_BOX);
        gr->color(back_color);
        
        Fl_Scroll* layer_scroll = new Fl_Scroll(5,5,490,475); //shouldnt be a memory leak as deleting m_dlg in destructor deletes all children
        layer_scroll->box(FL_FLAT_BOX);
        layer_scroll->color(back_color);
        gr->add(layer_scroll);
        
        generate();
        
        pass_close.point=this;
        pass_close.program=program;

        layer_scroll->end();

        button_extend* hide_all_button = new button_extend(20,495,80,25,"Hide all");
        hide_all_button->callback(hide_all_cb,&pass_data);
        hide_all_button->color(button_color);
        gr->add(hide_all_button);

        button_extend* show_all_button = new button_extend(110,495,80,25,"Show all");
        show_all_button->callback(show_all_cb,&pass_data);
        show_all_button->color(button_color);
        gr->add(show_all_button);

        button_extend* layer_dialogue_close = new button_extend(400,495,80,25,"Close");
        layer_dialogue_close->callback(close_layer_dialogue,&pass_close);
        layer_dialogue_close->color(button_color);
        gr->add(layer_dialogue_close);

        m_dlg->add(gr);

        m_dlg->callback(close_layer_dialogue,&pass_close);
        m_dlg->end();
        m_dlg->show();
        
    }  
    
};


class LayerWin: public SelectWin{

    void effect(int floor, int layer,check_extend* button){
        program->root.current->floor_plans[floor].layers[layer]=(int)button->value();
    }

    void generate(){

        const std::vector<plan>& floor_plans = root->current->floor_plans;
        int count=0;

        for (unsigned int i=0;i<floor_plans[floor].layers.size();i++){//we MUST push them all onto the vector FIRST
            
            pass temp;
            temp.floor=floor;
            temp.layer=i;
            temp.program=program;
            temp.point = this;
            temp.button = NULL;
            if (floor_plans[floor].layers_used.size()){
                if (floor_plans[floor].layers_used[i]){
                    pass_data.push_back(temp); //we MUST push them all back first
                }
            }
            else{
               pass_data.push_back(temp); //we MUST push them all back first
            }
        }
        
        for (unsigned int i=0;i<floor_plans[floor].layers.size();i++){                

            if (floor_plans[floor].layers_used.size()){
                if (floor_plans[floor].layers_used[i]){
                    
                    check_extend* button = new check_extend(10,10+count*20,460,20,floor_plans[floor].layers_name[i].c_str());
                    button->value(floor_plans[floor].layers[i]);                    
                    button->color(fl_rgb_color(95,95,110));
                    pass_data.at(count).button = button;
                    button->callback(layer_check_cb,&pass_data.at(count));
                    count++;
                  
                }
            }
            else{
                check_extend* button = new check_extend(10,10+count*20,460,20,floor_plans[floor].layers_name[i].c_str());
                button->value(floor_plans[floor].layers[i]);
                button->color(fl_rgb_color(95,95,110));
                pass_data.at(count).button = button;
                button->callback(layer_check_cb,&pass_data.at(count));
                count++;
                
            }
            
        }
   }

   void delete_win(){
      delete this;
   }

public:
   LayerWin(void* data) :SelectWin(data,std::string("Layers")){}
   ~LayerWin(){}

};

class BlockWin: public SelectWin{

   void effect(int floor, int layer,check_extend* button){
      program->root.current->floor_plans[floor].blocks_active[layer]=(int)button->value();
   }


   void generate(){
      std::vector<plan>& floor_plans = root->current->floor_plans;
      int count=0;
          
         for (unsigned int i=0;i<floor_plans[floor].blocks_active.size();i++){
            
            pass temp;
            temp.floor=floor;
            temp.layer=i;
            temp.program=program;
            temp.point = this;
            if (floor_plans[floor].blocks_used.size()){
                if (floor_plans[floor].blocks_used[i]){
                  pass_data.push_back(temp); //we MUST push them all back first
               }
            }
            else{
               pass_data.push_back(temp); //we MUST push them all back first
            }
         }

        for (unsigned int i=0;i<floor_plans[floor].blocks_active.size();i++){

            if (floor_plans[floor].blocks_used.size()){
                if (floor_plans[floor].blocks_used[i]){
                    
                    check_extend* button = new check_extend(10,10+count*20,460,20,floor_plans[floor].block_definitions[i].name.c_str());
                    button->value(floor_plans[floor].blocks_active[i]);                    
                    button->color(fl_rgb_color(95,95,110));
                    pass_data.at(count).button = button;

                    button->callback(layer_check_cb,&pass_data.at(count));
                    count++;
                    
                }
            }
            else{
                check_extend* button = new check_extend(10,10+count*20,460,20,floor_plans[floor].block_definitions[i].name.c_str());
                button->value(floor_plans[floor].blocks_active[i]);
                button->color(fl_rgb_color(95,95,110));
                pass_data.at(count).button = button;               

                button->callback(layer_check_cb,&pass_data.at(count));
                count++;
                
                
            }
            
        }
   }

   void delete_win(){
      delete this;
   }

public:
   BlockWin(void* data) :SelectWin(data,std::string("Blocks")){}
   ~BlockWin(){}

};


#endif //SELECTWIN_H