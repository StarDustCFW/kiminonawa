/*  
 * Copyright (c) 2018 Guillem96
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>
#include "menu/gui/gui_argon_menu.h"
#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"
#include "core/custom-gui.h"
#include "gfx/di.h"
#include "gfx/gfx.h"

#include "utils/types.h"
#include "utils/fs_utils.h"
#include "utils/dirlist.h"
#include "utils/util.h"
#include "utils/touch.h"
#include "utils/btn.h"
#include "core/launcher.h"
#include "core/payloads.h"
#include "core/custom-gui.h"

#include "mem/heap.h"

#define COLUMNS 4  
#define ROWS 2
#define ELEM_SIZE 230
#define MARGIN_TOP 100
#define MARGIN_LEFT 46
//static int tool_menu_rSD(void* param);
static int tool_extr_rSD(void* param);

static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);
static int tool_menu_rem(void* param);
int tool_theme(char* param);
static int tool_emu(u32 param);
static int zbackup(char* param);
static int tool_Menus(u32 param);
int tool_dir(char *param);
int tool_servises(u32 param);
int tool_servises_on(char* title);
int tool_servises_off(char* title);

int tool_filete(char* fil);
int tool_file_act(u32 fil);
//menus
u64 main_menu = 2;

//sub menus
u32 permsubY = 0;
u32 permsubX = 0;
u32 sub_buttonW = 0;
u32 sub_buttonH = 0;

//some indicators
u32 onemi = 0;
u32 retir = 0;

//dinamic directory
char *directory = "";
char *filete = "";

/* Generate entries dynamically */
static void generate_payloads_entries(char* payloads, gui_menu_t* menu)
{
    if (payloads == NULL)
    {
        g_gfx_con.scale = 4;
        gfx_con_setpos(&g_gfx_con, 140, 250);
        gfx_printf(&g_gfx_con, "Payloads directory is empty...\n");
        
        g_gfx_con.scale = 3;
        gfx_con_setpos(&g_gfx_con, 110, 370);
        gfx_printf(&g_gfx_con, "Place your payloads inside \"%s\"", PAYLOADS_DIR);

        return;
    }

    u32 i = 0;
    /* For each payload generate its logo, its name and its path */
    while(payloads[i * 256])
    {
        char* payload_path = (char*)malloc(256);
        payload_full_path(&payloads[i * 256], payload_path);
        
        char payload_logo[256];
        payload_logo_path(&payloads[i * 256], payload_logo);

			u32 row = i;
        u32 col = i % COLUMNS;
        u32 x = g_gfx_ctxt.width / COLUMNS * col + MARGIN_LEFT;
        u32 y = 140 / ROWS * row + MARGIN_TOP + MARGIN_TOP;

        const char* payload_wo_bin = str_replace(&payloads[i * 256], ".bin", "");
        gui_menu_append_entry(menu, 
            gui_create_menu_entry(payload_wo_bin,
                                    sd_file_read(payload_logo), 
                                    x, y,
                                    200, 200,
                                    (int (*)(void *))launch_payload, (void*)payload_path));
        i++;
    }
}
// Second Menu 
/*

static void generate_payloads_back(char* payback, gui_menu_t* menu)
{

    u32 e = 0;
    // For each payload generate its logo, its name and its path
    while(payback[e * 256])
    {
        char* payload_path = (char*)malloc(256);
        payload_full_back(&payback[e * 256], payload_path);
        
        char payload_logo[256];
        payload_logo_path(&payback[e * 256], payload_logo);

		u32 row = e / COLUMNS;
        u32 col = e % COLUMNS;
        u32 x = g_gfx_ctxt.width / COLUMNS * col + MARGIN_LEFT;
        u32 y = g_gfx_ctxt.height / ROWS * row + MARGIN_TOP + (row == 0 ? 30 : -60);

        const char* payload_wo_bin = str_replace(&payback[e * 256], ".bin", "");
        gui_menu_append_entry(menu, 
            gui_create_menu_entry(payload_wo_bin,
                                    sd_file_read(payload_logo),
                                    x, y,
                                    200, 200,
                                    (int (*)(void *))launch_payload, (void*)payload_path));
        e++;
	
    }
}
*/
/* Init needed menus for ArgonNX */
void gui_init_argon_menu(void)
{
    /* Init pool for menu */
    gui_menu_pool_init();
gui_menu_t* menu = gui_menu_create("ArgonNX");
sd_mount();


//		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("BETA WIP....", 500, 20, 150, 100, NULL, NULL));

	
//gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/arrowl.bmp"),100, 650, 70, 70,(int (*)(void *))tool_Menus, (void*)0));
char* files = listfil(directory, "*", true);
char* folder = listfol(directory, "*", true);
		
    u32 r = 0;
    u32 w = 0;
    u32 i = 0;
    u32 f = 0;
	u32 y = 90;
	u32 x = 10;
	u32 space = 50;
//	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory,600, y-20, 150, 100, (int (*)(void *))tool_Menus, (void*)33));
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),170, 30, 70, 70,(int (*)(void *))tool_Menus, (void*)33));
		if(!sd_file_exists ("StarDust/kimi.bmp"))
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("GO ROOT", 170, 40, 150, 100, NULL, NULL));

		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(directory, strlen(directory)*8-40+50, 98, 150, 100, NULL, NULL));
	
    while(folder[r * 256])
    {
	if(f >= 22){
	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("More...", 1100,670, 150, 100, NULL, NULL));
	break;}
		if(strlen(&folder[r * 256]) <= 100){			
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/folder.bmp"),x, y+30, 500, 25,(int (*)(void *))tool_dir, (void*)&folder[r * 256]));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&folder[r * 256], x+strlen(&folder[r * 256])*8-40, y+35, 150, 100, NULL, NULL));
		if(!sd_file_exists ("StarDust/Icons/folder.bmp"))
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("/", x-45, y+35, 150, 100, NULL, NULL));

		y = y + space;
		
		f++;
		i++;
			if(i == 11){
			y = 90;
			x = 700;
			i = 0;
			}
		}
	r++;
	}

    while(files[w * 256])
    {
	if(f >= 22){
	gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("More...", 1100,670, 150, 100, NULL, NULL));
	break;}
	if(strlen(&files[w * 256]) <= 100){
			if(strstr(&files[w * 256],".bin") != NULL)
			{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/files-bin.bmp"),x, y+30, 500, 25,(int (*)(void *))tool_filete, (void*)&files[w * 256]));
			if(!sd_file_exists ("StarDust/Icons/files-bin.bmp"))
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("*", x-45, y+35, 150, 100, NULL, NULL));
			}else{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/files.bmp"),x, y+30, 500, 25,(int (*)(void *))tool_filete, (void*)&files[w * 256]));			
			if(!sd_file_exists ("StarDust/Icons/files.bmp"))
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("-", x-45, y+35, 150, 100, NULL, NULL));
			}

		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(&files[w * 256], x+strlen(&files[w * 256])*8-40, y+35, 150, 100, NULL, NULL));
		y = y + space;

	f++;			
	i++;			
			
			if(i == 11){
			y = 90;
			x = 600;
			i = 0;
			}	

		}
	w++;
	}
		if(strlen(filete) >= 1)
		{
		u32 actX = 800;
		u32 actY = 1;
			if(strstr(filete,".bin") != NULL)
			{
			gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/launch.bmp"),actX+100, actY, 75, 75, (int (*)(void *))tool_file_act, (void*)1));
			if(!sd_file_exists ("StarDust/Icons/launch.bmp"))
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("RUN", actX+100, actY+20, 150, 100, NULL, NULL));
			}
			if(strstr(filete,".BIN") != NULL)
			{
			gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/launch.bmp"),actX+100, actY, 75, 75, (int (*)(void *))tool_file_act, (void*)1));
			if(!sd_file_exists ("StarDust/Icons/launch.bmp"))
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("RUN", actX+100, actY+20, 150, 100, NULL, NULL));
			}

		//gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/button.bmp"),1070, 600, 200, 75, (int (*)(void *))zbackup, (void*)"emummc.bk"));
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("/StarDust/Icons/del.bmp"),actX, actY, 75, 75, (int (*)(void *))tool_file_act, (void*)0));
			if(!sd_file_exists ("StarDust/Icons/del.bmp"))
			gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("DEL", actX, actY+20, 150, 100, NULL, NULL));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap(filete, actX, actY+70, 150, 100, NULL, NULL));
		}

 





