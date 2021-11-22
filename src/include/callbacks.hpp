/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/


 /*This file is included in base.h serving as an inserted list of function and nested-class definitions*/

#ifndef CALLBACKS_H
#define CALLBACKS_H

static void push_time_static(void*);
static void reset_console_buffer_and_push2(void*);
public:
void no_editing();
static void add_console_updater(void*);
void stop_console_output();
void wait_for_console();
private:
int choice(const char *,const char *,const char *,const char *);
int choice(const char *,const char *,const char *,const char *,const char *);

void find_floor_extremes();

static void select_all_cb_static(Fl_Widget*,void*);

static void plan_colors_menu_cb_static(Fl_Widget*, void*);
void plan_colors_menu_cb();

static void lattice_choice_cb_static(Fl_Widget*,void*);
void lattice_choice_cb();

static void limit_mesh_edge_cb_static(Fl_Widget*,void*);
void limit_mesh_edge_cb();

static void visibility_select_cb_static(Fl_Widget*, void*);
void visibility_select_cb();

static void floorplan_win_cb(Fl_Widget*,void*);

static void units_ok_static(Fl_Widget*,void*);
static void units_cancel_static(Fl_Widget*,void*);



static void panel_cb_static(Fl_Widget*, void*);
void panel_cb();

static void info_bar_cb_static(Fl_Widget*, void*);
void info_bar_cb();

static void save_screen_static(Fl_Widget*,void*);
void save_screen_cb();
static void* save_screen_wait(void *);
static void save_screen(void *);

static void areas_to_points_cb_static(Fl_Widget*,void*);
void* areas_to_points_cb();

void areas_to_points(double,int,double,int);

int pnpoly(double,double,const std::vector<vec>&);
int gcd(int,int);
static void toggle_deskmodel_cb_static(Fl_Widget*, void*);
static void toggle_segments_cb_static(Fl_Widget*, void*);
static void toggle_turns_cb_static(Fl_Widget*, void*);
static void areas_to_points_ok_cb_static(Fl_Widget*,void*);
static void areas_to_points_cancel_cb_static(Fl_Widget*,void*);
std::vector<int> remove_map_from_floor(int);
int remove_vertex_list(const std::vector<int>&);
void calc_links_with_walls(int,std::vector<vertex> *, int, int, int,double,int&,std::vector<wall>&,int,int,int,int,int,int,double);


void rebuild_widgets();
void activate_main_edit();
void deactivate_main_edit();


void activate_plan_edit();
void deactivate_plan_edit();
void two_element_deselect();



int save_binary(const file_in_out*);
int read_binary(file_in_out*);



public:
 void prepare_undo();
 void clear_and_prepare_undo();
 static void clear_and_prepare_undo_from_thread(void*);



public:
void calc_message_on();
void calc_message_off();
static void calc_message_off_cb(void*);
static void calc_message_on_cb(void*);

std::istream& tool_getline(std::istream& , std::string& );


static void awake_map_warning_off(void*);
static void awake_vis_warning_off(void*);
static void awake_map_warning_on(void*);
static void awake_vis_warning_on(void*);


int readin_desks(std::vector<vec> &,std::vector<location>&,std::string);
int readin_points(std::vector<vec> &,std::string);
int readin_matrix(std::vector<vec> &,std::vector<vertex> &,std::string );
int readin_adj_list(std::vector<vec> &,std::vector<vertex> &,std::string);


public:
   void floorpoly_reset();
void push_move_status(int); 
void push_output(std::string); 
void push_output_no_lock(std::string); 
void clear_output();
void push_data_output(std::string);
void clear_data_output();
void push_time();
void push_time_no_lock();
static void push_time(void*);
static void push_output_awake(void*);
static void reset_console_buffer(void*);
static void reset_console_buffer_and_push(void*);


private:

void data_null();
void data_points();
void data_matrix();
void data_segments();
void data_location(int);
void data_visibility();
void data_custom_metric();
void data_floor_info();

public:
void soft_reset_fields();
private:
    void minimum_reset_fields();

void reset_fields();

static void turn_def_cb_static(Fl_Widget*,void*);
void turn_def_cb();

static void open_adjlist_cb_static(Fl_Widget* /*widget*/,void* data);
void open_adjlist_cb();
static void* read_adj_list_pass_static(void*);
void* read_adj_list_pass();

static void close_route_cb_static(Fl_Widget*,void*);
void close_route_cb();
static void close_link_win_static(Fl_Widget*,void*);
void close_link_win();

static void close_memento_depth_win_static(Fl_Widget*,void*);
void close_memento_depth_win();


