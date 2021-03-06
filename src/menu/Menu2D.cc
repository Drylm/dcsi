#include "Menu2D.h"
#include "MenuItem.h"

pvr_init_params_t params = {
    /* Enable opaque and translucent polygons with size 16 */
    {PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_16, PVR_BINSIZE_0, PVR_BINSIZE_0},

    /* Vertex buffer size 512K */
    512 * 512
};

Menu2D ::Menu2D(const FontRenderer& font_renderer)
    : font_renderer_(font_renderer)
{
    //pvr_init(&params);
    pvr_init_defaults();
    textures = new pvr_ptr_t[9];
    auto pers_carlos = new Perso("carlos", 45, "Oisis Island", "Oisis Bottle");
    auto pers_momo = new Perso("momo", 20, "Massillia", "Skate of McFly");
    auto pers_noel = new Perso("noel", 500, "North pole", "Magic gift");

    pvr_ptr_t menuItemBackground = pvr_mem_malloc(1024 * 1024 * 2);
    png_to_texture("/rd/menupics/fondgris.png", menuItemBackground, PNG_FULL_ALPHA);

    menu_items_.clear();
    menu_items_.push_back(new MenuItem(font_renderer_, pers_carlos, "/rd/menupics/carlos.png", menuItemBackground));
    menu_items_.push_back(new MenuItem(font_renderer_, pers_momo, "/rd/menupics/momo.png", menuItemBackground));
    menu_items_.push_back(new MenuItem(font_renderer_, pers_noel, "/rd/menupics/noel.png", menuItemBackground));

    textures[0] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/menubase.png", textures[0], PNG_FULL_ALPHA);
    textures[1] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/team.png", textures[1], PNG_FULL_ALPHA);
    textures[2] = pvr_mem_malloc(256 * 256 * 2);
    png_to_texture("/rd/menupics/cadreselect.png", textures[2], PNG_FULL_ALPHA);
    textures[3] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/1.png", textures[3], PNG_FULL_ALPHA);
    textures[4] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/2.png", textures[4], PNG_FULL_ALPHA);
    textures[5] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/3.png", textures[5], PNG_FULL_ALPHA);
    textures[6] = pvr_mem_malloc(256 * 256 * 2);
    png_to_texture("/rd/menupics/Go.png", textures[6], PNG_FULL_ALPHA);
    textures[7] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/fondcolor.png", textures[7], PNG_FULL_ALPHA);


    pos = 0;
    posy = 85.0;
    poscadre = 0;
    poscadrex = 0.0;

    filter_mode = 0;
}

Menu2D::~Menu2D()
{
    delete[] textures;
}

void Menu2D::Resetfont(void)
{
    filter_mode = 0;
}

void Menu2D::Resetall()
{
    textures = new pvr_ptr_t[9];
    auto pers_carlos = new Perso("carlos", 45, "Oisis Island", "Oisis Bottle");
    auto pers_momo = new Perso("momo", 20, "Massillia", "Skate of McFly");
    auto pers_noel = new Perso("noel", 500, "North pole", "Magic gift");

    pvr_ptr_t menuItemBackground = pvr_mem_malloc(1024 * 1024 * 2);
    png_to_texture("/rd/menupics/fondgris.png", menuItemBackground, PNG_FULL_ALPHA);

    menu_items_.clear();
    menu_items_.push_back(new MenuItem(font_renderer_, pers_carlos, "/rd/menupics/carlos.png", menuItemBackground));
    menu_items_.push_back(new MenuItem(font_renderer_, pers_momo, "/rd/menupics/momo.png", menuItemBackground));
    menu_items_.push_back(new MenuItem(font_renderer_, pers_noel, "/rd/menupics/noel.png", menuItemBackground));

    textures[0] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/menubase.png", textures[0], PNG_FULL_ALPHA);
    textures[1] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/team.png", textures[1], PNG_FULL_ALPHA);
    textures[2] = pvr_mem_malloc(256 * 256 * 2);
    png_to_texture("/rd/menupics/cadreselect.png", textures[5], PNG_FULL_ALPHA);
    textures[3] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/1.png", textures[6], PNG_FULL_ALPHA);
    textures[4] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/2.png", textures[7], PNG_FULL_ALPHA);
    textures[5] = pvr_mem_malloc(64 * 64 * 2);
    png_to_texture("/rd/menupics/3.png", textures[8], PNG_FULL_ALPHA);
    textures[6] = pvr_mem_malloc(256 * 256 * 2);
    png_to_texture("/rd/menupics/Go.png", textures[9], PNG_FULL_ALPHA);
    textures[7] = pvr_mem_malloc(1024 * 1024 * 2);
    png_to_texture("/rd/menupics/fondgris.png", textures[10], PNG_FULL_ALPHA);
    textures[8] = pvr_mem_malloc(512 * 512 * 2);
    png_to_texture("/rd/menupics/fondcolor.png", textures[11], PNG_FULL_ALPHA);

    pos = 0;
    posy = 85.0;
    poscadre = 0;
    poscadrex = 0.0;

    filter_mode = 0;
}

