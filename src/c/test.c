/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <pebble.h>

static Window *my_window;
static TextLayer *text_layer;
static Layer *myCanvas;
static GBitmap *current_background, *back2;

static unsigned char game_mode = 0;
static unsigned char remember_back;
static unsigned char buttons[3];
static unsigned char titlescreen_press_start_time;
static unsigned char progress;
static unsigned char cum;
static unsigned char wait_press = 0;

static void sel_click_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 1;}
static void sel_release_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 0;}
static void sel_click_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 1;}
static void sel_release_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 0;}
static void sel_click_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 1;}
static void sel_release_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 0;}

const char text[53][38] =
{ 
	{"Harmonia: Hilbert, i must thank you."},
	{"Thanks to you, i have finally"},
	{"found the meaning of life."},
	{"Hilbert: It's okay harmonia."},
	{"Harmonia: But(t)..."},
	{"Before i go and ride mah cutie,"},
	{"there's something i need to ask you."},
	{"Hilbert: What is it ?"},
	{"Harmonia: Close your eyes and"},
	{"turn your back to me."},
	{">Hilbert closes his eyes"},
	{"and turns his back to him"},
	{"Harmonia: Now..."},
	{"GET READY TO BE RAPED !"},
	{"Hilbert: What the..."},
	{">Harmonia penetrates Hilbert"},
	{"Hilbert: !"},
	{"Harmonia: Are you ready ?"},
	{"Hilbert: GET OFF ME !"},
	{"Harmonia: ... No."},
	{"Hilbert: WHY ?"},
	{"Harmonia: ...Because i don't want to."},
	{"NOW PREPARE FOR THE BAREBACK SEX !"},
	{"*Hmpf! Hmpf! Hmpf!*"},
	{"Hilbert: Please, STOP !"},
	{"Harmonia: Okay."},
	{"..."},
	{"Harmonia: ..."},
	{"JUST KIDDING !!!"},
	{"*Oh yeah, eat my cock !*"},
	{"Oh my godness, it feels so GREAT !"},
	{"Hilbert: I'll get my revenge when"},
	{"my Smogleaf will knock you out."},
	{"Harmonia: No, Hilbert."},
	{"I'LL KNOCK YOU OUT !!!"},
	{"..."},
	{"Hilbert: Harmonia, thank you for"},
	{"the bareback sex."},
	{"It teached me a good lesson about"},
	{"obeissance. I shouldn't go against"},
	{"my government exven if they are"},
	{"killing innocents or building walls."},
	{"Thank you so much."},
	{"Harmonia:..."},
	{"I love you."},
	{"Hilbert: Love you too."},
	{"THE END. THANKS FOR PLAYING"},
	{"LIKED THE GAME? STAYED TUNED FOR THE"},
	{"SEQUEL 'I WOULD RATHER EAT MY TOES'"},
	{"COMING WHEN THE US GETS"},
	{"A BASIC INCOME."},
	{"WHICH MEANS..."},
	{"NEVER !!!"},
};

const unsigned char face[53] =
{ 
	1, 0, 0, 3,
	0, 0, 0, 4,
	0, 0, 5, 5,
	5, 5, 5, 5,
	8, 8, 8, 8,
	8, 2, 0, 9,
	9, 0, 6, 0,
	2, 9, 1, 9,
	9, 0, 1, 9,
	
	// After mini-game, progress == 36
	13,13,13,13,
	13,13,13, 0,
	0, 13, 
	
	5, 5, 5, 5,
	5, 5, 14,
};


static unsigned char anybutton_pressed()
{
	if (buttons[0] == 1 || buttons[1] == 1 || buttons[2] == 1)
		return 1;
		
	return 0;
}

static void Reset_buttons()
{
	buttons[0] = 0;
	buttons[1] = 0;
	buttons[2] = 0;
}


static void change_pict(unsigned char pict)
{
	if (remember_back == pict)
		return;
		
	remember_back = pict;
	
	if (current_background) gbitmap_destroy(current_background);
	
	switch(pict)
	{
		case 0:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HARMONIA_0);
		break;
		case 1:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HARMONIA_1);
		break;
		case 2:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HARMONIA_2);
		break;
		case 3:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HILBERT_0);
		break;
		case 4:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HILBERT_1);
		break;
		case 5:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLACKSCREEN);
		break;
		case 6:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FUCK_0);
		break;
		case 7:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FUCK_1);
		break;
		case 8:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FUCK_2);
		break;
		case 9:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FUCK_0);
			if (back2) gbitmap_destroy(back2);
			back2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FUCK_1);
		break;
		case 10:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CUM_0);
		break;
		case 11:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CUM_1);
		break;
		case 12:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CUM_2);
		break;
		case 13:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HILBERT_2);
		break;
		case 14:
			current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLACKSCREEN);
			if (back2) gbitmap_destroy(back2);
			back2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_REDSCREEN);
		break;
	}
}