static void floorplan_to_map_static(Fl_Widget*,void*);
void floorplan_to_map();
void floorplan_to_map_thread();
int construct_graph_from_floorplan(std::vector<vec> &,std::vector<vertex> &, int);
static void floorplan_to_areas_static(Fl_Widget*,void*);
void floorplan_to_areas();
void floorplan_to_areas_thread();

static void link_win_cb_static(Fl_Widget*,void*);
void link_win_cb();


static void add_link_cb_static(Fl_Widget*,void*);
static void remove_link_cb_static(Fl_Widget*,void*);
void add_link_cb();
void remove_link_cb();

static void add_vertex_cb_static(Fl_Widget*,void*);
static void remove_vertex_cb_static(Fl_Widget*,void*);
void add_vertex_cb();
void remove_vertex_cb();

static void add_loc_cb_static(Fl_Widget*,void*);
void add_loc_cb();
static void remove_loc_cb_static(Fl_Widget*,void*);
void remove_loc_cb();

static void show_ang_win_cb_static(Fl_Widget*,void*);
void show_ang_win_cb();
static void set_ang_cb_static(Fl_Widget*,void*);
void set_ang_cb();
static void close_ang_cb_static(Fl_Widget*,void*);
void close_ang_cb();


static void floorplan_cb_static(Fl_Widget*,void*);
void floorplan_cb();

static void floorplan_cb_button_static(Fl_Widget*,void*);
void floorplan_cb_button();


void* read_locations_pass(int);
static void open_locations_cb_static(Fl_Widget*,void* data);
void open_locations_cb(int);

static void open_points_cb_static(Fl_Widget*,void* data) ;
void open_points_cb() ;
static void open_matrix_cb_static(Fl_Widget*,void* data) ;
void open_matrix_cb() ;

static void create_structures_static(Fl_Widget* widget,void* data);
void create_structures();


void test(Fl_Widget* widget,void* data);

static void get_units_from_thread(void*);


static void* read_points_pass_static(void* data);
void* read_points_pass();
static void* read_matrix_pass_static(void* data);
void* read_matrix_pass();
static void visibility_cb_static(Fl_Widget* widget, void* data);
void visibility_cb();
static void* visibility_calc1_static(void* data);
void* visibility_calc1();

void vis_calc_for_desks(int ,int ,std::vector<int> *, std::vector<wall> *, std::vector<location> &,std::vector<location> *,int,long int&,long int,int, int, int,int &);

int get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y, double *i2_x, double *i2_y);
int get_line_intersection_fast(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y);
int visible_test(double,double,double,double,double,double,double,double);
static void desk_metric_cb_static(Fl_Widget*, void*);
void desk_metric_cb();
void* desk_metric_calc(void*);
static void save_binary_cb_static(Fl_Widget*, void* );
void save_binary_cb();
static void* save_binary_thread_static(void* );
void* save_binary_thread();
static void read_binary_cb_static(Fl_Widget* , void* );
void read_binary_cb();
static void* read_binary_thread_static(void* );
void* read_binary_thread();
static void* loading_thread_func_static(void*);
void* loading_thread_func();

static void open_dxf_cb_static(Fl_Widget* , void* );
void open_dxf_cb();
static void clear_dxf_cb_static(Fl_Widget* , void* );
void clear_dxf_cb();
static void* open_dxf_thread_static(void* );
void* open_dxf_thread();
static void scale_floorplan_cb_static(Fl_Widget*,void*);
void scale_floorplan_cb();
static void redraw_mark_static(Fl_Widget* widget,void*);
void redraw_mark(int);
static void floor_choice_cb_static(Fl_Widget*,void*);
void floor_choice_cb(int);
static void three_d_cb_static(Fl_Widget*,void*);
void three_d_cb();
void set_floor_choice();
void reset_floor_choice();
static void quiet_reset(void*);
static void undo_static(Fl_Widget*,void*);
void undo(int);
static void redo_static(Fl_Widget*,void*);
void redo(int);

static void memento_depth_win_cb_static(Fl_Widget*,void*);
void memento_depth_win_cb();

static void change_memento_depth_cb_static(Fl_Widget*,void*);
void change_memento_depth_cb();


static void add_floor_cb_static(Fl_Widget*,void*);
static void remove_floor_cb_static(Fl_Widget*,void*);
void add_floor_cb();
void remove_floor_cb();
void find_extrema_points();

static void redraw_data_static(Fl_Widget*, void*);
void redraw_data();

static void save_data_static(Fl_Widget*, void*);
void save_data();


static void clear_locations_cb_static(Fl_Widget*, void*);
static void clear_selected_location_cb_static(Fl_Widget*, void*);
static void clear_last_location_cb_static(Fl_Widget*, void*);

void clear_locations_cb();
int update_locations(std::vector<location>*,int);
void clear_selected_location_cb();
void clear_last_location_cb();

static void cursor_cb_static(Fl_Widget*, void*);
void cursor_cb();

