#include <gst/gst.h>
#include <glib.h>
//定义消息处理函数,
static gboolean bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;//这个是主循环的指针，在接受EOS消息时退出循环
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
    GstElement *pipeline,*source,*decoder,*sink;//定义组件
    GstBus *bus;

    gst_init(&argc,&argv);
    loop = g_main_loop_new(NULL,FALSE);//创建主循环，在执行 g_main_loop_run后正式开始循环

    if(argc != 2)
    {
        g_printerr("Usage:%s <mp3 filename>\n",argv[0]);
        return -1;
    }
    //创建管道和组件
    pipeline = gst_pipeline_new("audio-player");
    source = gst_element_factory_make("filesrc","file-source");
    decoder = gst_element_factory_make("vorbisdec", "vorbis-decoder");
    sink = gst_element_factory_make("autoaudiosink","audio-output");

    if(!pipeline||!source||!decoder||!sink) {
        g_printerr("One element could not be created.Exiting.\n");
        return -1;
    }
    //设置 source的location 参数。即 文件地址.
    g_object_set(G_OBJECT(source),"location",argv[1],NULL);
    //得到 管道的消息总线
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    //添加消息监视器
    gst_bus_add_watch(bus,bus_call,loop);
    gst_object_unref(bus);
    //把组件添加到管道中.管道是一个特殊的组件，可以更好的让数据流动
    gst_bin_add_many(GST_BIN(pipeline),source,decoder,sink,NULL);
    //依次连接组件
    gst_element_link_many(source,decoder,sink,NULL);
    //开始播放
    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    g_print("Running\n");
    //开始循环
    g_main_loop_run(loop);
    g_print("Returned,stopping playback\n");
    gst_element_set_state(pipeline,GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    return 0;
}

