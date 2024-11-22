/* nuklear - v1.05 - public domain */

#if AUDIO_EFFECTS_TESTER
#include "toolkit_gui.h"
#include "tester_effect.h"
#include "gui_common.h"
#include "nuklear.h"
#include "sound_picker.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define NK_MEMSET memset
/* macros */
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#ifdef __APPLE__
  #define NK_SHADER_VERSION "#version 150\n"
#else
  #define NK_SHADER_VERSION "#version 300 es\n"
#endif

#define MAX_SLIDER_COUNT 15

struct media {
	struct nk_font *font_13;
	struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;
    struct nk_image unchecked;
    struct nk_image checked;
    struct nk_image rocket;
    struct nk_image cloud;
    struct nk_image pen;
    struct nk_image play;
    struct nk_image pause;
    struct nk_image stop;
    struct nk_image prev;
    struct nk_image next;
    struct nk_image tools;
    struct nk_image dir;
    struct nk_image copy;
    struct nk_image convert;
    struct nk_image del;
    struct nk_image edit;
    struct nk_image images[9];
    struct nk_image menu[6];
};
typedef struct {
	int display_sliders;
	uint8_t effect_selected;
	EFFECT_COMPONENT *component;
	uint8_t slider_count;
	SLIDER_VALUES sliders[MAX_SLIDER_COUNT];
} DISPLAY_STATE;


typedef struct MUSIC_STATE {
	char *fileName;
	int music_is_playing;
	int start_music;
	int stop_music;
} MUSIC_STATE;

MUSIC_STATE gMUSIC;
DISPLAY_STATE gGUI;

void selected_music_file (char * fileName) {
	if (fileName) {
		gMUSIC.fileName = fileName;
		if (gGUI.effect_selected) {
			if (gMUSIC.music_is_playing) {
				gMUSIC.stop_music = 1;
			}
			gMUSIC.start_music = 1;
		}
	}
}


EFFECT_ITEM *g_effect_list = NULL;
uint8_t g_effects_count = 0;

static void initializeMusicState(MUSIC_STATE *music) {
	music->music_is_playing = 0;
	music->start_music = 0;
	music->stop_music = 0;
	//music->fileName = 0;
}
void update_effect_state_for_slider(SLIDER_VALUES *sliders, uint8_t index) {
	EFFECT_PARAMS *parameter = sliders[index].myParameter;
	void *effect = parameter->myEffect;
	if (parameter->recalculate) {
		if (parameter->previousValue != 0 && 
				(*(parameter->currentValue) != *(parameter->previousValue))) 
		{
			parameter->recalculate(effect);
			*(parameter->previousValue) = *(parameter->currentValue);
			EFFECT_PARAMS *pParameter = parameter->partnerParameter;
			if (pParameter) {
				if (pParameter->previousValue != 0 && 
						(*(pParameter->currentValue) != *(pParameter->previousValue)))
				{
					*(pParameter->previousValue) = *(pParameter->currentValue);
				}	

			}
		}
	}
	else if (parameter->previousValue != 0) { // if we don't use recalc
		*(parameter->previousValue) = *(parameter->currentValue);
	}
}
void update_effect_state(SLIDER_VALUES *sliders, uint8_t slider_count) {
	for (uint8_t index=0; index < slider_count; ++index) {
		update_effect_state_for_slider(sliders, index);
	}
}

void update_state_periodically () {
	static uint32_t count = 0;
	uint8_t index = count % gGUI.slider_count;
	update_effect_state_for_slider(gGUI.sliders, index);
	if (count++ == 400) {
		count = 0;
	}
}

static void setupSliders(DISPLAY_STATE *gui, EFFECT_COMPONENT * component);

static void clearDisplayState (DISPLAY_STATE * gui) {
	gui->display_sliders = 0;
	gui->effect_selected = 0;
	gui->slider_count = 0;
	if (gui->component) {
		freeComponent(gui->component);
		gui->component = NULL;
	}
}
static void initializeDisplayState(DISPLAY_STATE *gui, uint8_t selection) {
	clearDisplayState (gui);
	gui->component = createComponent(g_effect_list[selection].name, 0, 0);
	gui->effect_selected = 1;
	if (gui->component != NULL) {
		addChildComponent(gui->component, "Volume");
		gui->component->effect_bypass = 0;
		setupSliders(gui, gui->component);
		gui->display_sliders = 1;
	}
}
/* ===============================================================
 *
 *                          CUSTOM WIDGET
 *
 * ===============================================================*/
	static int
ui_piemenu(struct nk_context *ctx, struct nk_vec2 pos, float radius,
		struct nk_image *icons, int item_count)
{
	int ret = -1;
	struct nk_rect total_space;
	struct nk_rect bounds;
	int active_item = 0;

