//OggVorbis_player.c
#include <gst/gst.h>
int main (int argc,char *argv[])
{
    GstElement *player;
    /*init*/
    gst_init(&argc,&argv);
    /*create player*/
    player=gst_element_factory_make("oggvorbisplayer","player");
    /*set the source audio file*/
    g_object_set(G_OBJECT(player),"location","/work/git/sisiwuxi/helllo-git/file/gstreamer/1.ogg",NULL);
    /*start playback*/
    gst_element_set_state(GST_ELEMENT(player),GST_STATE_PLAYING);
}
