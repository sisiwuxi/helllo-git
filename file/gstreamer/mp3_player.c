#include <gst/gst.h>
#include <glib.h>
//定义消息处理函数
static gboolean bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;//这个是主循环的指针，在接收到EOS消息时退出循环
    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_EOS:
        g_print("End of stream\n");
        g_main_loop_quit(loop);
        break;
    case GST_MESSAGE_ERROR:
    {
        gchar *debug;
        GError *error;
        gst_message_parse_error(msg,&error,&debug);
        g_free(debug);
        g_printerr("ERROR:%s\n",error->message);
        g_error_free(error);
        g_main_loop_quit(loop);

        break;
    }
    default:
        break;
    }
    return TRUE;
}

int main(int argc,char *argv[])
{
    GMainLoop *loop;
    GstElement *pipeline,*source,*decoder, *filter, *sink;//定义组件
    GstBus *bus;

    gst_init(&argc,&argv);
    loop = g_main_loop_new(NULL,FALSE);//创建主循环，在执行g_main_loop_run后正式开始循环，Glib里面实现的

    if(argc != 2)
    {
        g_printerr("Usage:%s <mp3 filename>\n",argv[0]);
        return -1;
    }
    //创建管道和组件
    pipeline = gst_pipeline_new("mp3-audio-player");
    source = gst_element_factory_make("filesrc","mp3-file-source");//基于已有的factory新建element
    decoder = gst_element_factory_make("mad","mp3-mad-decoder");
    //sink = gst_element_factory_make("autoaudiosink","mp3-audio-output");
    filter   = gst_element_factory_make ("myfilter", "myfilter");
    sink = gst_element_factory_make("alsasink","alsa-output");

    if(!pipeline||!source||!decoder||!sink) {
        g_printerr("One element could not be created.Exiting.\n");
        return -1;
    }
    else if (!filter) {
        g_print ("Your self-written filter could not be found. Make sure it "
                 "is installed correctly in $(libdir)/gstreamer-1.0/ or "
                 "~/.gstreamer-1.0/plugins/ and that gst-inspect-1.0 lists it. "
                 "If it doesn't, check with 'GST_DEBUG=*:2 gst-inspect-1.0' for "
                 "the reason why it is not being loaded.");
        return -1;
    }
    //给source这个G_OBJECT设置location property
    g_object_set(G_OBJECT(source),"location",argv[1],NULL);
    //获取Glib维护的pipeline的bus +1
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    //给拿到bus添加callback
    gst_bus_add_watch(bus,bus_call,loop);
    //释放bus实例
    gst_object_unref(bus);
    //把source,decoder,sink添加到pipeline中，pipeline是一个特殊的组件；可变长函数所以结尾加NULL
    gst_bin_add_many(GST_BIN(pipeline),source,decoder,filter,sink,NULL);
    //依次连接source,decoder,sink
    gst_element_link_many(source,decoder,filter,sink,NULL);
    //debug方式生成mp3_pipeline.dot
    GST_DEBUG_BIN_TO_DOT_FILE((GstBin *)pipeline, GST_DEBUG_GRAPH_SHOW_ALL, "mp3_pipeline");
    //开始播放
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_print("Running\n");
    //类似C++的run
    g_main_loop_run(loop);
    g_print("Returned,stopping playback\n");
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}