	/* pie menu popup */
	struct nk_color border = ctx->style.window.border_color;
	struct nk_style_item background = ctx->style.window.fixed_background;
	ctx->style.window.fixed_background = nk_style_item_hide();
	ctx->style.window.border_color = nk_rgba(0,0,0,0);

	total_space  = nk_window_get_content_region(ctx);
	ctx->style.window.spacing = nk_vec2(0,0);
	ctx->style.window.padding = nk_vec2(0,0);

	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "piemenu", NK_WINDOW_NO_SCROLLBAR,
				nk_rect(pos.x - total_space.x - radius, pos.y - radius - total_space.y,
					2*radius,2*radius)))
	{
		int i = 0;
		struct nk_command_buffer* out = nk_window_get_canvas(ctx);
		const struct nk_input *in = &ctx->input;

		total_space = nk_window_get_content_region(ctx);
		ctx->style.window.spacing = nk_vec2(4,4);
		ctx->style.window.padding = nk_vec2(8,8);
		nk_layout_row_dynamic(ctx, total_space.h, 1);
		nk_widget(&bounds, ctx);

		/* outer circle */
		nk_fill_circle(out, bounds, nk_rgb(50,50,50));
		{
			/* circle buttons */
			float step = (2 * 3.141592654f) / (float)(MAX(1,item_count));
			float a_min = 0; float a_max = step;

			struct nk_vec2 center = nk_vec2(bounds.x + bounds.w / 2.0f, bounds.y + bounds.h / 2.0f);
			struct nk_vec2 drag = nk_vec2(in->mouse.pos.x - center.x, in->mouse.pos.y - center.y);
			float angle = (float)atan2(drag.y, drag.x);
			if (angle < -0.0f) angle += 2.0f * 3.141592654f;
			active_item = (int)(angle/step);

			for (i = 0; i < item_count; ++i) {
				struct nk_rect content;
				float rx, ry, dx, dy, a;
				nk_fill_arc(out, center.x, center.y, (bounds.w/2.0f),
						a_min, a_max, (active_item == i) ? nk_rgb(45,100,255): nk_rgb(60,60,60));

				/* separator line */
				rx = bounds.w/2.0f; ry = 0;
				dx = rx * (float)cos(a_min) - ry * (float)sin(a_min);
				dy = rx * (float)sin(a_min) + ry * (float)cos(a_min);
				nk_stroke_line(out, center.x, center.y,
						center.x + dx, center.y + dy, 1.0f, nk_rgb(50,50,50));

				/* button content */
				a = a_min + (a_max - a_min)/2.0f;
				rx = bounds.w/2.5f; ry = 0;
				content.w = 30; content.h = 30;
				content.x = center.x + ((rx * (float)cos(a) - ry * (float)sin(a)) - content.w/2.0f);
				content.y = center.y + (rx * (float)sin(a) + ry * (float)cos(a) - content.h/2.0f);
				nk_draw_image(out, content, &icons[i], nk_rgb(255,255,255));
				a_min = a_max; a_max += step;
			}
		}
		{
			/* inner circle */
			struct nk_rect inner;
			inner.x = bounds.x + bounds.w/2 - bounds.w/4;
			inner.y = bounds.y + bounds.h/2 - bounds.h/4;
			inner.w = bounds.w/2; inner.h = bounds.h/2;
			nk_fill_circle(out, inner, nk_rgb(45,45,45));

			/* active icon content */
			bounds.w = inner.w / 2.0f;
			bounds.h = inner.h / 2.0f;
			bounds.x = inner.x + inner.w/2 - bounds.w/2;
			bounds.y = inner.y + inner.h/2 - bounds.h/2;
			nk_draw_image(out, bounds, &icons[active_item], nk_rgb(255,255,255));
		}
		nk_layout_space_end(ctx);
		if (!nk_input_is_mouse_down(&ctx->input, NK_BUTTON_RIGHT)) {
			nk_popup_close(ctx);
			ret = active_item;
		}
	} else ret = -2;
	ctx->style.window.spacing = nk_vec2(4,4);
	ctx->style.window.padding = nk_vec2(8,8);
	nk_popup_end(ctx);

	ctx->style.window.fixed_background = background;
	ctx->style.window.border_color = border;
	return ret;
}

/* ===============================================================
 *
 *                          GRID
 *
 * ===============================================================*/

	static void
