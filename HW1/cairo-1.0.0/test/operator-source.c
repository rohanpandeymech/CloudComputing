/*
 * Copyright © 2005 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Red Hat, Inc. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. Red Hat, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * RED HAT, INC. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL RED HAT, INC. BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: Kristian Høgsberg <krh@redhat.com>
 *          Owen Taylor <otaylor@redhat.com>
 */

#include <math.h>
#include "cairo-test.h"
#include <stdio.h>

#define WIDTH 64
#define HEIGHT 64
#define PAD 10

const char	png_filename[]	= "romedalen.png";

static void
set_solid_pattern (cairo_t *cr, int x, int y)
{
    cairo_set_source_rgb (cr, 1.0, 0, 0.0);
}

static void
set_translucent_pattern (cairo_t *cr, int x, int y)
{
    cairo_set_source_rgba (cr, 1, 0, 0, 0.5);
}

static void
set_gradient_pattern (cairo_t *cr, int x, int y)
{
    cairo_pattern_t *pattern;

    pattern = cairo_pattern_create_linear (x, y, x + WIDTH, y + HEIGHT);
    cairo_pattern_add_color_stop_rgba (pattern, 0.2, 1, 0, 0, 1);
    cairo_pattern_add_color_stop_rgba (pattern, 0.8, 1, 0, 0, 0.0);
    cairo_set_source (cr, pattern);
    cairo_pattern_destroy (pattern);
}

static void
set_surface_pattern (cairo_t *cr, int x, int y)
{
    cairo_surface_t *source_surface;
    cairo_t *cr2;

    double width = (int)(0.6 * WIDTH);
    double height = (int)(0.6 * HEIGHT);
    x += 0.2 * WIDTH;
    y += 0.2 * HEIGHT;
  
    source_surface = cairo_surface_create_similar (cairo_get_target (cr),
						   CAIRO_CONTENT_COLOR_ALPHA,
						   width, height);
    cr2 = cairo_create (source_surface);

    cairo_set_source_rgb (cr2, 1, 0, 0); /* red */
    cairo_paint (cr2);

    cairo_set_source_rgb (cr2, 1, 1, 1); /* white */

    cairo_arc (cr2, 0.5 * width, 0.5 * height, 0.5 * height, 0, 2 * M_PI);
    cairo_fill (cr2);

    cairo_destroy (cr2);

    cairo_set_source_surface (cr, source_surface, x, y);

    cairo_surface_destroy (source_surface);
}

static void
draw_mask (cairo_t *cr, int x, int y)
{
    cairo_surface_t *mask_surface;
    cairo_t *cr2;

    double width = (int)(0.9 * WIDTH);
    double height = (int)(0.9 * HEIGHT);
    x += 0.05 * WIDTH;
    y += 0.05 * HEIGHT;
  
    mask_surface = cairo_surface_create_similar (cairo_get_target (cr),
						 CAIRO_CONTENT_ALPHA,
						 width, height);
    cr2 = cairo_create (mask_surface);

    cairo_set_source_rgb (cr2, 1, 1, 1); /* white */

    cairo_arc (cr2, 0.5 * width, 0.5 * height, 0.45 * height, 0, 2 * M_PI);
    cairo_fill (cr2);

    cairo_destroy (cr2);

    cairo_mask_surface (cr, mask_surface, x, y);

    cairo_surface_destroy (mask_surface);
}

static void
draw_glyphs (cairo_t *cr, int x, int y)
{
    cairo_text_extents_t extents;

    cairo_set_font_size (cr, 0.8 * HEIGHT);
    
    cairo_text_extents (cr, "FG", &extents);
    cairo_move_to (cr,
		   x + (WIDTH - extents.width) / 2 - extents.x_bearing,
		   y + (HEIGHT - extents.height) / 2 - extents.y_bearing);
    cairo_show_text (cr, "FG");
}