static void location_label_cb_static(Fl_Widget*, void*);
void location_label_cb();
static void point_in_cb_static(Fl_Widget*, void*);
void point_in_cb();
static void zfloor_cb_static(Fl_Widget*, void*);
void zfloor_cb();


static void shiftx_cb_static(Fl_Widget*, void*);
static void shifty_cb_static(Fl_Widget*, void*);
void shiftx_cb();
void shifty_cb();

static void plot_choice_cb_static(Fl_Widget*, void*);
void plot_choice_cb();

static void exit_desk_metric_static(Fl_Widget* /*widget*/, void* data);
void exit_desk_metric();

static void close_desk_metric_static(Fl_Widget* /*widget*/, void* data);
void close_desk_metric();
static void desk_metric_cb1_static(Fl_Widget* widget, void* data);
void desk_metric_cb1();
static void* custom_metric_thread_static(void* data);
void* custom_metric_thread();

void launch_custom_metric( int thread,
                           int start,
                           int end,
                           std::vector<double> &temp_metric,
                           const std::vector<vec>& points,
                           const std::vector<location>& desks, 
                           map_structures* map,                             
                           int dest_choice_metric,
                           int route_choice_metric,
                           int metric, 
                           int same_floor, 
                           int visible,
                           int stat,
                           int range_option,
                           std::vector<location>* destination_,                            
                           int is_desk,
                           double range,
                           double range2,
                           int desk_or_point, 
                           int self_count_aware,
                           int dest_is_points,
                           int dest_is_lines,
                           int iterations,                           
                           graph_routes& gr_dest,
                           graph_routes& gr_route,
                           std::vector<double>& weight_d,
                           std::vector<int>& visited_d, 
                           int inverse,
                           std::vector<std::vector<route_weight_number> >& choice_new,
                           int route_stat, 
                           int do_cache,
                           int &kill_flag,
                           ERROR_CODE &error_pass);


static void show_route_menu_cb_static(Fl_Widget*, void*);
void show_route_menu_cb();

static void show_route_cb_static(Fl_Widget*, void*);
void show_route_cb();


static void warning_map_cb_static(Fl_Widget* /*widget*/, void* data);
void warning_map_cb();
static void building_data_choice_cb_static(Fl_Widget* widget_, void* data);
void building_data_choice_cb();
static void building_choice_cb_static(Fl_Widget* widget_, void* data);
void building_choice_cb();
void main_callback();
static void main_callback_static(Fl_Widget*, void* data);


static void idle_static(void* data);
void idle();

static void idle_load_static(void* data);
void idle_load();
static void clear_metrics_cb_static(Fl_Widget* /*widget*/, void* data);
void clear_metrics_cb();
static void delete_metric_cb_static(Fl_Widget*, void* data);
void delete_metric_cb();
void* showmain();
void* showsecond();
static void redraw_static(Fl_Widget* /*widget*/,void* data);
void redraw();

static void redraw_from_thread(void* data);
static void check_from_thread(void * /*data*/);

static void redraw_show_static(Fl_Widget* widget, void* data);
void redraw_show();

static void scheme0_static(Fl_Widget* /*widget*/,void* /*data*/);
static void scheme1_static(Fl_Widget* /*widget*/,void* /*data*/);
static void scheme2_static(Fl_Widget* /*widget*/,void* /*data*/);
static void scheme3_static(Fl_Widget* /*widget*/,void* /*data*/);

void scheme0();
void scheme1();
void scheme2();
void scheme3();

static void main_tab_callback_static(Fl_Widget* widget,void* data);
void main_tab_callback();
static void tab_callback_static(Fl_Widget* widget,void* data);
void tab_callback();

static void plan_colors_cb_static(Fl_Widget* /*widget*/, void* data);
void plan_colors_cb();

static void splines_cb_static(Fl_Widget* /*widget*/, void* data);
void splines_cb();

static void LayerCB_static(Fl_Widget* /*widget*/, void* data);
void LayerCB();

static void BlockCB_static(Fl_Widget* /*widget*/, void* data);
void BlockCB();

static void go_fullscreen_static(Fl_Widget* /*widget*/, void* /*data*/);
void go_fullscreen();

static void about_cb_static(Fl_Widget* /*widget*/, void* data);
void about_cb();
static void about_ok_cb_static(Fl_Widget* /*widget*/, void* data);
void about_ok_cb();

static void calc_cb_static(Fl_Widget* /*widget*/, void* data);
void calc_cb();
static void calc_ok_cb_static(Fl_Widget* /*widget*/, void* data);
void calc_ok_cb();

static void guide_cb_static(Fl_Widget* /*widget*/, void* data);
void guide_cb();

static void ok_guide_cb_static(Fl_Widget* /*widget*/, void* /*data*/);
void ok_guide_cb();