effect_controls(struct nk_context *ctx, struct media *media)
{
	char *title = gGUI.component->effectName;
	//static char text[3][64];
	//static int text_len[3];
	//static const char *items[] = {"Gain","Feedback","Wet/Dry", "Delay", "Frequency"};
	//static int selected_item = 0;
	static int check = 1;
	static float volume = 0.5;
	//static float slider_value[] = {0.5};
	static char value_text[64];
	static float row_widths[] = {0.25, 0.55, 0.20};

	//int i;
	nk_style_set_font(ctx, &media->font_20->handle);
	if (nk_begin(ctx, title, nk_rect(650, 150, 575, 450),
				NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
				NK_WINDOW_NO_SCROLLBAR))
	{
		nk_style_set_font(ctx, &media->font_18->handle);
		nk_layout_row_dynamic(ctx, 30, 1);
		//nk_label(ctx, "Bypass Effect:", NK_TEXT_LEFT);
		nk_checkbox_label(ctx, "Bypass Effect", &check);
		nk_layout_row_dynamic(ctx, 30, 3);
		nk_layout_row(ctx, NK_DYNAMIC, 30, 3, row_widths);
		//nk_label(ctx, "Volume", NK_TEXT_LEFT);
		//nk_slider_float(ctx, 0, &volume, 1.0f, 0.01f);
		//sprintf(value_text, "%5.2f", volume);
		//nk_label(ctx, value_text, NK_TEXT_LEFT);
		for (int j=0; j < gGUI.slider_count; j++) {
			nk_label(ctx, gGUI.sliders[j].name, NK_TEXT_LEFT);
			if ( nk_slider_float(ctx, 0, &gGUI.sliders[j].slider_value, 1.0f, 0.01f)) {
			}
			*(gGUI.sliders[j].slOutput) = gGUI.sliders[j].slider_value * gGUI.sliders[j].slope + gGUI.sliders[j].intercept;
			sprintf(value_text, "%5.2f", *(gGUI.sliders[j].slOutput));
			nk_label(ctx, value_text, NK_TEXT_LEFT);
		}
	}
	if (check) {
		//printf("Bypass on.\n");
	}
	else {
		//printf("Bypass off.\n");
	}
	nk_end(ctx);
	nk_style_set_font(ctx, &media->font_14->handle);
}

/* ===============================================================
 *
 *                          BUTTON DEMO
 *
 * ===============================================================*/
	static void
ui_header(struct nk_context *ctx, struct media *media, const char *title)
{
	nk_style_set_font(ctx, &media->font_18->handle);
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_label(ctx, title, NK_TEXT_LEFT);
}

	static void
ui_widget(struct nk_context *ctx, struct media *media, float height)
{
	static const float ratio[] = {0.15f, 0.85f};
	nk_style_set_font(ctx, &media->font_22->handle);
	nk_layout_row(ctx, NK_DYNAMIC, height, 2, ratio);
	nk_spacing(ctx, 1);
}

#if 0
	static void
ui_widget_centered(struct nk_context *ctx, struct media *media, float height)
{
	static const float ratio[] = {0.15f, 0.50f, 0.35f};
	nk_style_set_font(ctx, &media->font_22->handle);
	nk_layout_row(ctx, NK_DYNAMIC, height, 3, ratio);
	nk_spacing(ctx, 1);
}
	static void