/* Generate reboot rcm and shutdown entry **/
		if(!sd_file_exists ("StarDust/kimi.bmp")){
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("picture", 400, 700, 150, 100, NULL, NULL));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("power", 600, 700, 150, 100, NULL, NULL));
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("RCM", 800, 700, 150, 100, NULL, NULL));
		}
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),400, 650, 70, 70, (int (*)(void *))screenshot, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),600, 650, 70, 70, tool_power_off, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read(""),800, 650, 70, 70, tool_reboot_rcm, NULL));




    /* Start menu */
    gui_menu_open(menu);

    /* Clear all entries and menus */
    gui_menu_pool_cleanup();
}

static int tool_reboot_rcm(void* param)
{
   reboot_rcm();
    return 0;
}

static int tool_Menus(u32 param)
{
if (!sd_mount()){BootStrapNX();}//check sd

if(param == 33){
directory = "";
gui_init_argon_menu();
return 0;
}
display_backlight_brightness(50, 1000);
//check if is the same menu
if(main_menu == param)
return 0;

//set menu number
main_menu = param;

//gui_menu_pool_cleanup();
gui_init_argon_menu();
return 0;
}


int tool_dir(char *param)
{
if (!sd_mount()){BootStrapNX();}//check sd
if(strlen(directory) <= 1)
{
directory = param;
}else{
strcat(directory, "/");
strcat(directory, param);

}
gui_init_argon_menu();
return 0;
}

int tool_filete(char* fil)
{

if (!sd_mount()){BootStrapNX();}//check sd
if(strlen(directory) <= 1)
{
filete = fil;
}else{
    char tmp[256];
    strcpy(tmp, directory);
    strcat(tmp, "/");
	strcat(tmp, fil);
filete = tmp;		
}
gui_init_argon_menu();
return 0;
}

int tool_file_act(u32 fil)
{
if (!sd_mount()){BootStrapNX();}//check sd
	if(fil == 0)
	{
	f_unlink(filete);
	free(filete);
	filete = "";
	}
	
	if(fil == 1)
	{
	launch_payload(filete);
	}
gui_init_argon_menu();
return 0;
}
static int tool_power_off(void* param)
{
    gui_menu_pool_cleanup();
    power_off();
    return 0;
}