static void show_floorplan_cb_static(Fl_Widget*,void* data);
static void show_grid_cb_static(Fl_Widget*,void* data);
static void snap_to_point_cb_static(Fl_Widget*,void* data);
static void transparent_walls_cb_static(Fl_Widget*,void* data);
static void points_label_cb_static(Fl_Widget*,void* data);
static void segments_label_cb_static(Fl_Widget*,void* data);
static void locations_label_cb_static(Fl_Widget*,void* data);
static void mark_transparent_walls_cb_static(Fl_Widget*,void* data);


void show_floorplan_cb();
void show_grid_cb();
void snap_to_point_cb();
void transparent_walls_cb();
void points_label_cb();
void segments_label_cb();
void locations_label_cb();
void mark_transparent_walls_cb();

static void toggle_autoconnect_cb_static(Fl_Widget*,void* data);
static void set_move_cb_static(Fl_Widget*,void* data);
static void color_scale_cb_static(Fl_Widget*,void* data);
static void show_scale_cb_static(Fl_Widget*,void* data);

void toggle_autoconnect_cb();
void set_move_cb();
void color_scale_cb();
void show_scale_cb();

static void* route_thread_static(void* data);
void* route_thread();
static void ok_route_cb_static(Fl_Widget* /*widget*/, void* data);
void ok_route_cb();

void quit_callback(Fl_Widget*, void*);
void hide(Fl_Widget*);

static void colour_cb_static(Fl_Widget* /*widget*/, void* data);
void colour_cb();


static void rebuild_hist_input_static(Fl_Widget*,void* data);
void rebuild_hist_input();

static void metric_cb_static(Fl_Widget*,void*);
void metric_cb();


static void show_metric_cb_static(Fl_Widget*, void*);
void show_metric_cb();

static void show_rot_axis_cb_static(Fl_Widget*, void*);
void show_rot_axis_cb();

static void floor_lock_cb_static(Fl_Widget*, void*);
void floor_lock_cb();

static void floor_ceil_cb_static(Fl_Widget*, void*);
void floor_ceil_cb();

static void limit_edge_cb_static(Fl_Widget*, void*);
void limit_edge_cb();

void custom_destination_cb(Fl_Widget* , void* );


static void reset_view_static(Fl_Widget* widget, void* data);
void reset_view();

static void kill_thread_static(Fl_Widget*, void* data);
void kill_thread();
public:
void show_warning_map();
void show_warning_visibility();
private:
static void show_warning_map_cb(void* data);
static void show_warning_vis_cb(void* data);
    
static void show_warning_map_start(void* data);
static void show_warning_vis_start(void* data);

static void line_calc_cb_static(Fl_Widget*, void*);
void line_calc_cb();

void open_walls(int type);
void open_walls_standing();
void open_walls_standing_transparent();
void open_walls_sitting();
void open_walls_sitting_transparent();

static void open_walls_standing_static(Fl_Widget*, void*);
static void open_walls_standing_transparent_static(Fl_Widget*, void*);
static void open_walls_sitting_static(Fl_Widget*, void*);
static void open_walls_sitting_transparent_static(Fl_Widget*, void*);

static void* read_walls_pass_static(void*);
void* read_walls_pass();
int readin_walls(std::vector<wall> &,std::vector<wall>&,int, int,std::string);

void floorplan_to_barriers(std::vector<wall> *,const container*,int,int);
void lines_to_walls(int);
void lines_to_walls_standing();
void lines_to_walls_standing_transparent();
void lines_to_walls_sitting();
void lines_to_walls_sitting_transparent();

static void lines_to_walls_standing_static(Fl_Widget*, void*);
static void lines_to_walls_standing_transparent_static(Fl_Widget*, void*);
static void lines_to_walls_sitting_static(Fl_Widget*, void*);
static void lines_to_walls_sitting_transparent_static(Fl_Widget*, void*);

void addline(const line_in,std::vector<wall>*,int, int,const container*,double,double,matrix_2d);
void addarc(const arc,std::vector<wall>*,int, int,const container*,double,double,matrix_2d);
void addcircle(const circle,std::vector<wall>*,int, int,const container*,double,double,matrix_2d);
void addellipse(const ellipse,std::vector<wall>*,int, int,const container*,double,double,matrix_2d);
void addspline(const spline,std::vector<wall>*,int, int,const container*,double,double,matrix_2d);
void add_block_nested(const plan*,const block_entry&,int,const container*,std::vector<wall>*,int,int,const matrix_2d&,const double&,const double&);

void clear_building();
static void clear_building_static(Fl_Widget*,void*);

static void map_consistency_static(Fl_Widget*, void*);
void map_consistency();
static void check_map_static(void*);
void check_map();

#endif //CALLBACKS_H