static void timer_handler(void *context)
{
   layer_mark_dirty(myCanvas);
   app_timer_register(34, timer_handler, NULL);
}

static void config_provider(Window *window)
{
   window_raw_click_subscribe(BUTTON_ID_SELECT, sel_click_handler, sel_release_handler, NULL);
   window_raw_click_subscribe(BUTTON_ID_UP, sel_click_handler_up, sel_release_handler_up, NULL);
   window_raw_click_subscribe(BUTTON_ID_DOWN, sel_click_handler_down, sel_release_handler_down, NULL);
}

static void Fuck_scene(GContext *ctx)
{
	if (titlescreen_press_start_time < 3)
	{		
		graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 140));
	}
	else
	{
		graphics_draw_bitmap_in_rect(ctx, back2, GRect(0, 0, 144, 140));
	}
	if (titlescreen_press_start_time > 4)
		titlescreen_press_start_time = 0;
}

static void Cum_scene(GContext *ctx)
{
	graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 140));
	
	if (titlescreen_press_start_time == 0)
	{		
		change_pict(10);
	}
	else if (titlescreen_press_start_time == 3)
	{		
		change_pict(11);
	}
	else if (titlescreen_press_start_time == 6)
	{
		change_pict(12);
	}
}

static void updateGame(Layer *layer, GContext *ctx)
{
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	graphics_context_set_text_color(ctx, GColorBlack);
	
	switch(game_mode)
	{
		// Titlescreen
		case 0:
			graphics_draw_text(ctx,"ABSTINENCE", fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), GRect(20, 0, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			graphics_draw_text(ctx,"OR DIE", fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), GRect(45, 24, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			graphics_draw_text(ctx,"A Forbidden love", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(25, 48, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

			titlescreen_press_start_time++;
			if (titlescreen_press_start_time < 10)
			{		
				graphics_draw_text(ctx,"SELECT TO START", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(20, 132, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			}
			else if (titlescreen_press_start_time > 20)
			{
				titlescreen_press_start_time = 0;
			}
			
			if (anybutton_pressed())
			{
				wait_press = 0;
				game_mode = 1;
				cum = 0;
				progress = 0;
				titlescreen_press_start_time = 0;
				remember_back = 100;
				change_pict(face[0]);
				Reset_buttons();
			}
		break;
		// In-game, basic visual novel engine
		case 1:

			if (face[progress] == 9 || face[progress] == 14)
			{
				titlescreen_press_start_time++;
				if (face[progress] == 14) titlescreen_press_start_time++;
				Fuck_scene(ctx);
			}	
			else
				graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 140));
			
			graphics_draw_text(ctx,text[progress], fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
						
			if (wait_press < 7) wait_press++;
			if (anybutton_pressed() && wait_press > 5)
			{
				progress++;
				change_pict(face[progress]);
				if (progress == 35) game_mode = 2;
				Reset_buttons();
			}
			
			if (progress >= 53)
				game_mode = 0;
		break;
		// The small mini-game where you have to fuck Hilbert once for all
		case 2:
			Fuck_scene(ctx);
			
			if (cum < 4)
				graphics_draw_text(ctx, "PRESS UP AND DOWN", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			else if (cum < 10)
				graphics_draw_text(ctx, "KEEP ON DOING IT !", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			else if (cum < 30)
				graphics_draw_text(ctx, "HE'S ABOUT TO DO IT !", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
				
			if (cum > 35)
			{
				game_mode = 3;
				titlescreen_press_start_time = 0;
				vibes_long_pulse();
			}
					
			if (anybutton_pressed())
			{
				vibes_short_pulse();
				cum++;
				titlescreen_press_start_time+=2;
				Reset_buttons();
			}
		
		break;
		// Hilbert came !
		case 3:
			Cum_scene(ctx);
			if (titlescreen_press_start_time<40) titlescreen_press_start_time+=1;
			
			if ((anybutton_pressed()) && titlescreen_press_start_time > 30)
			{
				wait_press = 0;
				game_mode = 1;
				progress = 36;
				change_pict(face[progress]);
				Reset_buttons();
			}
		break;
	}

}

static void init() 
{
   my_window = window_create();
   
   window_set_click_config_provider(my_window, (ClickConfigProvider)config_provider);

   myCanvas = layer_create(GRect(0, 0, 144, 168));

   window_stack_push(my_window, true);
   
   Layer* motherLayer = window_get_root_layer(my_window);
   layer_add_child(motherLayer, myCanvas);

   layer_set_update_proc(myCanvas, updateGame);
   app_timer_register(34, timer_handler, NULL);
}

static void deinit() 
{
   text_layer_destroy(text_layer);
   window_destroy(my_window);
}

int main(void) 
{
	init();
	app_event_loop();
	deinit();
}
