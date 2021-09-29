/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/


#include "./include/base.hpp"
#include "./include/splash.hpp"

#undef min
#undef max

base::base(){
        
    MEMENTO_DEPTH = DEFAULT_MEMENTO_DEPTH;
    root.re_container(this);       
    finished_console_output = 1;
    continue_console_output = 0;    
}

void base::build_and_run(){  

	root.undo_test=0;
   
    Fl::visual(FL_DOUBLE|FL_INDEX);

	text = new text_output;
    data_text = new text_output;

    buff = new Fl_Text_Buffer();
    data_buff = new Fl_Text_Buffer();

    std::vector<packaged_base_ptr> read_location_options(9);
    for (unsigned int i=0;i<read_location_options.size();i++){
        read_location_options[i].ptr = this;
        read_location_options[i].option = i;
    }

#if THEME == 0
    Fl::scheme("plastic"); 
    Fl_Color menu_color = FL_BLACK;//plastic
#elif THEME == 1
    Fl::scheme("gtk+"); 
    Fl_Color menu_color = fl_rgb_color(210,210,210);//GTK
#endif

	Fl_Menu_Item items[] = {
        {"File",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color}, //0
        {"&Open Project",FL_COMMAND+'o', read_binary_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"&Save Project",FL_COMMAND|'s', save_binary_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Import pre-existing structures",0,0,0,FL_SUBMENU|FL_MENU_DIVIDER,0,0,FL_NORMAL_SIZE,menu_color},
            {"Import vertices", 0, open_points_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {"Import adjacency matrix", 0, open_matrix_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {"Import adjacency list", 0, open_adjlist_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {"Import d&xf floorplan",FL_COMMAND|'x', open_dxf_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},            
            {"Import location markers",0, 0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import origin markers",0, open_locations_cb_static,&(read_location_options[0]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 1 markers",0, open_locations_cb_static,&(read_location_options[1]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 2 markers",0, open_locations_cb_static,&(read_location_options[2]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 3 markers",0, open_locations_cb_static,&(read_location_options[3]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 4 markers",0, open_locations_cb_static,&(read_location_options[4]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 5 markers",0, open_locations_cb_static,&(read_location_options[5]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 6 markers",0, open_locations_cb_static,&(read_location_options[6]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 7 markers",0, open_locations_cb_static,&(read_location_options[7]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Import destination 8 markers",0, open_locations_cb_static,&(read_location_options[8]),0,0,0,FL_NORMAL_SIZE,menu_color},
                    {0,0,0,0,0,0,0,0,0},
            {"Import barriers",0, 0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},
                    {"High opaque",0, open_walls_standing_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"High transparent",0, open_walls_standing_transparent_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Low opaque",0, open_walls_sitting_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Low transparent",0, open_walls_sitting_transparent_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0},
        {"&Quit", FL_COMMAND|'q', main_callback_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {0,0,0,0,0,0,0,0,0},
        {"Edit",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},
        {"Empty entire project slot",FL_COMMAND|'e',clear_building_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Select all edit objects on floor  ",FL_COMMAND+'a',select_all_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},///////
        {"Edit constructs (mouse-free)",FL_ALT+'e',link_win_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Undo",FL_COMMAND|'z',undo_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Redo",FL_COMMAND|'y',redo_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Adjust Undo stack length",0,memento_depth_win_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"&Cancel ongoing computation",FL_ALT+'c', kill_thread_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Check graph consistency",FL_COMMAND+'j',map_consistency_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Generate constructs",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},                
            {"Floorplans to barriers",0, 0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},
                    {"High opaque",0, lines_to_walls_standing_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"High transparent",0, lines_to_walls_standing_transparent_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Low opaque",0, lines_to_walls_sitting_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {"Low transparent",0, lines_to_walls_sitting_transparent_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
                    {0,0,0,0,0,0,0,0,0},
            {"Floorplan to spatial graph",0, floorplan_to_map_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {"Floorplan to floorareas",0,floorplan_to_areas_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {"Floorareas to spatial graph (Mesh)",0, areas_to_points_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
            {0,0,0,0,0,0,0,0,0},
        {"Floorplan dialogue",FL_ALT+'f',floorplan_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Metric dialogue",FL_ALT+'m',desk_metric_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {0,0,0,0,0,0,0,0,0},
        {"View",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},        
        {"Zoom in",FL_COMMAND+']',draw_window::mouseless_zoom_in_entry,this,0,0,0,FL_NORMAL_SIZE,menu_color}, 
        {"Zoom out",FL_COMMAND+'[',draw_window::mouseless_zoom_out_entry,this,0,0,0,FL_NORMAL_SIZE,menu_color},        
        {"Dark mode  ",FL_COMMAND|'i',colour_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},        
        {"Re-centre/reset view",FL_ALT+'r',reset_view_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Print/export screen",FL_ALT+'p',save_screen_static,this,FL_MENU_DIVIDER,0,0,FL_NORMAL_SIZE,menu_color},
        {"Render cursor graphically",0,cursor_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color}, 
        {"Edit/Display centre point/axis of rotation ",FL_ALT+'x',show_rot_axis_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color}, 
        {"Display barriers as transparent (3D)",FL_ALT+'t',transparent_walls_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Display origins as 3D desk models",0,toggle_deskmodel_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},    //60 deskmodel
        {"Lock view to floor (free view)",0,floor_lock_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color}, //61 - lock floor
        {"Visualise floor/ceiling (free view)",0,floor_ceil_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE|FL_MENU_DIVIDER,0,0,FL_NORMAL_SIZE,menu_color},    //62 -floor ceil 
        {"Toggle Fullscreen",FL_COMMAND|'k',go_fullscreen_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Show console/Info window",FL_ALT+'y',info_bar_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Show side panel",FL_ALT+'h',panel_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,0,menu_color},
        {0,0,0,0,0,0,0,0,0},
        {"Options",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,menu_color},        
        {"Floorplan",FL_COMMAND+'f',show_floorplan_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color}, //  68  show floorplan
        {"Floorplan colours",FL_ALT+'u',plan_colors_menu_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color}, // 69 floorplan_colors_index
        {"Grid",FL_COMMAND+'g',show_grid_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Scale/Orientation guide",FL_ALT+'s',show_scale_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Display route",FL_COMMAND+'r',show_route_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},//   72     - show route
        {"Display metric",FL_COMMAND+'d',show_metric_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},//  73     - show metric
        {"Vertex indices",FL_ALT+'v',points_label_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Edge indices",FL_ALT+'g',segments_label_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Location marker indices",FL_ALT+'l',locations_label_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Colour scale",FL_COMMAND+'u',color_scale_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},        
        {"Limit edge visualisation",FL_COMMAND+'l',limit_edge_cb_static,this,FL_MENU_TOGGLE|FL_MENU_DIVIDER,0,0,FL_NORMAL_SIZE,menu_color}, //    78      - limit edge
        {"Auto-connect spatial graph vertices",FL_ALT+'j',toggle_autoconnect_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Snap to point priority",FL_ALT+'i',snap_to_point_cb_static,this,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Move mode/Alt lock",FL_COMMAND+'m',set_move_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},//   81      move mode
        {"Constuct barriers as transparent/floor areas as negative  ",FL_COMMAND+'t',mark_transparent_walls_cb_static,this,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,menu_color},
        {"Set guide angle",FL_ALT+'a',show_ang_win_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {0,0,0,0,0,0,0,0,0},
        {"Help", 0, 0, 0, FL_SUBMENU ,0,0,FL_NORMAL_SIZE,menu_color},
        {"About",0,about_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {"Quick guide",0,guide_cb_static,this,0,0,0,FL_NORMAL_SIZE,menu_color},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0}
    };
        
    Fl::lock(); //multi threading            
    
    menu_height =25;
    int w=1200,h=828+menu_height-20;
    
    root.w=w;
    root.h=h;
    
    
    Fl::background(237,237,237);

    Fl_Color highlight_color = fl_rgb_color(220,220,225);
    Fl_Color dark_back = fl_rgb_color(70,70,70);
    Fl_Color dark_text = fl_rgb_color(235,235,235);
    Fl_Color dark_selection = fl_rgb_color(160,160,160);
    Fl_Color border_color = fl_rgb_color(120,120,120);
    Fl_Color back_color = fl_rgb_color(247,247,247);
    Fl_Color outer_box_color = fl_rgb_color(242,242,243);
    Fl_Color inner_box_color = fl_rgb_color(236,236,236);
    Fl_Color button_color = fl_rgb_color(BUTTON_R,BUTTON_G,BUTTON_B);


    //MAIN WINDOW
    
    win= new double_win_extend(0.5*(Fl::w()-w),0.5*(Fl::h()-h),w,h, "Spatial Metric Calculator",this);
    win->begin();
    
    Fl_Group *w_group = new Fl_Group(0,0,win->w(),win->h());
    w_group->box(FL_FLAT_BOX);
    w_group->color(back_color);

    

    win->callback(main_callback_static,this);
    
    win->size_range(w,h,0,0,1,1,0);
    
    win->box(FL_FLAT_BOX);
    win->color(back_color);

    Fl_Group *par = new Fl_Group(0,0,win->w(),win->h());
    par->box(FL_FLAT_BOX);
    par->color(back_color);
    

    menu = new menu_extend(0, 0, win->w(),menu_height);
    menu->menu(items);
    menu->box(FL_FLAT_BOX);
    menu->Fl_Menu_::box(FL_FLAT_BOX);
    menu->down_box(FL_FLAT_BOX);
    menu->global();

    menu->color(fl_rgb_color(MENU_R,MENU_G,MENU_B),fl_rgb_color(MENU_R,MENU_G,MENU_B));
    

    int temp =-140;
    ooo = new Fl_Group(10,menu_height-sysoffset,win->w()-20,win->h()+temp+sysoffset);

    ooo->box(FL_FLAT_BOX);
    ooo->color(back_color);
    
    win->add(menu);    

    oo = new tabs_extend(10,menu_height-sysoffset,win->w()-20,win->h()-temp+sysoffset);
    oo->color(button_color);
    oo->callback(main_tab_callback_static,this); 
    oo->when(FL_WHEN_CHANGED);

    o1 = new Fl_Group(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height, "Main");  
    {  
        o1->box(FL_FLAT_BOX);
        o1->color(back_color);
        
        draw_box = new Fl_Box(9,21+menu_height-sysoffset,o1->w()-255-15+2,oo->h()-12-menu_height+2);
        draw_box->box(FL_BORDER_FRAME);
        draw_box->color(border_color);
        draw_box->hide();  

        draw = new draw_window(10,22+menu_height-sysoffset,o1->w()-255-15,oo->h()-12-menu_height,0,&root,this);

        p1 = new Fl_Group(draw->x()+draw->w(),22+menu_height-sysoffset,260,o1->h()-2,0);

        Fl_Box* divider = new Fl_Box(p1->x(),p1->y(),5,p1->h(),0);
        divider->box(FL_NO_BOX);
        
        p1->add(divider);
        
        Fl_Group* content = new Fl_Group(p1->x()+5,p1->y(),p1->w()-5+10,p1->h(),0);
        content->box(FL_FLAT_BOX);
        content->color(back_color);
        
        building_choice = new choice_extend(content->x()+content->w()-190-3-2,content->y()+8,50,20,"Project:");
        building_choice->color(button_color,highlight_color);

        for (int i=0;i<NUM_BUILDINGS;i++){
            std::stringstream s;
            s<<i+1;
            building_choice->add(s.str().c_str());
        }

        building_choice->value(0);
        building_choice->callback(building_choice_cb_static,this);
        building_choice->tooltip("Project choice. Up to 4 projects can be worked on/compared concurrently.");
        
        
        three_d_choice = new choice_extend (content->x()+content->w()-130+40-5,content->y()+8,75,20,"View:");
        three_d_choice->add("2D");
        three_d_choice->add("3D Orthographic");
        three_d_choice->add("3D Perspective");
        three_d_choice->add("Free view");
        three_d_choice->value(0);
        three_d_choice->callback((Fl_Callback*) three_d_cb_static,this);
        three_d_choice->tooltip("Choose between 2D, 3D and first person views. Editing in 2D only.");
        three_d_choice->color(button_color,highlight_color);  


        Fl_Box* floor_box = new Fl_Box(content->x()+5,content->y()+65-15-5,content->w()-15,60-2,"Floors");
        floor_box->box(FL_GTK_DOWN_BOX);
        floor_box->labelsize(12);
        floor_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        floor_box->color(outer_box_color);

        button_extend *floor_button = new  button_extend(content->x()+content->w()-250,content->y()+70-15-5,90,20,"Floorplans");
        floor_button->tooltip("Edit floorplans.");
        floor_button->callback((Fl_Callback*) floorplan_cb_button_static,this);
        floor_button->color(button_color);

        floor_choice = new choice_extend (content->x()+content->w()-110,content->y()+70-15-5,90,20,"Floor:");
        floor_choice->add("All");
        floor_choice->add("0");
        floor_choice->value(0);
        floor_choice->callback((Fl_Callback*) floor_choice_cb_static,this); 
        floor_choice->tooltip("Choose which floor to display/edit");
        floor_choice->color(button_color,highlight_color);

        add_floor = new button_extend (content->x()+content->w()-250+10,content->y()+100-3-15-5,25,20,"+");
        add_floor->callback((Fl_Callback*) add_floor_cb_static,this);
        add_floor->tooltip("Add a distinct floor level.");
        add_floor->deactivate();
        add_floor->color(button_color);

        remove_floor = new button_extend (content->x()+content->w()-215+10,content->y()+100-3-15-5,25,20,"-");
        remove_floor->callback((Fl_Callback*) remove_floor_cb_static,this);
        remove_floor->tooltip("Remove a distinct floor level.");
        remove_floor->deactivate();
        remove_floor->color(button_color);

        
        

        zfloor = new undo_blocked_Fl_Float_Input(content->x()+content->w()-110,content->y()+100-3-15-5,55,20,"Height:");
        zfloor->callback((Fl_Callback*) zfloor_cb_static,this);
        zfloor->tooltip("Adjust height of current floor level.");
        zfloor->box(FL_GTK_DOWN_BOX);
        zfloor->color(fl_rgb_color(255,255,255));


        {
            std::stringstream s;
            s<<std::fixed<<std::setprecision(2)<<0.0;
            zfloor->value(s.str().c_str());
        }
        zfloor->deactivate();
        zfloor->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
        zfloor->selection_color(dark_selection);
       
        zfloor_ok = new button_ex (content->x()+content->w()-28-20,content->y()+100-3-15-5,28,20,"OK",this);
        zfloor_ok->callback((Fl_Callback*) zfloor_cb_static,this);
        zfloor_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        zfloor_ok->deactivate();
        zfloor_ok->color(button_color);


        Fl_Box* line_point_box = new Fl_Box(content->x()+5,content->y()+145-8+5-15-7,content->w()-5-10,50,"Spatial graph display options");
        line_point_box->box(FL_GTK_DOWN_BOX);
        line_point_box->labelsize(12);
        line_point_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        line_point_box->color(outer_box_color);
        
        line_thickness = new simple_counter_extend(content->x()+15,content->y()+155-8+5-15-7,0.5*(content->w()-30)-5-15,20,"Edge thickness");
        line_thickness->step(1.0);
        line_thickness->value(1.0);
        line_thickness->maximum(10);
        line_thickness->minimum(0);
        line_thickness->callback((Fl_Callback*) redraw_static,this);
        line_thickness->tooltip("Thickness in pixels");
        line_thickness->color(button_color);
        line_thickness->box(FL_GTK_DOWN_BOX);
        
        
        point_size = new counter_extend(content->x()+0.5*(content->w()-30)+22-5-15,content->y()+155-8+5-15-7,0.5*(content->w()-30)-5+15,20,"Vertex size");
        point_size->step(0.01,0.05);
        point_size->value(0.2);
        point_size->maximum(10.0);
        point_size->minimum(0.00);
        point_size->callback((Fl_Callback*) redraw_static,this);
        point_size->tooltip("Size in window units (metres)");
        point_size->color(button_color);
        point_size->box(FL_GTK_DOWN_BOX);
        
        Fl_Box* location_box = new Fl_Box(content->x()+5,content->y()+145-8+5-15+61,content->w()-15,110+8+3,"Display constructs");
        location_box->box(FL_GTK_DOWN_BOX);
        location_box->color(outer_box_color);
        location_box->labelsize(12);
        location_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));

        Fl_Box* locations_box = new Fl_Box(content->x()+12,content->y()+150+32+5-6+61,content->w()-15-10-4,65+3+3-9,"Locations");
        locations_box->box(FL_GTK_DOWN_BOX);
        locations_box->color(inner_box_color);
        
        locations_box->labelsize(10);
        locations_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));

        
        show_map = new check_extend (content->x()+10,content->y()+210-70+5-15+61,114,20,"Spatial graph");
        show_map->color(button_color);
        show_floor_areas = new check_extend (content->x()+125,content->y()+210-70+5-15+61,104,20,"Floor areas");
        show_floor_areas->color(button_color);
        show_walls_sitting = new check_extend (content->x()+10,content->y()+230-70+5-15+61,113,20,"Barriers (low)");
        show_walls_sitting->color(button_color);
        show_walls_standing = new check_extend (content->x()+125,content->y()+230-70+5-15+61,122,20,"Barriers (high)");
        show_walls_standing->color(button_color);
        
        show_walls_sitting->callback((Fl_Callback*) redraw_show_static,this);
        show_walls_standing->callback((Fl_Callback*) redraw_show_static,this);
        show_floor_areas->callback((Fl_Callback*) redraw_show_static,this);
        show_map->callback((Fl_Callback*) redraw_show_static,this);

        show_locations = std::vector<check_extend*>(9);

        show_locations[0] = new check_extend (content->x()+15,content->y()+150+32+5+5+6-15+61,70,20,"Origins");
        show_locations[1] = new check_extend (content->x()+90+4,content->y()+150+32+5+5+6-15+61,80,20,"Dest. 1");
        show_locations[2] = new check_extend (content->x()+165+8,content->y()+150+32+5+5+6-15+61,70,20,"Dest. 2");
        show_locations[3] = new check_extend (content->x()+15,content->y()+170+32+5+5+6-15+61,70,20,"Dest. 3");
        show_locations[4] = new check_extend (content->x()+90+4,content->y()+170+32+5+5+6-15+61,70,20,"Dest. 4");
        show_locations[5] = new check_extend (content->x()+165+8,content->y()+170+32+5+5+6-15+61,70,20,"Dest. 5");
        show_locations[6] = new check_extend (content->x()+15,content->y()+190+32+5+5+6-15+61,70,20,"Dest. 6");
        show_locations[7] = new check_extend (content->x()+90+4,content->y()+190+32+5+5+6-15+61,70,20,"Dest. 7");
        show_locations[8] = new check_extend (content->x()+165+8,content->y()+190+32+5+5+6-15+61,70,20,"Dest. 8");

        for (int i=0;i<9;i++){
            show_locations[i]->color(button_color);
        }

        show_map->value(1);
        
        for (unsigned int i=0;i<show_locations.size();i++){
            show_locations[i]->callback((Fl_Callback*) redraw_show_static,this);
        }
        
        Fl_Box* edit_box = new Fl_Box(content->x()+5,content->y()+360+5-50+20-5-2,content->w()-5-10,135-12,"Construct editing");
        edit_box->box(FL_GTK_DOWN_BOX);
        edit_box->labelsize(12);
        edit_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        edit_box->color(outer_box_color);


        mark_locations_choice = new choice_extend (content->x()+content->w()-160-30,content->y()+420-30+5-50-5-4,170,20,"Edit:");
        mark_locations_choice->add("-----");
        
        mark_locations_choice->add("Origin markers");
        mark_locations_choice->add("Destination 1 markers");
        mark_locations_choice->add("Destination 2 markers");
        mark_locations_choice->add("Destination 3 markers");
        mark_locations_choice->add("Destination 4 markers");
        mark_locations_choice->add("Destination 5 markers");
        mark_locations_choice->add("Destination 6 markers");
        mark_locations_choice->add("Destination 7 markers");
        mark_locations_choice->add("Destination 8 markers");
        mark_locations_choice->add("Barriers: low");
        mark_locations_choice->add("Barriers: high");
        mark_locations_choice->add("Spatial graph: vertices");
        mark_locations_choice->add("Spatial graph: edges");
        mark_locations_choice->add("Floor areas");
        mark_locations_choice->value(0);
        mark_locations_choice->callback((Fl_Callback*) redraw_mark_static,this);
        mark_locations_choice->tooltip("Main edit option. Constructs can only be added/edited/removed when appropriately selected here.");
        mark_locations_choice->color(button_color,highlight_color);
        
        
        location_label = new undo_blocked_Fl_Input (content->x()+content->w()-140,content->y()+450-30+5-50-5-4-5,85,20,"Location label:");
        location_label->callback(location_label_cb_static,this);
        location_label->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
        location_label->selection_color(dark_selection);

        location_label->box(FL_GTK_DOWN_BOX);
        location_label->color(fl_rgb_color(255,255,255));
       
        location_label_in = new button_ex (content->x()+content->w()-48,content->y()+450-30+5-50-5-4-5,28,20,"OK",this);
        location_label_in->callback((Fl_Callback*) location_label_cb_static,this);
        location_label_in->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        location_label_in->color(button_color);

        location_label->deactivate();
        location_label_in->deactivate();
        location_label->tooltip("Add label to construct (if relevant).");
        location_label_in->tooltip("Add label to construct (if relevant).");
            
        overwrite_location_point = new undo_blocked_Fl_Int_Input  (content->x()+content->w()-140,content->y()+480-30+5-50-5-4-10,85,20,"Vertex:");
        overwrite_location_point->callback(point_in_cb_static,this);
        overwrite_location_point->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
        overwrite_location_point->selection_color(dark_selection);

        overwrite_location_point->box(FL_GTK_DOWN_BOX);
        overwrite_location_point->color(fl_rgb_color(255,255,255));


        point_in = new button_ex (content->x()+content->w()-48,content->y()+480-30+5-50-5-4-10,28,20,"OK",this);
        point_in->callback((Fl_Callback*) point_in_cb_static,this);
        point_in->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        point_in->color(button_color);

        overwrite_location_point->deactivate();
        point_in->deactivate();
        overwrite_location_point->tooltip("Override default associated vertex (if relevant).");
        point_in->tooltip("Override default associated vertex (if relevant).");

        Fl_Box* delete_box = new Fl_Box(content->x()+5+7,content->y()+510-30+5-50-5-4-12,content->w()-15-14,30,"Delete");
        delete_box->box(FL_GTK_DOWN_BOX);
        delete_box->color(inner_box_color);
        delete_box->color(fl_rgb_color(200,200,200));
        delete_box->labelsize(10);
        delete_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        delete_box->color(inner_box_color);
            
        clear_selected_location = new button_extend(content->x()+5+7+5,content->y()+510-30+5-50+5-5-4-12,70,20,"Selected");
        clear_selected_location->callback((Fl_Callback*) clear_selected_location_cb_static,this);
        clear_selected_location->deactivate();
        clear_selected_location->tooltip("Clear selected construct.");
        clear_selected_location->shortcut(FL_Delete);
        clear_selected_location->color(button_color);
        
        clear_last_location = new button_extend (content->x()+5+7+5+70+8,content->y()+510-30+5-50+5-5-4-12,70,20,"Last");
        clear_last_location->callback((Fl_Callback*) clear_last_location_cb_static,this);
        clear_last_location->deactivate();
        clear_last_location->tooltip("Clear most recently added construct.");
        clear_last_location->shortcut(FL_BackSpace);
        clear_last_location->color(button_color);
            
        clear_locations= new button_extend (content->x()+178-5,content->y()+510-30+5-50+5-5-4-12,70,20,"All");
        clear_locations->callback((Fl_Callback*) clear_locations_cb_static,this);
        clear_locations->deactivate();
        clear_locations->tooltip("Clear all constructs on all floors.");
        clear_locations->color(button_color);
       

        Fl_Box* metric_box = new Fl_Box(content->x()+5,content->y()+360+5-50+20+150-5-12,content->w()-5-8,95,"Metrics, routes & visibility");
        metric_box->box(FL_GTK_DOWN_BOX);
        metric_box->labelsize(12);
        metric_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        metric_box->color(outer_box_color);
            
        show_visibility = new choice_extend (metric_box->x()+125+2,metric_box->y()+10-5+2,115,20,"Show visibility:");
     
        show_visibility->add("----");
        show_visibility->add("Low (all barriers)");
        show_visibility->add("Low (opaque barriers)");
        show_visibility->add("High (all barriers)");
        show_visibility->add("High (opaque barriers)");
        show_visibility->value(0);
        show_visibility->callback(visibility_select_cb_static,this);
        show_visibility->tooltip("Visibility illustration options.");
        show_visibility->color(button_color,highlight_color);

        button_extend *clear_metrics = new button_extend(metric_box->x()+12-2,metric_box->y()+35-5+2,107+2,20,"Clear");
        clear_metrics->callback((Fl_Callback*) clear_metrics_cb_static,this);
        clear_metrics->tooltip("Clear graph structures including metrics.");
        clear_metrics->color(button_color);
        
        button_extend *show_route_button = new button_extend(metric_box->x()+135-2,metric_box->y()+35-5+2,107+2,20,"Show Route");
        show_route_button->callback((Fl_Callback*) show_route_menu_cb_static,this);
        show_route_button->tooltip("Open route display dialogue.");
        show_route_button->color(button_color);

        desk_metric_button = new button_extend (content->x()+content->w()-245-3-2,metric_box->y()+60-5+2,222+6+4,30,"Metric dialogue");
        desk_metric_button->callback((Fl_Callback*) desk_metric_cb_static,this);
        desk_metric_button->tooltip("Open metric display/calculation dialogue.");
        desk_metric_button->color(button_color);

        warning_visibility = new Fl_Box(FL_BORDER_FRAME,content->x()+5,content->y()+610-30+5-6,content->w()-15,30,"Warning: visibility changed");
        warning_visibility->color(fl_rgb_color(20,20,20));
        warning_visibility->hide();
        
        warning_map = new Fl_Box(FL_BORDER_FRAME,content->x()+5,content->y()+650-30+5-6,content->w()-15,30,"Warning: spatial graph changed");
        warning_map->hide();
        warning_map->color(fl_rgb_color(20,20,20));
        
        calc_message = new Fl_Box(FL_BORDER_FRAME,content->x()+5,content->y()+630-30-6,content->w()-15,30,"Calculation in progress...");
        calc_message->color(fl_rgb_color(20,20,20));

        content->resizable(0);
       
        content->end();
        p1->add(content);
        
        p1->end();
       
       
    }
    o1->end();
    o1->resizable(draw);
    o2 = new Fl_Group(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height, "Data");
    {
        o2->box(FL_FLAT_BOX);
        o2->color(back_color);
        
        out_data_box = new Fl_Box(9,21+menu_height-sysoffset,o2->w()-255-15+2,oo->h()-12-menu_height+2);
        out_data_box->box(FL_BORDER_FRAME);
        out_data_box->color(border_color);
        out_data_box->hide(); 

        out_data = new reporting_text_display(10,22+menu_height-sysoffset,o2->w()-255-15,oo->h()-12-menu_height,0,this);
        
        out_data->textfont(4);

        out_data->textcolor(dark_text);
        out_data->color(dark_back);
        out_data->selection_color(dark_selection);

        data_text->buff=data_buff;
        data_text->display=out_data;

        
        
        clear_data_output();
        push_data_output("No data selected.\n");
        out_data->redraw();
        o2->resizable(out_data);
        
        p2 = new Fl_Group(out_data->x()+out_data->w(),22+menu_height-sysoffset,260,o2->h()-2+sysoffset,0);
        
        Fl_Box* divider = new Fl_Box(p2->x(),p2->y(),5,p2->h(),0);
        divider->box(FL_NO_BOX);

        p2->add(divider);
        p2->color(back_color);      

        Fl_Group* content = new Fl_Group(p2->x()+5,p2->y(),p2->w()-5+10,p2->h(),0);
        content->box(FL_FLAT_BOX);
        content->color(fl_rgb_color(210,210,210));
        content->color(back_color);

        Fl_Box* data_box = new Fl_Box(out_data->x()+out_data->w()+10,out_data->y()+20,o2->w()-out_data->w()-20,130,"Data");
        data_box->box(FL_GTK_DOWN_BOX);
        data_box->labelsize(12);
        data_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        data_box->color(outer_box_color);
        
        building_data_choice = new choice_extend (out_data->x()+out_data->w()+120,out_data->y()+30,130,20,"Project: ");
        for (int i=0;i<NUM_BUILDINGS;i++){
            std::stringstream s;
            s<<i+1;
            building_data_choice->add(s.str().c_str());
        }

        building_data_choice->value(0);
        building_data_choice->callback(building_data_choice_cb_static,this);
        building_data_choice->tooltip("Choose project to display data for.");
        building_data_choice->color(button_color,highlight_color);
        
        data_choice = new choice_extend (out_data->x()+out_data->w()+120,out_data->y()+60,130,20,"Display data: ");
        data_choice->add("------");
        data_choice->add("Vertices");       
        data_choice->add("Edges (Adjacency list)");
        
        data_choice->add("Origin marker info");
        data_choice->add("Destination 1 marker info");
        data_choice->add("Destination 2 marker info");
        data_choice->add("Destination 3 marker info");
        data_choice->add("Destination 4 marker info");
        data_choice->add("Destination 5 marker info");
        data_choice->add("Destination 6 marker info");
        data_choice->add("Destination 7 marker info");
        data_choice->add("Destination 8 marker info");
        data_choice->add("Visibility info");
        data_choice->add("Metrics");
        data_choice->add("Floor and Global info");        
        data_choice->value(0);
        data_choice->callback((Fl_Callback*) redraw_data_static,this);
        data_choice->tooltip("Choose data to output.");
        data_choice->color(button_color,highlight_color);
        
        data_format = new choice_extend (out_data->x()+out_data->w()+120,out_data->y()+90,130,20,"Include: ");
        data_format->add("Header + Data");
        data_format->add("Header only");
        data_format->add("Data only");
        data_format->value(0);
        data_format->callback((Fl_Callback*) redraw_data_static,this);
        data_format->tooltip("Choose to output meta-data though a header, data, or both.");
        data_format->color(button_color,highlight_color);
        
        save = new button_extend(out_data->x()+out_data->w()+120,out_data->y()+120,130,20,"Export");
        save->callback((Fl_Callback*) save_data_static,this);
        save->tooltip("Save current data buffer to text file.\n Any file type (specify .csv .txt .dat etc. if desired)");
        save->color(button_color);
        
        
        content->end();
        content->resizable(0);
        p2->end();
        
        
    }
    o2->end();
    o2->resizable(out_data);
    o3 = new Fl_Group(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height, "Plot");
    {
        
        o3->box(FL_FLAT_BOX);
        o3->color(back_color);

        graph_box = new Fl_Box(9,21+menu_height-sysoffset,o2->w()-255-15+2,oo->h()-12-menu_height+2);
        graph_box->box(FL_BORDER_FRAME);
        graph_box->color(border_color);
        graph_box->hide();  

        plot = new plot_window(10,22+menu_height-sysoffset,o3->w()-255-15,oo->h()-12-menu_height,0,&root,this);


        p3 = new Fl_Group(draw->x()+draw->w(),22+menu_height-sysoffset,260,o3->h()-2,0);
        p3->color(back_color);
        
        Fl_Box* divider = new Fl_Box(p3->x(),p3->y(),5,p3->h(),0);
        divider->box(FL_NO_BOX);


        p3->add(divider);
        
        Fl_Group* content = new Fl_Group(p3->x()+5,p3->y(),p3->w()+5+10,p3->h(),0);
        content->box(FL_FLAT_BOX);
        content->color(back_color);
        
        
        Fl_Box* options_box = new Fl_Box(plot->x()+plot->w()+10,plot->y()+20,o3->w()-plot->w()-20,115,"Options");
        options_box->box(FL_GTK_DOWN_BOX);
        options_box->labelsize(12);
        options_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        options_box->color(outer_box_color);

        plot_choice = new choice_extend (plot->x()+plot->w()+130,plot->y()+10+20,120,20,"Plot: ");
        plot_choice->add("Scatter Plot");
        plot_choice->add("Histogram");
        plot_choice->value(0);
        plot_choice->callback((Fl_Callback*) plot_choice_cb_static,this);// redraw_and_hist);
        plot_choice->tooltip("Choose graph type");
        plot_choice->color(button_color,highlight_color);

        hist_group = new Fl_Group(content->x(),content->y()+40,content->w(),content->h()-50);
        
        
        hist_interp_choice = new choice_extend(plot->x()+plot->w()+130,plot->y()+40-10+20+5,120,20,"Interpolation");
        hist_interp_choice->add("Yes");
        hist_interp_choice->add("No");
        hist_interp_choice->add("Yes (only)");
        hist_interp_choice->value(0);
        hist_interp_choice->callback(plot_choice_cb_static,this);
        hist_interp_choice->deactivate();
        hist_interp_choice->tooltip("Choose to display interpolation (histogram only)");
        hist_interp_choice->color(button_color,highlight_color);
        
        yaxis_choice = new choice_extend(plot->x()+plot->w()+130,plot->y()+70-20+20+10,120,20,"Y-values");
        yaxis_choice->add("N");
        yaxis_choice->add("Probability");
        yaxis_choice->add("Probability density");
        yaxis_choice->value(0);
        yaxis_choice->callback(plot_choice_cb_static,this);
        yaxis_choice->deactivate();
        yaxis_choice->tooltip("Choose units: number of basis objects in bin or probability/probability density (histogram only)");
        yaxis_choice->color(button_color,highlight_color);

        show_regression = new choice_extend(plot->x()+plot->w()+130,plot->y()+100-30+20+15,120,20,"Regression");
        show_regression->add("No");
        show_regression->add("Yes");
        show_regression->value(0);
        show_regression->callback(plot_choice_cb_static,this);
        show_regression->tooltip("Display simple linear regression fit (2 variables only)");
        show_regression->color(button_color,highlight_color);
        
        for (unsigned int i=0;i<4;i++){ // number of variables
            Fl_Box* temp = NULL;
            hist_box.push_back(temp);
            switch (i){
                case 0:
                    hist_box[i] = new Fl_Box(plot->x()+plot->w()+10,plot->y()+(i*120)+165,o3->w()-plot->w()-20,90,"Variable 1");
                    break;
                case 1:
                    hist_box[i] = new Fl_Box(plot->x()+plot->w()+10,plot->y()+(i*120)+165,o3->w()-plot->w()-20,90,"Variable 2");
                    break;
                case 2:
                    hist_box[i] = new Fl_Box(plot->x()+plot->w()+10,plot->y()+(i*120)+165,o3->w()-plot->w()-20,90,"Variable 3");
                    break;
                case 3:
                    hist_box[i] = new Fl_Box(plot->x()+plot->w()+10,plot->y()+(i*120)+165,o3->w()-plot->w()-20,90,"Variable 4");
                    break;
            }
            
            hist_box[i]->box(FL_GTK_DOWN_BOX);
            hist_box[i]->color(outer_box_color);
    
            hist_box[i]->labelsize(12);
            hist_box[i]->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        }

        hist_building.clear();
        hist_metric.clear();
        bin_count.clear();

        
        for (unsigned int i=0;i<4;i++){
            choice_extend* tempa = NULL;
            hist_building.push_back(tempa);
            hist_building[i] = new choice_extend(hist_box[i]->x()+130,hist_box[i]->y()+10,110,20,"Project");
            hist_building[i]->color(button_color,highlight_color);
            for (int ii=0;ii<NUM_BUILDINGS;ii++){
                std::stringstream s;
                s<<ii+1;
                hist_building[i]->add(s.str().c_str());
            }
           
            hist_building[i]->value(0);
            hist_building[i]->callback(rebuild_hist_input_static,this);
            if (i>1){
                hist_building[i]->deactivate();
            }
            std::stringstream s;
            s<<"Choose project for metric selection for variable "<<i+1;
            hist_building[i]->tooltip("Choose project for metric selection");


            choice_extend* tempb = NULL;
            hist_metric.push_back(tempb);
            hist_metric[i] = new choice_extend(hist_box[i]->x()+130,hist_box[i]->y()+35,110,20,"Metric");
            hist_metric[i]->add("-----");
            hist_metric[i]->value(0);
            hist_metric[i]->callback(plot_choice_cb_static,this);
            if (i>1){
                hist_metric[i]->deactivate();
            }
            s.str("");
            s<<"Choose metric variable";// "<<i+1;
            hist_metric[i]->tooltip("Choose metric variable");
            hist_metric[i]->color(button_color,highlight_color);

            counter_extend* tempc = NULL;
            bin_count.push_back(tempc);
            bin_count[i] = new counter_extend(hist_box[i]->x()+130,hist_box[i]->y()+60,110,20,"Number of bins");
            bin_count[i]->step(1,5);
            bin_count[i]->value(20);
            bin_count[i]->maximum(1000);
            bin_count[i]->minimum(4);
            bin_count[i]->align(Fl_Align(FL_ALIGN_LEFT));
            bin_count[i]->callback(plot_choice_cb_static,this);
            bin_count[i]->deactivate();
            s.str("");
            s<<"Specify number of bins for variable";// "<<i+1<<" (histogram only)";
            bin_count[i]->tooltip("Specify number of bins for variable");
            bin_count[i]->color(button_color);
        }
        
        hist_group->end();
        
        content->end();
        content->resizable(0);
        p3->add(content);
        
        p3->end();

    }
    o3->end();
    o3->resizable(plot);
    
    oo->add(o1);
    oo->add(o2);
    oo->add(o3);
    oo->end();
    oo->resizable(o1);

    ooo->add(oo);
    ooo->end();


    
    out_read = new cursor_text(10,win->h()-120,win->w()-20,110,0,this);
    out_read->textfont(4);
    out_read->wrap_mode(3,0);

    out_read->selection_color(dark_selection);
    out_read->textcolor(dark_text);
    out_read->color(dark_back);

    out_read_box = new Fl_Box(9,win->h()-121,win->w()-18,112,0);
    out_read_box->box(FL_BORDER_FRAME);
    out_read_box->color(border_color);
    out_read_box->hide();    

    text->buff=buff;
    text->display=out_read;
        
    {
        root.calculating=FREE;        
        root.draw_floorplan=1;
        root.snap_to_point=1;
        root.draw_grid=1;
        root.draw_route=1;
        root.transparent_walls=0;
        root.draw_points_label=0;
        root.draw_segments_label=0;
        root.color_scale=0;
        root.show_scale=1;
        root.draw_metric=0;        
        root.autoconnect = 0;
    }
      
    push_time();
    push_output("Program started.\n");
    push_time();
    push_output("Loading resources... ");

    out_read->redraw();
  
    win->resizable(ooo);
    win->end();
    win->hide();


    //END MAIN WINDOW

    //FLOORPLAN_WIN

    floorplan_win = new win_extend2(win->x()+0.5*win->w()-130,win->y()+0.5*win->h()-95,250,190+30,"Floorplan options");//350

    Fl_Group *fw_group = new Fl_Group(0,0,floorplan_win->w(),floorplan_win->h());
    fw_group->box(FL_FLAT_BOX);
    fw_group->color(outer_box_color);

    floorplan_win->callback(floorplan_win_cb,this);
    
    clear_dxf_button = new button_extend(35,10,80,20,"Clear");
    clear_dxf_button->tooltip("Clear floorplan form selected floor.");
    clear_dxf_button->callback(clear_dxf_cb_static,this);
    clear_dxf_button->color(button_color);

    load_dxf_button = new button_extend(135,10,80,20,"Load");
    load_dxf_button->tooltip("Load external dxf file into selected floor.");
    load_dxf_button->callback(open_dxf_cb_static,this);
    load_dxf_button->color(button_color);

    xshift = new undo_blocked_Fl_Float_Input(85,10+30,90,20,"Shift x");
    xshift->callback((Fl_Callback*) shiftx_cb_static,this);
    xshift->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    xshift->tooltip("Shift current floor floorplan in x direction.");
    xshift->value("0.00");
    xshift->selection_color(dark_selection);

    xshift->box(FL_GTK_DOWN_BOX);
    xshift->color(fl_rgb_color(255,255,255));

    xshift_ok = new button_ex(185,10+30,30,20,"OK",this);
    xshift_ok->callback((Fl_Callback*) shiftx_cb_static,this);
    xshift_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    xshift_ok->tooltip("Shift current floor floorplan in x direction.");
    xshift_ok->color(button_color);
    
    yshift = new undo_blocked_Fl_Float_Input(85,40+30,90,20,"Shift y");
    yshift->callback((Fl_Callback*) shifty_cb_static,this);
    yshift->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    yshift->tooltip("Shift current floor floorplan in y direction.");
    yshift->value("0.00");
    yshift->selection_color(dark_selection);

    yshift->box(FL_GTK_DOWN_BOX);
    yshift->color(fl_rgb_color(255,255,255));

    yshift_ok = new button_ex(185,40+30,30,20,"OK",this);
    yshift_ok->callback((Fl_Callback*) shifty_cb_static,this);
    yshift_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    yshift_ok->tooltip("Shift current floor floorplan in y direction.");
    yshift_ok->color(button_color);
    
    scale_floorplan = new undo_blocked_Fl_Float_Input(85,70+30,90,20,"Scale\n floorplan");
    scale_floorplan->value("1.00");
    scale_floorplan->tooltip("Resize floorplan by given factor.");
    scale_floorplan->callback((Fl_Callback*) scale_floorplan_cb_static,this);
    scale_floorplan->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    
    scale_floorplan->box(FL_GTK_DOWN_BOX);
    scale_floorplan->color(fl_rgb_color(255,255,255));

    scale_floorplan_ok = new button_extend(185,70+30,30,20,"OK");
    scale_floorplan_ok->callback((Fl_Callback*) scale_floorplan_cb_static,this);
    scale_floorplan_ok->tooltip("Resize floorplan by given factor.");
    scale_floorplan_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    scale_floorplan_ok->color(button_color);
    scale_floorplan->selection_color(dark_selection);

    plan_colors_button = new check_extend (floorplan_win->w()-220,105+30,200,20,"Display floorplan colours");
    plan_colors_button->callback(plan_colors_cb_static,this);
    plan_colors_button->value(0);
    plan_colors_button->do_callback();
    plan_colors_button->tooltip("Display colours as specified in the dxf file.\nThis has no functional consequence.");
    plan_colors_button->color(button_color);

    splines_button = new check_extend (floorplan_win->w()-220,125+30,200,20,"Include splines");
    splines_button->callback(splines_cb_static,this);
    splines_button->value(1);
    splines_button->do_callback();
    splines_button->tooltip("Display and utilise splines from the dxf file.\nThese tend to disproportionately negatively affect performance.");
    splines_button->color(button_color);

    blocks_button = new button_extend(35,155+30,80,20,"Blocks");
    blocks_button->callback(BlockCB_static,this);
    blocks_button->tooltip("Open block dialogue for current floorplan.");
    blocks_button->color(button_color);


    layers_button = new button_extend(135,155+30,80,20,"Layers");
    layers_button->callback(LayerCB_static,this);
    layers_button->tooltip("Open layer dialogue for current floorplan.");
    layers_button->color(button_color);

    

    clear_dxf_button->deactivate();
    load_dxf_button->deactivate();
    xshift->deactivate();
    yshift->deactivate();
    xshift_ok->deactivate();
    yshift_ok->deactivate();
    scale_floorplan->deactivate();
    scale_floorplan_ok->deactivate();
    layers_button->deactivate();
    blocks_button->deactivate();

    fw_group->end();
    floorplan_win->end();

    floorplan_win->hide();
    

    //END FLOORPLAN_WIN


    //CHOOSE UNITS WIN
    
    units_win= new Fl_Window(win->x()+0.5*win->w()-170,win->y()+0.5*win->h()-100,340,150,"Confirm units");
    Fl_Group *un_g = new Fl_Group(0,0,units_win->w(),units_win->h());
    un_g->box(FL_FLAT_BOX);
    un_g->color(back_color);
    Fl_Box *units_box = new Fl_Box(0,0,units_win->w(),80,"DXF file provided no units.\nConfirm floor plan units to proceed.\nWill be converted to program units (metres).");
    units_box->align(FL_ALIGN_CENTER);
    unit_choice = new choice_extend(110,75,160,20,"Units:");
    unit_choice->add("Metres (M)");
    unit_choice->add("Centimetres (cm)");
    unit_choice->add("Millimetres (mm)");
    unit_choice->add("Kilometers (km)");
    unit_choice->add("Microns");
    unit_choice->add("Inches");
    unit_choice->add("Feet");
    unit_choice->add("Yards");
    unit_choice->add("Miles");
    unit_choice->value(2);
    unit_choice->color(button_color,highlight_color);
    
    button_extend* unit_select_ok_button = new button_extend(170+20,110,80,20,"OK");
    unit_select_ok_button->callback(units_ok_static,this);
    unit_select_ok_button->color(button_color);
    button_extend* unit_select_cancel_button = new button_extend(170-100,110,80,20,"Cancel");
    unit_select_cancel_button->callback(units_cancel_static,this);
    unit_select_cancel_button->color(button_color);

    units_win->end();
    units_win->hide();
    units_win->set_modal();
    
    //END CHOOSE UNITS WIN


    
    //MESH WINDOW
    {
        int dy = 45;
    
        mesh_win = new Fl_Window(win->x()+0.5*win->w()-200,win->y()+0.5*win->h()-75-25,500,545+50+15+dy,"Mesh graph generation");//350
        Fl_Group *mesh_g = new Fl_Group(0,0,mesh_win->w(),mesh_win->h());
        mesh_g->box(FL_FLAT_BOX);
        mesh_g->color(back_color);
            
        Fl_Box* mesh_box = new Fl_Box(0,30,500,210,"Proceed with mesh generation?\n\n Proceeding will remove the existing graph structures\n and associated constructs on this floor.\n It will then fill regions that lie in marked positive floor areas\n on this floor, but *not* negative areas, with vertices.\nEdges will be connected using barriers selected below.\n\n WARNING: this can be *VERY* computationally intensive.\n It is recommended you save your work and \nexperiment with lower densities first.\n\nFor a smoother experience and for tractable graph measures \nedge and turn based metrics will be disabled, the display\n width of edges will be set to 0, and edge visualisation\n will be limited. All can be reset view/options/display options.");
        mesh_win->add(mesh_box);


        Fl_Box* mesh_vertex_box = new Fl_Box(40,285,mesh_win->w()-80,40,"Vertices");
        mesh_vertex_box->box(FL_GTK_DOWN_BOX);
        mesh_vertex_box->labelsize(12);
        mesh_vertex_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        mesh_vertex_box->color(outer_box_color);

        lattice_choice = new choice_extend(120,295,100,20,"Lattice");
        lattice_choice->add("Square");
        lattice_choice->add("Triangular");
        lattice_choice->value(0);
        lattice_choice->tooltip("Vertices are placed in either a square or triangular lattice arrangement.");
        lattice_choice->color(button_color,highlight_color);
        lattice_choice->callback(lattice_choice_cb_static,this);

        mesh_density = new undo_blocked_Fl_Float_Input(250+100,295,80,20,"Vertex spacing");
        mesh_density->tooltip("Distance between added spatial graph vertices, in metres, in each of the x and y directions.");
        mesh_density->selection_color(dark_selection);
        mesh_density->box(FL_GTK_DOWN_BOX);
        mesh_density->color(fl_rgb_color(255,255,255));
        {
            std::stringstream s;
            s<<"1.00";
            mesh_density->value(s.str().c_str()); 
        }

        Fl_Box* mesh_barrier_box = new Fl_Box(40,310+35,mesh_win->w()-80,145,"Barriers");
        mesh_barrier_box->box(FL_GTK_DOWN_BOX);
        mesh_barrier_box->labelsize(12);
        mesh_barrier_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        mesh_barrier_box->color(outer_box_color);

        use_stand_walls_opaque = new check_extend(60,315+35,380,25,"Use opaque high barriers?");
        use_stand_walls_opaque->value(1);
        use_stand_walls_opaque->tooltip("Selected barrier type will prevent edges being associated between spatial graph vertices if they intersect the direct line between those vertices.");
        use_stand_walls_opaque->color(button_color);

        use_stand_walls_transparent = new check_extend(60,340+35,380,25,"Use transparent high barriers?");
        use_stand_walls_transparent->value(1);
        use_stand_walls_transparent->tooltip("Selected barrier type will prevent edges being associated between spatial graph vertices if they intersect the direct line between those vertices.");
        use_stand_walls_transparent->color(button_color);

        use_sit_walls_opaque = new check_extend(60,365+35,390,25,"Use opaque low barriers?");
        use_sit_walls_opaque->value(1);
        use_sit_walls_opaque->tooltip("Selected barrier type will prevent edges being associated between spatial graph vertices if they intersect the direct line between those vertices.");
        use_sit_walls_opaque->color(button_color);

        use_sit_walls_transparent = new check_extend(60,390+35,380,25,"Use transparent low barriers?");
        use_sit_walls_transparent->value(1);
        use_sit_walls_transparent->tooltip("Selected barrier type will prevent edges being associated between spatial graph vertices if they intersect the direct line between those vertices.");
        use_sit_walls_transparent->color(button_color);


        detect_boundaries = new check_extend(60,415+35,380,35,"Detect edges of floor areas \nand use as non-traversable boundaries?");
        detect_boundaries->value(1);
        detect_boundaries->tooltip("When checked boundaries of negative floor areas, and the parts of positive floor areas which form the boundary of the union of all positive floor areas, are treated as barriers to movement and thus prevent association of edges between vertices.");
        detect_boundaries->color(button_color);

        Fl_Box* mesh_edge_box = new Fl_Box(40,465+dy,mesh_win->w()-80,75,"Edges");
        mesh_edge_box->box(FL_GTK_DOWN_BOX);
        mesh_edge_box->labelsize(12);
        mesh_edge_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        mesh_edge_box->color(outer_box_color);


        movement_vis_choice = new choice_extend(190,475+dy,180,20,"Graph type");
        movement_vis_choice->add("Movement");
        movement_vis_choice->add("Visibility");
        movement_vis_choice->value(0);
        movement_vis_choice->tooltip("Movement graphs do not form edges betwen vertices if the same line can be traversed with multiple smaller edges.\nI.e. a line of vertices for movement graphs only have edges between neighbouring vertices.\nVisbility graphs have edges between all vertices if they are not blocked.");
        movement_vis_choice->color(button_color,highlight_color);

        limit_mesh_edge = new check_extend(55,510+dy,145,25,"Limit edge length");
        limit_mesh_edge->value(0);
        limit_mesh_edge->tooltip("Set a maximum allowed distance between vertices for an edge to connect them.");
        limit_mesh_edge->callback((Fl_Callback*) limit_mesh_edge_cb_static,this);
        limit_mesh_edge->color(button_color);

        mesh_edge_max = new Fl_Float_Input(385,510+dy,60,20,"Maximum edge length");
        mesh_edge_max->value(0);
        mesh_edge_max->tooltip("Set a maximum allowed distance between vertices for an edge to connect them.");
        mesh_edge_max->selection_color(dark_selection);
        mesh_edge_max->box(FL_GTK_DOWN_BOX);
        mesh_edge_max->color(fl_rgb_color(255,255,255));

        {
            std::stringstream s;
            s<<"1.00";
            mesh_edge_max->value(s.str().c_str());
        }
        mesh_edge_max->deactivate();


        button_extend* mesh_ok = new button_extend(130,560+dy,80,30,"OK");
        mesh_ok->callback((Fl_Callback*) areas_to_points_ok_cb_static, this);
        mesh_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);//|FL_WHEN_NOT_CHANGED
        mesh_ok->color(button_color);
        button_extend* mesh_cancel = new button_extend(290,560+dy,80,30,"Cancel");
        mesh_cancel->callback((Fl_Callback*)areas_to_points_cancel_cb_static, this);
        mesh_cancel->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);//|FL_WHEN_NOT_CHANGED
        mesh_cancel->color(button_color);

        mesh_win->end();
        mesh_win->hide();
        mesh_win->set_modal();
    }

    //END MESH WINDOW

    //ABOUT WINDOW
    
    about_win = new Fl_Window(win->x()+0.5*win->w()-165,win->y()+0.5*win->h()-75,330,250,"About");
    
    Fl_Group *ab_g = new Fl_Group(0,0,about_win->w(),about_win->h());
    ab_g->box(FL_FLAT_BOX);
    ab_g->color(back_color);

    std::stringstream s_about;
    s_about<<"Spatial Metric Calculator v1.4\n\n"<<"An application for constructing\n"<<"and visualising spatially embedded\n"<<"undirected graphs, and the\n"<<"calculation of graph metrics.\n\n"<<"Copyright (C), 2014-2021, Richard Spinney.\n\n"<<"GUI with FLTK "<<FL_MAJOR_VERSION<<"."<<FL_MINOR_VERSION<<"."<<FL_PATCH_VERSION<<std::endl<<"http://www.fltk.org/";

    std::cout<<"\n          Spatial Metric Calculator v1.4\n\n"<<"          An application for constructing\n"<<"          and visualising spatially embedded\n"<<"          undirected graphs, and the\n"<<"          calculation of graph metrics.\n\n"<<"          Copyright (C), 2014-2021, Richard Spinney.\n\n"<<"          GUI with FLTK "<<FL_MAJOR_VERSION<<"."<<FL_MINOR_VERSION<<"."<<FL_PATCH_VERSION<<std::endl<<"          http://www.fltk.org/\n";

    std::string string_about = s_about.str();    
    Fl_Box* about_box = new Fl_Box(0,0,330,200,string_about.c_str());
   
    
    button_extend* about_ok = new button_extend(0.5*(about_win->w()-70),205,70,30,"OK");
    about_ok->callback((Fl_Callback*) about_ok_cb_static, this);
    about_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);//|FL_WHEN_NOT_CHANGED
    about_ok->color(button_color);
    about_win->add(about_box);
    about_win->add(about_ok);
    about_win->end();
    about_win->hide();
    about_win->set_modal();
    
    //END ABOUT WINDOW

       //CALC WINDOW
    
    calc_win = new Fl_Window(win->x()+0.5*win->w()-150,win->y()+0.5*win->h()-75,300,150,"Warning");
    Fl_Group *calc_g = new Fl_Group(0,0,calc_win->w(),calc_win->h());
    calc_g->box(FL_FLAT_BOX);
    calc_g->color(back_color);
    std::stringstream s_calc;


    s_calc<<"Program is processing...\n\nPlease wait before editing/computing.";


    std::string string_calc= s_calc.str();    
    Fl_Box* calc_box = new Fl_Box(0,0,300,100,string_calc.c_str());
    
    
    button_extend* calc_ok = new button_extend(120,100,50,30,"OK");
    calc_ok->callback((Fl_Callback*) calc_ok_cb_static, this);
    calc_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    calc_ok->color(button_color);
    calc_win->add(calc_box);
    calc_win->add(calc_ok);
    calc_win->end();
    calc_win->hide();
    calc_win->set_modal();
    
    //END CALC WINDOW
    
    
    //ADD LINK WINDOW
    
    add_link_win = new win_extend_edit(win->x()+0.5*win->w()-165,win->y()+0.5*win->h()-155,330,312,"Edit constructs");
    Fl_Group *ad_g = new Fl_Group(0,0,add_link_win->w(),add_link_win->h());
    ad_g->box(FL_FLAT_BOX);
    ad_g->color(back_color);

    Fl_Box* vertex_box = new Fl_Box(20,20,add_link_win->w()-40,60,"Vertices");
    vertex_box->box(FL_GTK_DOWN_BOX);
    vertex_box->color(outer_box_color);
    vertex_box->labelsize(12);
    vertex_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));

    x_vert = new Fl_Float_Input(60-25+4,25,68,20,"x");
    x_vert->value("0.0");
    x_vert->selection_color(dark_selection);
    x_vert->box(FL_GTK_DOWN_BOX);
    x_vert->color(fl_rgb_color(255,255,255));

    y_vert = new Fl_Float_Input(130-10+2,25,68,20,"y");
    y_vert->value("0.0");
    y_vert->selection_color(dark_selection);
    y_vert->box(FL_GTK_DOWN_BOX);
    y_vert->color(fl_rgb_color(255,255,255));
    
    button_extend *add_vertex_button = new button_extend(110+20+80-10,25,80+20,20,"Add");
    add_vertex_button->callback(add_vertex_cb_static,this);
    add_vertex_button->tooltip("Create a vertex, on the current floor, at the entered x and y positions.");
    add_vertex_button->color(button_color);

    remove_vert = new Fl_Int_Input(110,55,80,20,"Vertex");
    remove_vert->value("0");
    remove_vert->selection_color(dark_selection);
    remove_vert->box(FL_GTK_DOWN_BOX);
    remove_vert->color(fl_rgb_color(255,255,255));

    
    button_extend *remove_vertex_button = new button_extend(110+20+80-10,55,80+20,20,"Remove");
    remove_vertex_button->callback(remove_vertex_cb_static,this);
    remove_vertex_button->tooltip("Remove an existing vertex given by the entered index.");
    remove_vertex_button->color(button_color);

    Fl_Box* link_box = new Fl_Box(20,65+35,add_link_win->w()-40,60,"Edges");
    link_box->box(FL_GTK_DOWN_BOX);
    link_box->labelsize(12);
    link_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    link_box->color(outer_box_color);
    
    start_add_link = new Fl_Int_Input(110,70+35,80,20,"Vertex 1");
    start_add_link->value("0");
    start_add_link->selection_color(dark_selection);
    start_add_link->box(FL_GTK_DOWN_BOX);
    start_add_link->color(fl_rgb_color(255,255,255));

    end_add_link = new Fl_Int_Input(110,100+35,80,20,"Vertex 2");
    end_add_link->value("0");
    end_add_link->selection_color(dark_selection);
    end_add_link->box(FL_GTK_DOWN_BOX);
    end_add_link->color(fl_rgb_color(255,255,255));

    button_extend *add_link_button = new button_extend(110+20+80-10,70+35,80+20,20,"Add");
    add_link_button->callback(add_link_cb_static,this);
    add_link_button->tooltip("Create an edge between the two entered vertices.");
    add_link_button->color(button_color);
    button_extend *remove_link_button = new button_extend(110+20+80-10,100+35,80+20,20,"Remove");
    remove_link_button->callback(remove_link_cb_static,this);
    remove_link_button->tooltip("Remove an existing edge between the two entered vertices.");
    remove_link_button->color(button_color);

    Fl_Box* loc_edit_box = new Fl_Box(20,180,add_link_win->w()-40,90,"Location markers");
    loc_edit_box->box(FL_GTK_DOWN_BOX);
    loc_edit_box->color(outer_box_color);
    loc_edit_box->labelsize(12);
    loc_edit_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));

    loc_choice = new choice_extend(210-10,185,100,20,"Location:");
    loc_choice->add("Origins");
    loc_choice->add("Destination 1");
    loc_choice->add("Destination 2");
    loc_choice->add("Destination 3");
    loc_choice->add("Destination 4");
    loc_choice->add("Destination 5");
    loc_choice->add("Destination 6");
    loc_choice->add("Destination 7");
    loc_choice->add("Destination 8");
    loc_choice->value(0);
    loc_choice->color(button_color,highlight_color);


    x_loc = new Fl_Float_Input(60-25+4,215,68,20,"x");
    x_loc->value("0.0");
    x_loc->selection_color(dark_selection);
    x_loc->box(FL_GTK_DOWN_BOX);
    x_loc->color(fl_rgb_color(255,255,255));

    y_loc = new Fl_Float_Input(130-10+2,215,68,20,"y");
    y_loc->value("0.0");
    y_loc->selection_color(dark_selection);
    y_loc->box(FL_GTK_DOWN_BOX);
    y_loc->color(fl_rgb_color(255,255,255));
    

    button_extend *add_loc_button = new button_extend(110+20+80-10,215,80+20,20,"Add");
    add_loc_button->callback(add_loc_cb_static,this);
    add_loc_button->tooltip("Create a location marker, on the current floor, at the entered x and y positions.");
    add_loc_button->color(button_color);

    loc_remove_index = new Fl_Int_Input(110,215+30,80,20,"Index");
    loc_remove_index->value("0");
    loc_remove_index->selection_color(dark_selection);
    loc_remove_index->box(FL_GTK_DOWN_BOX);
    loc_remove_index->color(fl_rgb_color(255,255,255));

    button_extend *remove_loc_button = new button_extend(110+20+80-10,215+30,80+20,20,"Remove");
    remove_loc_button->callback(remove_loc_cb_static,this);
    remove_loc_button->tooltip("Remove an existing location marker with the given index.");
    remove_loc_button->color(button_color);


    button_extend *cancel_link_button = new button_extend(110+20+80-10,135+35+60+20+30,80+20,20,"Close");
    cancel_link_button->callback(close_link_win_static,this);
    cancel_link_button->color(button_color);
    add_link_win->end();
    add_link_win->hide();
    add_link_win->set_modal();
    
    //END ADD LINK WINDOW

    //ADD MEMENTO DEPTH WINDOW
    
    memento_depth_win = new Fl_Window(win->x()+0.5*win->w()-140,win->y()+0.5*win->h()-45,280,90,"Adjust maximum Undo stack length");
    Fl_Group *mem_gr = new Fl_Group(0,0,memento_depth_win->w(),memento_depth_win->h());
    mem_gr->box(FL_FLAT_BOX);
    mem_gr->color(back_color);
    memento_depth_input = new Fl_Int_Input(160,20,80,20,"New maximum \nUndo depth");
    memento_depth_input->tooltip("Higher numbers give a longer Undo/Redo history, but may use more memory. If memory use is excessive, try reducing the value.");
    memento_depth_input->selection_color(dark_selection);
    memento_depth_input->box(FL_GTK_DOWN_BOX);
    memento_depth_input->color(fl_rgb_color(255,255,255));

    button_extend *enter_memento_depth_button = new button_extend(160,60,80,20,"OK");
    enter_memento_depth_button->tooltip("Higher numbers give a longer Undo/Redo history, but may use more memory. If memory use is excessive, try reducing the value.");
    enter_memento_depth_button->callback(change_memento_depth_cb_static,this);
    enter_memento_depth_button->color(button_color);
    button_extend *cancel_memento_depth_button = new button_extend(40,60,80,20,"Cancel");
    cancel_memento_depth_button->callback(close_memento_depth_win_static,this);
    cancel_memento_depth_button->color(button_color);
    memento_depth_win->end();
    memento_depth_win->hide();
    memento_depth_win->set_modal();
    
    //END ADD LINK WINDOW


    //SET ANGLE GUIDE WINDOW
    
    set_ang_win = new win_extend_angle(win->x()+0.5*win->w()-120,win->y()+0.5*win->h()-80,240,70,"Set guide angle");
    Fl_Group *aw_group = new Fl_Group(0,0,set_ang_win->w(),set_ang_win->h());
    aw_group->box(FL_FLAT_BOX);
    aw_group->color(outer_box_color);

    ang_input = new Fl_Float_Input(110,10,80,20,"Angle");
    ang_input->value("0.0");
    ang_input->callback(set_ang_cb_static,this);
    ang_input->when(FL_WHEN_ENTER_KEY_ALWAYS);
    ang_input->selection_color(dark_selection);
    ang_input->box(FL_GTK_DOWN_BOX);
    ang_input->color(fl_rgb_color(255,255,255));

    set_ang_win->add(ang_input);
    button_extend *set_ang_button = new button_extend(30,40,80,20,"Set");
    set_ang_button->callback(set_ang_cb_static,this);
    set_ang_button->when(FL_WHEN_ENTER_KEY_ALWAYS|FL_WHEN_RELEASE_ALWAYS);
    set_ang_button->color(button_color);
    set_ang_win->add(set_ang_button);
    button_extend *cancel_ang_button = new button_extend(130,40,80,20,"Cancel");
    cancel_ang_button->callback(close_ang_cb_static,this);
    cancel_ang_button->when(FL_WHEN_ENTER_KEY_ALWAYS|FL_WHEN_RELEASE_ALWAYS);
    cancel_ang_button->color(button_color);
    set_ang_win->add(cancel_ang_button);
    set_ang_win->end();
    set_ang_win->hide();
    set_ang_win->set_modal();
    
    //END ANGLE GUIDE WINDOW

    
    //ROUTE DISPLAY WINDOW
    
    route_display = new Fl_Window(win->x()+0.5*win->w()-135,win->y()+0.5*win->h()-75,270,150,"Display routes");
    Fl_Group *rd_group = new Fl_Group(0,0,route_display->w(),route_display->h());
    rd_group->box(FL_FLAT_BOX);
    rd_group->color(outer_box_color);
    
    route_type_c = new choice_extend (100-10,15,150,20,"Route\nmeasure");
    route_type_c->add("least (x,y) graph distance");
    route_type_c->add("least (x,y,z) graph distance");
    route_type_c->add("least angular distance");
    route_type_c->add("least number of edges");
    route_type_c->add("least number of turns");
    route_type_c->value(0);
    route_type_c->tooltip("Route minimises this distance measure.\nEnable angular distance/turn in metric dialogue.");
    route_type_c->color(button_color,highlight_color);
    
    start_route_input = new Fl_Int_Input(150-10,50-5,100,20,"Start vertex");
    end_route_input = new Fl_Int_Input(150-10,80-5,100,20,"End vertex");
    start_route_input->selection_color(dark_selection);
    end_route_input->selection_color(dark_selection);
    start_route_input->box(FL_GTK_DOWN_BOX);
    start_route_input->color(fl_rgb_color(255,255,255));
    end_route_input->box(FL_GTK_DOWN_BOX);
    end_route_input->color(fl_rgb_color(255,255,255));
    
    button_extend *cancel_route = new button_extend(40-10,110,100,25,"Cancel");
    cancel_route->callback(close_route_cb_static,this);
    cancel_route->color(button_color);

    button_extend *ok_route = new button_extend(150-10,110,100,25,"OK");
    ok_route->callback(ok_route_cb_static,this);
    ok_route->color(button_color);

    route_display->end();
    route_display->hide();
    route_display->set_modal();
    
    //END ROUTE DISPLAY WINDOW
    
    //CUSTOM METRIC CALC WINDOW    
    
    int ll = 50-41,lll=600-41;

    custom_metric_calc = new win_extend(win->x()+0.5*win->w()-225,win->y()+0.5*win->h()-363,450,726,"Metric Dialogue");
    {
        Fl_Group *par = new Fl_Group(0,0,custom_metric_calc->w(),custom_metric_calc->h());
        par->box(FL_FLAT_BOX);
        par->color(back_color);

        Fl_Box* box00 = new Fl_Box(20,15+ll-4,410,30,"Metric slot choice");
        box00->box(FL_GTK_DOWN_BOX);
        box00->color(FL_DARK1);
        box00->labelsize(12);
        box00->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box00->color(outer_box_color);
        
        metric_slot_choice = new choice_extend (220,20+ll-4,170,20,"Metric #/display metric");
        metric_slot_choice->tooltip("Choice of metric to display/slot to populate with new metric");
        metric_slot_choice->color(button_color,highlight_color);
        
        Fl_Box* box0 = new Fl_Box(20,60+ll,410,30,"Step 1: Metric basis");
        box0->box(FL_GTK_DOWN_BOX);
        box0->color(FL_DARK1);
        box0->labelsize(12);
        box0->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box0->color(outer_box_color);
        
        custom_metric_basis=new choice_extend (220,65+ll,170,20,"Metric basis");
        custom_metric_basis->add("Origin loction based");
        custom_metric_basis->add("Vertex based");
        custom_metric_basis->add("Edge based");
        custom_metric_basis->tooltip("Origin based: metric for each origin location\nVertex based: metric for each vertex\nEdge based: metric for each edge");
        custom_metric_basis->value(0);
        custom_metric_basis->callback(metric_cb_static,this);
        custom_metric_basis->color(button_color,highlight_color);

        ask1 = new button_extend(400,65+ll,20,20,"?");
        ask1->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("The metric basis is the set of all objects for which a value (a metric) will be computed. The choices are origin locations, vertices, and edges, which lead to a metric value being calculated for each origin location, vertex, and edge, respectively.","Close",0,0,"Metric basis.");
                            },this);
        ask1->color(button_color);
       
        
        Fl_Box* box1 = new Fl_Box(20,110+ll,410,77,"Step 2: Metric measure");
        box1->box(FL_GTK_DOWN_BOX);
        box1->color(FL_DARK1);
        box1->labelsize(12);
        box1->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box1->color(outer_box_color);
        
        custom_metric_choice=new choice_extend (220,115+ll,170,20,"Metric measure");
        
        
        custom_metric_choice->add("euclidian distance x,y (non-graph distance) to destinations");
        custom_metric_choice->add("euclidian distance x,y,z (non-graph distance) to destinations");
        custom_metric_choice->add("(x,y) graph distance to destinations");
        custom_metric_choice->add("(x,y,z) graph distance to destinations");
        custom_metric_choice->add("angular distance to destinations");
        custom_metric_choice->add("number of edges to destinations");
        custom_metric_choice->add("number of turns to destinations");
        custom_metric_choice->add("number of qualifying destinations");
        //custom_metric_choice->add("number of equivalent weighted route metric minimising paths to qualifying destinations");
        custom_metric_choice->add("number of shortest paths to qualifying destinations");
        custom_metric_choice->add("number of shortest paths through basis element between qualifying destinations");
        custom_metric_choice->add("betweenness centrality (normalised) generalised to qualifying destinations");
        custom_metric_choice->add("length of subgraph formed from all shortest paths between qualifying destinations");
        custom_metric_choice->add("eigenvector centrality");
        custom_metric_choice->value(0);
        custom_metric_choice->callback(metric_cb_static,this);
        custom_metric_choice->tooltip("Choice of what quantity to measure");
        custom_metric_choice->color(button_color,highlight_color);
       
        ask4 = new button_extend(400,115+ll,20,20,"?");
        ask4->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("The metric measure determines what quantity (i.e. what metric) is calculated for each element of the metric basis. These can be defined between one, many or all destinations on the graph, thus sometimes necessitating destination selection criteria.","Close",0,0,"Metric measure.");
                            },this);
        ask4->color(button_color);
        
        
        inverse_option = new check_extend(220,137+ll,85,20,"Inverse");
        inverse_option->color(button_color);
        
        inverse_option->value(0);
        inverse_option->tooltip("Take the inverse of the metric (after statistic taken where relevant)");
        
        custom_metric_stat_choice=new choice_extend (220,160+ll,170,20,"Statistic over destinations");
       
        custom_metric_stat_choice->add("Minimum");
        custom_metric_stat_choice->add("Maximum");
        custom_metric_stat_choice->add("Mean");
        custom_metric_stat_choice->add("Median");
        custom_metric_stat_choice->add("Geometric mean");
        custom_metric_stat_choice->add("Harmonic mean");
        custom_metric_stat_choice->add("Variance");
        custom_metric_stat_choice->value(0);
        custom_metric_stat_choice->tooltip("Statistic of metric for the qualifying locations (where relevant)");
        custom_metric_stat_choice->color(button_color,highlight_color);

        ask5 = new button_extend(400,160+ll,20,20,"?");
        ask5->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("If the selected metric measure applies to each destination in conjunction with the basis element, a metric value exists between that element and each destination. This option specifies what statistic to take of all these values in order to give a single value to the basis element.","Close",0,0,"Statistic over destinations.");
                            },this);
        ask5->color(button_color);
        
        Fl_Box* box2 = new Fl_Box(20,207+ll,410,210,"Step 3: Destination selection");
        box2->box(FL_GTK_DOWN_BOX);
        box2->color(FL_DARK1);
        box2->labelsize(12);
        box2->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box2->color(outer_box_color);
        
        
        custom_metric_destination_choice=new choice_extend (220,212+ll,170,20,"Destination Type");
        custom_metric_destination_choice->add("Origin locations");
        custom_metric_destination_choice->add("Destination 1 locations");
        custom_metric_destination_choice->add("Destination 2 locations");
        custom_metric_destination_choice->add("Destination 3 locations");
        custom_metric_destination_choice->add("Destination 4 locations");
        custom_metric_destination_choice->add("Destination 5 locations");
        custom_metric_destination_choice->add("Destination 6 locations");
        custom_metric_destination_choice->add("Destination 7 locations");
        custom_metric_destination_choice->add("Destination 8 locations");
        custom_metric_destination_choice->add("Vertices");
        
        custom_metric_destination_choice->value(0);
        
        custom_metric_destination_choice->callback(metric_cb_static,this);
        custom_metric_destination_choice->tooltip("Identify destinations");
        custom_metric_destination_choice->color(button_color,highlight_color);
        
        ask6 = new button_extend(400,212+ll,20,20,"?");
        ask6->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("When the metric concerns a specific set of locations the choice of such locations must first be filtered through their type. These can be all the basis elements (e.g. vertices or edges) or a specific location type (origin, destination 1 etc.) when the basis is either origins or vertices.","Close",0,0,"Destination type.");
                            },this);
        ask6->color(button_color);


        destination_criteria_choice=new choice_extend (220,242+ll,170,20,"Selection measure");
        destination_criteria_choice->add("euclidian distance x,y (non-graph distance) between basis and destination");
        destination_criteria_choice->add("euclidian distance x,y,z (non-graph distance) between basis and destination");
        destination_criteria_choice->add("minimum (x,y) graph distance between basis and destination");
        destination_criteria_choice->add("minimum (x,y,z) graph distance between basis and destination");
        destination_criteria_choice->add("minimum angular distance between basis and destination");
        destination_criteria_choice->add("minimum number of edges between basis and destination");
        destination_criteria_choice->add("minimum number of turns between basis and destination");
        destination_criteria_choice->value(0);
        destination_criteria_choice->deactivate();
        destination_criteria_choice->tooltip("Measure used to identify and select destinations");
        destination_criteria_choice->callback(metric_cb_static,this);
        destination_criteria_choice->color(button_color,highlight_color);
        
        ask3 = new button_extend(400,242+ll,20,20,"?");
        ask3->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("The selection measure is used as a means to include/exclude members of the selected destination type into the set of qualifying destinations used in the metric calculation. The measure determines how a generalised 'distance' value is associated between the basis and destination which can be used to filter the destinations through the 'Range 1', 'Range 2' and 'Include destinations' fields.","Close",0,0,"Selection measure.");
                            },this);
        ask3->color(button_color);
        
        custom_metric_range_choice = new choice_extend (220,272+ll,170,20,"Include destinations");
        custom_metric_range_choice->add("All");
        custom_metric_range_choice->add("Minimum of selection measure");
        custom_metric_range_choice->add("Maximum of selection measure");
        custom_metric_range_choice->add("Less than or equal to 'range 1' from origin (based on selection measure)");
        custom_metric_range_choice->add("Greater than or equal to 'range 1' from origin (based on selection measure)");
        custom_metric_range_choice->add("Maximum of selection metric whilst less than or equal to 'range 1' from origin (based on selection measure)");
        custom_metric_range_choice->add("Minimum of selection metric whilst greater than or equal to 'range 1' from origin (based on selection measure)");
        custom_metric_range_choice->add("Equal to 'range 1' from origin (based on selection measure)");
        custom_metric_range_choice->add("Lie between 'range 1' and 'range 2' from origin (based on selection measure)");
        custom_metric_range_choice->add("Lie outside 'range 1' and 'range 2' from origin (based on selection measure)");
        custom_metric_range_choice->add("Maximum of selection metric whilst lying in 'range 1' and 'range 2' from origin (based on selection measure)");
        custom_metric_range_choice->add("Minimum of selection metric whilst lying in 'range 1' and 'range 2' from origin (based on selection measure)");
        custom_metric_range_choice->value(0);
        custom_metric_range_choice->callback(metric_cb_static,this);
        custom_metric_range_choice->tooltip("Specify which destinations to include in the calculation based on the selection measure");
        custom_metric_range_choice->color(button_color,highlight_color);

        ask7 = new button_extend(400,272+ll,20,20,"?");
        ask7->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("This selection specifies which elements of the set of 'destination type' destinations are used in the metric calculation based on the selection measure. The selected choice specifies which destinations are included in the calculation. If a maximum/minimum is selected all destinations with the maximum/minimum value are included.","Close",0,0,"Include destinations.");
                            },this);
        ask7->color(button_color);

        custom_metric_range =new Fl_Value_Input(120,302+ll,100,20,"Range 1");
        custom_metric_range->selection_color(dark_selection);
        custom_metric_range->value(0);
        custom_metric_range->deactivate();
        custom_metric_range->box(FL_GTK_DOWN_BOX);
        custom_metric_range->color(fl_rgb_color(255,255,255));

        custom_metric_range2 =new Fl_Value_Input(290,302+ll,100,20,"Range 2");
        custom_metric_range2->selection_color(dark_selection);
        custom_metric_range2->value(0);
        custom_metric_range2->deactivate();
        custom_metric_range2->box(FL_GTK_DOWN_BOX);
        custom_metric_range2->color(fl_rgb_color(255,255,255));
        
        ask8 = new button_extend(400,302+ll,20,20,"?");
        ask8->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("These options specify the numerical value or values of the metric measure which are used in conjunction with the 'Include destinations' field to include destinations in the metric calculation.","Close",0,0,"Range.");
                            },this);
        ask8->color(button_color);

        
        custom_metric_floor_choice=new choice_extend (220,332+ll,170,20,"Floor based criteria");
        custom_metric_floor_choice->add("No constraint");
        custom_metric_floor_choice->add("Same floor");
        custom_metric_floor_choice->value(0);
        custom_metric_floor_choice->tooltip("Restrict destinations based on floor");
        custom_metric_floor_choice->color(button_color,highlight_color);
       
        ask9 = new button_extend(400,332+ll,20,20,"?");
        ask9->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("This options chooses whether to exclude any destinations of the selected 'destination type' if they are situated on a different floor to the basis element for which the metric is being calculated.","Close",0,0,"Floor based criteria.");
                            },this);
        ask9->color(button_color);

        
        custom_metric_visibility_choice=new choice_extend (220,362+ll,170,20,"Visibility criteria");
        custom_metric_visibility_choice->add("No constraint");
        custom_metric_visibility_choice->add("Visible (all high barriers)");
        custom_metric_visibility_choice->add("Visible (all low + high barriers)");
        custom_metric_visibility_choice->add("Visible (opaque high barriers)");
        custom_metric_visibility_choice->add("Visible (opaque low + high barriers)");
        custom_metric_visibility_choice->value(0);
        custom_metric_visibility_choice->tooltip("Restrict destinations based on visibility (origin location based only)");
        custom_metric_visibility_choice->color(button_color,highlight_color);

        ask10 = new button_extend(400,362+ll,20,20,"?");
        ask10->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("This options chooses whether to exclude any destinations of the selected 'destination type' if they are not visible from the origin location for which the metric is being calculated for. This choice only applies to metrics with an origin location basis and where destinations are marked locations (e.g. origins or destinations 1-8). Four visibility criteria can be chosen based on barrier height and opacity.","Close",0,0,"Visibility criteria.");
                            },this);
        ask10->color(button_color);

        
        custom_metric_self_count = new choice_extend(220,392+ll,170,20,"Self include");
        custom_metric_self_count->add("No");
        custom_metric_self_count->add("Yes (if qualifying)");
        custom_metric_self_count->add("Yes (force)");        
        custom_metric_self_count->value(0);
        custom_metric_self_count->tooltip("Exclude or Include the origin, origin's vertex, vertex, edge in the pool of possible/used destinations if it satifies selection criteria or not");
        custom_metric_self_count->color(button_color,highlight_color);

        ask11 = new button_extend(400,392+ll,20,20,"?");
        ask11->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("Often, the basis element for which the metric is being calculated for can also be treated as a destination used in that calculation, but this is not always desired. Selecting 'No' ensures the basis element is never included in the set of destinations. Selecting 'Yes (if qualifying)' leads to the basis element being included if it is of the 'destination type' and meets all other include criteria. Selecting 'Yes (force)' leads to the basis element being included even if it is not of the 'destination type' or otherwise meets the other include criteria.","Close",0,0,"Self include.");
                            },this);
        ask11->color(button_color);
        
        Fl_Box* box3 = new Fl_Box(20,437+ll,410,90,"Step 4: Route selection");
        box3->box(FL_GTK_DOWN_BOX);
        box3->color(FL_DARK1);
        box3->labelsize(12);
        box3->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box3->color(outer_box_color);
        
        route_type_choice=new choice_extend (220,442+ll,170,20,"Route measure");
        route_type_choice->add("(x,y) least graph distance");
        route_type_choice->add("(x,y,z) least graph distance");
        route_type_choice->add("least angular distance");
        route_type_choice->add("least number of edges");
        route_type_choice->add("least number of turns");
        route_type_choice->value(0);
        route_type_choice->deactivate();
        route_type_choice->tooltip("Measure to determine path/route from origin to destinaion");
        route_type_choice->callback(metric_cb_static,this);
        route_type_choice->color(button_color,highlight_color);

        ask2 = new button_extend(400,442+ll,20,20,"?");
        ask2->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("Certain metrics require a precise route to be specified on the graph between the basis element and each destination. These routes are specified by minimising the route measure between the basis element and each destination.","Close",0,0,"Route measure.");
                            },this);
        ask2->deactivate();
        ask2->color(button_color);


        max_iterations = new Fl_Int_Input (220,472+ll,170,20,"Max. number of routes");
        max_iterations->selection_color(dark_selection);
        {
            std::stringstream s;
            s<<20;
            max_iterations->value(s.str().c_str());
        }
        max_iterations->deactivate();
        max_iterations->tooltip("Maximum considered number of equally weighted routes. Cyclical routes are ignored so a low value on a graph with zero weight cycles may not find a valid existing route. Warnings are given if this occurs.");
        max_iterations->box(FL_GTK_DOWN_BOX);
        max_iterations->color(fl_rgb_color(255,255,255));
       
        ask12 = new button_extend(400,472+ll,20,20,"?");
        ask12->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("Some metrics require the specific paths/routes between elements of the graph, but there may be more than one such path/route between each origin and destination that minimise the route measure. The max. number of routes field provides an upper limit on the number of such routes that are considered in the metric calculation. Note: if there are multiple routes and the max. number of routes is too small not all routes will be found. Warnings will be given in this case.","Close",0,0,"Max number of routes.");
                            },this);
        ask12->color(button_color);

        route_stat_choice=new choice_extend (220,502+ll,170,20,"Statistic over routes");
        route_stat_choice->add("Minimum of metric measure");
        route_stat_choice->add("Maximum of metric measure");
        route_stat_choice->add("Mean of metric measures");
        route_stat_choice->value(0);
        route_stat_choice->deactivate();
        route_stat_choice->tooltip("If there are multiple routes corresponding to the minimum route measure and which lead to distinct metric measures, we must determine a unique value for the metric measure");
        route_stat_choice->color(button_color,highlight_color);
        
        ask13 = new button_extend(400,502+ll,20,20,"?");
        ask13->callback([](Fl_Widget *, void *data) {
                            base* program = static_cast<base*> (data);
                            program->choice("When the metric measure is a distinct quantification of distance on the graph to the route measure there many be many routes which minimise the route measure with distinct metric measure values. In this case the max number of routes field determines how many such routes to consider. The statistic over routes option then specifies how to determine a single metric measure value from the metric measure values for each route. The options are to use the minimum metric measure out of all the routes, to use the maximum metric measure out of all the routes, or to use the mean metric measure of all the routes.","Close",0,0,"Statistic over routes.");
                            },this);
        ask13->color(button_color);


        Fl_Box* box4 = new Fl_Box(20,547+ll,410,120,"Options");
        box4->box(FL_GTK_DOWN_BOX);
        box4->color(FL_DARK1);
        box4->labelsize(12);
        box4->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        box4->color(outer_box_color);

        cache_metric_data = new check_extend(30,552+ll,115,20,"Cache routes");
        cache_metric_data->value(0);
        cache_metric_data->tooltip("Save route information when calculating metrics to vastly speed up calculations that use them later. Warning: can dramatically increase memory usage (not recommended for large graphs). Becomes invalid/cleared when graph or turn definition is changed");
        cache_metric_data->color(button_color);
        
        
        eig_tol = new Fl_Float_Input(360,552+ll,60,20,"Eigenvector tolerance");
        eig_tol->selection_color(dark_selection);
        {
            std::stringstream s;
            s<<"1e-3";
            eig_tol->value(s.str().c_str());
        }
        eig_tol->deactivate();
        eig_tol->tooltip("Convergence acceptance tolerance for eigenvector centrality computation as fractional difference beteen largest and smallest eigenvalue estimates");
        eig_tol->box(FL_GTK_DOWN_BOX);
        eig_tol->color(fl_rgb_color(255,255,255));

        Fl_Box* recalc_box = new Fl_Box(30,35+lll,100+50,44,"Force recalculate");
        recalc_box->box(FL_GTK_DOWN_BOX);
        recalc_box->color(fl_rgb_color(185,185,185));
        recalc_box->labelsize(10);
        recalc_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        recalc_box->color(inner_box_color);

        force_create_button = new check_extend(35,35+2+lll,90+53,20,"Graph constructs");
        force_create_button->tooltip("Rebuild graph constructs on metric calculation even if graph has not changed.");
        force_create_button->color(button_color);
        force_visibility_button = new check_extend(35,55+2+lll,90+53,20,"Visibility");
        force_visibility_button->tooltip("Recalculate visibility data on metric calculation even if not used in metric or if locations and/or barriers have not changed.");
        force_visibility_button->color(button_color);

        Fl_Box* options_box = new Fl_Box(145+45,35+lll,275-45,44,"Analysis options");
        options_box->box(FL_GTK_DOWN_BOX);
        options_box->color(fl_rgb_color(185,185,185));
        options_box->labelsize(10);
        options_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        options_box->color(inner_box_color);

        edges_button = new check_extend(150+45,37+lll,265-45,20,"Enable edge based metrics");
        edges_button->tooltip("Enable/disable edge based metrics");
        edges_button->callback(toggle_segments_cb_static,this);
        edges_button->value(1);
        edges_button->color(button_color);

        turns_button = new check_extend(150+45,57+lll,265-45,20,"Enable ang. deviation/turns");
        turns_button->tooltip("Enable/disable metrics which use angular deviation/turns as a measure of distance.");
        turns_button->callback(toggle_turns_cb_static,this);
        turns_button->value(1);
        turns_button->color(button_color);

        turn_ang = new Fl_Float_Input(300-25-5,85+lll+2,80,20,"Turn definition (degrees)");
        turn_ang->selection_color(dark_selection);
        turn_ang->callback((Fl_Callback*) turn_def_cb_static,this);
        turn_ang->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
        turn_ang->tooltip("Angular deviation above this value, measured in degrees, constitutes a turn.\nEffects all subsequently calculated metrics when set.\nOK button must be pressed to take effect");
        {
            std::stringstream s;
            s<<"45.00";
            turn_ang->value(s.str().c_str());
        }
        turn_ang->box(FL_GTK_DOWN_BOX);
        turn_ang->color(fl_rgb_color(255,255,255));

        turn_ang_ok = new button_ex(390-25-5,85+lll+2,30,20,"OK",this);
        turn_ang_ok->callback((Fl_Callback*) turn_def_cb_static,this);
        turn_ang_ok->tooltip("Angular deviation above this value, measured in degrees, constitutes a turn.\nEffects all subsequently calculated metrics when set.\nOK button must be pressed to take effect");
        turn_ang_ok->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        turn_ang_ok->color(button_color);


        button_extend* desk_metric_display = new button_extend (20,592+ll+90,122,20,"Display/Cancel");
        desk_metric_display->callback((Fl_Callback*) close_desk_metric_static,this);
        desk_metric_display->tooltip("Display metric in chosen slot");
        desk_metric_display->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        desk_metric_display->color(button_color);

        button_extend* desk_metric_button = new button_extend (164,592+ll+90,122,20,"Calculate");
        desk_metric_button->callback((Fl_Callback*) desk_metric_cb1_static,this);
        desk_metric_button->tooltip("Calculate metric");
        desk_metric_button->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        desk_metric_button->color(button_color);
        

        button_extend* desk_metric_delete = new button_extend (308,592+ll+90,122,20,"Delete");
        desk_metric_delete->callback((Fl_Callback*) delete_metric_cb_static,this);
        desk_metric_delete->tooltip("Delete metric");
        desk_metric_delete->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
        desk_metric_delete->color(button_color);
        
        par->end();

    }

    custom_metric_calc->end();
    custom_metric_calc->callback(exit_desk_metric_static,this);
    custom_metric_calc->hide();
    custom_metric_calc->set_modal();
    
    //END CUSTOM METRIC CALC WINDOW
    
        
    //BEGIN GUIDE
    
    guide= new Fl_Window(win->x()+0.5*win->w()-300,win->y()+0.5*win->h()-265,600,530,"Quick Guide");
    Fl_Group *g_gr = new Fl_Group(0,0,guide->w(),guide->h());
    g_gr->box(FL_FLAT_BOX);
    g_gr->color(back_color);
     button_extend *ok_guide = new button_extend(490,495,100,25,"OK");
    ok_guide->callback(ok_guide_cb_static,this);
    ok_guide->when(FL_WHEN_RELEASE|FL_WHEN_ENTER_KEY);
    ok_guide->color(button_color);

    out_guide = new Fl_Text_Display(10,25,600-20,530-80);
    out_guide->textcolor(dark_text);
    out_guide->color(dark_back);
    out_guide->selection_color(dark_selection);
    std::stringstream s;
 s<<"NOTE:\n\nFor more complete information refer to the manual that should have been provided with this software\n\n"
 "Program fundamentals:\n\nWork is encapsulated into a 'project' that can be saved from/imported into one of four project slots. Projects are saved as proprietary '.proj' files.\n\n"
 "A project contains a spatial graph, consisting of vertices and edges, location markers, barriers, floor areas, and floorplans.\n\n"
 "All vertices, location markers, barriers, floor areas, and floorplans belong to 'floors'. Floors allow for 3 dimensionality with all such objects on a given floor possessing the same height in space controlled by the floor height field. "
 "As such, to have many different heights you need many different floors. Edges between vertices in the spatial graph allow for the spatial graph to span the whole project.\n\n"
 "Location markers can be of two types, origins or destinations - origins form a potential basis for metrics and behave differently to destinations.\n"
 "Location markers' placement relates to their visibility, whilst the vertex they are associated with relates to their position for the purposes of metrics. Whether a single marker is identified as a separate location depends on the location type (origin vs. destination).\n"
 "All origin location markers count as separate locations regardless of the vertex they are associated with.\n"
 "For destination locations, all location markers at a given spatial graph vertex count as a single location with the markers providing many visibility points for that location.\n\n"
 "Once such constructs are marked, metrics are then computed through the metric dialogue. Here properties of the spatial graph, in conjunction with locations and barriers, are computed for each element of the metric basis. "
 "The metric basis can be the set of all origin locations, graph vertices, or graph edges.\n\n"
 "Interface & Shortcuts:\n\n"
 "Unmarked shortcuts:\n\tCtrl + Left/Right: Navigate Main/Data/Plot tabs\n\tShift + 1-4: Select project 1-4\n\tShift + [/]: Cycle through floor selection\n\tCtrl + 1-4: Select view mode\n\t\t(1: 2D, 2: 3D Orthographic, 3: 3D Perspective, 4: Free view)\n\tAlt + 0: Set edit selector to none/off\n\tAlt + 1: Set edit selector to origins"
 "\n\tAlt + 2: Set edit selector to low barriers\n\tAlt + 3: Set edit selector to high barriers\n\tAlt + 4: Set edit selector to spatial graph vertices\n\tAlt + 5: Set edit selector to spatial graph edges\n\tAlt + 6: Set edit selector to floor areas\n\tAlt + [/]: Cycle through edit selector\n"
 "\nView:\n\t2D:\n\t\tMiddle mouse: Translate\n\t\tLeft mouse: Translate (when not editing)\n\t\tScroll: Zoom\n\t\tCtrl + Left mouse: Translate\n\t\tAlt + X: Edit central point/rotation axis of view"
 "\n\t3D: Orthographic:\n\t\tMiddle mouse: Translate\n\t\tLeft mouse: Rotate\n\t\tScroll: Zoom\n\t\tCtrl + Left mouse: Translate"
 "\n\t3D: Perspective:\n\t\tMiddle mouse: Translate\n\t\tLeft mouse: Rotate\n\t\tScroll: Adjust field of view\n\t\tCtrl + Left mouse: Translate\n\t\tW: Reduce distance\n\t\tS: Increase distance\n\t\tCtrl + Mousewheel: Increase/reduce distance"
 "\n\tFree view:\n\t\tW: Move forward\n\t\tS: Move backward\n\t\tA: Pan left\n\t\tD: Pan right\n\t\tR: Pan up (when unlocked - see view options)\n\t\tF: Pan down (when unlocked - see view options)\n\t\tMousewheel: Adjust field of view\n\t\tDrag mouse: change direction"
 "\n\nEditing:\n\tSpatial graph vertices/locations:\n\t\tLeft mouse: Add construct\n\t\tRight mouse: Select construct (single click or drag)\n\t\tShift: Snap on guidelines (vertices + origins only)\n\t\tShift + Ctrl: Rotate guide lines\n\t\tAlt + Left mouse: Drag construct\n\t\t\t(see also move mode Ctrl + M)\n\t"
 "Spatial graph edges:\n\t\tRight mouse: Select (single click or drag)\n\t\tLeft mouse on vertex:\n\t\t\tStart edge (first press), end edge (second press)\n\t\tAfter first left press, any other press: Cancel\n\t\tShift: Snap on to nearest vertex\n\t"
 "Barriers:\n\t\tShift: Snap onto floor plans and existing barriers\n\t\tShift + Alt: Draw guidelines from existing barriers\n\t\tRight mouse: Select (single click or drag)\n\t\tO (When selected): Make opaque\n\t\tT (When selected): Make transparent\n\t\tLeft mouse:\n\t\t\tStart barrier (first press), end barrier (second press)\n\t\tAny other mouse press: Cancel\n\t"
 "Floor areas:\n\t\tShift: Snap onto floor plans\n\t\tRight mouse: Select (single click or drag)\n\t\tP (When selected): Set as positive\n\t\tN (When selected): Set as negative\n\t\tLeft mouse: Start polygon/extend polygon\n\t\tCtrl + Right mouse: Close geometry\n\t\t\t(minimal polygon is a triangle)\n\t\tAfter first left mouse, Right mouse: Cancel current polygon\n\n"
 "Auto-construct generation:\n\n\tBarrier-generation from floorplan: Barriers are generated based on\n\tobjects in the currently displayed floorplan layers and blocks.\n\n\tSpatial-graph generation: Vertices and edges are generated based\n\ton straight lines in the currently displayed floorplan layers.\n\tThis is not recommended unless the dxf file has specifically been \n\tconstructed for this purpose.\n\n\tMesh generation: Populates all marked floor areas^ with a spatial\n\tgraph formed of an appropriately connected (user-defined) \n\tequi-spaced mesh of vertices.\n\tVisibility barriers prevent edges between vertices being made.\n\tAn option allows uncontained* floor area edges to prevent edges \n\tbetween vertices.\n\n\t"
 "TIPS:\n\n\tThis functionality is fundamentally different to a user-defined spatial \n\tgraph - it is designed to capture all possible movement in the limit \n\tof a tight mesh.\n\tIt is more computationally intensive so caution is advised with high \n\tmesh densities, especially when using angular deviations. Edge \n\tanalysis is generally intractable and inappropriate and is strongly \n\tdiscouraged.\n\n\t"
 "^Behaviour depends on the type of floorarea.\n\tVertices are added to the union of all positive floorareas\n\t - i.e. overlapping floor areas are only counted once.\n\t"
 "However, any of such vertices which lie in negative floor area \n\tare not added.\n\t"
 "Consequently, one can cover awkward geometries by mutlitple \n\toverlapping positive areas and/or removing regions from \n\tpositive areas by means of negative areas.\n\n\t"
 "*Again behaviour depends on the type of floorarea.\n\tEdges of negative floor areas are straightforwardly used as barriers.\n\t"
 "However, for positive areas, only parts of floor area edges that \n\tare not contained in other floor area polygons\n\tare used to block edges between vertices. \n\tShared edges between polygons are deemed to be overlapping - the \n\tuse of such cases should be avoided if possible due to the \n\tpossibility of floating point errors.";
    Fl_Text_Buffer* buff=new Fl_Text_Buffer;
    buff->append(s.str().c_str());
    out_guide->buffer(buff);
    out_guide->wrap_mode(1,0);
    
    out_guide->labelsize(20);
    out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  

    guide->end();
    guide->hide();
    guide->set_modal();
    
    
    ///END GUIDE
    

    
    //BEGIN SPLASH SCREEN
    win2= new Fl_Window(0.5*(Fl::w()-679),0.5*(Fl::h()-266),679,266);
    win2->begin();
    splash = new Fl_Box(0,0,679,266);
     
    
    Fl_Pixmap splash_pixmap(splash_screen);

    splash->image(&splash_pixmap);
    

    splash->box(FL_UP_BOX);
    win2->end();
    win2->clear_border();
    

    win2->set_modal();
    win2->show();
    //END SPLASH SCREEN
        
    for (int i=0;i<NUM_BUILDINGS;i++){
        
        while(root.calculating){
            std::this_thread::sleep_for (std::chrono::milliseconds(100));
            //wait until free
        }

        root.current=root.comp_buildings[i];

        prepare_undo(); // intialise first memento frame in each building
    }

    root.current=root.comp_buildings[0];
        
    start_time=clock();
   
    root.calculating=INITIALISING;
    
    //resize main window offscreen to reconfigure sizes (HACK)

    win->resize(win->x(),win->y(),win->w()+1,win->h());
    win->resize(win->x(),win->y(),win->w()-1,win->h());

    Fl::add_idle(idle_load_static,this);
    //Fl::add_timeout(1.0,push_time_static,this); 
    
    Fl::run(); // Turn control over to FLTK
    
}