button_demo(struct nk_context *ctx, struct media *media)
{
	static int option = 1;
	static int toggle0 = 1;
	static int toggle1 = 0;
	static int toggle2 = 1;

	nk_style_set_font(ctx, &media->font_20->handle);
	nk_begin(ctx, "Button Demo", nk_rect(50,50,255,610),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE);

	/*------------------------------------------------
	 *                  MENU
	 *------------------------------------------------*/
	nk_menubar_begin(ctx);
	{
		/* toolbar */
		nk_layout_row_static(ctx, 40, 40, 4);
		if (nk_menu_begin_image(ctx, "Music", media->play, nk_vec2(110,120)))
		{
			/* settings */
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_menu_item_image_label(ctx, media->play, "Play", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->stop, "Stop", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->pause, "Pause", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->next, "Next", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->prev, "Prev", NK_TEXT_RIGHT);
			nk_menu_end(ctx);
		}
		nk_button_image(ctx, media->tools);
		nk_button_image(ctx, media->cloud);
		nk_button_image(ctx, media->pen);
	}
	nk_menubar_end(ctx);

	/*------------------------------------------------
	 *                  BUTTON
	 *------------------------------------------------*/
	ui_header(ctx, media, "Push buttons");
	ui_widget(ctx, media, 35);
	if (nk_button_label(ctx, "Push me"))
		fprintf(stdout, "pushed!\n");
	ui_widget(ctx, media, 35);
	if (nk_button_image_label(ctx, media->rocket, "Styled", NK_TEXT_CENTERED))
		fprintf(stdout, "rocket!\n");

	/*------------------------------------------------
	 *                  REPEATER
	 *------------------------------------------------*/
	ui_header(ctx, media, "Repeater");
	ui_widget(ctx, media, 35);
	if (nk_button_label(ctx, "Press me"))
		fprintf(stdout, "pressed!\n");

	/*------------------------------------------------
	 *                  TOGGLE
	 *------------------------------------------------*/
	ui_header(ctx, media, "Toggle buttons");
	ui_widget(ctx, media, 35);
	if (nk_button_image_label(ctx, (toggle0) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
		toggle0 = !toggle0;

	ui_widget(ctx, media, 35);
	if (nk_button_image_label(ctx, (toggle1) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
		toggle1 = !toggle1;

	ui_widget(ctx, media, 35);
	if (nk_button_image_label(ctx, (toggle2) ? media->checked: media->unchecked, "Toggle", NK_TEXT_LEFT))
		toggle2 = !toggle2;

	/*------------------------------------------------
	 *                  RADIO
	 *------------------------------------------------*/
	ui_header(ctx, media, "Radio buttons");
	ui_widget(ctx, media, 35);
	if (nk_button_symbol_label(ctx, (option == 0)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
		option = 0;
	ui_widget(ctx, media, 35);
	if (nk_button_symbol_label(ctx, (option == 1)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
		option = 1;
	ui_widget(ctx, media, 35);
	if (nk_button_symbol_label(ctx, (option == 2)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Select", NK_TEXT_LEFT))
		option = 2;

	/*------------------------------------------------
	 *                  CONTEXTUAL
	 *------------------------------------------------*/
	nk_style_set_font(ctx, &media->font_18->handle);
	if (nk_contextual_begin(ctx, NK_WINDOW_NO_SCROLLBAR, nk_vec2(150, 300), nk_window_get_bounds(ctx))) {
		nk_layout_row_dynamic(ctx, 30, 1);
		if (nk_contextual_item_image_label(ctx, media->copy, "Clone", NK_TEXT_RIGHT))
			fprintf(stdout, "pressed clone!\n");
		if (nk_contextual_item_image_label(ctx, media->del, "Delete", NK_TEXT_RIGHT))
			fprintf(stdout, "pressed delete!\n");
		if (nk_contextual_item_image_label(ctx, media->convert, "Convert", NK_TEXT_RIGHT))
			fprintf(stdout, "pressed convert!\n");
		if (nk_contextual_item_image_label(ctx, media->edit, "Edit", NK_TEXT_RIGHT))
			fprintf(stdout, "pressed edit!\n");
		nk_contextual_end(ctx);
	}
	nk_style_set_font(ctx, &media->font_14->handle);
	nk_end(ctx);
}
#endif
/* ===============================================================
 *
 *                          BASIC DEMO
 *
 * ===============================================================*/
	static void
effect_selector(struct nk_context *ctx, struct media *media)
{
	//static int image_active;
	//static int check0 = 1;
	//static int check1 = 0;
	//static size_t prog = 80;
	//static float slider_value[10] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
	static int selected_item = 0;
	//static int selected_image = 3;
	//static int selected_icon = 0;
	//static const char *items[] = {"Item 0","item 1","item 2"};
	static int piemenu_active = 0;
	static struct nk_vec2 piemenu_pos;

	int i = 0;
	nk_style_set_font(ctx, &media->font_20->handle);
	nk_begin(ctx, "Effect Selector", nk_rect(320, 50, 275, 610),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE);

	/*------------------------------------------------
	 *                  MENU
	 *------------------------------------------------*/
	nk_menubar_begin(ctx);
	{
		/* toolbar */
		nk_layout_row_static(ctx, 40, 40, 4);
		if (nk_menu_begin_image(ctx, "Music", media->play, nk_vec2(110,120)))
		{
			/* settings */
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_menu_item_image_label(ctx, media->play, "Play", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->stop, "Stop", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->pause, "Pause", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->next, "Next", NK_TEXT_RIGHT);
			nk_menu_item_image_label(ctx, media->prev, "Prev", NK_TEXT_RIGHT);
			nk_menu_end(ctx);
		}
		nk_button_image(ctx, media->tools);
		nk_button_image(ctx, media->cloud);
		nk_button_image(ctx, media->pen);
	}
	nk_menubar_end(ctx);
#if 0
	/*------------------------------------------------
	 *                  POPUP BUTTON
	 *------------------------------------------------*/
	ui_header(ctx, media, "Popup & Scrollbar & Images");
	ui_widget(ctx, media, 35);
	if (nk_button_image_label(ctx, media->dir, "Images", NK_TEXT_CENTERED))
		image_active = !image_active;

	/*------------------------------------------------
	 *                  SELECTED IMAGE
	 *------------------------------------------------*/
	ui_header(ctx, media, "Selected Image");
	ui_widget_centered(ctx, media, 100);
	nk_image(ctx, media->images[selected_image]);

	/*------------------------------------------------
	 *                  IMAGE POPUP
	 *------------------------------------------------*/
	if (image_active) {
		if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Image Popup", 0, nk_rect(265, 0, 320, 220))) {
			nk_layout_row_static(ctx, 82, 82, 3);
			for (i = 0; i < 9; ++i) {
				if (nk_button_image(ctx, media->images[i])) {
					selected_image = i;
					image_active = 0;
					nk_popup_close(ctx);
				}
			}
			nk_popup_end(ctx);
		}
	}
#endif
	/*------------------------------------------------
	 *                  COMBOBOX
	 *------------------------------------------------*/
	ui_header(ctx, media, "Select Effect");
	ui_widget(ctx, media, 40);
	if (nk_combo_begin_label(ctx, effects_list[selected_item].name, nk_vec2(nk_widget_width(ctx), 200))) {
		nk_layout_row_dynamic(ctx, 35, 1);
		for (i = 0; i < g_effects_count; ++i) {
			if (nk_combo_item_label(ctx, g_effect_list[i].name, NK_TEXT_LEFT)) {
				selected_item = i;
				initializeDisplayState(&gGUI, selected_item);
				// this is called by play music
				//gui_initialize(gGUI.component, 0, 44100.0f);
				if (gMUSIC.fileName) { 
					gMUSIC.start_music = 1;
					gMUSIC.stop_music = 1;
				}
			}
		}
		nk_combo_end(ctx);
	}
#if 0
	ui_widget(ctx, media, 40);
	if (nk_combo_begin_image_label(ctx, effects_list[selected_icon].name, media->images[selected_icon], nk_vec2(nk_widget_width(ctx), 200))) {
		nk_layout_row_dynamic(ctx, 35, 1);
		for (i = 0; i < count; ++i)
			if (nk_combo_item_image_label(ctx, media->images[i], effects_list[i].name, NK_TEXT_RIGHT))
				selected_icon = i;
		nk_combo_end(ctx);
	}
	/*------------------------------------------------
	 *                  CHECKBOX
	 *------------------------------------------------*/
	ui_header(ctx, media, "Checkbox");
	ui_widget(ctx, media, 30);
	nk_checkbox_label(ctx, "Flag 1", &check0);
	ui_widget(ctx, media, 30);
	nk_checkbox_label(ctx, "Flag 2", &check1);

	/*------------------------------------------------
	 *                  PROGRESSBAR
	 *------------------------------------------------*/
	ui_header(ctx, media, "Progressbar");
	ui_widget(ctx, media, 35);
	nk_progress(ctx, &prog, 100, nk_true);
	/*------------------------------------------------
	 *                   SLIDER
	 *------------------------------------------------*/
	ui_header(ctx, media, "Slider");
	ui_widget(ctx, media, 35);
	nk_slider_float(ctx, 0, &slider_value[0], 1.0f, 0.01f);

#endif
	/*------------------------------------------------
	 *                  PIEMENU
	 *------------------------------------------------*/
	if (nk_input_is_mouse_click_down_in_rect(&ctx->input, NK_BUTTON_RIGHT,
				nk_window_get_bounds(ctx),nk_true)){
		piemenu_pos = ctx->input.mouse.pos;
		piemenu_active = 1;
	}

	if (piemenu_active) {
		int ret = ui_piemenu(ctx, piemenu_pos, 140, &media->menu[0], 6);
		if (ret == -2) piemenu_active = 0;
		if (ret != -1) {
			fprintf(stdout, "piemenu selected: %d\n", ret);
			piemenu_active = 0;
		}
	}
	nk_style_set_font(ctx, &media->font_14->handle);
	nk_end(ctx);
}

/* ===============================================================
 *
 *                          DEVICE
 *
 * ===============================================================*/
struct nk_glfw_vertex {
	float position[2];
	float uv[2];
	nk_byte col[4];
};

struct device {
	struct nk_buffer cmds;
	struct nk_draw_null_texture tex_null;
	GLuint vbo, vao, ebo;
	GLuint prog;
	GLuint vert_shdr;
	GLuint frag_shdr;
	GLint attrib_pos;
	GLint attrib_uv;
	GLint attrib_col;
	GLint uniform_tex;
	GLint uniform_proj;
	GLuint font_tex;
};
	static void
die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputs("\n", stderr);
	exit(EXIT_FAILURE);
}
	static struct nk_image
icon_load(const char *filename)
{
	int x,y,n;
	GLuint tex;
	unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	if (!data) die("[SDL]: failed to load image: %s", filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return nk_image_id((int)tex);
}

	static void
device_init(struct device *dev)
{
	GLint status;
	static const GLchar *vertex_shader =
		NK_SHADER_VERSION
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 TexCoord;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main() {\n"
		"   Frag_UV = TexCoord;\n"
		"   Frag_Color = Color;\n"
		"   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
		"}\n";
	static const GLchar *fragment_shader =
		NK_SHADER_VERSION
		"precision mediump float;\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main(){\n"
		"   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	nk_buffer_init_default(&dev->cmds);
	dev->prog = glCreateProgram();
	dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
	dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
	glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
	glCompileShader(dev->vert_shdr);
	glCompileShader(dev->frag_shdr);
	glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glAttachShader(dev->prog, dev->vert_shdr);
	glAttachShader(dev->prog, dev->frag_shdr);
	glLinkProgram(dev->prog);
	glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
	assert(status == GL_TRUE);

	dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
	dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
	dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
	dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
	dev->attrib_col = glGetAttribLocation(dev->prog, "Color");

	{
		/* buffer setup */
		GLsizei vs = sizeof(struct nk_glfw_vertex);
		size_t vp = offsetof(struct nk_glfw_vertex, position);
		size_t vt = offsetof(struct nk_glfw_vertex, uv);
		size_t vc = offsetof(struct nk_glfw_vertex, col);

		glGenBuffers(1, &dev->vbo);
		glGenBuffers(1, &dev->ebo);
		glGenVertexArrays(1, &dev->vao);

		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glEnableVertexAttribArray((GLuint)dev->attrib_pos);
		glEnableVertexAttribArray((GLuint)dev->attrib_uv);
		glEnableVertexAttribArray((GLuint)dev->attrib_col);

		glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
		glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
		glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

	static void
device_upload_atlas(struct device *dev, const void *image, int width, int height)
{
	glGenTextures(1, &dev->font_tex);
	glBindTexture(GL_TEXTURE_2D, dev->font_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image);
}

	static void
device_shutdown(struct device *dev)
{
	glDetachShader(dev->prog, dev->vert_shdr);
	glDetachShader(dev->prog, dev->frag_shdr);
	glDeleteShader(dev->vert_shdr);
	glDeleteShader(dev->frag_shdr);
	glDeleteProgram(dev->prog);
	glDeleteTextures(1, &dev->font_tex);
	glDeleteBuffers(1, &dev->vbo);
	glDeleteBuffers(1, &dev->ebo);
	nk_buffer_free(&dev->cmds);
}

	static void
device_draw(struct device *dev, struct nk_context *ctx, int width, int height,
		struct nk_vec2 scale, enum nk_anti_aliasing AA)
{
	GLfloat ortho[4][4] = {
		{2.0f, 0.0f, 0.0f, 0.0f},
		{0.0f,-2.0f, 0.0f, 0.0f},
		{0.0f, 0.0f,-1.0f, 0.0f},
		{-1.0f,1.0f, 0.0f, 1.0f},
	};
	ortho[0][0] /= (GLfloat)width;
	ortho[1][1] /= (GLfloat)height;

	/* setup global state */
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	/* setup program */
	glUseProgram(dev->prog);
	glUniform1i(dev->uniform_tex, 0);
	glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
	{
		/* convert from command queue into draw list and draw to screen */
		const struct nk_draw_command *cmd;
		void *vertices, *elements;
		const nk_draw_index *offset = NULL;

		/* allocate vertex and element buffer */
		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_MEMORY, NULL, GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_MEMORY, NULL, GL_STREAM_DRAW);

		/* load draw vertices & elements directly into vertex + element buffer */
		vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		{
			/* fill convert configuration */
			struct nk_convert_config config;
			static const struct nk_draw_vertex_layout_element vertex_layout[] = {
				{NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, position)},
				{NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, uv)},
				{NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_glfw_vertex, col)},
				{NK_VERTEX_LAYOUT_END}
			};
			NK_MEMSET(&config, 0, sizeof(config));
			config.vertex_layout = vertex_layout;
			config.vertex_size = sizeof(struct nk_glfw_vertex);
			config.vertex_alignment = NK_ALIGNOF(struct nk_glfw_vertex);
			config.tex_null = dev->tex_null;
			config.circle_segment_count = 22;
			config.curve_segment_count = 22;
			config.arc_segment_count = 22;
			config.global_alpha = 1.0f;
			config.shape_AA = AA;
			config.line_AA = AA;

			/* setup buffers to load vertices and elements */
			{struct nk_buffer vbuf, ebuf;
				nk_buffer_init_fixed(&vbuf, vertices, MAX_VERTEX_MEMORY);
				nk_buffer_init_fixed(&ebuf, elements, MAX_ELEMENT_MEMORY);
				nk_convert(ctx, &dev->cmds, &vbuf, &ebuf, &config);}
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		/* iterate over and execute each draw command */
		nk_draw_foreach(cmd, ctx, &dev->cmds)
		{
			if (!cmd->elem_count) continue;
			glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
			glScissor(
					(GLint)(cmd->clip_rect.x * scale.x),
					(GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * scale.y),
					(GLint)(cmd->clip_rect.w * scale.x),
					(GLint)(cmd->clip_rect.h * scale.y));
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count;
		}
		nk_clear(ctx);
		nk_buffer_clear(&dev->cmds);
	}

	/* default OpenGL state */
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);
}

/* glfw callbacks (I don't know if there is a easier way to access text and scroll )*/
static void error_callback(int e, const char *d){printf("Error %d: %s\n", e, d);}
static void text_input(GLFWwindow *win, unsigned int codepoint)
{nk_input_unicode((struct nk_context*)glfwGetWindowUserPointer(win), codepoint);}
static void scroll_input(GLFWwindow *win, double _, double yoff)
{UNUSED(_);nk_input_scroll((struct nk_context*)glfwGetWindowUserPointer(win), nk_vec2(0, (float)yoff));}

#define INITIAL_FLOAT_VALUE -1888.8888f

static void setupSlidersComponent(DISPLAY_STATE *gui, EFFECT_PARAMS *parameter) {
	uint8_t count = gui->slider_count;
	gui->sliders[count]useCheckBox = 0;

	assert (parameter->currentValue != 0);
	gui->sliders[count].myParameter = parameter;
	gui->sliders[count].slope =
		parameter->floatParameter[2] - parameter->floatParameter[0];
	gui->sliders[count].intercept = parameter->floatParameter[0];
	gui->sliders[count].slider_value =
		(parameter->floatParameter[1] - parameter->floatParameter[0])/ gui->sliders[count].slope;
	gui->sliders[count].slOutput = parameter->currentValue;
	*(parameter->currentValue) =  parameter->floatParameter[1];
	gui->sliders[count].previousOutput = INITIAL_FLOAT_VALUE;
	parameter->previousValue = &(gui->sliders[count].previousOutput);
}
static void setupSliders(DISPLAY_STATE *gui, EFFECT_COMPONENT * component) {
	for(int i=0; i < component->parameterCount; i++) {
		char *name = component->strParameters[i];
		if (component->strTypes[i][0] == 'S') {
			EFFECT_PARAMS *parameter = component->parameters + i;
			setupSlidersComponent(gui, parameter);
			gui->sliders[gui->slider_count].name = name;
			gui->slider_count++;
		}
		else if (component->strTypes[i][0] == 'C') {
			uint8_t count = gui->slider_count;
			gui->sliders[count]useCheckBox = 1;
			gui->sliders[count].name = name;
			gui->sliders[count].chkOutput = parameter->currentValue;
			*(gui->sliders[count].chkOutput) = parameter->intParameter[0];
			gui->sliders[count].previousCheck = 15;
			gui->slider_count++;
		}
	}
	for (int i=0; i < component->childrenCount; i++) {
		EFFECT_COMPONENT *childComponent = component->childComponents[i];
		setupSliders(gui, childComponent);
	}
}

void generate_gui(EFFECT_ITEM *eList)
{
	/* Platform */
	static GLFWwindow *win;
	int width = 0, height = 0;
	int display_width=0, display_height=0;
	//char *fileName = "sounds/rampb-acoustic-guitar-loop-1-70bpm-143363.wav";
	//fileName = "sounds/alan-walker-type-guitar-loop-1-246365.wav";
	//fileName = "sounds/relaxing-guitar-128296.wav";

	/* GUI */
	struct device device;
	struct nk_font_atlas atlas;
	struct media media;
	struct nk_context ctx;

	gGUI.display_sliders = 0;
	gGUI.component = NULL;
	gGUI.slider_count = 0;


	g_effect_list = eList;
	uint8_t eCount;
	for (eCount = 0; eList[eCount].eType != None; eCount++);

	g_effects_count = eCount;
	/* GLFW */
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		fprintf(stdout, "[GFLW] failed to init!\n");
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
	glfwMakeContextCurrent(win);
	glfwSetWindowUserPointer(win, &ctx);
	glfwSetCharCallback(win, text_input);
	glfwSetScrollCallback(win, scroll_input);
	glfwGetWindowSize(win, &width, &height);
	glfwGetFramebufferSize(win, &display_width, &display_height);

	/* OpenGL */
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glewExperimental = 1;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to setup GLEW\n");
		exit(1);
	}

	{/* GUI */
		device_init(&device);
		{const void *image; int w, h;
			struct nk_font_config cfg = nk_font_config(0);
			cfg.oversample_h = 3; cfg.oversample_v = 2;
			/* Loading one font with different heights is only required if you want higher
			 * quality text otherwise you can just set the font height directly
			 * e.g.: ctx->style.font.height = 20. */
			nk_font_atlas_init_default(&atlas);
			nk_font_atlas_begin(&atlas);
			// font_13 for file browser
			media.font_13 = nk_font_atlas_add_from_file(&atlas, "extra_font/Roboto-Regular.ttf", 13.0f, &cfg);
			media.font_14 = nk_font_atlas_add_from_file(&atlas, "extra_font/Roboto-Regular.ttf", 14.0f, &cfg);
			media.font_18 = nk_font_atlas_add_from_file(&atlas, "extra_font/Roboto-Regular.ttf", 18.0f, &cfg);
			media.font_20 = nk_font_atlas_add_from_file(&atlas, "extra_font/Roboto-Regular.ttf", 20.0f, &cfg);
			media.font_22 = nk_font_atlas_add_from_file(&atlas, "extra_font/Roboto-Regular.ttf", 22.0f, &cfg);
			image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
			device_upload_atlas(&device, image, w, h);
			nk_font_atlas_end(&atlas, nk_handle_id((int)device.font_tex), &device.tex_null);}
		nk_init_default(&ctx, &media.font_14->handle);
	}
	glEnable(GL_TEXTURE_2D);
	media.unchecked = icon_load("icon/unchecked.png");
	media.checked = icon_load("icon/checked.png");
	media.rocket = icon_load("icon/rocket.png");
	media.cloud = icon_load("icon/cloud.png");
	media.pen = icon_load("icon/pen.png");
	media.play = icon_load("icon/play.png");
	media.pause = icon_load("icon/pause.png");
	media.stop = icon_load("icon/stop.png");
	media.next =  icon_load("icon/next.png");
	media.prev =  icon_load("icon/prev.png");
	media.tools = icon_load("icon/tools.png");
	media.dir = icon_load("icon/directory.png");
	media.copy = icon_load("icon/copy.png");
	media.convert = icon_load("icon/export.png");
	media.del = icon_load("icon/delete.png");
	media.edit = icon_load("icon/edit.png");
	media.menu[0] = icon_load("icon/home.png");
	media.menu[1] = icon_load("icon/phone.png");
	media.menu[2] = icon_load("icon/plane.png");
	media.menu[3] = icon_load("icon/wifi.png");
	media.menu[4] = icon_load("icon/settings.png");
	media.menu[5] = icon_load("icon/volume.png");

	{int i;
		for (i = 0; i < 9; ++i) {
			char buffer[256];
			sprintf(buffer, "images/image%d.png", (i+1));
			media.images[i] = icon_load(buffer);
		}}
	file_selector_init ();
	initializeMusicState(&gMUSIC);
	gMUSIC.fileName = 0;
	while (!glfwWindowShouldClose(win))
	{
		/* High DPI displays */
		struct nk_vec2 scale;
		glfwGetWindowSize(win, &width, &height);
		glfwGetFramebufferSize(win, &display_width, &display_height);
		scale.x = (float)display_width/(float)width;
		scale.y = (float)display_height/(float)height;

		/* Input */
		{double x, y;
			nk_input_begin(&ctx);
			glfwPollEvents();
			nk_input_key(&ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
			nk_input_key(&ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);
			if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
					glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
				nk_input_key(&ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
				nk_input_key(&ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
				nk_input_key(&ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
				nk_input_key(&ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
				nk_input_key(&ctx, NK_KEY_SHIFT, 1);
			} else {
				nk_input_key(&ctx, NK_KEY_COPY, 0);
				nk_input_key(&ctx, NK_KEY_PASTE, 0);
				nk_input_key(&ctx, NK_KEY_CUT, 0);
				nk_input_key(&ctx, NK_KEY_SHIFT, 0);
			}
			glfwGetCursorPos(win, &x, &y);
			nk_input_motion(&ctx, (int)x, (int)y);
			nk_input_button(&ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
			nk_input_button(&ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
			nk_input_button(&ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
			nk_input_end(&ctx);}


		file_selector (&ctx);
		/* GUI */
		effect_selector(&ctx, &media);
		//button_demo(&ctx, &media);
		if (gGUI.display_sliders) {
			effect_controls(&ctx, &media);
		}
		// check for stopping first
		if (gMUSIC.stop_music) {
			if (gMUSIC.music_is_playing) {
				stop_music_playing ();
			}
			gMUSIC.music_is_playing = 0;
			gMUSIC.stop_music = 0;
		}

		// can we start the music here?
		if (gGUI.effect_selected & gMUSIC.start_music && !gMUSIC.stop_music && !gMUSIC.music_is_playing && gGUI.component != 0) {
			int success = play_music (gMUSIC.fileName, gGUI.component);
			if (success == 0) {
				update_effect_state(gGUI.sliders, gGUI.slider_count);
				gMUSIC.music_is_playing = 1;
			}
			gMUSIC.start_music = 0;
		}
		/* Draw */
		glViewport(0, 0, display_width, display_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		device_draw(&device, &ctx, width, height, scale, NK_ANTI_ALIASING_ON);
		glfwSwapBuffers(win);
	}
	// cleanup if needed
	if (gMUSIC.music_is_playing) {
		stop_music_playing ();
	}
	clearDisplayState (&gGUI);


	glDeleteTextures(1,(const GLuint*)&media.unchecked.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.checked.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.rocket.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.cloud.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.pen.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.play.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.pause.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.stop.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.next.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.prev.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.tools.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.dir.handle.id);
	glDeleteTextures(1,(const GLuint*)&media.del.handle.id);
	nk_font_atlas_clear(&atlas);
file_browser_uninit();
	nk_free(&ctx);

	device_shutdown(&device);
	glfwTerminate();
}
#endif