/* Methode pour faire appariatre le titre en transparence*/
void Menu2D::draw_tr_intro(float alpha)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t vert;

    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB4444, 512, 512, textures[team], PVR_FILTER_BILINEAR);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(alpha, 1.0f, 1.0f, 1.0f);
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;

    vert.x = 64;
    vert.y = -16;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 576;
    vert.y = -16;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 64;
    vert.y = 496;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 1.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 576;
    vert.y = 496;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 1.0;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
}

void Menu2D::draw_alp_tex_box(float x1, float y1, float x2, float y2, float z, int taille, pvr_ptr_t texture, float alpha)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t vert;

    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB4444, taille, taille, texture, PVR_FILTER_BILINEAR);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(alpha, 1.0f, 1.0f, 1.0f);
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;

    vert.x = x1;
    vert.y = y1;
    vert.z = z;
    vert.u = 0.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = x2;
    vert.y = y1;
    vert.z = z;
    vert.u = 1.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = x1;
    vert.y = y2;
    vert.z = z;
    vert.u = 0.0;
    vert.v = 1.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = x2;
    vert.y = y2;
    vert.z = z;
    vert.u = 1.0;
    vert.v = 1.0;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
}


void Menu2D::draw_menu()
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t vert;

    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB4444, 512, 512, textures[menu], PVR_FILTER_BILINEAR);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(0.6f, 1.0f, 1.0f, 1.0f);
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;

    vert.x = 70;
    vert.y = 50;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 570;
    vert.y = 50;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 70;
    vert.y = 430;
    vert.z = 1;
    vert.u = 0.0;
    vert.v = 1.0;
    pvr_prim(&vert, sizeof(vert));

    vert.x = 570;
    vert.y = 430;
    vert.z = 1;
    vert.u = 1.0;
    vert.v = 1.0;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
}

void Menu2D::draw_poly_box(float x1, float y1, float x2, float y2, float z, float a1, float r1, float g1, float b1, float a2, float r2, float g2, float b2)
{
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t poly;
    pvr_vertex_t vert;

    pvr_poly_cxt_col(&cxt, PVR_LIST_TR_POLY);
    pvr_poly_compile(&poly, &cxt);
    pvr_prim(&poly, sizeof(poly));

    vert.flags = PVR_CMD_VERTEX;
    vert.x = x1;
    vert.y = y2;
    vert.z = z;
    vert.argb = PVR_PACK_COLOR(
        (a1 + a2) / 2,
        (r1 + r2) / 2,
        (g1 + g2) / 2,
        (b1 + b2) / 2);
    vert.oargb = 0;
    pvr_prim(&vert, sizeof(vert));

    vert.y = y1;
    vert.argb = PVR_PACK_COLOR(a1, r1, g1, b1);
    pvr_prim(&vert, sizeof(vert));

    vert.x = x2;
    vert.y = y2;
    vert.argb = PVR_PACK_COLOR(a2, r2, g2, b2);
    pvr_prim(&vert, sizeof(vert));

    vert.flags = PVR_CMD_VERTEX_EOL;
    vert.y = y1;
    vert.argb = PVR_PACK_COLOR(
        (a1 + a2) / 2,
        (r1 + r2) / 2,
        (g1 + g2) / 2,
        (b1 + b2) / 2);
    pvr_prim(&vert, sizeof(vert));
}

