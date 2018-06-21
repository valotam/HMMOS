/* ===============================================================
 *
 *                          CANVAS
 *
 * ===============================================================*/
struct nk_canvas {
    struct nk_command_buffer *painter;
    struct nk_vec2 item_spacing;
    struct nk_vec2 panel_padding;
    struct nk_style_item window_background;
};

int
canvas_begin(struct nk_context *ctx, struct nk_canvas *canvas, const char* title, struct nk_rect bounds,
     nk_flags flags,struct nk_color background_color)
{
    int ret = 0;
    /* save style properties which will be overwritten */
    canvas->panel_padding = ctx->style.window.padding;
    canvas->item_spacing = ctx->style.window.spacing;
    canvas->window_background = ctx->style.window.fixed_background;

    /* use the complete window space and set background */
    ctx->style.window.spacing = nk_vec2(0,0);
    ctx->style.window.padding = nk_vec2(0,0);
    ctx->style.window.fixed_background = nk_style_item_color(background_color);

    /* create/update window and set position + size */
    flags = flags & ~NK_WINDOW_DYNAMIC;
    nk_window_set_bounds(ctx, title, bounds);
    ret = nk_begin(ctx, title, bounds, NK_WINDOW_NO_SCROLLBAR|flags);

    if (ret)
    {
        /* allocate the complete window space for drawing */
        {struct nk_rect total_space;
        total_space = nk_window_get_content_region(ctx);
        nk_layout_row_dynamic(ctx, total_space.h, 1);
        nk_widget(&total_space, ctx);
        canvas->painter = nk_window_get_canvas(ctx);}
    }
    return ret;
}

void
canvas_end(struct nk_context *ctx, struct nk_canvas *canvas)
{
    nk_end(ctx);
    ctx->style.window.spacing = canvas->panel_padding;
    ctx->style.window.padding = canvas->item_spacing;
    ctx->style.window.fixed_background = canvas->window_background;
}

static int
canvas(struct nk_context *ctx, int win_width, int win_height, SS::Nurbs &nurbs)
{
    /* window flags */
    static int border = nk_true;
    static int resize = nk_false;
    static int movable = nk_false;
    static int no_scrollbar = nk_false;
    static int scale_left = nk_false;
    static int minimizable = nk_true;
    static nk_flags window_flags = 0;

    /* window flags */
    window_flags = 0;
    ctx->style.window.header.align = NK_HEADER_RIGHT;
    if (border) window_flags |= NK_WINDOW_BORDER;
    if (resize) window_flags |= NK_WINDOW_SCALABLE;
    if (movable) window_flags |= NK_WINDOW_MOVABLE;
    if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
    if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;
    if (minimizable) window_flags |= NK_WINDOW_MINIMIZABLE;

    /* draw */
    {struct nk_canvas canvas;
    int start = 420;
    if (canvas_begin(ctx, &canvas, "Canvas", nk_rect(start, 0, win_width-start, win_height), window_flags, nk_white))
    {
        const struct nk_color color = nk_default_color_style[NK_COLOR_CHART_COLOR];
        unsigned int size = nurbs.control_points.size();
        if (size != 0) {
            for(unsigned int i = 0; i < size; i++)
            {
                SS::Vertex2f cp = nurbs.control_points.at(i);
                nk_stroke_circle(canvas.painter, nk_rect(cp.x - 2, cp.y - 2, 5, 5), 5, nk_rgb(0,255,120));
                
                if (i < size - 1) {
                    SS::Vertex2f cp2 = nurbs.control_points.at(i + 1);
                    nk_stroke_line(canvas.painter, cp.x, cp.y, cp2.x, cp2.y, 2.0f, nk_blue);
                }
            }
            
            size = nurbs.curve.size();
            for(unsigned int i = 0; i < size; i++)
            {
                SS::Vertex2f cp = nurbs.curve.at(i);
                if (i < size - 1) {
                    SS::Vertex2f cp2 = nurbs.curve.at(i + 1);
                    nk_stroke_line(canvas.painter, cp.x, cp.y, cp2.x, cp2.y, 2.0f, color);
                }
            }
        }
        
    }
    canvas_end(ctx, &canvas);}
    return !nk_window_is_closed(ctx, "Canvas");
}