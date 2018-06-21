static int
menu(struct nk_context *ctx, int win_width, int win_height, SS::Nurbs &nurbs)
{
    /* window flags */
    static int show_menu = nk_true;
    static int border = nk_true;
    static int resize = nk_false;
    static int movable = nk_false;
    static int no_scrollbar = nk_false;
    static int scale_left = nk_false;
    static int minimizable = nk_true;
    static nk_flags window_flags = 0;

    /* popups */
    static enum nk_style_header_align header_align = NK_HEADER_RIGHT;
    static int show_app_about = nk_true;

    /* window flags */
    window_flags = 0;
    ctx->style.window.header.align = header_align;
    if (border) window_flags |= NK_WINDOW_BORDER;
    if (resize) window_flags |= NK_WINDOW_SCALABLE;
    if (movable) window_flags |= NK_WINDOW_MOVABLE;
    if (no_scrollbar) window_flags |= NK_WINDOW_NO_SCROLLBAR;
    if (scale_left) window_flags |= NK_WINDOW_SCALE_LEFT;
    if (minimizable) window_flags |= NK_WINDOW_MINIMIZABLE;

    /* create/update window and set position + size */
    int start = 420;
    struct nk_rect bounds = nk_rect(0, 0, start, win_height);
    const char *title = "NURBs";
    window_flags = window_flags & ~NK_WINDOW_DYNAMIC;
    nk_window_set_bounds(ctx, title, bounds);

    if (nk_begin(ctx, title, bounds, window_flags))
    {
        if (show_menu)
        {
            /* menubar */
            nk_menubar_begin(ctx);

            /* menu */
            nk_layout_row_begin(ctx, NK_STATIC, 25, 1);

            nk_layout_row_push(ctx, 60);
            if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(500, 60)))
            {
                nk_layout_row_dynamic(ctx, 40, 4); // wrapping row
                if (nk_group_begin(ctx, "column1", NK_WINDOW_BORDER)) { // column 1
                    nk_layout_row_dynamic(ctx, 20, 1); // nested row
                    nk_label(ctx, "New", NK_TEXT_CENTERED);
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "column2", NK_WINDOW_BORDER)) { // column 2
                    nk_layout_row_dynamic(ctx, 20, 1); // nested row
                    nk_label(ctx, "Open", NK_TEXT_CENTERED);
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "column3", NK_WINDOW_BORDER)) { // column 3
                    nk_layout_row_dynamic(ctx, 20, 1); // nested row
                    nk_label(ctx, "Save", NK_TEXT_CENTERED);
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "column4", NK_WINDOW_BORDER)) { // column 4
                    nk_layout_row_dynamic(ctx, 20, 1); // nested row
                    nk_label(ctx, "Close", NK_TEXT_CENTERED);
                    nk_group_end(ctx);
                }
                nk_menu_end(ctx);
            }
            /* menu widgets */
            nk_menubar_end(ctx);
        }

        if (show_app_about)
        {
            /* about popup */
            static struct nk_rect s = {start+ 100, 150, start, 350};
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s))
            {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Non-uniform rational B-spline", NK_TEXT_LEFT);
                nk_label(ctx, "By S.S. Park", NK_TEXT_LEFT);
                nk_label(ctx, "Lecture: Hull Modeling Methods for Offshore Structures",  NK_TEXT_CENTERED);
                nk_popup_end(ctx);
            } else show_app_about = nk_false;
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Guide", NK_MAXIMIZED))
        {
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Input", NK_TEXT_LEFT);
            nk_label(ctx, "-You can add a control point whenever you", NK_TEXT_LEFT);
            nk_label(ctx, "click LMB on 'the Canvas', or can delete a control", NK_TEXT_LEFT);
            nk_label(ctx, "point whenever you click RMB, vice versa.", NK_TEXT_LEFT);
            nk_label_colored(ctx, "You must set degree!", NK_TEXT_LEFT, nk_yellow);
            nk_label(ctx, "", NK_TEXT_LEFT);
            nk_label(ctx, "Method", NK_TEXT_LEFT);
            nk_label(ctx, "1. Assign parametric values", NK_TEXT_LEFT);
            nk_label(ctx, "-You can choose centripetal, chord length, or", NK_TEXT_LEFT);
            nk_label(ctx, "equally spaced parametrization.", NK_TEXT_LEFT);
            nk_label(ctx, "2. Select knots and make a knot vector", NK_TEXT_LEFT);
            nk_label(ctx, "-You can choose equally spaced, or averaging knots.", NK_TEXT_LEFT);
            nk_label(ctx, "", NK_TEXT_LEFT);

            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Widget", NK_MAXIMIZED))
        {
            enum options_to_parameter { Centripetal, Chord_Length, Equally_Spaced };
            enum options_to_knot { Averaging, Equally_Spacing };

            static int option_to_para;
            static int option_to_knot;
            
            if (nk_tree_push(ctx, NK_TREE_NODE, "Options", NK_MAXIMIZED))
            {
                /* Basic widgets */
                static const float ratio[] = {120, 150};
                static float float_slider = 1.0f;
                static int property_degree = 2;

                nk_layout_row(ctx, NK_STATIC, 25, 2, ratio);
                nk_label(ctx, "Degree:", NK_TEXT_LEFT);
                nk_property_int(ctx, "Int:", 0, &property_degree, 10, 1, 1);

                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Prametrization", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 30, 3);
                option_to_para = nk_option_label(ctx, "Centripetal", option_to_para == Centripetal) ? Centripetal : option_to_para;
                option_to_para = nk_option_label(ctx, "Chord Length", option_to_para == Chord_Length) ? Chord_Length : option_to_para;
                option_to_para = nk_option_label(ctx, "Equally Spaced", option_to_para == Equally_Spaced) ? Equally_Spaced : option_to_para;
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Knot vector", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 30, 2);
                option_to_knot = nk_option_label(ctx, "Averaging", option_to_knot == Averaging) ? Averaging : option_to_knot;
                option_to_knot = nk_option_label(ctx, "Equally Spaced", option_to_knot == Equally_Spacing) ? Equally_Spacing : option_to_knot;
               
                nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
                nk_labelf(ctx, NK_TEXT_LEFT, "Slider t: %.2f", float_slider);
                nk_slider_float(ctx, 0.0f, &float_slider, 1.0f, 0.01f);

                nurbs.degree = property_degree;

                if (option_to_para == Centripetal) {
                    nurbs.set_parameter_function(SS::Para::centripetal);
                }
                else if (option_to_para == Chord_Length) {
                    nurbs.set_parameter_function(SS::Para::chord_length);
                }
                else if (option_to_para == Equally_Spaced) {
                    nurbs.set_parameter_function(SS::Para::equally_spaced);
                }
                if (option_to_knot == Averaging) {
                    nurbs.set_knot_vector_function(SS::Knot::averageing);
                }
                else if (option_to_knot == Equally_Spacing) {
                    nurbs.set_knot_vector_function(SS::Knot::equal_spacing);
                }

                if (nurbs.control_points.size() > nurbs.degree) {
                    nurbs.get_curve();
                }
                else
                {
                    nurbs.clear_curve();
                }
                
                
                nk_tree_pop(ctx);
            }




            if (nk_tree_push(ctx, NK_TREE_NODE, "Status", NK_MAXIMIZED))
            {
                static const float ratio[] = {60, 90};

                nk_layout_row_dynamic(ctx, 25, 2);
                nk_label(ctx, "Control Points (X, Y)", NK_TEXT_CENTERED);
                nk_label(ctx, "Weights", NK_TEXT_CENTERED);

                nk_layout_row_dynamic(ctx, 300, 2);
                if (nk_group_begin(ctx, "Group_Without_Border", 0)) {
                    string buffer;
                    nk_layout_row_static(ctx, 18, 150, 1);
                    for (unsigned int i = 0; i < nurbs.control_points.size(); ++i) {
                        SS::Vertex2f p = nurbs.control_points[i];
                        ostringstream string_maker;
                        string_maker << setfill('0') << setw(2) << i << ": (" 
                            << setfill(' ') << setw(4) << (int)p.x << ", " << setw(4) << (int)p.y << ")";
                        buffer = string_maker.str();
                        nk_labelf(ctx, NK_TEXT_LEFT, "%s", buffer.c_str());
                    }
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "Group_Without_Border", NK_WINDOW_BORDER)) {
                    nk_layout_row(ctx, NK_STATIC, 18, 2, ratio);
                    for (unsigned int i = 0; i < nurbs.weights.size(); ++i) {
                        nk_labelf(ctx, NK_TEXT_LEFT, "w: %.1f", nurbs.weights[i]);
                        nk_slider_float(ctx, 0.0f, &nurbs.weights[i], 10.0f, 1.0f);
                    }
                    nk_group_end(ctx);
                }
                
                nk_layout_row_dynamic(ctx, 25, 2);
                nk_label(ctx, "Parameters", NK_TEXT_CENTERED);
                nk_label(ctx, "Knots", NK_TEXT_CENTERED);

                nk_layout_row_dynamic(ctx, 300, 2);
                if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
                    string buffer;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    for (unsigned int i = 0; i < nurbs.parameters.size(); ++i) {
                        ostringstream string_maker;
                        string_maker << nurbs.parameters[i];
                        buffer = string_maker.str();
                        nk_button_label(ctx, buffer.c_str());
                    }
                    nk_group_end(ctx);
                }
                if (nk_group_begin(ctx, "Group_With_Border", NK_WINDOW_BORDER)) {
                    string buffer;
                    nk_layout_row_dynamic(ctx, 25, 1);
                    for (unsigned int i = 0; i < nurbs.knots.size(); ++i) {
                        ostringstream string_maker;
                        string_maker << nurbs.knots[i];
                        buffer = string_maker.str();
                        nk_button_label(ctx, buffer.c_str());
                    }
                    nk_group_end(ctx);
                }
                nk_tree_pop(ctx);
            }

            nk_tree_pop(ctx);
        }
    }
    nk_end(ctx);
    return !nk_window_is_closed(ctx, "NURBS");
}