void Menu2D::draw_intro()
{
    float alpha = 0.0;
    int i, done = 0;
    int cpt = 0;

    ogg introduction("/rd/IntroductionShort.ogg");
    wav DcsiPresents("/rd/DcsiPresents.wav");
    wav Pause("/rd/Pause.wav");

    introduction.play(1);
    for (i = 0; i < 100; i++)
    {
        pvr_wait_ready();
        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        pvr_list_finish();

        pvr_list_begin(PVR_LIST_TR_POLY);
        draw_poly_box(1.0f, 1.0f, 640.0f, 480.0f, 0.1f, 0.99f, 1.0f, 1.0f, 1.0f, 0.99f, 1.0f, 1.0f, 1.0f);
        draw_tr_intro(alpha);
        pvr_list_finish();
        pvr_scene_finish();
        alpha += 0.01;
        thd_sleep(50);
    }
    DcsiPresents.play();
    thd_sleep(2000);
    vmu_lcd_update(1);
    draw_save_init();
    while (!done)
    {
        MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        if (st->buttons & CONT_START)
            done = 1;
        MAPLE_FOREACH_END()
        pvr_wait_ready();
        pvr_scene_begin();
        pvr_list_begin(PVR_LIST_TR_POLY);
        draw_alp_tex_box(0.0, 0.0, 640.0, 600.0, 0.1, 512, textures[fondcolor], 1.0);
        if (cpt < 75)
            font_renderer_.WriteWord("Press Start !", filter_mode, 30, 180, 350, 0.4, 0.7, 0.93);
        pvr_list_finish();
        pvr_scene_finish();
        cpt = (cpt + 1) % 100;
        thd_sleep(20);
    }
    Pause.play();
    thd_sleep(200);
    vmu_lcd_update(0);
    introduction.stop();
    DcsiPresents.stop();
    Pause.stop();
}

void Menu2D::draw_genEric(int toplay)
{
    /*  printf("je lance le film %d\n",toplay);
  if(toplay==0)
    {
      video Eric("/rd/video/Eric.MPG");
      Eric.play();
      thd_sleep(4000);
    }
  else if(toplay==1)
    {
      video Tom("/rd/video/Tom.MPG");
      Tom.play();
      thd_sleep(5300);
    }
  else if(toplay==2)
    {
      video Gui("/rd/video/Gui.MPG");
      Gui.play();
      thd_sleep(5040);
    }
  else if(toplay==3)
    {
      video John("/rd/video/John.MPG");
      John.play();
      thd_sleep(5200);
    }
  else if(toplay==4)
    {
      video Mike("/rd/video/Mike.MPG");
      Mike.play();
      thd_sleep(8000);
    }

    pvr_shutdown();
    vid_shutdown();
    pvr_init(&params);
    Resetfont();
*/
}

