static void read_video_props (GstCaps *caps)
{
    gint width, height;
    const GstStructure *str;
    g_return_if_fail (gst_caps_is_fixed (caps));
    str = gst_caps_get_structure (caps, 0);
    if (!gst_structure_get_int (str, "width", &width) ||
            !gst_structure_get_int (str, "height", &height)) {
        g_print ("No width/height available\n");
        return;
    }
    g_print ("The video size of this set of capabilities is %dx%d\n",width, height);
}

static gboolean link_elements_with_filter (GstElement *element1, GstElement *element2)
{
    gboolean link_ok;
    GstCaps *caps;
    caps = gst_caps_new_simple ("video/x-raw-yuv",
                                "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('I', '4', '2','0'),
                                "width", G_TYPE_INT, 384,
                                "height", G_TYPE_INT, 288,
                                "framerate", GST_TYPE_FRACTION, 25, 1,
                                NULL);
    link_ok = gst_element_link_filtered (element1, element2, caps);
    gst_caps_unref (caps);
    if (!link_ok) {
        g_warning ("Failed to link element1 and element2!");
    }
    return link_ok;
}

static gboolean link_elements_with_filter (GstElement *element1, GstElement *element2)
{
    gboolean link_ok;
    GstCaps *caps;
    caps = gst_caps_new_full (
               gst_structure_new ("video/x-raw-yuv",
                                  "width", G_TYPE_INT, 384,
                                  "height", G_TYPE_INT, 288,
                                  "framerate", GST_TYPE_FRACTION, 25, 1,
                                  NULL),
               gst_structure_new ("video/x-raw-rgb",
                                  "width", G_TYPE_INT, 384,
                                  "height", G_TYPE_INT, 288,
                                  "framerate", GST_TYPE_FRACTION, 25, 1,
                                  NULL),
               NULL);
    link_ok = gst_element_link_filtered (element1, element2, caps);
    gst_caps_unref (caps);
    if (!link_ok) {
        g_warning ("Failed to link element1 and element2!");
    }
    return link_ok;
}
