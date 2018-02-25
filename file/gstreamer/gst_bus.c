//gst_bus.c
#include<gst/gst.h>
static GMainLoop *loop;
static gboolean my_bus_callback(GstBus *bus,GstMessage *message,gpointer data)
{
    g_print("Got %s message\n",GST_MESSAGE_TYPE_NAME(message));
    switch(GST_MESSAGE_TYPE(message))
    {
        case GST_MESSAGE_ERROR:
        {
            GError *err;
            gchar *debug;
            gst_message_parse_error(message,&err,&debug);
            g_print("Error:%s\n",err->message);
            g_error_free(err);
            g_free(debug);
            g_main_loop_quit(loop);
            break;
        }
        case GST_MESSAGE_EOS:
            g_main_loop_quit(loop);
            break;
        default:
            break;
    }
    return TRUE;
}
gint main(gint argc, gchar *argv[])
{
    GstElement *pipeline;
    GstBus *bus;
    /*init*/
    gst_init(&argc,&argv);
    /*create pipeline, add handler*/
    pipeline=gst_pipeline_new("my_pipeline");
    /*adds a watch for new message*/
    bus=gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus,my_bus_callback,NULL);
    gst_object_unref(bus);
    /*create a mainloop*/
    loop=g_main_loop_new(NULL,FALSE);
    g_main_loop_run(loop);
    /*clean up*/
    gst_element_set_state(pipeline,GST_STATE_NULL);
    //gst_element_unref(pipeline);
    //gst_main_loop_unref(loop);
    return 0;
}