void Menu2D::draw_gen_font(int choixtexte)
{
    for (int i = 0; i < 70; i++)
    {
        pvr_wait_ready();
        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        pvr_list_finish();

        pvr_list_begin(PVR_LIST_TR_POLY);
        draw_poly_box(0.0f, 0.0f, 640.0f, 480.0f, 0.1f, 0.9f, 0.0f, 0.0f, 0.0f, 0.9f, 0.0f, 0.0f, 0.0f);
        if (choixtexte == 0)
        {
            font_renderer_.WriteWord("Hyun_Chul06", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Physical Engine", filter_mode,  18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Application Synchroniser", filter_mode,  18, 80, 205, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("\"GPL is not dead\"", filter_mode,  18, 80, 255, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 1)
        {
            font_renderer_.WriteWord("HebusXIII", filter_mode,  25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("2D Engine(SDL,PVR)", filter_mode,  18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("VMU Managing", filter_mode, 18, 80, 205, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("3D and textures design", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Drawings", filter_mode, 18, 80, 280, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("\"Charles Ingalls\"", filter_mode, 18, 80, 330, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("The little house on prairie creator", filter_mode, 18, 80, 355, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 2)
        {
            font_renderer_.WriteWord("Imhotep", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Collision system", filter_mode, 18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Camera Engine", filter_mode, 18, 80, 205, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Voices", filter_mode, 18, 80, 230, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Specifications", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("\"Demain matin �� 15h... C est bon ?\"", filter_mode, 18, 80, 305, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 3)
        {
            font_renderer_.WriteWord("Bhaal22", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("3D and camera engine", filter_mode, 18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("System developement", filter_mode, 18, 80, 205, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Preliminaries studies", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("\"The gnou : an animal really?\"", filter_mode, 18, 80, 305, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Dormir? J essaie d arreter", filter_mode, 18, 80, 330, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 4)
        {
            font_renderer_.WriteWord("Sypher", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Sound and video Engine", filter_mode, 18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("2D design and 3D object developement", filter_mode, 18, 80, 205, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Multimedia and communication", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Musical and sonor compositor", filter_mode, 18, 80, 280, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("\"Attention j upload\"", filter_mode, 18, 80, 330, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("DJ Marcas aka \"le Raleur\"", filter_mode, 18, 80, 355, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 5)
        {
            font_renderer_.WriteWord("Special thanks", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Ultimo", filter_mode, 18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Wyvern", filter_mode, 18, 80, 230, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("http://www.truemetal.org/wyvern/", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Vicent R (Euclidian transformations)", filter_mode, 18, 80, 280, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("Dan Potter for KOS lib", filter_mode, 18, 80, 330, 1.0, 1.0, 1.0);
        }
        else if (choixtexte == 6)
        {
            font_renderer_.WriteWord("Thanks for playing", filter_mode, 25, 180, 100, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("The DreamTeamCast :", filter_mode, 18, 80, 180, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("BOLORE Eric", filter_mode, 18, 80, 230, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("LEGRAND Guillaume", filter_mode, 18, 80, 255, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("MARTIN Mickael", filter_mode, 18, 80, 280, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("MULLER Jonathan", filter_mode, 18, 80, 305, 1.0, 1.0, 1.0);
            font_renderer_.WriteWord("TRIOL Thomas", filter_mode, 18, 80, 330, 1.0, 1.0, 1.0);
        }
        pvr_list_finish();
        pvr_scene_finish();
        thd_sleep(100);
    }
}

/*affichage du menu de selection des personnages*/
void Menu2D::draw_perso()
{
    pvr_wait_ready();
    pvr_scene_begin();

    pvr_list_begin(PVR_LIST_OP_POLY);

    pvr_list_finish();

    pvr_list_begin(PVR_LIST_TR_POLY);
    draw_alp_tex_box(0.0, 0.0, 1024.0, 1024.0, 0.1, 1024, textures[fondgris], 1.0);
    draw_poly_box(30.0f, 30.0f, 610.0f, 450.0f, 0.1f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.8f, 0.8f, 0.8f);
    draw_alp_tex_box(0.0, 0.0, 256.0, 256.0, 1.0, 256, textures[carlos], 1.0);
    draw_alp_tex_box(192.0, 0.0, 448.0, 256.0, 1.0, 256, textures[momo], 1.0);
    draw_alp_tex_box(384.0, 0.0, 640.0, 256.0, 1.0, 256, textures[noel], 1.0);
    draw_alp_tex_box(poscadrex, 0.0, poscadrex + 256.0, 256.0, 1.0, 256, textures[select], 1.0);
    if (poscadre == 0)
        font_renderer_.WriteWord("Carlos", filter_mode, 25, 270, 320, 0.0, 0.0, 0.0);
    else if (poscadre == 1)
        font_renderer_.WriteWord("Momo", filter_mode, 25, 275, 320, 0.0, 0.0, 0.0);
    else if (poscadre == 2)
        font_renderer_.WriteWord("Noel", filter_mode, 25, 275, 320, 0.0, 0.0, 0.0);
    pvr_list_finish();
    pvr_scene_finish();
}

/* Affichage des menus*/
void Menu2D::draw_frame(void)
{
    pvr_wait_ready();
    pvr_scene_begin();

    pvr_list_begin(PVR_LIST_OP_POLY);

    pvr_list_finish();

    pvr_list_begin(PVR_LIST_TR_POLY);
    draw_alp_tex_box(0.0, 0.0, 1024.0, 1024.0, 0.1, 1024, textures[fondgris], 1.0);
    draw_menu();
    draw_poly_box(80.0f, posy, 560.0f, posy + 40.0f, 1.1f, 0.4f, 1.0f, 0.9f, 0.1f, 0.4f, 1.0f, 0.9f, 0.1f);
    pvr_list_finish();
    pvr_scene_finish();
}

/*Affichage de la creation de sauvegarde*/
void Menu2D::draw_save_init()
{
    int done = 0;
    if (!check_savefile())
    {
        while (!done)
        {
            MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
            if (st->buttons & CONT_START)
                done = 1;
            MAPLE_FOREACH_END()
            pvr_wait_ready();
            pvr_scene_begin();

            pvr_list_begin(PVR_LIST_TR_POLY);
            draw_poly_box(1.0f, 1.0f, 640.0f, 480.0f, 0.1f, 0.99f, 1.0f, 1.0f, 1.0f, 0.99f, 1.0f, 1.0f, 1.0f);
            draw_poly_box(10.0f, 10.0f, 630.0f, 470.0f, 0.0f, 0.3f, 0.6f, 0.55f, 0.9f, 0.3f, 0.6f, 0.55f, 0.9f);
            font_renderer_.WriteWord("Press start to create a DCSI file", filter_mode, 25, 30, 200, 0.0, 0.0, 0.0);
            font_renderer_.WriteWord("on your VMU", filter_mode, 25, 200, 240, 0.0, 0.0, 0.0);
            pvr_list_finish();
            pvr_scene_finish();
        }
        write_on_vmu();
    }
}

void Menu2D::draw_perso_choisi()
{
    menu_items_[poscadre]->RenderDetails(filter_mode);
}

void Menu2D::vmu_lcd_update(int choixim)
{
    if (choixim == 0)
        vmu_set_icon(tex_blanc);
    else if (choixim == 1)
        vmu_set_icon(tex_game);
    else if (choixim == 2)
        vmu_set_icon(tex_carlos);
    else if (choixim == 3)
        vmu_set_icon(tex_1);
    else if (choixim == 4)
        vmu_set_icon(tex_2);
    else if (choixim == 5)
        vmu_set_icon(tex_3);
    else if (choixim == 6)
        vmu_set_icon(tex_go);
}

/*Affichage du compte �� rebour*/
void Menu2D::draw_game(void)
{
    int i, done;
    done = i = 0;
    while (!done)
    {
        MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        if (st->buttons & CONT_START)
            done = 1;
        MAPLE_FOREACH_END()
        pvr_wait_ready();
        pvr_scene_begin();
        pvr_list_begin(PVR_LIST_TR_POLY);
        draw_poly_box(1.0f, 1.0f, 640.0f, 480.0f, 0.1f, 0.99f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        if (i == 0)
        {
            draw_alp_tex_box(288.0, 208.0, 352.0, 272.0, 1.0, 64, textures[three], 1.0);
        }
        if (i == 1)
        {
            wav c1("/rd/3.wav");
            c1.play();
            draw_alp_tex_box(288.0, 208.0, 352.0, 272.0, 1.0, 64, textures[two], 1.0);
            vmu_lcd_update(5);
            c1.stop();
        }
        if (i == 2)
        {
            draw_alp_tex_box(288.0, 208.0, 352.0, 272.0, 1.0, 64, textures[one], 1.0);
            wav c2("/rd/2.wav");
            c2.play();
            vmu_lcd_update(4);
            c2.stop();
        }
        if (i == 3)
        {
            draw_alp_tex_box(192.0, 112.0, 448.0, 368.0, 1.0, 256, textures[go], 1.0);
            wav c3("/rd/1.wav");
            c3.play();
            vmu_lcd_update(3);
            c3.stop();
        }
        if (i == 4)
        {
            wav Gooo("/rd/Go.wav");
            Gooo.play();
            vmu_lcd_update(6);
            Gooo.stop();
        }
        if (i > 4)
            vmu_lcd_update(0);
        pvr_list_finish();
        pvr_scene_finish();
        i++;
        thd_sleep(1000);
    }
}

void Menu2D::write_on_vmu()
{
    vmu_pkg_t pkg;
    uint8 data[1024], *pkg_out;
    int pkg_size;
    int i, j;
    file_t f;
    const char *mot = "mom 1.10.123 noe 1.12.198 car 1.15.356";

    strcpy(pkg.desc_short, "DCSI");
    strcpy(pkg.desc_long, "Driving Curse Syndrom : Ignition");
    strcpy(pkg.app_id, "DreamTeamCast");
    pkg.icon_cnt = 0;
    pkg.icon_anim_speed = 0;
    pkg.eyecatch_type = VMUPKG_EC_NONE;
    pkg.data_len = 1024;
    pkg.data = data;

    for (i = 0; i < 1023 - 38; i++)
        data[i] = (uint8)'a';
    for (j = 0; mot[j] != '\0'; j++)
        data[i + j] = (uint8)mot[j];
    data[1023] = '\0';

    vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

    fs_unlink("/vmu/a1/DCSIRecord");
    f = fs_open("/vmu/a1/DCSIRecord", O_WRONLY);
    if (!f)
    {
        printf("error writing\n");
        return;
    }
    fs_write(f, pkg_out, pkg_size);
    fs_close(f);
}

int Menu2D::check_savefile(void)
{
    file_t d;

    d = fs_open("/vmu/a1/DCSIRecord", O_RDONLY);
    if (!d)
    {
        return 0;
    }
    else
        return 1;
}

/* Fais un screenshot*/
void Menu2D::screenShot(void)
{
    printf("shot launch!\n");
    static int count = 1;
    char buffer[256];
    sprintf(buffer, "/pc/tmp/shot%4d.ppm", count++);
    vid_screen_shot(buffer);
    printf("shot done!");
}

/*Lance le menu de pause*/
void Menu2D::draw_pause()
{
    pvr_wait_ready();
    pvr_scene_begin();

    pvr_list_begin(PVR_LIST_TR_POLY);

    draw_alp_tex_box(192.0, 112.0, 448.0, 320.0, 0.1, 256, textures[pause], 0.6);
    draw_poly_box(192.0, 172.0 + pospause * 45, 448.0f, 172 + pospause * 45 + 20.0f, 1.1f, 0.4f, 1.0f, 0.9f, 0.1f, 0.4f, 1.0f, 0.9f, 0.1f);

    pvr_list_finish();
    pvr_scene_finish();
}

void Menu2D::draw_records()
{
    file_t f;
    char buff[1024];
    char mesrecords[39];
    int i, j;
    int valret = 1;
    int done = 0;

    f = fs_open("/vmu/a1/DCSIRecord", O_RDONLY);
    if (!f)
    {
        printf("error reading\n");
        return;
    }
    while (valret)
    {
        valret = fs_read(f, buff, 1024);
    }
    for (i = 0; buff[i] == 'a'; i++)
    {
    }
    for (j = 0; buff[i + j] != '\0'; j++)
        mesrecords[j] = buff[i + j];
    mesrecords[38] = '\0';
    fs_close(f);

    char mot1[11], mot2[11], mot3[11];
    char nom1[4], nom2[4], nom3[4];
    int k, l, m;
    j = 0;
    k = 1;
    for (i = 0; j != 2; i++)
    {
        if (i < 3)
            nom1[i] = mesrecords[i];
        else
        {
            if (mesrecords[i] == ' ')
                j++;
            if (mesrecords[i] == '.')
                k++;
            mot1[i - 3] = mesrecords[i];
            if (k == 2 && mesrecords[i] == '.')
                mot1[i - 3] = '\"';
            if (k == 3 && mesrecords[i] == '.')
                mot1[i - 3] = '\"';
        }
    }
    mot1[10] = '\0';
    j = 0;
    k = 1;
    for (l = 0; j != 2; l++)
    {
        if (l < 3)
            nom2[l] = mesrecords[l + i];
        else
        {
            if (mesrecords[l + i] == ' ')
                j++;
            if (mesrecords[l + i] == '.')
                k++;
            mot2[l - 3] = mesrecords[i + l];
            if (k == 2 && mesrecords[i + l] == '.')
                mot2[l - 3] = '\"';
            if (k == 3 && mesrecords[i + l] == '.')
                mot2[l - 3] = '\"';
        }
    }
    mot2[10] = '\0';
    j = 0;
    k = 1;
    for (m = 0; j != 2; m++)
    {
        if (m < 3)
            nom3[m] = mesrecords[m + l + i];
        else
        {
            if (mesrecords[m + l + i] == ' ' || mesrecords[m + l + i] == '\0')
                j++;
            if (mesrecords[m + l + i] == '.')
                k++;
            mot3[m - 3] = mesrecords[i + l + m];
            if (k == 2 && mesrecords[i + l + m] == '.')
                mot3[m - 3] = '\"';
            if (k == 3 && mesrecords[i + l + m] == '.')
                mot3[m - 3] = '\"';
        }
    }
    mot3[10] = '\0';
    nom1[3] = nom2[3] = nom3[3] = '\0';

    while (!done)
    {
        MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        if (st->buttons & CONT_B)
            done = 1;
        MAPLE_FOREACH_END()
        pvr_wait_ready();
        pvr_scene_begin();

        pvr_list_begin(PVR_LIST_OP_POLY);

        pvr_list_finish();

        pvr_list_begin(PVR_LIST_TR_POLY);
        draw_alp_tex_box(0.0, 0.0, 1024.0, 1024.0, 0.1, 1024, textures[fondgris], 1.0);
        if (!strcmp(nom1, "mom"))
            draw_alp_tex_box(180.0, 130.0, 244.0, 194.0, 1.0, 256, textures[momo], 1.0);
        else if (!strcmp(nom1, "noe"))
            draw_alp_tex_box(180.0, 130.0, 244.0, 194.0, 1.0, 256, textures[noel], 1.0);
        else if (!strcmp(nom1, "car"))
            draw_alp_tex_box(180.0, 130.0, 244.0, 194.0, 1.0, 256, textures[carlos], 1.0);

        if (!strcmp(nom2, "mom"))
            draw_alp_tex_box(180.0, 170.0, 244.0, 234.0, 1.0, 256, textures[momo], 1.0);
        else if (!strcmp(nom2, "noe"))
            draw_alp_tex_box(180.0, 170.0, 244.0, 234.0, 1.0, 256, textures[noel], 1.0);
        else if (!strcmp(nom2, "car"))
            draw_alp_tex_box(180.0, 170.0, 244.0, 234.0, 1.0, 256, textures[carlos], 1.0);

        if (!strcmp(nom3, "mom"))
            draw_alp_tex_box(180.0, 210.0, 244.0, 274.0, 1.0, 256, textures[momo], 1.0);
        else if (!strcmp(nom3, "noe"))
            draw_alp_tex_box(180.0, 210.0, 244.0, 274.0, 1.0, 256, textures[noel], 1.0);
        else if (!strcmp(nom3, "car"))
            draw_alp_tex_box(180.0, 210.0, 244.0, 274.0, 1.0, 256, textures[carlos], 1.0);

        font_renderer_.WriteWord("Records", filter_mode, 25, 220, 110, 0.0, 0.0, 0.0);
        font_renderer_.WriteWord(mot1, filter_mode, 25, 240, 180, 0.0, 0.0, 0.0);
        font_renderer_.WriteWord(mot2, filter_mode, 25, 240, 220, 0.0, 0.0, 0.0);
        font_renderer_.WriteWord(mot3, filter_mode, 25, 240, 260, 0.0, 0.0, 0.0);
        pvr_list_finish();
        pvr_scene_finish();
        thd_sleep(110);
    }
}
