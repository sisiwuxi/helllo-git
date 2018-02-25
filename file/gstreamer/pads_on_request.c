//pads_on_request.c
#include<gst/gst.h>
static void some_function(GstElement *tee)
{
    GstPad *pad;
    gchar *name;
    pad=gst_element_get_request_pad(tee,"src%d");
    name=gst_pad_get_name(pad);
    g_print("A new pad %d was created\n",name);
    g_free(name);
    /*here,you would link the pad*/
    /*free*/
    gst_object_unref(GST_OBJECT(pad));
}

static void link_to_multiplexer (GstPad *tolink_pad, GstElement *mux)
{
    GstPad *pad;
    gchar *srcname, *sinkname;
    srcname = gst_pad_get_name (tolink_pad);
    pad = gst_element_get_compatible_pad (mux, tolink_pad);
    gst_pad_link (tolinkpad, pad);
    sinkname = gst_pad_get_name (pad);
    gst_object_unref (GST_OBJECT (pad));
    g_print ("A new pad %s was created and linked to %s\n", srcname, sinkname);
    g_free (sinkname);
    g_free (srcname);
}