static void
draw_polygon (cairo_t *cr, int x, int y)
{ 
    double width = (int)(0.9 * WIDTH);
    double height = (int)(0.9 * HEIGHT);
    x += 0.05 * WIDTH;
    y += 0.05 * HEIGHT;
  
    cairo_new_path (cr);
    cairo_move_to (cr, x, y);
    cairo_line_to (cr, x, y + height);
    cairo_line_to (cr, x + width / 2, y + 3 * height / 4);
    cairo_line_to (cr, x + width, y + height);
    cairo_line_to (cr, x + width, y);
    cairo_line_to (cr, x + width / 2, y + height / 4);
    cairo_close_path (cr);
    cairo_fill (cr);
}

static void
draw_rects (cairo_t *cr, int x, int y)
{ 
    double block_width = (int)(0.33 * WIDTH + 0.5);
    double block_height = (int)(0.33 * HEIGHT + 0.5);
    int i, j;

    for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	    if ((i + j) % 2 == 0)
		cairo_rectangle (cr,
				 x + block_width * i, y + block_height * j,
				 block_width,         block_height);

    cairo_fill (cr);
}

static void (*pattern_funcs[])(cairo_t *cr, int x, int y) = {
    set_solid_pattern,
    set_translucent_pattern,
    set_gradient_pattern,
    set_surface_pattern,
};

static void (*draw_funcs[])(cairo_t *cr, int x, int y) = {
    draw_mask,
    draw_glyphs,
    draw_polygon,
    draw_rects
};

#define ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))
#define IMAGE_WIDTH (ARRAY_SIZE (pattern_funcs) * (WIDTH + PAD) + PAD)
#define IMAGE_HEIGHT (ARRAY_SIZE (draw_funcs) * (HEIGHT + PAD) + PAD)

static cairo_test_t test = {
    "operator-source",
    "Test of CAIRO_OPERATOR_SOURCE",
    IMAGE_WIDTH, IMAGE_HEIGHT
};

static cairo_test_status_t
draw (cairo_t *cr, int width, int height)
{
    int i, j, x, y;
    cairo_font_options_t *font_options;
    cairo_pattern_t *pattern;

    cairo_select_font_face (cr, "Bitstream Vera Sans",
			    CAIRO_FONT_SLANT_NORMAL,
			    CAIRO_FONT_WEIGHT_NORMAL);

    font_options = cairo_font_options_create ();

    cairo_font_options_set_hint_style (font_options, CAIRO_HINT_STYLE_NONE);
    cairo_font_options_set_antialias (font_options, CAIRO_ANTIALIAS_GRAY);

    cairo_set_font_options (cr, font_options);
    cairo_font_options_destroy (font_options);

    for (j = 0; j < ARRAY_SIZE (draw_funcs); j++) {
	for (i = 0; i < ARRAY_SIZE (pattern_funcs); i++) {
	    x = i * (WIDTH + PAD) + PAD;
	    y = j * (HEIGHT + PAD) + PAD;
	    
	    cairo_save (cr);

	    pattern = cairo_pattern_create_linear (x + WIDTH, y,
						   x,         y + HEIGHT);
	    cairo_pattern_add_color_stop_rgba (pattern, 0.2,
					       0.0, 0.0, 1.0, 1.0); /* Solid blue */
	    cairo_pattern_add_color_stop_rgba (pattern, 0.8,
					       0.0, 0.0, 1.0, 0.0); /* Transparent blue */
	    cairo_set_source (cr, pattern);
	    cairo_pattern_destroy (pattern);
	    
	    cairo_rectangle (cr, x, y, WIDTH, HEIGHT);
	    cairo_fill_preserve (cr);
	    cairo_clip (cr);

	    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	    pattern_funcs[i] (cr, x, y);
	    draw_funcs[j] (cr, x, y);
	    if (cairo_status (cr))
		cairo_test_log ("%d %d HERE!\n", i, j);
	    
	    cairo_restore (cr);
	}
    }

    if (cairo_status (cr) != CAIRO_STATUS_SUCCESS)
	cairo_test_log ("%d %d .HERE!\n", i, j);

    return CAIRO_TEST_SUCCESS;
}

int
main (void)
{
    return cairo_test (&test, draw);
}
